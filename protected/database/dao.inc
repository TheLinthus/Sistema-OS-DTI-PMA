<?php

namespace database;

require_once './protected/database/mysql.inc';

/**
 * Esta é a classe padrão parar ser extendida a fim de se comunicar com o banco.
 * 
 * @author Caio Alexandre
 * @abstract
 * @version 2.0
 * @since 2.0
 */
abstract class DefaultTable {

    protected $tablename;         // Nome da tabela no banco
    protected $dbname;            // Nome do banco de dados
    protected $rows_per_page;     // Número de linhas a retornarem por página (páginação)
    protected $pageno;            // Número da página atual
    protected $lastpage;          // Número da ultima pagina
    protected $fieldlist;         // Lista de campos nesta tabela
    protected $data_array;        // Dados retornados da base de dados
    public $errors;               // Lista de mensagens de erro
    public $last_insert_id;       // Id da ultima insersão por auto_increment

    /*
     * Construtor da Classe.
     * Deve ter seus valores padrões substituidos no seu herdeiro.
     */

    function DefaultTable() {
        $this->tablename = 'default';
        $this->dbname = 'sos';
        $this->rows_per_page = 10;

        $this->fieldlist = array('column1', 'column2', 'column3');
        $this->fieldlist['column1'] = array('pkey' => 'y');
    }

    /**
     * Define o número da página 
     * 
     * @param int $page Número da página 
     */
    function setPageNo($page) {
        if (is_numeric($page)) {
            $this->pageno = $page;
        } else {
            $this->pageno = 1;
        }
    }

    /**
     * Retorna o número de páginas
     * 
     * @return número inteiro de páginas
     */
    function getNoPages() {
        return $this->lastpage;
    }

    /**
     * Retorna o número da página
     * 
     * @return número inteiro da página
     */
    function getPageNo() {
        return $this->pageno;
    }

    function setRowsPerPage($number) {
        $this->rows_per_page = $number;
    }

