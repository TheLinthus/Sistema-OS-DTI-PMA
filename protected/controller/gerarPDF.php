<?php

$urlBase = 'http://' . $_SERVER['HTTP_HOST'] . '/chamados/';
$objetoClasse = new gerarPDF();

if ($_REQUEST["funcao"] == "finalizarRelatorio") {
    $nomeRelatorio = $_REQUEST["nomeRelatorio"];
    $nomeTecnico = $_REQUEST["nomeTecnico"];
    $nomeResponsavel = $_REQUEST["nomeResponsavel"];
    $objetoClasse->finalizarRelatorio($nomeRelatorio, $nomeTecnico, $nomeResponsavel);
}

if ($_REQUEST["funcao"] == "criarArquivoRelatorio") {
    $nomeRelatorio = $_REQUEST["nomeRelatorio"];
    $arrayRecebido = $_REQUEST["array"];
    $prioridade = $_REQUEST["prioridade"];
    $estado = $_REQUEST["estado"];
    $area = $_REQUEST["area"];
    $usuario = $_REQUEST["usuario"];
    $data = $_REQUEST["data"];
    $descricao = $_REQUEST["descricao"];
    
    $objetoClasse->criarArquivoRelatorio($nomeRelatorio, $arrayRecebido, $prioridade, $estado, $area, $usuario, $data, $descricao);
}

class gerarPDF {

    function finalizarRelatorio($nomeRelatorio, $nomeTecnico, $nomeResponsavel) {
        require "C:/xampp/htdocs/chamados/dompdf/dompdf_config.inc.php";
        $nomeArquivoPDF = "Arquivo_Chamado_" . $nomeRelatorio;
        date_default_timezone_set('America/Sao_Paulo');
        $data = date("d/m/Y");
        $html = "<html><body>";
        $html .= '<table>
            <tr><td><img src="C:/xampp/htdocs/chamados/img/brasao.jpg" height="100"></td> 
                <td style="width: 633px; padding:10px; _width: 250px;"> <h2 align="center">PREFEITURA MUNICIPAL DE ALEGRETE</h2>
                    <h3 align="center">Estado do Rio Grande do Sul</h3>
                    <h3 align="center">Divisão de Tecnologia da Informação - DTI</h3>
                </td> </tr>
        </table>';
        $html .= "<br><br>";
        $html .= "_______________________ <br> Técnico: $nomeTecnico<br><br>";
        $html .= "_______________________ <br> Responsável: $nomeResponsavel<br><br><br>";
        $html .= "Alegrete, $data";
        $html .= "</body></html>";
        $dompdf = new DOMPDF();
        $dompdf->load_html($html);
        ob_clean();
        $dompdf->render();
        $dompdf->stream("$nomeArquivoPDF.pdf");
    }

    function criarArquivoRelatorio($nomeRelatorio, $arrayRecebido, $prioridade, $estado, $area, $usuario, $data, $descricao) {

        require "C:/xampp/htdocs/chamados/dompdf/dompdf_config.inc.php";

        $nomeArquivoPDF = "Arquivo_Chamado_" . $nomeRelatorio;
        date_default_timezone_set('America/Sao_Paulo');
        $data = date("d/m/Y");
        $html = "<html><body>";
        $html .= '<table>
            <tr><td><img src="C:/xampp/htdocs/chamados/img/brasao.jpg" height="100"></td> 
                <td style="width: 633px; padding:10px; _width: 250px;"> <h2 align="center">PREFEITURA MUNICIPAL DE ALEGRETE</h2>
                    <h3 align="center">Estado do Rio Grande do Sul</h3>
                    <h3 align="center">Divisão de Tecnologia da Informação - DTI</h3>
                </td> </tr>
        </table>';
        
        for ($k = 0; $k < sizeof($arrayChamado); $k++) {

                             $estado = $arrayChamado[$k]['estado']; 


                          $descricao = $arrayChamado[$k]['descricao']; 

            }
            
        $html .= 'Chamado: '.$nomeRelatorio.'<br>';
        $html .= 'Prioridade: '.$prioridade.'<br>';
        $html .= 'Estado: '.$estado.'<br>';
        $html .= 'Área: '.$area.'<br>';
        $html .= 'Usuário: '.$usuario.'<br>';
        $html .= 'Data: '.$data.'<br>';
        $html .= 'Descrição: '.$descricao.'<br>';
        $html .= "<br><br>";
        $html .= "Alegrete, $data";
        $html .= "</body></html>";
        $dompdf = new DOMPDF();
        $dompdf->load_html($html);
        ob_clean();
        $dompdf->render();
        $dompdf->stream("$nomeArquivoPDF.pdf");
    }

    function criarArquivo($stringHTML, $nomeChamado, $nomeTecnico, $nomeResponsavel) {

//       
//$this->dbname = 'sos';
//$db = new DataBase();
//$dbconnect = $db->db_connect($this->dbname) or internalServerError('SQL: ' . mysql_error());
//
//$query = "SELECT count(*) FROM {$this->tablename} {$where_str}";
//$sql = "SELECT * FROM chamado WHERE id = 491";
//
//$result = mysql_query($sql, $dbconnect) or internalServerError('SQL - ' . mysql_error());
//// trigger_error(..., E_USER_ERROR);
//$query_data = mysql_fetch_row($result);
//$this->numrows = $query_data[0];
//include '../../dompdf/dompdf_config.inc.php';

        require "/dompdf/dompdf_config.inc.php";

        echo getcwd() . "\n";

        $nomeArquivoPDF = "Arquivo_Chamado_" . $nomeChamado;
        date_default_timezone_set('America/Sao_Paulo');
        $data = date("d/m/Y");
        $html = "<html><bodyS>";
        $html .= $stringHTML;
        $html .= "<br><br>";
        $html .= "_______________________ <br> Técnico: $nomeTecnico<br><br>";
        $html .= "_______________________ <br> Responsável: $nomeResponsavel<br><br><br>";
        $html .= "Alegrete, $data";
        $html .= "</body></html>";
        $dompdf = new DOMPDF();
        $dompdf->load_html($html);
        ob_clean();
        $dompdf->render();
        $dompdf->stream("$nomeArquivoPDF.pdf");
    }

}
?>