<?php 

/* Inclui arquivos necessarios para execução do sistema */
require_once './protected/controller/UploadHandler.inc';
require_once './protected/controller/controller.inc';
require_once './protected/controller/login.inc';
/**
 * Redireciona para página de erro 404
 */
function notFound()
{
    header('HTTP/1.0 404 Not Found');
    include '404.php';
    die;
}
/**
 * Redireciona para página de erro 403
 */
function notAllowed()
{
    header('HTTP/1.0 401 Unauthorized');
    include '401.php';
    die;
}
/**
 * Redireciona para página de erro 403
 */
function badRequest()
{
    header('HTTP/1.0 400 Bad Request');
    include '400.php';
    die;
}
/**
 * Redireciona para página de erro Interno 500
 */
function internalServerError($error = '')
{
    header('HTTP/1.0 500 Internal Server Error');
    include '500.php';
    die;
}
function getIP()
{
    if (isset($_SERVER['HTTP_X_FORWARDED_FOR'])) {
        list($IP1, $IP2) = explode(',', trim($_SERVER['HTTP_X_FORWARDED_FOR']));
        if (trim($IP1) != '' || trim($IP2) != '') {
            $ip = trim(empty($IP2) ? $IP1 : $IP2);
        } else {
            $ip = trim($_SERVER['REMOTE_ADDR']);
        }
    } else {
        $ip = trim($_SERVER['REMOTE_ADDR']);
    }
    return $ip;
}
function working()
{
    header('HTTP/1.0 501 Not Implemented');
    include 'working.php';
    die;
}
$load_time = microtime();
$load_time = explode(' ', $load_time);
$load_time = $load_time[1] + $load_time[0];
$load_start = $load_time;
setlocale(LC_ALL, 'pt_BR.UTF-8', 'ptb');
session_start();
// Captura página solicitada
$req = filter_input(INPUT_GET, 'req');
unset($_GET['req']);
// Remove variavel usada para requisição no GET
$input = array();
// Prepara Input para receber parametros de entrada pra processamento
$input['args'] = array();
// Inicializa array
$input['get'] = $_GET;
// Armazena dados GET para processamento de entrada
$input['post'] = $_POST;
// Armazena dados POST para processamento de entrada
if ($req == null || empty($req)) {
    // Caso não seja passado uma view especifica
    $req = 'chamado/index';
    $mod = 'chamado';
    $act = 'index';
} else {
    // Caso se queira uma view epscifica, separa modulo, ação e argumentos
    list($mod, $act, $arg) = array_pad(split('/', $req, 3), 3, null);
    // Separa Modulo, Ação e Argumnetos extras da requisição
    $act = $act ? $act : 'index';
    // Caso nenhuma requisição de página especifica foi feita chamar index
    $arg = $arg ? explode('/', $arg) : array();
    // Separa Argumentos
    for ($i = 0; $i < count($arg); $i += 2) {
        // Arnazana Argumentos extras em chave e valor ('true' caso valor não seja passado)
        if (!empty($arg[$i])) {
            $input['args'][$arg[$i]] = isset($arg[$i + 1]) ? $arg[$i + 1] : true;
        }
    }
}
$class = "controller\\{$mod}Controller";
// prepara nome da classe a ser inicializada
if (class_exists($class) && file_exists("protected/view/{$mod}/{$act}.php")) {
    // verifica se a classe e arquivo de view existe antes de chamar (proteção de erro e segurança)
    $controller = new $class();
    // Instancia nova Classe requisistada
    if (method_exists($controller, $act)) {
        // verifica se a classe contém o metodo requisitado (proteção de erro)
        try {
            $response = $controller->{$act}($input);
            // chama o metodo requisitado enviando uma entrada de dados aguardando uma resposta
            if (isset($response['error'])) {
                // se a resposta informa que um erro ocorreu
                $response['error']();
            } else {
                if (isset($response['redirect'])) {
                    // se a resposta informa a necessidade de um redirecionamento
                    header("location:{$response['redirect']}");
                } else {
                    include "protected/view/{$mod}/{$act}.php";
                }
            }
        } catch (Exception $ex) {
            internalServerError($ex->getMessage());
        }
    } else {
        notFound();
    }
} else {
    notFound();
}
$load_time = microtime();
$load_time = explode(' ', $load_time);
$load_time = $load_time[1] + $load_time[0];
$load_finish = $load_time;
$load_total_time = round($load_finish - $load_start, 4);
echo "<script>console.log('Página gerada em {$load_total_time} segundos.');</script>";