    /**
     * Retorna dados da tabela.
     * 
     * @param string $where [optional]<br>Lista de em formato textual de filtro
     * @param array $orderby [optional]<br>Lista de colunas e a ordenação padrão. ex.: array('coluna1' => 'DESC')
     * @param array $columns [optional]<br>Lista de colunas a serem extraidas
     * 
     * @return array Lista em formato de um arranjo associativo,
     * Havera uma entrada para cada linha retornada da pesquisa,iniciando de 0.
     * Cada linha haverá uma série de pares "name=value" para cada coluna na tabela
     */
    function getData($where = "", $orderby = [], $columns = []) {
        $this->data_array = array();
        $pageno = $this->pageno;
        $rows_per_page = $this->rows_per_page;
        $this->numrows = 0;
        $this->lastpage = 0;

        $columns_str = "";

        $db = new DataBase();
        $dbconnect = $db->db_connect($this->dbname) or internalServerError("SQL: " . mysql_error()); // trigger_error(..., E_USER_ERROR);
        // Tratar clausula WHERE se ela existir
        if (empty($where)) {
            $where_str = NULL;
        } else {
            $where_str = "WHERE $where";
        }

        $orderby_str = "ORDER BY ";

        // Tratar clausula ORDER BY se ela existir
        if (empty($orderby)) {
            $orderby_str = NULL;
        } else {
            foreach ($orderby as $ordercol => $order) {
                $orderby_str .= "$ordercol $order, ";
            }
        }

        // E remove o ", " após o ultimo item na clausula ORDER BY
        $orderby_str = rtrim($orderby_str, ', ');

        // Contar o números de saidas na base de dados que satisfazem a busca
        $query = "SELECT count(*) FROM $this->tablename $where_str";
        $result = mysql_query($query, $dbconnect) or internalServerError("SQL - " . mysql_error()); // trigger_error(..., E_USER_ERROR);
        $query_data = mysql_fetch_row($result);
        $this->numrows = $query_data[0];

        // Caso não haja dados retornados da base de dados retornar neste ponto
        if ($this->numrows <= 0) {
            $this->pageno = 0;
            return array();
        }

        // Calcular quantas páginas existirão baseado no número de exebição por página
        if ($rows_per_page > 0) {
            $this->lastpage = (int) ceil($this->numrows / $rows_per_page);
        } else {
            $this->lastpage = 1;
        }

        // Validar se a página atual está entre os limites máximo e minimo de páginas, 1 à n
        if ($pageno == '' OR $pageno <= '1') {
            $pageno = 1;
        } elseif ($pageno > $this->lastpage) {
            $pageno = $this->lastpage;
        } // if
        $this->pageno = $pageno;

        // Define variavel limite para retornar os itens da página atual
        if ($rows_per_page > 0) {
            $limit_str = 'LIMIT ' . ($pageno - 1) * $rows_per_page . ',' . $rows_per_page;
        } else {
            $limit_str = NULL;
        }

        // Tratar clausula SELECT para colunas se ela existir
        if (empty($columns)) {
            $columns_str = "*";
        } else {
            foreach ($columns as $column) {
                $columns_str .= "$column, ";
            }
        }

        // E remove o ", " após o ultimo item na clausula SELECT
        $columns_str = rtrim($columns_str, ', ');

        // Constrói a busca na base de dados para retornas os dados
        $query = "SELECT $columns_str FROM $this->tablename $where_str $orderby_str $limit_str";
        $result = mysql_query($query, $dbconnect) or internalServerError("SQL - " . mysql_error()); // trigger_error(..., E_USER_ERROR);
        // Extrai dados do retorno da base de dados em formato de um arranjo associativo
        // Havera uma entrada para cada linha retornada, iniciando de 0
        // Cada linha haverá uma série de pares "name=value" para cada coluna na tabela
        while ($row = mysql_fetch_assoc($result)) {
            $this->data_array[] = $row;
        }

        // Libera espaço da memória referente a consulta SQL
        mysql_free_result($result);

        // Dados retornados da base continuam no data_array e retornam na função
        return $this->data_array;
    }

    /**
     * Insere dados na tabela. 
     * $fieldarray pode ser uma lista retornada direto do $_POST ou $_GET.
     * Atenção para os dados de $fieldarray serem iguais os nomes das colunas no banco de dados
     * 
     * @param array $fieldarray Lista de campos e seus valores a serem inseridos
     * @param string $extra [optional]<br>Comandos extra a serem utilizados na insersão
     */
    function insertRecord(array $fieldarray, $extra = "") {
        $this->errors = array();

        $db = new DataBase();
        $dbconnect = $db->db_connect($this->dbname) or internalServerError("SQL"); // trigger_error(..., E_USER_ERROR);

        $fieldlist = $this->fieldlist;

        // Ira percorrer os dados de $fieldarray e removera os dados que não pertencem a tabela
        // remove o botão submit por exemplo ou qualquer outro dado que cause erro na insersão
        foreach ($fieldarray as $field => $fieldvalue) {
            if (!in_array($field, $fieldlist)) {
                unset($fieldarray[$field]);
            }
        }

        $query = "INSERT INTO $this->tablename SET ";

        // Constrói a clausula SET para a Insersão no banco
        foreach ($fieldarray as $item => $value) {
            $query .= "$item='" . mysql_real_escape_string($value) . "', ";
        }

        // E remove o ", " após o ultimo item na clausula SET
        $query = rtrim($query, ', ');

        // Adiciona clausulas extras se existirem
        $query .= $extra;

        // Executa consulta de Insersão
        $result = mysql_query($query, $dbconnect) or internalServerError("SQL " . mysql_error()); // trigger_error(..., E_USER_ERROR);
        // Busca pelo erro 1062 referente a chave primaria duplicada
        if (mysql_errno() <> 0) {
            if (mysql_errno() == 1062) {
                $this->errors[] = "Identificador unico já utilizado (PK)";
            } else {
                $this->errors[] = mysql_error();
            }
        } else {
            // Salva a ultima chave primaria inserida
            $query = "SELECT LAST_INSERT_ID()";
            $result = mysql_query($query, $dbconnect);
            $query_data = mysql_fetch_row($result);
            $this->last_insert_id = $query_data[0];
        }

        return;
    }

