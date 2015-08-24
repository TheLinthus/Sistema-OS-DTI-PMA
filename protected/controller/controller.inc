<?php

namespace controller;

require_once './protected/model/model.inc';
require_once './protected/database/postgresql.inc';
require_once './protected/mailer/class.phpmailer.php';
require_once './protected/mailer/class.smtp.php';

use model;

class SistemaController {

    function index(&$input) {
        $output = array();



        return $output;
    }

}

class ArquivosController {

    /**
     * Busca dados de um arquivo para download
     * 
     * @param type $input
     * @return array 'bytes' contém dados do arquivo para download
     */
    function download(&$input) {
        $output = array();
        if (isset($input['args']['id'])) {
            $arquivos = new model\Arquivos();
            $arquivo = $arquivos->getData('id = \'' . $input['args']['id'] . '\'');
            if (isset($arquivo[0])) {
                $output['bytes'] = $arquivo[0]['arquivo'];
                $output['type'] = $arquivo[0]['mimetype'];
                $output['name'] = $arquivo[0]['nome'];
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'batRequest';
        }
        return $output;
    }

}

class ChamadoController {

    /**
     * Gera página para abertura de chamado. Se está definido secretaria, setor, area, modulo ou problema define etapa da abertura
     * Requer estar logado
     * 
     * @param array $input
     * @return array Dados de areas, modulos, problemas e secretarias para geração das listas (opções selecionaveis)
     */
    function cadastro(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $secretaria = new model\Secretaria();
            $output['secretarias'] = $secretaria->getData();
            $output['sec'] = -1;
            $output['set'] = -1;
            $output['area'] = -1;
            $output['mod'] = -1;
            $output['problema'] = -1;
            $output['setores'] = array();
            $output['etapa'] = 1;
            if (isset($input['args']['sec'])) {
                $setor = new model\Setor();
                $setor->setRowsPerPage(0);
                $output['setores'] = $setor->getData('secretaria = ' . mysql_escape_string($input['args']['sec']));
                if (empty($output['setores'])) {
                    $output['sec'] = 0;
                    $output['etapa'] = 1;
                } else {
                    $output['sec'] = $input['args']['sec'];
                    $output['etapa'] = 2;
                }
                if (isset($input['args']['set'])) {
                    $output['set'] = $input['args']['set'];
                    $output['etapa'] = 3;
                }
                if (isset($input['args']['area'])) {
                    $output['area'] = $input['args']['area'];
                }
                if (isset($input['args']['prob'])) {
                    $output['problema'] = $input['args']['prob'];
                }
                if (isset($input['args']['mod'])) {
                    $output['mod'] = $input['args']['mod'];
                }
            }
            $area = new model\Area();
            $modulo = new model\Modulo();
            $problema = new model\Problema();
            $output['areas'] = $area->getData();
            $output['modulos'] = $modulo->getData();
            $output['problemas'] = $problema->getData();
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Calcula a prioridade do chamado baseado  no fator de multiplicação do setor e da opção de piroridade escolhida pelo usuario,
     * fornecendo um calculo de prioridade adequado para cada setor
     * 
     * @param int $i nível de prioridade do setor
     * @param int $f nível de prioridade escolhido pelo usuário
     * @return int prioridade calculada
     */
    function calculo($i, $f) {
        $base = ($i - 1) * 25;
        // Base do Setor (0, 25, 50, 75, 100)
        $diferenca = $f - $i;
        // Diferença entre prioridade do Usuário e do Setor
        $fatordomeio = (2 - sqrt(pow(3 - $i, 2))) * 4 + 8;
        // Diferença entre um Setor de nível 3, para Médio ficar mais espalhado
        $diferencapositiva = sqrt(pow($diferenca, 2));
        // Valor sempre positivo da diferença
        $soma = $fatordomeio + $diferencapositiva;
        // Valor para somar/subtrair da base
        $multiplicacao = $soma * $diferenca;
        // Multiplica para transformar numa subtração ou adição
        return $base + $multiplicacao;
    }

    /**
     * Gera has de cor RGB para uma determinada porcentagem de prioridade.
     * 
     * @param int $v valor de 0 a 100, maior vermelho, menor verde, meio amarelo
     * @return String has #RGB
     */
    function corPrioridade($v) {
        $g = 355 - $v * 2;
        $r = 155 + $v * 2;
        $b = 155;
        if ($g > 255) {
            $g = 255;
        }
        if ($r > 255) {
            $r = 255;
        }
        $R = str_pad(dechex($r), 2, '0', STR_PAD_LEFT);
        $G = str_pad(dechex($g), 2, '0', STR_PAD_LEFT);
        $B = str_pad(dechex($b), 2, '0', STR_PAD_LEFT);
        return "#{$R}{$G}{$B}";
    }

    /**
     * Coleta dados da abertura de chamado e 
     * 
     * Requer estar logado
     * 
     * @param type $input
     * @return array
     */
    function confirmar(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            if (isset($input['post']['secretaria']) && isset($input['post']['setor']) && isset($input['post']['area']) && isset($input['post']['modulo']) && isset($input['post']['problema']) && isset($input['post']['descricao']) && isset($input['post']['patrimonio']) && isset($input['post']['prioridade'])) {
                $setor = new \model\Setor();
                $chamado = new \model\Chamado();
                $arquivos = new \model\Arquivos();
                $estado = new \model\Estado();
                $usuario = new \model\Usuario();
                // Força insersão de descrição
                if (strlen($input['post']['descricao']) < 10) {
                    echo '<script>';
                    echo 'alert(\'A descrição está muito curta (no mínimo 10 caracteres, escritos ' . strlen($input['post']['descricao']) . ')!\');';
                    echo 'history.go(-1);</script>';
                    echo '</script>';
                    return $output;
                }
                // Definição de prioridade baseado em setor
                $rs = $setor->getData('id = \'' . mysql_escape_string($input['post']['setor']) . '\'');
                $prioridade = $rs[0]['prioridade'];
                // Rastrear ip de origem do chamado
                $input['post']['iporigem'] = getIP();
                // Se o chamado está sendo aberto internamente pelo DTI pode haver um usuario vinculado, para 'abrir como'
                if (\Login::isDti() && !(empty($input['post']['usuario-cgm']) || $input['post']['usuario-cgm'] < 0)) {
                    $rs = $usuario->getData('cgm = \'' . mysql_escape_string($input['post']['usuario-cgm']) . '\'');
                    if (isset($rs[0])) {
                        $input['post']['usuario_cpf'] = $rs[0]['cpf'];
                    } else {
                        $ecidade = new \database\ECidade();
                        $where = "cgm_id = '" . mysql_escape_string($input['post']['usuario-cgm']) . "'";
                        //$table = 'public.v_cgm INNER JOIN public.v_funcionario ON (v_cgm.cgm_cpf = v_funcionario.fun_cpf)';
                        $rs = $ecidade->getData('public.v_cgm', $where);
                        if (isset($rs[0])) {
                            $newrecord['cgm'] = $rs[0]['cgm_id'];
                            $newrecord['cpf'] = $rs[0]['cgm_cpf'];
                            $input['post']['usuario_cpf'] = $rs[0]['cgm_cpf'];
                            $newrecord['nome'] = $rs[0]['cgm_nome'];
                            $newrecord['email'] = $rs[0]['cgm_email'];
                            $where = "fun_cpf = '" . $rs[0]['cgm_cpf'] . "'";
                            $rs = $ecidade->getData('public.v_funcionario', $where);
                            if (isset($rs[0])) {
                                $newrecord['matricula'] = $rs[0]['fun_matricula'];
                                $newrecord['cargo'] = $rs[0]['fun_cargo'];
                            } else {
                                $newrecord['matricula'] = "";
                                $newrecord['cargo'] = "Não Registrado";
                            }
                            $usuario->insertRecord($newrecord);
                        }
                    }
                } else {
                    $input['post']['usuario_cpf'] = $_SESSION['cpf'];
                }
                $input['post']['prioridade'] = $this->calculo($prioridade, $input['post']['prioridade']);
                if (empty($input['post']['patrimonio']) || $input['post']['patrimonio'] < 0) {
                    unset($input['post']['patrimonio']);
                } else {
                    if ($input['post']['patrimonio'] == 0 && !empty($input['post']['placa'])) {
                        $input['post']['descricao'] .= ' (Placa do patrimônio: ' . $input['post']['patrimonio'] . ')';
                    }
                }
                // Limpa os campos não selecionados
                if (empty($input['post']['problema']) || $input['post']['problema'] < 0) {
                    unset($input['post']['problema']);
                }
                if (empty($input['post']['modulo']) || $input['post']['modulo'] < 0) {
                    unset($input['post']['modulo']);
                }
                if (empty($input['post']['area']) || $input['post']['area'] < 0) {
                    unset($input['post']['area']);
                }
                $chamado->insertRecord($input['post']);
                // Adicionar estado inicial de chamado apra que possa ser atendido pela triagem
                // Por padrão o nível inicial é 1, sem Técnico designado, Data hora atual
                $estadoini = array();
                $estadoini['chamado'] = $chamado->last_insert_id;
                $estadoini['estado'] = 'Aguardando Atendimento';
                $estadoini['tipo'] = 1;
                $estado->insertRecord($estadoini);
                foreach ($_FILES as $uploadedfile) {
                    // Para cada arquivo enviado junto com o chamado
                    if ($uploadedfile['error'] == 0) {
                        // Os 'em branco' apresentarão código de erro 4
                        $file = array();
                        $file['nome'] = $uploadedfile['name'];
                        $file['arquivo'] = file_get_contents($uploadedfile['tmp_name']);
                        $file['chamado'] = $chamado->last_insert_id;
                        $finfo = finfo_open(FILEINFO_MIME_TYPE);
                        $file['mimetype'] = finfo_file($finfo, $uploadedfile['tmp_name']);
                        finfo_close($finfo);
                        $arquivos->insertRecord($file);
                    }
                }
                $output['redirect'] = '/v/chamado/ver/id/' . $chamado->last_insert_id;
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    function avisarTecnicos($chamado) {
        $areastecnico = new \model\AreasTecnico();
        $usuario = new \model\Usuario();
        $area = new \model\Area();
        $modulo = new \model\Modulo();
        $problema = new \model\Problema();
        $patrimonio = new \model\Patrimonio();

        $chamado['area'] = $chamado['area'] != null ? ($tmp = $area->getData('id = \'' . $chamado['area'] . '\'')) ? $tmp[0]['area'] : $tmp[0]['area'] : 'Não Informado';
        $chamado['modulo'] = $chamado['modulo'] != null ? ($tmp = $modulo->getData('id = \'' . $chamado['modulo'] . '\'')) ? $tmp[0]['modulo'] : $tmp[0]['modulo'] : 'Não Informado';
        $chamado['problema'] = $chamado['problema'] != null ? ($tmp = $problema->getData('id = \'' . $chamado['problema'] . '\'')) ? $tmp[0]['problema'] : $tmp[0]['problema'] : 'Não Informado';
        $chamado['patrimonio'] = $chamado['patrimonio'] != null ? ($tmp = $patrimonio->getData('id = \'' . $chamado['patrimonio'] . '\'')) ? $tmp[0]['descricao'] : $tmp[0]['descricao'] : 'Não Informado';

        if ((int) $chamado['prioridade'] < 20) {
            $chamado['nomeprioridade'] = 'Baixa';
        } elseif ((int) $chamado['prioridade'] < 40) {
            $chamado['nomeprioridade'] = 'Média';
        } elseif ((int) $chamado['prioridade'] < 60) {
            $chamado['nomeprioridade'] = 'Alta';
        } elseif ((int) $chamado['prioridade'] < 80) {
            $chamado['nomeprioridade'] = 'Muito Alta';
        } else {
            $chamado['nomeprioridade'] = 'Urgente';
        }

        $rs = $areastecnico->getData('area_id = \'' . $chamado['area'] . '\'');
        $emails = array();

        foreach ($rs as $vinculo) {
            $tmp = $usuario->getData('cpf = \'' . $vinculo['tecnico_cpf'] . '\'');
            if (!empty($tmp[0]['email'])) {
                $emails[] = $tmp[0]['email'];
            }
        }

        if (empty($emails)) {
            return false;
        }

        $mail = new \PHPMailer();

        $mail->CharSet = 'UTF-8';
        $mail->isSMTP();
        // Set mailer to use SMTP
        $mail->Host = '172.17.2.146';
        // Specify main and backup SMTP servers
        $mail->SMTPAuth = true;
        // Enable SMTP authentication
        $mail->Username = 'suporte@alegrete.rs.gov.br';
        // SMTP username
        $mail->Password = 'PhV8_141721';
        // SMTP password
        $mail->SMTPSecure = 'tls'; //'ssl';
        // Enable TLS encryption, `ssl` also accepted
        $mail->Port = 587;
        // TCP port to connect to
        $mail->From = 'suporte@alegrete.rs.gov.br';
        $mail->FromName = 'Sistema de Ordens de Serviço';
        foreach ($emails as $email) {
            $mail->addAddress($email);
        }
        $mail->addReplyTo('suporte@alegrete.rs.gov.br');
        $mail->isHTML(true);
        // Set email format to HTML
        $mail->Subject = 'Novo Chamado para sua Area';
        $mail->Body = "<h1>Sistema de Ordens de Serviço</h1>";
        $mail->Body .= "Há um novo chamado designado para a sua área, favor coferir no sistema SOS em ";
        $mail->Body .= "<b>Chamados Designados</b>";
        $mail->Body .= "<ul>";
        $mail->Body .= "<li>Area: ";
        $mail->Body .= $chamado['area'];
        $mail->Body .= "</li>";
        $mail->Body .= "<li>Modulo: ";
        $mail->Body .= $chamado['modulo'];
        $mail->Body .= "</li>";
        $mail->Body .= "<li>Problema: ";
        $mail->Body .= $chamado['problema'];
        $mail->Body .= "</li>";
        $mail->Body .= "<li>Patrimonio: ";
        $mail->Body .= $chamado['patrimonio'];
        $mail->Body .= "</li>";
        $mail->Body .= "<li>Prioridade: ";
        $mail->Body .= $chamado['nomeprioridade'];
        $mail->Body .= "</li>";
        $mail->Body .= "<li>Descrição: ";
        $mail->Body .= $chamado['problema'];
        $mail->Body .= "</li>";
        $mail->Body .= "</ul>";
        $mail->AltBody = "Há um jogo chamado designado para a sua área";

        return $mail->send();
    }

    function encaminhar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $chamado = new \model\Chamado();
            $arquivos = new \model\Arquivos();
            $estado = new \model\Estado();
            $area = new \model\Area();
            if (isset($input['post']['id'])) {
                $rs = $chamado->getData("id = '{$input['post']['id']}'");
                if (isset($rs[0])) {
                    $estados = $estado->getData('chamado = \'' . $rs[0]['id'] . '\'');
                    $endestados = end($estados);
                    if ($rs[0]['area'] == null && (!isset($input['post']['area']) || $input['post']['id'] == 'null')) {
                        $output['mensagem'] = 'Não pode encaminhar sem área';
                        return $output;
                    }
                    // Se o estado atual do chamado é finalizado ou baixa
                    if ($endestados['nivel'] > '2') {
                        $output['mensagem'] = 'O Chamado solicitado já está finalizado';
                        return $output;
                    }
                    // Se o estado do chamado já é de em atendimento e não é o mesmo usuário que está atendendo logado
                    if ($endestados['tipo'] == '2' && $endestados['tecnico_cpf'] != $_SESSION['cpf']) {
                        $output['mensagem'] = 'O Chamado solicitado já está em atendimento por outro técnico';
                        return $output;
                    }
                    // Se o seu nível é diferente do estado do chamado ou o usuário logado não possui a área designada
                    if ($_SESSION['nivel'] != 3 && $endestados['nivel'] != $_SESSION['nivel'] || $endestados['nivel'] != 1 && !isset($_SESSION['areas'][$rs[0]['area']])) {
                        $output['mensagem'] = 'O Chamado solicitado não está designado para uma área que você atende.';
                        return $output;
                    }
                    $update = array();
                    $update['modulo'] = $input['post']['modulo'];
                    $update['problema'] = $input['post']['problema'];
                    $update['prioridade'] = $input['post']['prioridade'];
                    $update['descricao'] = $input['post']['descricao'];
                    $update['solucao'] = $input['post']['solucao'];
                    $update['patrimonio'] = $input['post']['patrimonio'];
                    $update['id'] = $input['post']['id'];
                    if ($endestados['nivel'] == 1) {
                        $update['area'] = $input['post']['area'];
                    }
                    $chamado->updateRecord($update);
                    $areachamado = ($tmp = $area->getData('id = ' . $rs[0]['area'])) ? $tmp[0]['area'] : $tmp[0]['area'];
                    $atendendo = array();
                    $atendendo['chamado'] = $input['post']['id'];
                    if ($endestados['nivel'] == 1) {
                        $atendendo['estado'] = 'Encaminhado para Técnico (' . $areachamado . ')';
                        $atendendo['tipo'] = 1;
                        if (!$this->avisarTecnicos($rs[0])) {
                            $output['mensagem'] = 'Não foi possivel enviar e-mail aos Técnicos';
                        }
                    } elseif ($endestados['nivel'] == 2 && $endestados['tipo'] == 5) {
                        $atendendo['estado'] = 'Em Análise ' . $areachamado;
                        $atendendo['tecnico_cpf'] = $_SESSION['cpf'];
                        $atendendo['tipo'] = 2;
                        $output['stay'] = true;
                    } else {
                        $atendendo['estado'] = 'Encaminhado para Terceiro';
                        $atendendo['tipo'] = 5;
                        $output['stay'] = true;
                    }
                    $atendendo['nivel'] = 2;
                    $estado->insertRecord($atendendo);
                    $excluir = explode(',', $input['post']['excluir']);
                    foreach ($excluir as $deletefile) {
                        $arquivos->deleteRecord(array('id' => $deletefile));
                    }
                    foreach ($_FILES as $uploadedfile) {
                        // Para cada arquivo enviado junto com o chamado
                        if ($uploadedfile['error'] == 0) {
                            // Os 'em branco' apresentarão código de erro 4
                            $file = array();
                            $file['nome'] = $uploadedfile['name'];
                            $file['arquivo'] = file_get_contents($uploadedfile['tmp_name']);
                            $file['chamado'] = $update['id'];
                            $finfo = finfo_open(FILEINFO_MIME_TYPE);
                            $file['mimetype'] = finfo_file($finfo, $uploadedfile['tmp_name']);
                            finfo_close($finfo);
                            $arquivos->insertRecord($file);
                        }
                    }
                    $output['ok'] = true;
                } else {
                    $output['error'] = 'notFound';
                }
            } else {
                $output['error'] = 'batRequest';
            }
        }
        return $output;
    }

    function finalizar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $chamado = new \model\Chamado();
            $arquivos = new \model\Arquivos();
            $estado = new \model\Estado();
            if (isset($input['post']['id'])) {
                $rs = $chamado->getData("id = '{$input['post']['id']}'");
                if (isset($rs[0])) {
                    $estados = $estado->getData('chamado = \'' . $rs[0]['id'] . '\'');
                    // Se o estado atual do chamado é finalizado ou baixa
                    if ((($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) > '2') {
                        $output['mensagem'] = 'O Chamado solicitado já está finalizado';
                        return $output;
                    }
                    // Se o chamado está encaminhado para um terceiro
                    if ((($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) == 2 && (($tmp = end($estados)) ? $tmp['tipo'] : $tmp['tipo']) == 5) {
                        $output['mensagem'] = 'O Chamado está encaminhado para terceiro, deve voltar para \'Em atendimento\'';
                        return $output;
                    }
                    // Se o estado do chamado já é de em atendimento e não é o mesmo usuário que está atendendo logado
                    if ((($tmp = end($estados)) ? $tmp['tipo'] : $tmp['tipo']) == '2' && (($tmp = end($estados)) ? $tmp['tecnico_cpf'] : $tmp['tecnico_cpf']) != $_SESSION['cpf']) {
                        $output['mensagem'] = 'O Chamado solicitado já está em atendimento por outro técnico';
                        return $output;
                    }
                    // Se o seu nível é diferente do estado do chamado ou o usuário logado não possui a área designada
                    if ($_SESSION['nivel'] != 3 && (($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) != $_SESSION['nivel'] || (($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) != 1 && !isset($_SESSION['areas'][$rs[0]['area']])) {
                        $output['mensagem'] = 'O Chamado solicitado não está designado para uma área que você atende.';
                        return $output;
                    }
                    $update = array();
                    $update['modulo'] = $input['post']['modulo'];
                    $update['problema'] = $input['post']['problema'];
                    $update['prioridade'] = $input['post']['prioridade'];
                    $update['descricao'] = $input['post']['descricao'];
                    $update['solucao'] = $input['post']['solucao'];
                    $update['patrimonio'] = $input['post']['patrimonio'];
                    $update['id'] = $input['post']['id'];
                    if ((($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) == 1) {
                        $update['area'] = $input['post']['area'];
                    }
                    $chamado->updateRecord($update);
                    $atendendo = array();
                    $atendendo['chamado'] = $input['post']['id'];
                    $atendendo['estado'] = 'Finalizado';
                    $atendendo['tecnico_cpf'] = $_SESSION['cpf'];
                    $atendendo['tipo'] = 3;
                    $atendendo['nivel'] = 3;
                    $estado->insertRecord($atendendo);
                    $excluir = explode(',', $input['post']['excluir']);
                    foreach ($excluir as $deletefile) {
                        $arquivos->deleteRecord(array('id' => $deletefile));
                    }
                    foreach ($_FILES as $uploadedfile) {
                        // Para cada arquivo enviado junto com o chamado
                        if ($uploadedfile['error'] == 0) {
                            // Os 'em branco' apresentarão código de erro 4
                            $file = array();
                            $file['nome'] = $uploadedfile['name'];
                            $file['arquivo'] = file_get_contents($uploadedfile['tmp_name']);
                            $file['chamado'] = $update['id'];
                            $finfo = finfo_open(FILEINFO_MIME_TYPE);
                            $file['mimetype'] = finfo_file($finfo, $uploadedfile['tmp_name']);
                            finfo_close($finfo);
                            $arquivos->insertRecord($file);
                        }
                    }
                    $output['ok'] = true;
                } else {
                    $output['error'] = 'notFound';
                }
            } else {
                $output['error'] = 'batRequest';
            }
        }
        return $output;
    }

    function salvar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $chamado = new \model\Chamado();
            $area = new \model\Area();
            $arquivos = new \model\Arquivos();
            $estado = new \model\Estado();
            if (isset($input['post']['id'])) {
                $rs = $chamado->getData("id = '{$input['post']['id']}'");
                if (isset($rs[0])) {
                    $estados = $estado->getData('chamado = \'' . $rs[0]['id'] . '\'');
                    $uestado = end($estados);
                    // Se o estado atual do chamado é finalizado ou baixa
                    if ((($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) > '2') {
                        $output['mensagem'] = 'O Chamado solicitado já está finalizado';
                        return $output;
                    }
                    // Se o estado do chamado já é de em atendimento e não é o mesmo usuário que está atendendo logado
                    if ((($tmp = end($estados)) ? $tmp['tipo'] : $tmp['tipo']) == '2' && (($tmp = end($estados)) ? $tmp['tecnico_cpf'] : $tmp['tecnico_cpf']) != $_SESSION['cpf']) {
                        $output['mensagem'] = 'O Chamado solicitado já está em atendimento por outro técnico';
                        return $output;
                    }
                    // Se o seu nível é diferente do estado do chamado ou o usuário logado não possui a área designada
                    if ($_SESSION['nivel'] != 3 && (($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) != $_SESSION['nivel'] || (($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) != 1 && !isset($_SESSION['areas'][$rs[0]['area']])) {
                        $output['mensagem'] = 'O Chamado solicitado não está designado para uma área que você atende.';
                        return $output;
                    }
                    $update = array();
                    $update['modulo'] = $input['post']['modulo'];
                    $update['problema'] = $input['post']['problema'];
                    $update['prioridade'] = $input['post']['prioridade'];
                    $update['descricao'] = $input['post']['descricao'];
                    $update['solucao'] = $input['post']['solucao'];
                    $update['patrimonio'] = $input['post']['patrimonio'];
                    $update['id'] = $input['post']['id'];
                    if ($uestado['nivel'] == 1) {
                        $update['area'] = $input['post']['area'];
                    } else {
                        $input['post']['area'] = $rs[0]['area'];
                    }
                    $chamado->updateRecord($update);
                    $areachamado = ($tmp = $area->getData('id = ' . $input['post']['area'])) ? $tmp[0]['area'] : $tmp[0]['area'];
                    $atendendo = array();
                    $atendendo['chamado'] = $input['post']['id'];
                    $atendendo['estado'] = 'Em Análise ' . (($uestado['nivel']) == 1 ? 'Triagem' : $areachamado) . ' (Alteração)';
                    if (!empty($uestado['tecnico_cpf'])) {
                        $atendendo['tecnico_cpf'] = $_SESSION['cpf'];
                    }
                    $atendendo['tipo'] = $uestado['tipo'];
                    $atendendo['nivel'] = $uestado['nivel'];
                    $estado->insertRecord($atendendo);
                    $excluir = explode(',', $input['post']['excluir']);
                    foreach ($excluir as $deletefile) {
                        $arquivos->deleteRecord(array('id' => $deletefile));
                    }
                    foreach ($_FILES as $uploadedfile) {
                        // Para cada arquivo enviado junto com o chamado
                        if ($uploadedfile['error'] == 0) {
                            // Os 'em branco' apresentarão código de erro 4
                            $file = array();
                            $file['nome'] = $uploadedfile['name'];
                            $file['arquivo'] = file_get_contents($uploadedfile['tmp_name']);
                            $file['chamado'] = $update['id'];
                            $finfo = finfo_open(FILEINFO_MIME_TYPE);
                            $file['mimetype'] = finfo_file($finfo, $uploadedfile['tmp_name']);
                            finfo_close($finfo);
                            $arquivos->insertRecord($file);
                        }
                    }
                    $output['ok'] = true;
                } else {
                    $output['error'] = 'notFound';
                }
            } else {
                $output['error'] = 'batRequest';
            }
        }
        return $output;
    }

    /**
     * Consulta dados e atributos de um chamado e os retorna para exibição apenas
     * 
     * @param array $input recebe o id como argumento para busca de um chamado especifico
     * @return array Retorna lista compilada de dados e atributos de um determinado chamado
     */
    function atender(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $chamado = new \model\Chamado();
            $arquivos = new \model\Arquivos();
            $area = new \model\Area();
            $modulo = new \model\Modulo();
            $problema = new \model\Problema();
            $secretaria = new \model\Secretaria();
            $setor = new \model\Setor();
            $patrimonio = new \model\Patrimonio();
            $estado = new \model\Estado();
            $usuario = new \model\Usuario();
            if (isset($input['args']['id'])) {
                $rs = $chamado->getData("id = '{$input['args']['id']}'");
                if (isset($rs[0])) {
                    $estados = $estado->getData('chamado = \'' . $rs[0]['id'] . '\'');
                    // Se o estado atual do chamado é finalizado ou baixa
                    if ((($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) > '2') {
                        $output['mensagem'] = array();
                        $output['mensagem']['tipo'] = 'highlight';
                        $output['mensagem']['icone'] = 'notice';
                        $output['mensagem']['titulo'] = 'Finalizado:';
                        $output['mensagem']['texto'] = 'O Chamado solicitado já está finalizado';
                        return $output;
                    }
                    // Se o estado do chamado já é de em atendimento e não é o mesmo usuário que está atendendo logado
                    if ((($tmp = end($estados)) ? $tmp['tipo'] : $tmp['tipo']) == '2' && (($tmp = end($estados)) ? $tmp['tecnico_cpf'] : $tmp['tecnico_cpf']) != $_SESSION['cpf']) {
                        $output['mensagem'] = array();
                        $output['mensagem']['tipo'] = 'highlight';
                        $output['mensagem']['icone'] = 'notice';
                        $output['mensagem']['titulo'] = 'Em atendimento:';
                        $output['mensagem']['texto'] = 'O Chamado solicitado já está em atendimento por outro técnico';
                        return $output;
                    }
                    // Se o seu nível é diferente do estado do chamado ou o usuário logado não possui a área designada
                    if ($_SESSION['nivel'] != 3 && (($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) != $_SESSION['nivel'] || (($tmp = end($estados)) ? $tmp['nivel'] : $tmp['nivel']) != 1 && !isset($_SESSION['areas'][$rs[0]['area']])) {
                        $output['mensagem'] = array();
                        $output['mensagem']['tipo'] = 'highlight';
                        $output['mensagem']['icone'] = 'notice';
                        $output['mensagem']['titulo'] = 'Não designado:';
                        $output['mensagem']['texto'] = 'O Chamado solicitado não está designado para uma área que você atende.';
                        return $output;
                    }
                    $output['data'] = $rs[0];
                    $output['areas'] = $area->getData();
                    $output['modulos'] = $modulo->getData();
                    $output['problemas'] = $problema->getData();
                    $output['data']['area'] = $output['data']['area'] != null ? ($tmp = $area->getData('id = \'' . $output['data']['area'] . '\'')) ? $tmp[0] : $tmp[0] : array('area' => 'Não Informado', 'id' => '-1');
                    $output['data']['modulo'] = $output['data']['modulo'] != null ? ($tmp = $modulo->getData('id = \'' . $output['data']['modulo'] . '\'')) ? $tmp[0] : $tmp[0] : array('modulo' => 'Não Informado', 'id' => '-1');
                    $output['data']['problema'] = $output['data']['problema'] != null ? ($tmp = $problema->getData('id = \'' . $output['data']['problema'] . '\'')) ? $tmp[0] : $tmp[0] : array('problema' => 'Não Informado', 'id' => '-1');
                    $output['data']['patrimonio'] = $output['data']['patrimonio'] != null ? ($tmp = $patrimonio->getData('id = \'' . $output['data']['patrimonio'] . '\'')) ? $tmp[0] : $tmp[0] : array('descricao' => 'Não Informado', 'placa' => '-1', 'id' => '-1');
                    $output['data']['secretaria'] = ($tmp = $secretaria->getData('id = \'' . $output['data']['secretaria'] . '\'')) ? $tmp[0]['secretaria'] : $tmp[0]['secretaria'];
                    $output['data']['setor'] = ($tmp = $setor->getData('id = \'' . $output['data']['setor'] . '\'')) ? $tmp[0]['setor'] : $tmp[0]['setor'];
                    $output['data']['usuario'] = ($tmp = $usuario->getData('cpf = \'' . $output['data']['usuario_cpf'] . '\'')) ? $tmp[0] : $tmp[0];
                    $output['data']['estado'] = $estados;
                    if ((($tmp = end($output['data']['estado'])) ? $tmp['tipo'] : $tmp['tipo']) == '1') {
                        $atendendo = array();
                        $atendendo['chamado'] = $output['data']['id'];
                        $atendendo['estado'] = 'Em Análise ' . ((($tmp = end($output['data']['estado'])) ? $tmp['nivel'] : $tmp['nivel']) == 1 ? 'Triagem' : $output['data']['area']['area']);
                        $atendendo['tecnico_cpf'] = $_SESSION['cpf'];
                        $atendendo['tipo'] = 2;
                        $atendendo['nivel'] = ($tmp = end($output['data']['estado'])) ? $tmp['nivel'] : $tmp['nivel'];
                        $estado->insertRecord($atendendo);
                        $atendendo['data'] = 'NOW';
                        $atendendo['id'] = $estado->last_insert_id;
                        $output['data']['estado'][] = $atendendo;
                    }
                    foreach ($output['data']['estado'] as &$val) {
                        $val['data'] = new \DateTime($val['data']);
                        $val['data'] = $val['data']->format('d/m/Y H:i');
                    }
                    $output['arquivos'] = $arquivos->getData('chamado = \'' . $output['data']['id'] . '\'', array(), array('id', 'nome'));
                    foreach ($output['arquivos'] as &$arquivo) {
                        $arquivo['tipo'] = substr($arquivo['nome'], strrpos($arquivo['nome'], '.') + 1);
                    }
                } else {
                    $output['error'] = 'notFound';
                }
            } else {
                $output['error'] = 'batRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Consulta dados e atributos de um chamado e os retorna para exibição apenas
     * 
     * @param array $input recebe o id como argumento para busca de um chamado especifico
     * @return array Retorna lista compilada de dados e atributos de um determinado chamado
     */
    function ver(&$input) {
        $output = array();
        $chamado = new \model\Chamado();
        $arquivos = new \model\Arquivos();
        $area = new \model\Area();
        $modulo = new \model\Modulo();
        $problema = new \model\Problema();
        $secretaria = new \model\Secretaria();
        $setor = new \model\Setor();
        $patrimonio = new \model\Patrimonio();
        $estado = new \model\Estado();
        $usuario = new \model\Usuario();
        if (isset($input['args']['id'])) {
            $rs = $chamado->getData("id = '{$input['args']['id']}'");
            if (isset($rs[0])) {
                $output['data'] = $rs[0];
                $output['data']['area'] = $output['data']['area'] != null ? ($tmp = $area->getData('id = \'' . $output['data']['area'] . '\'')) ? $tmp[0]['area'] : $tmp[0]['area'] : 'Não Informado';
                $output['data']['modulo'] = $output['data']['modulo'] != null ? ($tmp = $modulo->getData('id = \'' . $output['data']['modulo'] . '\'')) ? $tmp[0]['modulo'] : $tmp[0]['modulo'] : 'Não Informado';
                $output['data']['problema'] = $output['data']['problema'] != null ? ($tmp = $problema->getData('id = \'' . $output['data']['problema'] . '\'')) ? $tmp[0]['problema'] : $tmp[0]['problema'] : 'Não Informado';
                $output['data']['patrimonio'] = $output['data']['patrimonio'] != null ? ($tmp = $patrimonio->getData('id = \'' . $output['data']['patrimonio'] . '\'')) ? $tmp[0]['descricao'] : $tmp[0]['descricao'] : 'Não Informado';
                $output['data']['secretaria'] = ($tmp = $secretaria->getData('id = \'' . $output['data']['secretaria'] . '\'')) ? $tmp[0]['secretaria'] : $tmp[0]['secretaria'];
                $output['data']['setor'] = ($tmp = $setor->getData('id = \'' . $output['data']['setor'] . '\'')) ? $tmp[0]['setor'] : $tmp[0]['setor'];
                $output['data']['usuario'] = ($tmp = $usuario->getData('cpf = \'' . $output['data']['usuario_cpf'] . '\'')) ? $tmp[0] : $tmp[0];
                $output['data']['estado'] = $estado->getData('chamado = \'' . $output['data']['id'] . '\'');
                if ((int) $output['data']['prioridade'] < 20) {
                    $output['data']['nomeprioridade'] = 'Baixa';
                } elseif ((int) $output['data']['prioridade'] < 40) {
                    $output['data']['nomeprioridade'] = 'Média';
                } elseif ((int) $output['data']['prioridade'] < 60) {
                    $output['data']['nomeprioridade'] = 'Alta';
                } elseif ((int) $output['data']['prioridade'] < 80) {
                    $output['data']['nomeprioridade'] = 'Muito Alta';
                } else {
                    $output['data']['nomeprioridade'] = 'Urgente';
                }
                foreach ($output['data']['estado'] as &$estado) {
                    $estado['data'] = new \DateTime($estado['data']);
                    $estado['data'] = $estado['data']->format('d/m/Y H:i');
                }
                $output['arquivos'] = $arquivos->getData('chamado = \'' . $output['data']['id'] . '\'', array(), array('id', 'nome'));
                foreach ($output['arquivos'] as &$arquivo) {
                    $arquivo['tipo'] = substr($arquivo['nome'], strrpos($arquivo['nome'], '.') + 1);
                }
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'batRequest';
        }
        return $output;
    }

    private $debug;

    /**
     * Lista chamados baseado em uma série de filtros
     * 
     * @param int $nivel nível do estado do chamado
     * @param boolean $restringirtecnico restringir somente para chamados disponiveis para o técnico logado
     * @param string $filtro filtro para localização de palavras chave (seguir sintaxe MySQL)
     * @param string $sort ordena os resuldados encontrados, deve seguir o padrão do exemplo: "coluna-oredem"
     * @param array $tipos Lista de inteiros, que referenciam o tipo dos estados a serem buscados
     * @return type
     */
    function listaChamados($nivel = 0, $restringirtecnico = false, $filtro = '', $sort = 'prioridade-desc', $tipos = array(), $dataA = '', $dataB = '') {
        $chamado = new \model\Chamado();
        $area = new \model\Area();
        $usuario = new \model\Usuario();
        $estado = new \model\Estado();
        $problema = new \model\Problema();
        $sql = '';
        if (!empty($filtro)) {
            $sql .= '(' . $filtro . ')';
            $sql .= ' AND ';
        }
        if (!empty($dataA)) {
            $sql .= 'id IN';
            $sql .= '(SELECT chamado ';
            $sql .= 'FROM estado ';
            $sql .= 'WHERE data >= \'' . $dataA . '\' AND ';
            $sql .= 'data < \'' . $dataB . '\' + INTERVAL 1 DAY)';
            $sql .= ' AND ';
        }
        if ($nivel > 0) {
            // SELECIONA SOMENTE CHAMADOS EM QUE O ULTIMO ESTADO REPRESENTA UM ESTADO DE NIVEL $nivel E QUE PODEM SER ATENDIDAS PELO TECNICO LOGADO
            $sql .= 'id IN ';
            $sql .= '(SELECT chamado ';
            $sql .= 'FROM (select * from (select * from estado ORDER BY data DESC) es GROUP BY chamado) es ';
            // INVERTE A LISTA DE ESTADOS PARA O GROUP BY PEGAR A ULTIMA INSERSÃO
            $sql .= "WHERE nivel = {$nivel} AND (tecnico_cpf is null or tecnico_cpf = '{$_SESSION['cpf']}') ";
            $sql .= 'GROUP BY chamado)';
            if ($restringirtecnico) {
                $sql .= ' AND (';
                foreach ($_SESSION['areas'] as $aid => $anome) {
                    $sql .= "area = '{$aid}' OR ";
                }
                $sql = trim($sql, ' OR ');
                $sql .= ')';
            }
            $sql .= ' AND ';
        }
        if (!empty($tipos)) {
            $sql .= 'id IN ';
            $sql .= '(SELECT chamado ';
            $sql .= 'FROM (select * from (select * from estado ORDER BY data DESC) es GROUP BY chamado) es ';
            // INVERTE A LISTA DE ESTADOS PARA O GROUP BY PEGAR A ULTIMA INSERSÃO
            $sql .= 'WHERE (';
            foreach ($tipos as $tipo) {
                $sql .= "tipo = {$tipo} OR ";
            }
            $sql = trim($sql, ' OR ');
            $sql .= ') GROUP BY chamado)';
        }
        $sql = trim($sql, ' AND ');
        list($sortby, $sortdirection) = explode('-', $sort);
        $this->debug = $sql;
        $rs = $chamado->getData($sql, array($sortby => $sortdirection));
        $data = $rs;
        $usuarios = array();
        $areas = array();
        $problemas = array();
        foreach ($data as &$os) {
            $os['estados'] = $estado->getData('chamado = \'' . $os['id'] . '\'', array('data' => 'DESC'));
            if (!isset($problemas[$os['problema']])) {
                $rs = $problema->getData('id = \'' . $os['problema'] . '\'');
                $problemas[$os['problema']] = isset($rs[0]) ? $rs[0]['problema'] : 'Não informado';
            }
            $os['problema'] = $problemas[$os['problema']];
            if (!isset($usuarios[$os['usuario_cpf']])) {
                $usuarios[$os['usuario_cpf']] = ($tmp = $usuario->getData('cpf  = \'' . $os['usuario_cpf'] . '\'')) ? $tmp[0]['nome'] : $tmp[0]['nome'];
            }
            $os['usuario'] = $usuarios[$os['usuario_cpf']];
            // Vefifica se já não buscou por essa area no banco
            if (!isset($areas[$os['area']])) {
                $rs = $area->getData('id  = \'' . $os['area'] . '\'');
                $areas[$os['area']] = isset($rs[0]) ? $rs[0]['area'] : 'Não informado';
            }
            // Armazena área como array (contento o nome em string, não só id)
            $os['area'] = $areas[$os['area']];
            // Converte data em padrão dia/mês/ano hora:minuto
            $os['estados'][0]['data'] = new \DateTime($os['estados'][0]['data']);
            $os['estados'][0]['data'] = $os['estados'][0]['data']->format('d/m/Y H:i');
            // Calcula feedback de priorida
            $os['corprioridade'] = $this->corPrioridade($os['prioridade']);
            if ((int) $os['prioridade'] < 20) {
                $os['nomeprioridade'] = 'Baixa';
            } elseif ((int) $os['prioridade'] < 40) {
                $os['nomeprioridade'] = 'Média';
            } elseif ((int) $os['prioridade'] < 60) {
                $os['nomeprioridade'] = 'Alta';
            } elseif ((int) $os['prioridade'] < 80) {
                $os['nomeprioridade'] = 'Muito Alta';
            } else {
                $os['nomeprioridade'] = 'Urgente';
            }
        }
        return $data;
    }

    /**
     * Retorna lista em JSON de chamados para triagem
     * Requer estar logado como triagem
     * 
     * @param array $input filtros para listagem
     * @return array arranjo vetorial com os chamados
     */
    function triagemupdate(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isTriagem()) {
            $data = $this->listaChamados(1);
            $output['md5'] = md5(serialize($data));
            if ($input['get']['md5'] == $output['md5']) {
                $output['data'] = array();
            } else {
                $output['data'] = $data;
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Retorna lista de chamados para triagem
     * Requer estar logado como triagem
     * 
     * @param array $input filtros para listagem
     * @return array arranjo vetorial com os chamados
     */
    function triagem(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isTriagem()) {
            $output['data'] = $this->listaChamados(1);
            $output['md5'] = md5(serialize($output['data']));
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Retorna lista em JSON de chamados para técnico
     * Requer estar logado como técnico
     * 
     * @param array $input filtros para listagem
     * @return array arranjo vetorial com os chamados
     */
    function tecnicoupdate(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isTecnico()) {
            $data = $this->listaChamados(2, true);
            $output['md5'] = md5(serialize($data));
            if ($input['get']['md5'] == $output['md5']) {
                $output['data'] = array();
            } else {
                $output['data'] = $data;
            }
            $output['debug'] = $this->debug;
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Retorna lista de chamados para técnico
     * Requer estar logado como técnico
     * 
     * @param array $input filtros para listagem
     * @return array arranjo vetorial com os chamados
     */
    function tecnico(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isTecnico()) {
            $output['data'] = $this->listaChamados(2, true);
            $output['md5'] = md5(serialize($output['data']));
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Retorna lista em JSON de chamados para exibição geral
     * Nenhum requisito especifico
     * 
     * @param array $input filtros para listagem
     * @return array arranjo vetorial com os chamados
     */
    function indexupdate(&$input) {
        $output = array();
        $output['pesquisa'] = isset($input['post']['pesquisa']) ? urldecode($input['post']['pesquisa']) : '';

        $filtro = '';
        if (isset($input['post']['pesquisa']) && !empty($input['post']['pesquisa'])) {
            $filtro = 'concat_ws(\'|\',id,descricao,iporigem) LIKE \'%'
                    . mysql_escape_string($output['pesquisa']) . '%\' OR '
                    . 'usuario_cpf IN (SELECT cpf FROM usuario WHERE '
                    . 'concat_ws(\'|\',cpf,nome,matricula,cgm,email,cargo,lotacao,localdetrabalho) LIKE \'%'
                    . mysql_escape_string($output['pesquisa']) . '%\') OR '
                    . 'patrimonio IN (SELECT id FROM patrimonio WHERE '
                    . 'concat_ws(\'|\',descricao,placa) LIKE \'%'
                    . mysql_escape_string($output['pesquisa']) . '%\')';
        }
        if (!isset($input['post']['tipo']) || empty($input['post']['tipo'])) {
            $input['post']['tipo'] = array();
        }
        $data = $this->listaChamados(0, false, $filtro, $input['post']['sort'], $input['post']['tipo'], $input['post']['dataA'], $input['post']['dataB']);
        $output['md5'] = md5(serialize($data));
        if ($output['md5'] != $input['post']['md5']) {
            $output['data'] = $data;
        } else {
            $output['data'] = array();
        }
        return $output;
    }

    /**
     * Retorna lista de chamados para exibição geral
     * Nenhum requisito especifico
     * 
     * @param array $input filtros para listagem
     * @return array arranjo vetorial com os chamados
     */
    function index(&$input) {
        $output = array();
        $output['pesquisa'] = isset($input['args']['b']) ? urldecode($input['args']['b']) : '';
        $filtro = '';
        if (isset($input['args']['b']) && !empty($input['args']['b'])) {
            $filtro = 'concat_ws(\'|\',id,descricao,iporigem) LIKE \'%'
                    . mysql_escape_string($output['pesquisa']) . '%\' OR '
                    . 'usuario_cpf IN (SELECT cpf FROM usuario WHERE '
                    . 'concat_ws(\'|\',cpf,nome,matricula,cgm,email,cargo,lotacao,localdetrabalho) LIKE \'%'
                    . mysql_escape_string($output['pesquisa']) . '%\') OR '
                    . 'patrimonio IN (SELECT id FROM patrimonio WHERE '
                    . 'concat_ws(\'|\',descricao,placa) LIKE \'%'
                    . mysql_escape_string($output['pesquisa']) . '%\')';
        }
        if (isset($input['get']['dataA']) && $input['get']['dataB']) {
            $output['data'] = $this->listaChamados(0, false, $filtro, 'prioridade-desc', array(1, 2, 5), $input['get']['dataA'], $input['get']['dataB']);
            $output['dataA'] = $input['get']['dataA'];
            $output['dataB'] = $input['get']['dataB'];
        } else {
            $dataB = date('Y-m-d');
            $dataA = date('Y-m-d', strtotime('-1 month', strtotime($dataB)));
            $output['data'] = $this->listaChamados(0, false, $filtro, 'prioridade-desc', array(1, 2, 5), $dataA, $dataB);
        }
        $output['md5'] = md5(serialize($output['data']));
        return $output;
    }

}

class AreaController {

    /**
     * Recebe um id e um nome, então altera o nome de uma Área dado este id. Não permite repetição de nomes, assim como nomes em branco.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function alterar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $area = new model\Area();
            if (isset($input['post']['id']) && isset($input['post']['area']) && isset($input['post']['nivel'])) {
                $rs = $area->getData('area LIKE \'' . $input['post']['area'] . '\' AND id != \'' . $input['post']['id'] . '\'');
                if (empty($input['post']['area'])) {
                    $output['mensagem'] = 'Erro! nome do modulo não pode ser em branco!';
                } else {
                    if (isset($rs[0])) {
                        $output['mensagem'] = 'Erro! Já existe outra área com esse nome!';
                    } else {
                        $area->updateRecord($input['post']);
                        $output['ok'] = true;
                    }
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Recebe um nome e nível, então cadastra nova área na base local. Não permite repetição de nomes, assim como nomes em branco.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function cadastro(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $area = new model\Area();
            if (isset($input['post']['area']) && isset($input['post']['nivel'])) {
                $rs = $area->getData('area = \'' . $input['post']['area'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! Já existe outra área com esse nome!';
                } else {
                    $area->insertRecord($input['post']);
                    $output['ok'] = true;
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * lista dados de áreas da base local.
     * Requer estar logado com usuário nível 1 +
     * 
     * @param array $input
     * @return array
     */
    function listar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $area = new model\Area();
            $rs = $area->getData();
            $output['data'] = $rs;
            if (empty($rs)) {
                $output['mensagem'] = array();
                $output['mensagem']['tipo'] = 'highlight';
                $output['mensagem']['icone'] = 'notice';
                $output['mensagem']['titulo'] = 'Vazio:';
                $output['mensagem']['texto'] = 'Não foi possivel localizar nenhum resuldado dos dados solicitados';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Remove uma área da base local de dados.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function remove(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $area = new model\Area();
            if (isset($input['args']['id'])) {
                $area->deleteRecord($input['args']);
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

}

class ModuloController {

    /**
     * Recebe um id, uma área e um nome, então altera o nome de um Modulo dado este id nessa área. Não permite repetição de nomes, assim como nomes em branco.
     * Requer estar logado técnico com a área correspondente
     * 
     * @param array $input
     * @return array
     */
    function alterar(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $modulo = new model\Modulo();
            if (isset($input['post']['id']) && isset($input['post']['modulo']) && isset($input['post']['area'])) {
                if (!isset($_SESSION['areas'][$input['post']['area']])) {
                    $output['mensagem'] = 'Erro! Você não pode editar a Área solicitada';
                    return $output;
                }
                $rs = $modulo->getData('modulo LIKE \'' . $input['post']['modulo'] . '\' AND id != \'' . $input['post']['id'] . '\' AND area = \'' . $input['post']['area'] . '\'');
                if (empty($input['post']['modulo'])) {
                    $output['mensagem'] = 'Erro! nome do modulo não pode ser em branco!';
                } else {
                    if (isset($rs[0])) {
                        $output['mensagem'] = 'Erro! Já existe outro modulo com esse nome!';
                    } else {
                        $modulo->updateRecord($input['post']);
                        $output['ok'] = true;
                    }
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Recebe uma área e um nome então cadastra novo modulo na base local relacionado com a respctive área.
     * Não permite repetição de nomes, assim como nomes em branco.
     * Requer estar logado técnico com a área correspondente
     * 
     * @param array $input
     * @return array
     */
    function cadastro(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $modulo = new model\Modulo();
            if (isset($input['post']['modulo']) && isset($input['post']['area'])) {
                if (!isset($_SESSION['areas'][$input['post']['area']])) {
                    $output['mensagem'] = 'Erro! Você não pode editar a Área solicitada';
                    return $output;
                }
                $rs = $modulo->getData('modulo LIKE \'' . $input['post']['modulo'] . '\' AND area = \'' . $input['post']['area'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! Já existe outro modulo com esse nome!';
                } else {
                    $modulo->insertRecord($input['post']);
                    $output['ok'] = true;
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * lista dados de modulos da base local.
     * Requer estar logado técnico com a área correspondente
     * 
     * @param array $input
     * @return array
     */
    function listar(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $modulo = new model\Modulo();
            $area = new model\Area();
            if (isset($input['args']['area'])) {
                $rs = $area->getData('id = ' . $input['args']['area']);
                if (empty($rs)) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'notice';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'A Área solicitada não existe!';
                    $output['area'] = 'Desconhecida';
                    return $output;
                }
                $output['area'] = $rs[0];
                if (!isset($_SESSION['areas'][$input['args']['area']])) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'notice';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Você não pode editar a Área solicitada';
                    return $output;
                }
                $rs = $modulo->getData('area = ' . $input['args']['area']);
                if (empty($rs)) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'highlight';
                    $output['mensagem']['icone'] = 'notice';
                    $output['mensagem']['titulo'] = 'Vazio:';
                    $output['mensagem']['texto'] = 'Não foi possivel localizar nenhum resuldado dos dados solicitados';
                }
                $output['data'] = $rs;
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Remove uma área da base local de dados.
     * Requer estar logado técnico com a área correspondente
     * 
     * @param array $input
     * @return array
     */
    function remove(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $modulo = new model\Modulo();
            if (isset($input['args']['id'])) {
                $rs = $modulo->getData('id = \'' . $input['args']['id'] . '\'');
                if (empty($rs)) {
                    $output['mensagem'] = 'Erro! Módulo solicitado não existe!';
                    return $output;
                }
                if (!isset($_SESSION['areas'][$rs[0]['area']])) {
                    $output['mensagem'] = 'Erro! Você não pode editar Módulos a Área correspondente';
                    return $output;
                }
                $modulo->deleteRecord($input['args']);
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

}

class ProblemaController {

    /**
     * Recebe um id, um módulo, um nome e uma dica, então altera o nome e dica de um Problema dado este id nesse módulo. Não permite repetição de nomes, assim como nomes em branco.
     * Requer estar logado técnico com o módulo correspondente
     * 
     * @param array $input
     * @return array
     */
    function alterar(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $problema = new model\Problema();
            $modulo = new model\Modulo();
            if (isset($input['post']['id']) && isset($input['post']['problema']) && isset($input['post']['dica']) && isset($input['post']['modulo'])) {
                $rs = $modulo->getData('id = \'' . $input['post']['modulo'] . '\'');
                if (!isset($_SESSION['areas'][$rs[0]['area']])) {
                    $output['mensagem'] = 'Erro! Você não pode editar o Módulo solicitado';
                    return $output;
                }
                $rs = $problema->getData('problema LIKE \'' . $input['post']['problema'] . '\' AND id != \'' . $input['post']['id'] . '\' AND modulo = \'' . $input['post']['modulo'] . '\'');
                if (empty($input['post']['problema'])) {
                    $output['mensagem'] = 'Erro! nome do problema não pode ser em branco!';
                } else {
                    if (isset($rs[0])) {
                        $output['mensagem'] = 'Erro! Já existe outro problema com esse nome!';
                    } else {
                        $problema->updateRecord($input['post']);
                        $output['ok'] = true;
                    }
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Recebe um modulo, um nome e dica, então cadastra novo problema na base local relacionado ao respectivo modulo.
     * Não permite repetição de nomes, assim como nomes em branco.
     * Requer estar logado técnico com a módulo correspondente
     * 
     * @param array $input
     * @return array
     */
    function cadastro(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $problema = new model\Problema();
            $modulo = new model\Modulo();
            if (isset($input['post']['problema']) && isset($input['post']['modulo'])) {
                $rs = $modulo->getData('id = \'' . $input['post']['modulo'] . '\'');
                if (!isset($_SESSION['areas'][$rs[0]['area']])) {
                    $output['mensagem'] = 'Erro! Você não pode editar o Módulo solicitado';
                    return $output;
                }
                $rs = $problema->getData('problema LIKE \'' . $input['post']['problema'] . '\' AND modulo = \'' . $input['post']['modulo'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! Já existe outro problema com esse nome!';
                } else {
                    $problema->insertRecord($input['post']);
                    $output['ok'] = true;
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * lista dados de problemas da base local dado um módulo.
     * Requer estar logado técnico com a módulo correspondente
     * 
     * @param array $input
     * @return array
     */
    function listar(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $problema = new model\Problema();
            $modulo = new model\Modulo();
            if (isset($input['args']['modulo'])) {
                $rs = $modulo->getData('id = ' . $input['args']['modulo']);
                if (empty($rs)) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'notice';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'O módulo solicitado não existe!';
                    $output['modulo'] = 'Desconhecido';
                    return $output;
                }
                $output['modulo'] = $rs[0];
                if (!isset($_SESSION['areas'][$output['modulo']['area']])) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'notice';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Você não pode editar o Modulo solicitado';
                    return $output;
                }
                $rs = $problema->getData('modulo = ' . $input['args']['modulo']);
                if (empty($rs)) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'highlight';
                    $output['mensagem']['icone'] = 'notice';
                    $output['mensagem']['titulo'] = 'Vazio:';
                    $output['mensagem']['texto'] = 'Não foi possivel localizar nenhum resuldado dos dados solicitados';
                }
                $output['data'] = $rs;
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Remove um problema da base local de dados.
     * Requer estar logado técnico com a módulo correspondente
     * 
     * @param array $input
     * @return array
     */
    function remove(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $problema = new model\Problema();
            $modulo = new model\Modulo();
            if (isset($input['args']['id'])) {
                $rs = $problema->getData('id = \'' . $input['args']['id'] . '\'');
                if (empty($rs)) {
                    $output['mensagem'] = 'Erro! Problema solicitado não existe!';
                    return $output;
                }
                $rs = $modulo->getData('modulo = \'' . $rs[0]['modulo'] . '\'');
                if (!isset($_SESSION['areas'][$rs[0]['area']])) {
                    $output['mensagem'] = 'Erro! Você não pode remover esse Problema, você não tem permissão para o Módulo e Área!';
                    return $output;
                }
                $problema->deleteRecord($input['args']);
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

}

class PesquisaController {

    /**
     * Habilita acesso a página de pesquisas
     * Requer estar logado com usuário nível 1 +
     * 
     * @return string
     */
    function index() {
        $output = array();
        if (!\Login::isLogedIn() || !\Login::isDti()) {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

}

class PatrimonioController {

    /**
     * Recebe um id, e atributos de patrimônio, então altera os atribudos de um Patrimônio dado este id.
     * Não permite repetição de placas.
     * Não permite alterar id = 0 (zero)
     * Requer estar logado com usuário nível 2
     * 
     * @param array $input
     * @return array
     */
    function alterar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isTecnico()) {
            $patrimonio = new model\Patrimonio();
            if (isset($input['post']['id']) && isset($input['post']['placa']) && isset($input['post']['descricao']) && isset($input['post']['observacoes'])) {
                if ($input['args']['id'] == 0) {
                    $output['error'] = 'notAllowed';
                    return $output;
                }
                $rs = $patrimonio->getData('id = \'' . $input['post']['id'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! O patrimônio a ser editado não existe!';
                    return $output;
                }
                $rs = $patrimonio->getData('placa LIKE \'' . $input['post']['placa'] . '\' AND id != \'' . $input['post']['id'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! Já existe outro patrimônio com essa placa!';
                } else {
                    $patrimonio->updateRecord($input['post']);
                    $output['ok'] = true;
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Recebe um id, e atributos de patrimônio, então insere novo patrimônio na base local com os atribudos passados.
     * Requer estar logado com usuário nível 2
     * 
     * @param array $input
     * @return array
     */
    function cadastro(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isTecnico()) {
            $patrimonio = new model\Patrimonio();
            if (isset($input['post']['placa']) && isset($input['post']['descricao']) && isset($input['post']['observacoes'])) {
                $rs = $patrimonio->getData('placa LIKE \'' . $input['post']['placa'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! Já existe outro patrimônio com essa placa!';
                } else {
                    $patrimonio->insertRecord($input['post']);
                    $output['ok'] = true;
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * lista patrimonios buscados na base local.
     * Requer estar logado
     * oculda resultado id = 0 (zero)
     * 
     * @param array $input
     * @return array
     */
    function json(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $patrimonio = new model\Patrimonio();
            $patrimonio->setRowsPerPage(9);
            if (isset($input['get']['b']) && !empty($input['get']['b'])) {
                $output = $patrimonio->getData('placa LIKE \'%' . mysql_escape_string($input['get']['b']) . '%\' AND id != 0');
            }
            $output[] = array('id' => 0, 'placa' => '????', 'descricao' => 'Não Cadatrado', 'obeservacoes' => '');
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * lista dados de patrimonios da base local.
     * Requer estar logado com usuário nível 1 +
     * oculda resultado id = 0 (zero)
     * 
     * @param array $input
     * @return array
     */
    function listar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $page = isset($input['args']['p']) ? $input['args']['p'] : 1;
            $patrimonio = new model\Patrimonio();
            $patrimonio->setPageNo($page);
            $output['pesquisa'] = isset($input['args']['b']) ? urldecode($input['args']['b']) : '';
            $filtro = isset($input['args']['b']) ? 'concat_ws(\'|\',id,placa,descricao,observacoes) LIKE \'%' . mysql_escape_string($output['pesquisa']) . '%\' AND ' : '';
            $rs = $patrimonio->getData($filtro . 'id != 0');
            $output['data'] = $rs;
            if (empty($rs)) {
                $output['mensagem'] = array();
                $output['mensagem']['tipo'] = 'highlight';
                $output['mensagem']['icone'] = 'notice';
                $output['mensagem']['titulo'] = 'Vazio:';
                $output['mensagem']['texto'] = 'Não foi possivel localizar nenhum resuldado dos dados solicitados';
            }
            $output['pageno'] = $page;
            $output['lastpage'] = $patrimonio->getNoPages();
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Remove um patrimônio da base local de dados.
     * Requer estar logado com usuário nível 2
     * Não permite alterar id = 0 (zero)
     * 
     * @param array $input
     * @return array
     */
    function remove(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isTecnico()) {
            $patrimonio = new model\Patrimonio();
            if (isset($input['args']['id'])) {
                if ($input['args']['id'] == 0) {
                    $output['error'] = 'notAllowed';
                    return $output;
                }
                $rs = $patrimonio->getData('id = \'' . $input['post']['id'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! O patrimônio a ser removido não existe!';
                } else {
                    $patrimonio->deleteRecord($input['args']);
                }
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    function seletor(&$input) {
        $output = array();
        return $output;
    }

}

class SetorController {

    /**
     * Recebe um id e um nome, então altera o nome de um Setor dado este id. Não permite repetição de nomes, assim como nomes em branco.
     * Recebe também um identificador de referencia a Secretaria relacionada e um booleano para escola.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function alterar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $setor = new model\Setor();
            if (isset($input['post']['id']) && isset($input['post']['setor']) && isset($input['post']['secretaria']) && isset($input['post']['escola'])) {
                $rs = $setor->getData('setor LIKE \'' . $input['post']['setor'] . '\' AND id != \'' . $input['post']['id'] . '\' AND secretaria = \'' . $input['post']['secretaria'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! Já existe outro setor com esse nome nessa secretaria!';
                } else {
                    $setor->updateRecord($input['post']);
                    $output['ok'] = true;
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Recebe um nome e um id, então cadastra nova setor na base local com esse nome e id unico. Não permite repetição de nomes, assim como nomes em branco,
     * o id deve ser um número inteiro positivo não repetido.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function cadastro(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $setor = new model\Setor();
            if (isset($input['post']['setor']) && isset($input['post']['secretaria']) && isset($input['post']['escola'])) {
                $rs = $setor->getData('setor LIKE \'' . $input['post']['setor'] . '\' AND secretaria = \'' . $input['post']['secretaria'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! Já existe outra setor com esse nome!';
                } else {
                    $setor->insertRecord($input['post']);
                    $output['ok'] = true;
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * retorna somente a lista de setores da secretaria indicada
     * Requer estar logado
     * 
     * @param type $input deve conter a secretaria
     */
    function json($input) {
        $output = array();
        if (\Login::isLogedIn()) {
            if (isset($input['get']['sec'])) {
                $output['secretaria'] = $input['get']['sec'];
                $setor = new model\Setor();
                $setor->setRowsPerPage(0);
                $output['setores'] = $setor->getData('secretaria = ' . $input['get']['sec']);
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * lista dados de setors da base local.
     * Requer estar logado com usuário nível 1 +
     * 
     * @param array $input
     * @return array
     */
    function listar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $page = isset($input['args']['p']) ? $input['args']['p'] : 1;
            $setor = new model\Setor();
            $secretaria = new model\Secretaria();
            $setor->setPageNo($page);
            $output['pesquisa'] = isset($input['args']['b']) ? urldecode($input['args']['b']) : '';
            $output['filtro'] = isset($input['args']['f']) ? $input['args']['f'] : '';
            $filtro = '';
            $filtro .= isset($input['args']['b']) ? 'setor LIKE \'%' . mysql_escape_string($output['pesquisa']) . '%\'' : '';
            $filtro .= isset($input['args']['b']) && isset($input['args']['f']) ? ' AND ' : '';
            $filtro .= isset($input['args']['f']) ? 'secretaria = \'' . $output['filtro'] . '\'' : '';
            $rs = $secretaria->getData();
            $output['secretarias'] = array();
            foreach ($rs as $data) {
                $output['secretarias'][$data['id']] = $data['secretaria'];
            }
            if (empty($rs)) {
                $output['mensagem'] = array();
                $output['mensagem']['tipo'] = 'highlight';
                $output['mensagem']['icone'] = 'notice';
                $output['mensagem']['titulo'] = 'Vazio:';
                $output['mensagem']['texto'] = 'Não foi possivel localizar nenhum Setor para o filtro';
            }
            $rs = $setor->getData($filtro, array('id' => 'ASC'));
            $output['data'] = $rs;
            if (empty($rs)) {
                $output['mensagem'] = array();
                $output['mensagem']['tipo'] = 'highlight';
                $output['mensagem']['icone'] = 'notice';
                $output['mensagem']['titulo'] = 'Vazio:';
                $output['mensagem']['texto'] = 'Não foi possivel localizar nenhum resuldado dos dados solicitados';
            }
            $output['pageno'] = $page;
            $output['lastpage'] = $setor->getNoPages();
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Remove um setor da base local de dados.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function remove(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $setor = new model\Setor();
            if (isset($input['args']['id'])) {
                $setor->deleteRecord($input['args']);
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

}

/**
 * Controlador
 *
 * @author Caio Alexandre
 */
class TecnicoController {

    /**
     * Metodo simples para requição de dados de um determinado técnico
     * 
     * @param string $cpf CPF do usuário que deve ser resgatado
     * @return array ResultSet contendo os dados do usuário requisitado, ou null caso não econtrado
     */
    function getByCPF($cpf) {
        $tecnico = new model\Tecnico();
        $rs = $tecnico->getData("cpf = {$cpf}");
        return isset($rs[0]) ? $rs[0] : null;
    }

    /**
     * Processa a requisição de uma página de acesso no sistema de um técnico e verifica
     * se existe uma requisição de login de um técnico, é passado por metodo $input['post']
     * os valores de cgm ou matricula, cpf e senha que são validadeos com a base local,
     * então autorizado ou negado login no sistema. A senha é armazenada como md5.
     * Requer estar deslogado
     * 
     * @param array $input Entrada de dados
     * @return array Saida de dados
     */
    function acesso(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $output['mensagem'] = array();
            $output['mensagem']['tipo'] = 'error';
            $output['mensagem']['icone'] = 'alert';
            $output['mensagem']['titulo'] = 'Erro:';
            $output['mensagem']['texto'] = 'Você já está logado no sistema';
        } else {
            if (isset($input['post']['cpf']) && isset($input['post']['senha'])) {
                $input['post']['cpf'] = str_replace(array('.', '-'), '', $input['post']['cpf']);
                // limpa o CPF de marcadores
                $tecnico = new \model\Tecnico();
                $where = "cpf = '{$input['post']['cpf']}' AND senha = md5('{$input['post']['senha']}')";
                // Resgada dados do tecnico no banco local
                $rs = $tecnico->getData($where, array(), array('cpf', 'telefone'));
                // Se existe um tecnico com os dados correspondetens CPF e senha
                if (isset($rs[0])) {
                    // Armazena dados do tecnico em variavel temporaria
                    $tecnico_data = $rs[0];
                    // Parte do nível 1 (mesmo sem area definida)
                    $tecnico_data['nivel'] = 1;
                    $tecnico_data['triagem'] = 0;
                    $tecnico_data['areas'] = array();
                    $usuario = new \model\Usuario();
                    // Armazena dados de usuario em vairiavel temporaria
                    $rs = $tmp = $usuario->getData("cpf = '{$input['post']['cpf']}'");
                    $usuario_data = $rs[0];
                    $areastecnico = new \model\AreasTecnico();
                    $area = new \model\Area();
                    // Armazena areas desse tecnico em variavel temporaria
                    $tecnico_areas = $areastecnico->getData("tecnico_cpf = '{$input['post']['cpf']}'", array(), array('area_id'));
                    foreach ($tecnico_areas as $tecnico_area) {
                        // Armazena dados de uma area em variavel temporaria
                        $rs = $area->getData("id = {$tecnico_area['area_id']}");
                        $area_data = $rs[0];
                        // Adiciona uma nova area às areas do tecnico
                        $tecnico_data['areas'][$tecnico_area['area_id']] = $area_data['area'];
                        // Verifica o nível de acesso que o usuario tera baseado na area (se tem area de nível 3 será nível 3)
                        $tecnico_data['nivel'] = max($tecnico_data['nivel'], $area_data['nivel']);
                        // Adiciona permições de triagem se tiver a Área de triagem
                        if ($tecnico_area['nivel'] == 1 || $tecnico_area['nivel'] == 3) {
                            $tecnico_data['triagem'] = 1;
                        }
                    }
                    // Junta os dados da tabela técnico com a de usuário
                    $merge = array_merge($usuario_data, $tecnico_data);
                    // Salva dados de tecnico na sessao
                    \Login::loginTecnico($merge);
                    $output['redirect'] = '/';
                } else {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Usuário não autenticado no sistema!';
                }
            }
        }
        return $output;
    }

    /**
     * Executa uma busca e retorna apenas areas ralacionadas so CPF informado
     * 
     * @param string $cpf CPF para relacionar a busca
     * @return array Lista de areas ralacionadas
     */
    function getAreasWithCPF($cpf) {
        $area = new \model\Area();
        $areas = array();
        $resultset = $area->getData("id IN (select area_id from areas_tecnico where tecnico_cpf = '{$cpf}')");
        foreach ($resultset as $data) {
            $areas[$data['id']] = array('area' => $data['area'], 'nivel' => $data['nivel']);
        }
        return $areas;
    }

    /**
     * Executa uma busca e retorna apenas areas NÃO ralacionadas so CPF informado
     * 
     * @param string $cpf CPF para relacionar a busca
     * @return array Lista de areas NÃO ralacionadas
     */
    function getAreasWithoutCPF($cpf) {
        $area = new \model\Area();
        $areas = array();
        $resultset = $area->getData("id NOT IN (select area_id from areas_tecnico where tecnico_cpf = '{$cpf}')");
        foreach ($resultset as $data) {
            $areas[$data['id']] = array('area' => $data['area'], 'nivel' => $data['nivel']);
        }
        return $areas;
    }

    /**
     * Retorna uma lista de areas realacionadas e não relacionadas a um determinado CPF.
     * E também atualiza os dados de um determinado usuario.
     * Requer estar logado com usuário nível 3
     * 
     * 
     * @param array $input Entrada de dados. Aceita uma entrada ['args']['cpf'] para listagem e um ['post']['areas'] para alteração
     * @return array Saida de dados.
     * <br>$output['data']['tecnico']['areas'] < lista de areas relacionadas ao CPF
     * <br>$output['data']['areas'] < losta de areas não relacionadas ao CPF
     */
    function areas(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            if (isset($input['args']['cpf'])) {
                $cpf = $input['args']['cpf'];
                $areatecnico = new \model\AreasTecnico();
                // Lista de todas as áreas as quais o técnico não possui relacionamento
                $output['data'] = array();
                $output['data']['areas'] = $this->getAreasWithoutCPF($cpf);
                // Lista de todas as áreas as quais o técnico possui privilegios
                $output['data']['tecnico'] = array('cpf' => $cpf);
                $output['data']['tecnico']['areas'] = $this->getAreasWithCPF($cpf);
                // Se está adicionando ou removendo área
                if (isset($input['post']['areas'])) {
                    // Pra cada área adicionada
                    foreach ($input['post']['areas'] as $area) {
                        // Inserir relação
                        $areatecnico->insertRecord(array('tecnico_cpf' => $cpf, 'area_id' => $area));
                        // Corrige, envia área pra lista correta
                        $output['data']['tecnico']['areas'][$area] = $output['data']['areas'][$area];
                        unset($output['data']['areas'][$area]);
                    }
                }
                if (isset($input['post']['remover'])) {
                    foreach ($input['post']['remover'] as $area) {
                        // Remover relação
                        $areatecnico->deleteRecord(array('tecnico_cpf' => $cpf, 'area_id' => $area));
                        // Corrige, envia área pra lista correta
                        $output['data']['areas'][$area] = $output['data']['tecnico']['areas'][$area];
                        unset($output['data']['tecnico']['areas'][$area]);
                    }
                }
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Processa a requisição de uma página de cadastro de um técnico e verifica se
     * um cadastro está sendo requisitado através do $input['post'],
     * então cria o cadastro ou exibe mensagens adequadas.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input Entrada de dados
     * @return array Saida de dados
     */
    function cadastro(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            if (isset($input['post']['cpf'])) {
                $tecnico = new model\Tecnico();
                $input['post']['cpf'] = str_replace(array('.', '-'), '', $input['post']['cpf']);
                // limpa o CPF de marcadores
                if (($tmp = $tecnico->getData('cpf = ' . $input['post']['cpf'])) ? isset($tmp[0]) : isset($tmp[0])) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Usuário já é um técnico!';
                    return $output;
                }
                $usuariocontroller = new UsuarioController();
                $usuario = $usuariocontroller->getByCPF($input['post']['cpf']);
                if ($usuario == null) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Usuário não cadastrado no sistema!';
                } else {
                    $input['post']['senha'] = md5('dti');
                    $tecnico->insertRecord($input['post']);
                    $output['redirect'] = '/v/tecnico/areas/cpf/' . $input['post']['cpf'];
                }
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Resgata os dados de determinado técnico ou do próprio usuário logado.
     * Requer estar logado com usuário nível 1 +
     * 
     * @param array $input Entrada de dados
     * @return array Saida de dados
     */
    function info(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $cpf = isset($input['args']['cpf']) ? $input['args']['cpf'] : $_SESSION['cpf'];
            if (\Login::isDti() || $cpf == $_SESSION['cpf']) {
                $tecnico = $this->getByCPF($cpf);
                if ($tecnico == null) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Técnico não cadastrado na base local';
                } else {
                    $usuariocontroller = new UsuarioController();
                    $usuario = $usuariocontroller->getByCPF($cpf);
                    $tecnico['areas'] = $this->getAreasWithCPF($cpf);
                    $output['data'] = array_merge($usuario, $tecnico);
                }
            } else {
                $output['error'] = 'notAllowed';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * lista dados de usuários técnicos da base local.
     * Requer estar logado com usuário nível 1 +
     * 
     * @param type $input
     * @return string
     */
    function listar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $page = isset($input['args']['p']) ? $input['args']['p'] : 1;
            $usuario = new model\Usuario();
            $usuario->setPageNo($page);
            $output['pesquisa'] = isset($input['args']['b']) ? urldecode($input['args']['b']) : '';
            $filtro = isset($input['args']['b']) ? 'AND concat_ws(\'|\',cpf,nome,matricula,cgm,email,cargo,lotacao,localdetrabalho,ip) LIKE \'%' . mysql_escape_string($output['pesquisa']) . '%\'' : '';
            $rs = $usuario->getData("cpf in (select cpf from tecnico) {$filtro}");
            $output['data'] = $rs;
            if (empty($rs)) {
                $output['mensagem'] = array();
                $output['mensagem']['tipo'] = 'highlight';
                $output['mensagem']['icone'] = 'notice';
                $output['mensagem']['titulo'] = 'Vazio:';
                $output['mensagem']['texto'] = 'Não foi possivel localizar nenhum resuldado dos dados solicitados';
            }
            $output['pageno'] = $page;
            $output['lastpage'] = $usuario->getNoPages();
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Remove os atributos de técnico de um determinado usuário.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function remove(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $tecnico = new model\Tecnico();
            if (isset($input['args']['cpf'])) {
                $tecnico->deleteRecord($input['args']);
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Altera a senha do usuário que está logado no sistema.
     * Requer estar logado como um técnico com senha.
     * 
     * @param type $input
     * @return array
     */
    function senha(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            if (isset($input['post']['antiga']) && isset($input['post']['nova1']) && isset($input['post']['nova2'])) {
                if ($input['post']['nova1'] === $input['post']['antiga']) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'A senha nova é igual a senha antiga';
                    return $output;
                }
                if ($input['post']['nova1'] !== $input['post']['nova2']) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Os campos de nova senha não conferem';
                    return $output;
                }
                $cpf = $_SESSION['cpf'];
                $tecnico = new \model\Tecnico();
                $rs = $tecnico->getData("cpf = '{$cpf}' AND senha = md5('" . $input['post']['antiga'] . '\')');
                // Verifica se a senha atual bate com a senha digitada
                if (!isset($rs[0])) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Senha Antiga incorreta';
                    return $output;
                }
                // Altera senha (em md5 hash)
                $tecnico->updateRecord(array('cpf' => $cpf, 'senha' => md5($input['post']['nova1'])));
                \Login::logout();
                $output['redirect'] = '/v/tecnico/acesso';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

}

class SecretariaController {

    /**
     * Recebe um id e um nome, então altera o nome de uma Secretaria dado este id. Não permite repetição de nomes, assim como nomes em branco.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function alterar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $secretaria = new model\Secretaria();
            if (isset($input['post']['id']) && isset($input['post']['secretaria'])) {
                $rs = $secretaria->getData('secretaria LIKE \'' . $input['post']['secretaria'] . '\' AND id != \'' . $input['post']['id'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! Já existe outra secretaria com esse nome!';
                } else {
                    $secretaria->updateRecord($input['post']);
                    $output['ok'] = true;
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Recebe um nome e um id, então cadastra nova secretaria na base local com esse nome e id unico. Não permite repetição de nomes, assim como nomes em branco,
     * o id deve ser um número inteiro positivo não repetido.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function cadastro(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $secretaria = new model\Secretaria();
            if (isset($input['post']['secretaria']) && isset($input['post']['id'])) {
                $rs = $secretaria->getData('secretaria LIKE \'' . $input['post']['secretaria'] . '\'');
                if (isset($rs[0])) {
                    $output['mensagem'] = 'Erro! Já existe outra secretaria com esse nome!';
                } else {
                    $rs = $secretaria->getData('id =  ' . $input['post']['id'] . '');
                    if (isset($rs[0])) {
                        $output['mensagem'] = 'Erro! Já existe outra secretaria com esse id!';
                    } else {
                        $secretaria->insertRecord($input['post']);
                        $output['ok'] = true;
                    }
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * lista dados de secretarias da base local.
     * Requer estar logado com usuário nível 1 +
     * 
     * @param array $input
     * @return array
     */
    function listar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $page = isset($input['args']['p']) ? $input['args']['p'] : 1;
            $secretaria = new model\Secretaria();
            $secretaria->setPageNo($page);
            $output['pesquisa'] = isset($input['args']['b']) ? urldecode($input['args']['b']) : '';
            $filtro = isset($input['args']['b']) ? 'secretaria LIKE \'%' . mysql_escape_string($output['pesquisa']) . '%\'' : '';
            $rs = $secretaria->getData($filtro);
            $output['data'] = $rs;
            if (empty($rs)) {
                $output['mensagem'] = array();
                $output['mensagem']['tipo'] = 'highlight';
                $output['mensagem']['icone'] = 'notice';
                $output['mensagem']['titulo'] = 'Vazio:';
                $output['mensagem']['texto'] = 'Não foi possivel localizar nenhum resuldado dos dados solicitados';
            }
            $output['pageno'] = $page;
            $output['lastpage'] = $secretaria->getNoPages();
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Remove uma secretaria da base local de dados.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function remove(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $secretaria = new model\Secretaria();
            if (isset($input['args']['id'])) {
                $secretaria->deleteRecord($input['args']);
            } else {
                $output['error'] = 'notFound';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

}

/**
 * Controlador
 *
 * @author Caio Alexandre
 */
class UsuarioController {

    /**
     * Metodo simples para requição de dados de um determinado usuário
     * 
     * @param string $cpf CPF do usuário que deve ser resgatado
     * @return array ResultSet contendo os dados do usuário requisitado, ou null caso não econtrado
     */
    function getByCPF($cpf) {
        $usuario = new model\Usuario();
        $rs = $usuario->getData("cpf = {$cpf}");
        return isset($rs[0]) ? $rs[0] : null;
    }

    /**
     * Metodo simples para requição de dados de um determinado usuário
     * 
     * @param string $cgm CGM do usuário que deve ser resgatado
     * @return array ResultSet contendo os dados do usuário requisitado, ou null caso não econtrado
     */
    function getByCGM($cgm) {
        $usuario = new model\Usuario();
        $rs = $usuario->getData("cgm = {$cgm}");
        return isset($rs[0]) ? $rs[0] : null;
    }

    /**
     * Metodo simples para requição de dados de um determinado usuário
     * 
     * @param string $matricula Matricula do usuário que deve ser resgatado
     * @return array ResultSet contendo os dados do usuário requisitado, ou null caso não econtrado
     */
    function getByMatricula($matricula) {
        $usuario = new model\Usuario();
        $rs = $usuario->getData("matricula = {$matricula}");
        return isset($rs[0]) ? $rs[0] : null;
    }

    /**
     * lista 4 usuarios buscados na base local.
     * Requer estar logado
     * 
     * @param array $input
     * @return array
     */
    function json(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $b = mysql_escape_string($input['get']['b']);
            $where = 'cgm_nome ILIKE \'%' . $b . '%\''
                    . ' OR fun_matricula LIKE \'%' . $b . '%\''
                    . ' OR cgm_cpf LIKE \'%' . $b . '%\''
                    . ' OR cgm_id LIKE \'%' . $b . '%\'';
            $ecidade = new \database\ECidade();
            $rs = $ecidade->getData('public.v_cgm INNER JOIN public.v_funcionario ON (v_cgm.cgm_cpf = v_funcionario.fun_cpf)', $where, 'LIMIT 10');
            foreach ($rs as $row) {
                $usuario = array();
                $usuario['cgm'] = $row['cgm_id'];
                $usuario['nome'] = $row['cgm_nome'];
                $usuario['matricula'] = $row['fun_matricula'];
                $output[] = $usuario;
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Executa logout de sessão, remove dados da sessão existente então redireciona para página inicial
     * 
     * @return array Saida de dados
     */
    function sair() {
        $output = array();
        \Login::logout();
        $output['redirect'] = '/';
        return $output;
    }

    /**
     * Processa a requisição de uma página de acesso no sistema de um usuário padrão e verifica
     * se existe uma requisição de login de um usuário, é passado por metodo $input['post']
     * os valores de cgm ou matricula e cpf que são validadeos com a base local,
     * então autorizado ou negado login no sistema.
     * Requer estar deslogado
     * 
     * @param array $input Entrada de dados
     * @return array Saida de dados
     */
    function acesso(&$input) {
        $output = array();
        if (isset($input['args']['ok'])) {
            $output['mensagem'] = array();
            $output['mensagem']['tipo'] = 'highlight';
            $output['mensagem']['icone'] = 'info';
            $output['mensagem']['titulo'] = 'Ok!';
            $output['mensagem']['texto'] = 'Você já pode fazer login';
        }
        if (\Login::isLogedIn()) {
            $output['mensagem'] = array();
            $output['mensagem']['tipo'] = 'error';
            $output['mensagem']['icone'] = 'alert';
            $output['mensagem']['titulo'] = 'Erro:';
            $output['mensagem']['texto'] = 'Você já está logado no sistema';
        } else {
            if (isset($input['post']['cpf']) && (isset($input['post']['matricula']) || isset($input['post']['cgm']))) {
                $usuario = new \model\Usuario();
                $input['post']['cpf'] = str_replace(array('.', '-'), '', $input['post']['cpf']);
                // limpa o CPF de marcadores
                $rs = $usuario->getData("(cgm = '{$input['post']['cgm']}' OR matricula = '{$input['post']['matricula']}') " . "AND cpf = '{$input['post']['cpf']}' AND cpf NOT IN (select cpf from tecnico)");
                if (isset($rs[0])) {
                    \Login::loginUsuario($rs[0]);
                    $output['redirect'] = '/v/chamados/historico';
                } else {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Usuário não autenticado no sistema!';
                }
            }
        }
        return $output;
    }

    /**
     * Permite que um técnico de nivel 3 possa alterar os dados de um determinado usuário ou técnico
     * dado um argumento de cpf de entrada para exibição de dados e um formulario post para alteração.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input Entrada de dados
     * @return array Saida de dados
     */
    function alterar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            if (isset($input['post']['cpf']) && isset($input['post']['ip']) && isset($input['post']['email'])) {
                $input['post']['cpf'] = str_replace(array('.', '-'), '', $input['post']['cpf']);
                $usuario = new \model\Usuario();
                $usuario->updateRecord($input['post']);
            }
            if (isset($input['args']['cpf'])) {
                $cpf = str_replace(array('.', '-'), '', $input['args']['cpf']);
                $usuario = $this->getByCPF($cpf);
                // Resgata dadso do usuário
                if ($usuario == null) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Usuário não cadastrado na base local';
                } else {
                    $tecnico = new TecnicoController();
                    // Verifica se não é um técnico
                    $rs = $tecnico->getByCPF($cpf);
                    $usuario['tecnico'] = $rs != null;
                    // Verdadeiro se é um técnico
                    $output['data'] = $usuario;
                }
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Processa a requisição de uma página de cadastro de um usuário e verifica se
     * um cadastro está sendo requisitado através do $input['post'], que são validados
     * com a base de dados do sistema e-Cidade e se não há redundancias na base local,
     * então cria o cadastro ou exibe mensagens adequadas.
     * Requer estar deslogado
     * 
     * @param array $input Entrada de dados
     * @return array Saida de dados
     */
    function cadastro(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $output['error'] = 'notAllowed';
            return $output;
        }
        if (isset($input['post']['cpf']) && (isset($input['post']['matricula']) || isset($input['post']['cgm'])) && isset($input['post']['email'])) {
            $input['post']['cpf'] = str_replace(array('.', '-'), '', $input['post']['cpf']);
            $usuario = new \model\Usuario();
            $input['post']['base'] = 'ecidade';
            if (empty($input['post']['cgm']) && empty($input['post']['matricula']) || $input['post']['cpf'] === '') {
                $output['mensagem'] = array();
                $output['mensagem']['tipo'] = 'error';
                $output['mensagem']['icone'] = 'alert';
                $output['mensagem']['titulo'] = 'Erro:';
                $output['mensagem']['texto'] = 'Matricula ou CGM devem ser preenchidos';
            } else {
                if (isset($input['post']['matricula']) && !empty($input['post']['matricula'])) {
                    if ($this->getByMatricula($input['post']['matricula']) !== null) {
                        $output['mensagem'] = array();
                        $output['mensagem']['tipo'] = 'error';
                        $output['mensagem']['icone'] = 'alert';
                        $output['mensagem']['titulo'] = 'Erro:';
                        $output['mensagem']['texto'] = 'Usuário já cadastrado';
                    } else {
                        if (!(($tmp = $this->matriculavalidate($input)) ? $tmp['valid'] : $tmp['valid'])) {
                            $output['mensagem'] = array();
                            $output['mensagem']['tipo'] = 'error';
                            $output['mensagem']['icone'] = 'alert';
                            $output['mensagem']['titulo'] = 'Erro:';
                            $output['mensagem']['texto'] = 'Usuário não autenticado com o sistema e-Cidade';
                        } else {
                            $usuario->insertRecord($input['post']);
                            $output['redirect'] = '/v/usuario/acesso/ok';
                        }
                    }
                } else {
                    if ($this->getByCGM($input['post']['cgm']) !== null) {
                        $output['mensagem'] = array();
                        $output['mensagem']['tipo'] = 'error';
                        $output['mensagem']['icone'] = 'alert';
                        $output['mensagem']['titulo'] = 'Erro:';
                        $output['mensagem']['texto'] = 'Usuário já cadastrado';
                    } else {
                        if (!(($tmp = $this->cgmvalidate($input)) ? $tmp['valid'] : $tmp['valid'])) {
                            $output['mensagem'] = array();
                            $output['mensagem']['tipo'] = 'error';
                            $output['mensagem']['icone'] = 'alert';
                            $output['mensagem']['titulo'] = 'Erro:';
                            $output['mensagem']['texto'] = 'Usuário não autenticado com o sistema e-Cidade';
                        } else {
                            $input['post']['ip'] = getIP();
                            $usuario->insertRecord($input['post']);
                            $output['redirect'] = '/v/usuario/acesso/ok';
                        }
                    }
                }
            }
        }
        return $output;
    }

    /**
     * Verifica se o CGM e o CPF combinam em um registro.
     * Retorna dados se usuário é valido
     * 
     * @param string $input['post']['base'] Base de dados em que se deve ser feita a validação 'local' ou 'ecidade'
     * @param string $input['post']['cpf'] CPF para ser validado
     * @param string $input['post']['cgm'] CMG para ser validado
     * @return array Saida de dados. $output['valid'] true ou false para o caso de ser validado ou não
     */
    function cgmvalidate(&$input) {
        $output = array();
        if (isset($input['post']['cgm']) && isset($input['post']['cpf']) && isset($input['post']['base'])) {
            $input['post']['cpf'] = str_replace(array('.', '-'), '', $input['post']['cpf']);
            switch ($input['post']['base']) {
                case 'local':
                    $usuario = new model\Usuario();
                    $where = "cgm = '{$input['post']['cgm']}' AND cpf = '{$input['post']['cpf']}'";
                    $rs = $usuario->getData($where);
                    break;
                case 'ecidade':
                    $ecidade = new \database\ECidade();
                    $where = "fun_cgm = '{$input['post']['cgm']}' AND fun_cpf = '{$input['post']['cpf']}'";
                    $rs = $ecidade->getData('public.v_funcionario', $where);
                    if (!isset($rs[0])) {
                        $where = "cgm_id = '{$input['post']['cgm']}' AND cgm_cpf = '{$input['post']['cpf']}'";
                        $rs = $ecidade->getData('public.v_cgm', $where);
                        if (isset($rs[0])) {
                            $rs[0]['fun_cgm'] = $rs[0]['cgm_id'];
                            $rs[0]['fun_cpf'] = $rs[0]['cgm_cpf'];
                            $rs[0]['fun_nome'] = $rs[0]['cgm_nome'];
                            $rs[0]['fun_email'] = $rs[0]['cgm_email'];
                            $rs[0]['fun_cargo'] = 'ESTAGIARIO';
                        }
                    }
                    break;
                default:
                    $rs = array();
                    break;
            }
            if (isset($rs[0])) {
                $output['valid'] = true;
                $output['data'] = $rs[0];
            } else {
                $output['valid'] = false;
            }
        } else {
            $output['error'] = 'badRequest';
        }
        return $output;
    }

    /**
     * Resgata os dados de determinado usuário ou do próprio usuário logado.
     * Requer estar logado com usuário nível 1 + ou Ser o próprio usuário
     * 
     * @param type $input['args']['cpf'] CPF para resgatar dados
     * @return array Saida de dados
     */
    function info(&$input) {
        $output = array();
        if (\Login::isLogedIn()) {
            $cpf = isset($input['args']['cpf']) ? $input['args']['cpf'] : $_SESSION['cpf'];
            if (\Login::isDti() || $cpf == $_SESSION['cpf']) {
                $usuario = $this->getByCPF($cpf);
                if ($usuario == null) {
                    $output['mensagem'] = array();
                    $output['mensagem']['tipo'] = 'error';
                    $output['mensagem']['icone'] = 'alert';
                    $output['mensagem']['titulo'] = 'Erro:';
                    $output['mensagem']['texto'] = 'Usuário não cadastrado na base local';
                } else {
                    $tecnicocontroller = new TecnicoController();
                    $tecnico = $tecnicocontroller->getByCPF($cpf);
                    if ($tecnico != null) {
                        $output['redirect'] = '/v/tecnico/info/';
                    } else {
                        $output['data'] = $usuario;
                    }
                }
            } else {
                $output['error'] = 'notAllowed';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Retorna uma lista com 10 usuários da base local
     * Requer estar logado com usuário nível 1 +
     * 
     * @param type $input['args']['p'] (opcional) número da página para exibir usuários
     * @return array Saida de dados. data=> lista de 10 usuários dada a página
     */
    function listar(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isDti()) {
            $page = isset($input['args']['p']) ? $input['args']['p'] : 1;
            $usuario = new model\Usuario();
            $usuario->setPageNo($page);
            $output['pesquisa'] = isset($input['args']['b']) ? urldecode($input['args']['b']) : '';
            $filtro = isset($input['args']['b']) ? 'AND concat_ws(\'|\',cpf,nome,matricula,cgm,email,cargo,lotacao,localdetrabalho,ip) LIKE \'%' . mysql_escape_string($output['pesquisa']) . '%\'' : '';
            $rs = $usuario->getData("cpf not in (select cpf from tecnico) {$filtro}");
            $output['data'] = $rs;
            if (empty($rs)) {
                $output['mensagem'] = array();
                $output['mensagem']['tipo'] = 'highlight';
                $output['mensagem']['icone'] = 'notice';
                $output['mensagem']['titulo'] = 'Vazio:';
                $output['mensagem']['texto'] = 'Não foi possivel localizar nenhum resuldado dos dados solicitados';
            }
            $output['pageno'] = $page;
            $output['lastpage'] = $usuario->getNoPages();
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    /**
     * Verifica se a Matricula e o CPF combinam em um registro.
     * semelhante a cmg validate
     * 
     * @param array $input Entrada de dados
     * @param string $input['post']['base'] Base de dados em que se deve ser feita a validação 'local' ou 'ecidade'
     * @param string $input['post']['cpf'] CPF para ser validado
     * @param string $input['post']['matricula'] Matricula para ser validado
     * @return array Saida de dados. $output['valid'] true ou false para o caso de ser validado ou não
     */
    function matriculavalidate(&$input) {
        $output = array();
        if (isset($input['post']['matricula']) && isset($input['post']['cpf'])) {
            $input['post']['cpf'] = str_replace(array('.', '-'), '', $input['post']['cpf']);
            switch ($input['post']['base']) {
                case 'local':
                    $usuario = new model\Usuario();
                    $where = "matricula = '{$input['post']['matricula']}' AND cpf = '{$input['post']['cpf']}'";
                    $rs = $usuario->getData($where);
                    break;
                case 'ecidade':
                    $ecidade = new \database\ECidade();
                    $where = "fun_matricula = '{$input['post']['matricula']}' AND fun_cpf = '{$input['post']['cpf']}'";
                    $rs = $ecidade->getData('public.v_funcionario', $where);
                    break;
                default:
                    $rs = array();
                    break;
            }
            if (isset($rs[0])) {
                $output['valid'] = true;
                $output['data'] = $rs[0];
            } else {
                $output['valid'] = false;
            }
        } else {
            $output['error'] = 'badRequest';
        }
        return $output;
    }

    /**
     * Remove um determinado usuário da base local, efeito CASCATE do banco removerá também chamados relacionados.
     * Requer estar logado com usuário nível 3
     * 
     * @param array $input
     * @return array
     */
    function remove(&$input) {
        $output = array();
        if (\Login::isLogedIn() && \Login::isAdmin()) {
            $usuario = new model\Usuario();
            if (isset($input['args']['cpf'])) {
                $usuario->deleteRecord($input['args']);
            } else {
                $output['error'] = 'badRequest';
            }
        } else {
            $output['error'] = 'notAllowed';
        }
        return $output;
    }

    function seletor(&$input) {
        $output = array();
        return $output;
    }

}

class Files {

    /**
     * Exclui um diretório contendo arquivos
     * (Recursivo)
     * 
     * @param string $dir Diretório a ser excluido
     */
    public static function rrmdir($dir) {
        if (is_dir($dir)) {
            $objects = scandir($dir);
            foreach ($objects as $object) {
                if ($object != '.' && $object != '..') {
                    if (filetype($dir . '/' . $object) == 'dir') {
                        Files::rrmdir($dir . '/' . $object);
                    } else {
                        unlink($dir . '/' . $object);
                    }
                }
            }
            reset($objects);
            rmdir($dir);
        }
    }

}
