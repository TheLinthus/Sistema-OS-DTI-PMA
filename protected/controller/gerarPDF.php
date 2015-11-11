<?php

$urlBase = 'http://' . $_SERVER['HTTP_HOST'] . '/chamados/';
$objetoClasse = new gerarPDF();

if ($_REQUEST["funcao"] == "finalizarRelatorio") {
    $nomeRelatorio = $_REQUEST["nomeRelatorio"];
    $nomeTecnico = $_REQUEST["nomeTecnico"];
    $nomeResponsavel = $_REQUEST["nomeResponsavel"];

    $usuario = $_REQUEST["usuario"];
    $setor = $_REQUEST["setor"];
    $solucao = $_REQUEST["solucao"];
    $data = $_REQUEST["data"];
    $descricao = $_REQUEST["descricao"];
    $historico = $_REQUEST["historico"];
    $objetoClasse->finalizarRelatorio($nomeRelatorio, $nomeTecnico, $nomeResponsavel, $descricao, $setor, $solucao, $historico);
}

if ($_REQUEST["funcao"] == "criarArquivoRelatorio") {
    $todasInfoChamado = $_REQUEST["todasInfoChamado"];
    $objetoClasse->criarArquivoRelatorio($todasInfoChamado);
}

class gerarPDF {

    //método para criar arquivo PDF que será assinado e posteriormente scaneado 
    //para upload no sistema e assim finalizar o chamado.S
    function finalizarRelatorio($nomeRelatorio, $nomeTecnico, $nomeResponsavel, $descricao, $setor, $solucao, $historico) {
        require "C:/xampp/htdocs/chamados/dompdf/dompdf_config.inc.php";
        $nomeArquivoPDF = "Arquivo_Chamado_" . $nomeRelatorio;
        date_default_timezone_set('America/Sao_Paulo');
        $data = date("d/m/Y");
        $html = "<html><body>";
        $html .= '<table>
            <tr><td><img src="C:/xampp/htdocs/chamados/img/brasao.jpg" height="100"></td> 
                <td style="width: 633px; padding:10px; _width: 250px;"> <h2 align="center">PREFEITURA MUNICIPAL DE ALEGRETE</h2>
                    <h3 align="center">Secretaria de Governo</h3>
                    <h3 align="center">Divisão de Tecnologia da Informação - DTI</h3>
                </td> </tr>
        </table>';
        $html .= "<br><br>";
        $html .= "<b>Ordem de serviço número: " . $nomeRelatorio . "</b> <br>";
        $html .= "<b>Local de atendimento: " . $setor . "</b> <br>";
        $html .= "<p>";
        $html .= "Eu, " . $nomeResponsavel . ", diretor(a) ou responsável, verifiquei presencialmente que o seguinte problema: ";
        $html .= "'" . $descricao . "', ";
        $html .= "foi solucionado ou está em pleno funcionamento após a Divisão de Tecnologia da Informação executar os seguintes procedimentos: '";
        $html .= $solucao . "'.</p><br>";
        $html .= "<p>";
        $html .= "Histórico do Chamado: ";
        $html .= "</p>";
        $html .= "<p>";
        $html .= $historico;
        $html .= "</p><br>";

        $html .= "_______________________ <br> $nomeTecnico [Técnico em Informática] <br><br>";
        $html .= "_______________________ <br> $nomeResponsavel [Responsável] <br><br><br>";
        $html .= "Alegrete, $data";
        $html .= "<p> <h6 align=\"center\" > PREFEITURA DE ALEGRETE <br>
SECRETARIA DE GOVERNO <br>
DIVISÃO DE TECNOLOGIA DA INFORMAÇÃO - DTI <br>
CENTRO ADMINISTRATIVO MUNICIPAL <br>
'DOE ÓRGÃOS, DOE SANGUE: SALVE VIDAS' <br>
CENTRO ADMINISTRATIVO MUNICIPAL – Maj. João Cezimbra Jaques 200 – Cep: 97543‐390 <br>
Fone: 55 3961 1616/ 3961 1612 <br>
Email: informatica@alegrete.rs.gov.br <br> </h6>
</p>";
        $html .= "</body></html>";
        $dompdf = new DOMPDF();
        $dompdf->load_html($html);
        ob_clean();
        $dompdf->render();
        $dompdf->stream("$nomeArquivoPDF.pdf");
    }

    //método teste para relatórios em filtros
    function criarArquivoRelatorio($todasInfoChamado) {

        require "C:/xampp/htdocs/chamados/dompdf/dompdf_config.inc.php";

        $nomeArquivoPDF = "Arquivo_Relatorio_Chamados";
        date_default_timezone_set('America/Sao_Paulo');
        $data = date("d/m/Y");
        $html = "<html><body>";
        $html .= '<table>
            <tr><td><img src="C:/xampp/htdocs/chamados/img/brasao.jpg" height="100"></td> 
                <td style="width: 633px; padding:10px; _width: 250px;"> <h2 align="center">PREFEITURA MUNICIPAL DE ALEGRETE</h2>
                    <h3 align="center">Secretaria de Governo</h3>
                    <h3 align="center">Divisão de Tecnologia da Informação - DTI</h3>
                </td> </tr>
        </table>';
        $html .= "<p>";
        $html .= $todasInfoChamado;
        $html .= "</p><br>";
        $html .= "Alegrete, $data";
        $html .= "<p> <h6 align=\"center\" > PREFEITURA DE ALEGRETE <br>
SECRETARIA DE GOVERNO <br>
DIVISÃO DE TECNOLOGIA DA INFORMAÇÃO - DTI <br>
CENTRO ADMINISTRATIVO MUNICIPAL <br>
'DOE ÓRGÃOS, DOE SANGUE: SALVE VIDAS' <br>
CENTRO ADMINISTRATIVO MUNICIPAL – Maj. João Cezimbra Jaques 200 – Cep: 97543‐390 <br>
Fone: 55 3961 1616/ 3961 1612 <br>
Email: informatica@alegrete.rs.gov.br <br> </h6>
</p>";
        $html .= "</body></html>";
        $dompdf = new DOMPDF();
        $dompdf->load_html($html);
        ob_clean();
        $dompdf->render();
        $dompdf->stream("$nomeArquivoPDF.pdf");
    }

}

?>