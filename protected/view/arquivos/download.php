<?php 

header('Content-type: ' . $response['type']);
header('Content-disposition: attachment; filename="' . $response['name'] . '"');
echo $response['bytes'];