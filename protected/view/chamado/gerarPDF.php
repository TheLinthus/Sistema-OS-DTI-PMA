<?php

include '../../../dompdf/dompdf_config.inc.php';

$nomeArquivoPDF = "testerelatorio";
date_default_timezone_set('America/Sao_Paulo');
$data = date("d/m/Y");
$html = "<html><body> <h1> $data LOLEEES </h1></body></html>";
$dompdf = new DOMPDF();
$dompdf->load_html($html);
$dompdf->render();
$dompdf->stream("$nomeArquivoPDF.pdf");
?>