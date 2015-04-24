<?php

if (isset($_GET['id'])) {
    $output = [];

    $id = str_replace([",", "."], "", $_GET['id']);

    $dbconnect = pg_connect("host=192.168.2.8 port=5432 dbname=alegrete user=view_os password=dti#pma@view");
    pg_setclientencoding($dbconnect, "utf8");
    pg_exec("SET NAMES 'utf8'");
    pg_exec("SET CLIENT_ENCODING TO 'utf8'");

    $column = (strlen($id) > 5) ? "cpf" : "matricula";
    
    $output['strlen'] = strlen($id);

    $query = "SELECT * FROM v_funcionario WHERE fun_$column = '$id'";

    $result = pg_query($query) or die("PostgreSQL erro: " . pg_last_error());

    $rs = pg_fetch_all($result);

    pg_close($dbconnect);

    if (isset($rs[0])) {
        $output['valid'] = true;
        $output['data'] = $rs[0];
    } else {
        $output['valid'] = false;
    }

    echo json_encode($output);
}