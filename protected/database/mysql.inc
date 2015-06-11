<?php

namespace database;

class DataBase {

    private $dbconnect = NULL;          // Conexão com o banco
    private $dbhost = "172.17.2.23";      // Endereço do servidor de BD
    private $dbusername = "root";    // Usúario no servidor de BD
    private $dbuserpass = "2747";           // Senha no servidor de BD
    private $query = NULL;              // Variavel para armazenar consulta no banco
//    private $dbconnect = NULL;          // Conexão com o banco
//    private $dbhost = "172.17.6.3";      // Endereço do servidor de BD
//    private $dbusername = "chamadosdti";    // Usúario no servidor de BD
//    private $dbuserpass = "z@@m270";           // Senha no servidor de BD
//    private $query = NULL;              // Variavel para armazenar consulta no banco
    
    /*
     *  Conecta-se com o banco de dados com os dados preenchidos
     */
    function db_connect($dbname) {

        // Caso não haja uma conexão existente, conectar
        if (!$this->dbconnect) {
            $this->dbconnect = mysql_connect($this->dbhost, $this->dbusername, $this->dbuserpass);
        }
        if (!$this->dbconnect) {
            // Caso a conexão não fora um sucesso, retornar 0
            return 0;
        } elseif (!mysql_select_db($dbname, $this->dbconnect)) {
            // Caso não for possivel se conectar com o banco (schema), retornar 0
            return 0;
        } else {
            mysql_query('SET character_set_connection=utf8');
            mysql_query('SET character_set_client=utf8');
            mysql_query('SET character_set_results=utf8');
            // Caso a conexão com o servidor e o banco fora um sucesso, retornar a conexão
            return $this->dbconnect;
        }
    }

    function close() {
        mysql_close($this->dbconnect);
        $this->dbconnect = null;
    }
}
