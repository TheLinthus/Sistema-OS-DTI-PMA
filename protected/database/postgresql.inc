<?php

namespace database;

class PostgreSQLBase {

    private $dbconnect = NULL;          // Conexão com o banco
    private $dbname = "alegrete";        // Base de dados padrão
    private $dbhost = "192.168.2.8";      // Endereço do servidor de BD
    private $dbport = 5432;
    private $dbusername = "view_os";    // Usúario no servidor de BD
    private $dbuserpass = "dti#pma@view";           // Senha no servidor de BD
    private $query = NULL;              // Variavel para armazenar consulta no banco

    /*
     *  Conecta-se com o banco de dados com os dados preenchidos
     */

    function db_connect() {

        // Caso não haja uma conexão existente, conectar
        if (!$this->dbconnect) {
            $this->dbconnect = pg_connect("host=$this->dbhost port=$this->dbport dbname=$this->dbname user=$this->dbusername password=$this->dbuserpass");
        }
        if (!$this->dbconnect) {
            // Caso a conexão não fora um sucesso, retornar 0
            return 0;
        } else {
            // Caso a conexão com o servidor e o banco fora um sucesso, retornar a conexão
            pg_setclientencoding($this->dbconnect, "utf8");
            pg_exec("SET NAMES 'utf8'");
            pg_exec("SET CLIENT_ENCODING TO 'utf8'");

            return $this->dbconnect;
        }
    }

    function close() {
        pg_close($this->dbconnect);
        $this->dbconnect = null;
    }

}

class ECidade {

    function getData($table, $where = "", $extra = "") {
        $pg = new PostgreSQLBase();

        $pg->db_connect() or die("Não foi possivel conectar com o servidor do e-Cidade: " . pg_last_error());

        $query = "SELECT * FROM $table WHERE $where $extra";

        $result = pg_query($query) or die("PostgreSQL erro: " . pg_last_error());

        $rs = pg_fetch_all($result);

        $pg->close();

        return $rs;
    }
}
