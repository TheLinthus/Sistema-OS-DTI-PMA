<?php

namespace model;

require_once './protected/database/dao.inc';

use database;

class Chamado extends database\DefaultTable {
    
    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Chamado();
    }

    function Chamado() {
        $this->tablename = 'chamado';
        $this->dbname = 'sos';
        $this->rows_per_page = 0;

        $this->fieldlist = array('id','area','modulo','problema','descricao','patrimonio','secretaria','setor','prioridade','iporigem','usuario_cpf','solucao');
        $this->fieldlist['id'] = array('pkey' => 'y');
    }
}

class Estado extends database\DefaultTable {
    
    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Estado();
    }

    function Estado() {
        $this->tablename = 'estado';
        $this->dbname = 'sos';
        $this->rows_per_page = 0;

        $this->fieldlist = array('id','chamado','estado','tecnico_cpf','data','tipo','nivel');
        $this->fieldlist['id'] = array('pkey' => 'y');
        $this->fieldlist['chamado'] = array('pkey' => 'y');
    }
}

class Arquivos extends database\DefaultTable {
    
    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Arquivos();
    }

    function Arquivos() {
        $this->tablename = 'arquivos';
        $this->dbname = 'sos';
        $this->rows_per_page = 20;

        $this->fieldlist = array('id','nome','arquivo','chamado');
        $this->fieldlist['id'] = array('pkey' => 'y');
    }
}

class Patrimonio extends database\DefaultTable {
    
    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Patrimonio();
    }

    function Patrimonio() {
        $this->tablename = 'patrimonio';
        $this->dbname = 'sos';
        $this->rows_per_page = 10;

        $this->fieldlist = array('id','placa','descricao','observacoes');
        $this->fieldlist['id'] = array('pkey' => 'y');
    }
}

class Secretaria extends database\DefaultTable {
    
    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Secretaria();
    }

    function Secretaria() {
        $this->tablename = 'secretaria';
        $this->dbname = 'sos';
        $this->rows_per_page = 0;

        $this->fieldlist = array('id','secretaria');
        $this->fieldlist['id'] = array('pkey' => 'y');
    }
}

class Setor extends database\DefaultTable {
    
    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Setor();
    }

    function Setor() {
        $this->tablename = 'setor';
        $this->dbname = 'sos';
        $this->rows_per_page = 10;

        $this->fieldlist = array('id','setor','secretaria','escola','telefone','prioridade');
        $this->fieldlist['id'] = array('pkey' => 'y');
        //$this->fieldlist['secretaria'] = array('pkey' => 'y');
    }
}

class Usuario extends database\DefaultTable {

    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Usuario();
    }

    function Usuario() {
        $this->tablename = 'usuario';
        $this->dbname = 'sos';
        $this->rows_per_page = 10;

        $this->fieldlist = array('cpf', 'matricula', 'cgm', 'nome', 'email', 'cargo', 'lotacao', 'localdetrabalho', 'ip');
        $this->fieldlist['cpf'] = array('pkey' => 'y');
    }
}

class Tecnico extends database\DefaultTable {

    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Tecnico();
    }

    function Tecnico() {
        $this->tablename = 'tecnico';
        $this->dbname = 'sos';
        $this->rows_per_page = 10;

        $this->fieldlist = array('cpf', 'telefone', 'senha');
        $this->fieldlist['cpf'] = array('pkey' => 'y');
    }
}

class AreasTecnico extends database\DefaultTable {

    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->AreasTecnico();
    }

    function AreasTecnico() {
        $this->tablename = 'areas_tecnico';
        $this->dbname = 'sos';
        $this->rows_per_page = 0;

        $this->fieldlist = array('tecnico_cpf', 'area_id');
        $this->fieldlist['tecnico_cpf'] = array('pkey' => 'y');
        $this->fieldlist['area_id'] = array('pkey' => 'y');
    }
}

class Area extends database\DefaultTable {

    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Area();
    }

    function Area() {
        $this->tablename = 'area';
        $this->dbname = 'sos';
        $this->rows_per_page = 0;

        $this->fieldlist = array('id', 'area', 'nivel');
        $this->fieldlist['id'] = array('pkey' => 'y');
    }
}

class Problema extends database\DefaultTable {

    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Problema();
    }

    function Problema() {
        $this->tablename = 'problema';
        $this->dbname = 'sos';
        $this->rows_per_page = 0;

        $this->fieldlist = array('id', 'modulo', 'problema', 'dica');
        $this->fieldlist['id'] = array('pkey' => 'y');
    }
}

class Modulo extends database\DefaultTable {

    /**
     * Devido a problemas de retrocompatibilidade do PHP4 com o PHP5
     * deve se usar o metodo __construct() para que o metodo construtor
     * seja chamado em namespace diferentes
     */
    function __construct() {
        $this->Modulo();
    }

    function Modulo() {
        $this->tablename = 'modulo';
        $this->dbname = 'sos';
        $this->rows_per_page = 0;

        $this->fieldlist = array('id', 'area', 'modulo');
        $this->fieldlist['id'] = array('pkey' => 'y');
    }
}