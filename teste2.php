<?php
include './protected/database/postgresql.inc';

$ecidade = new \database\ECidade();

$divisoes = $ecidade->getDivisoes($_GET['dep']);

$response = [];

echo json_encode($response);