    /**
     * Altera dados na tabela
     * $fieldarray pode ser uma lista retornada direto do $_POST ou $_GET
     * Atenção para os dados de $fieldarray serem iguais os nomes das colunas no banco de dados
     * Colunas com identificadores de Chave Primaria (PK) serão usadas na clausula WHERE
     * 
     * @param array $fieldarray Lista de campos e seus valores a serem inseridos
     */
    function updateRecord(array $fieldarray) {
        $this->errors = array();

        $db = new DataBase();
        $dbconnect = $db->db_connect($this->dbname) or internalServerError("SQL"); // trigger_error(..., E_USER_ERROR);

        $fieldlist = $this->fieldlist;

        // Ira percorrer os dados de $fieldarray e removera os dados que não pertencem a tabela
        // remove o botão submit por exemplo ou qualquer outro dado que cause erro na insersão
        foreach ($fieldarray as $field => $fieldvalue) {
            if (!in_array($field, $fieldlist)) {
                unset($fieldarray[$field]);
            }
        }

        $where = "";
        $update = "";

        // Constrói as clausulas WHERE com Chaves Primarias e clausula SET com as outras colunas
        foreach ($fieldarray as $item => $value) {
            if (isset($fieldlist[$item]['pkey'])) {
                $where .= "$item='" . mysql_escape_string($value) . "' AND ";
            } else {
                if ($value == null || strtolower($value) == "null") {
                    $update .= "$item=NULL, ";
                } else {
                    $update .= "$item='" . mysql_escape_string($value) . "', ";
                }
            }
        }

        // Remove o ", " e " AND " após o ultimo item na clausula SET e WHERE
        $where = rtrim($where, ' AND ');
        $update = rtrim($update, ', ');

        // Constrói consulta
        $query = "UPDATE $this->tablename SET $update WHERE $where";

        // Executa consulta de Alteração
        $result = mysql_query($query, $dbconnect) or internalServerError("SQL " . mysql_error()); // trigger_error(..., E_USER_ERROR);

        return;
    }

    /**
     * Altera dados na tabela
     * $fieldarray pode ser uma lista retornada direto do $_POST ou $_GET
     * Atenção para os dados de $fieldarray serem iguais os nomes das colunas no banco de dados
     * Colunas com identificadores de Chave Primaria (PK) serão usadas na clausula WHERE
     * 
     * @param array $fieldarray Lista de campos e seus valores a serem inseridos
     */
    function deleteRecord(array $fieldarray) {
        $this->errors = array();

        $db = new DataBase();
        $dbconnect = $db->db_connect($this->dbname) or internalServerError("SQL"); // trigger_error(..., E_USER_ERROR);

        $fieldlist = $this->fieldlist;
        $where = NULL;

        // Constrói as clausulas WHERE com Chaves Primarias
        foreach ($fieldarray as $item => $value) {
            if (isset($fieldlist[$item]['pkey'])) {
                $where .= "$item='" . mysql_escape_string($value) . "' AND ";
            }
        }

        // E remove o " AND " após o ultimo item na clausula WHERE
        $where = rtrim($where, ' AND ');

        // Constrói consulta 
        $query = "DELETE FROM $this->tablename WHERE $where";

        // Executa consulta de Remoção
        $result = mysql_query($query, $dbconnect) or internalServerError("SQL - " . mysql_error()); // trigger_error(..., E_USER_ERROR);

        return;
    }

}
