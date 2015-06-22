<?php 

require_once 'model.inc';
$img = new Imagem();
if ($_SERVER['REQUEST_METHOD'] == 'DELETE') {
    header('Content-type: application/json');
    if (isset($_GET['anuncio']) && isset($_GET['file'])) {
        $delete = array();
        $delete['idanuncio'] = $_GET['anuncio'];
        $delete['nome'] = $_GET['file'];
        $img->deleteRecord($delete);
        $response_array['status'] = 'success';
    } else {
        $response_array['status'] = 'error';
    }
    echo json_encode($response_array);
} else {
    if (isset($_GET['anuncio']) && isset($_GET['file'])) {
        $anuncio = $_GET['anuncio'];
        $filename = $_GET['file'];
        header('Content-type: image/jpeg');
        echo ($tmp = $img->getData("idanuncio = '{$anuncio}' AND nome = '{$filename}'")) ? $tmp[0]['arquivo'] : $tmp[0]['arquivo'];
    } else {
        echo '<img src=\'/img/error.png\'/>';
    }
}