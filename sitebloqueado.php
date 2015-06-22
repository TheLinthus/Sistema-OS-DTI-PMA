<?php 

header('Content-Type: text/html; charset=utf-8');
if (isset($_POST['id']) && isset($_GET['site']) && $_GET['site'] !== '') {
    foreach ($_POST as $value) {
        if ($value == null || $value == '') {
            $mensagem = 'Preencha os campos corretamente!';
            break;
        }
    }
    if (!isset($mensagem)) {
        require_once './class.phpmailer.php';
        require_once './class.smtp.php';
        $motivo = str_replace('
', '<br>', $_POST['motivo']);
        $idtipo = strlen($_POST['id']) > 5 ? 'CPF' : 'Matricula';
        $mail = new PHPMailer();
        $mail->CharSet = 'UTF-8';
        $mail->isSMTP();
        // Set mailer to use SMTP
        $mail->Host = '172.17.2.146';
        // Specify main and backup SMTP servers
        $mail->SMTPAuth = true;
        // Enable SMTP authentication
        $mail->Username = 'suporte@alegrete.rs.gov.br';
        // SMTP username
        $mail->Password = 'PhV8_!141721';
        // SMTP password
        $mail->SMTPSecure = 'ssl';
        // Enable TLS encryption, `ssl` also accepted
        $mail->Port = 465;
        // TCP port to connect to
        $mail->From = 'suporte@alegrete.rs.gov.br';
        $mail->FromName = 'Sistema Bloqueio de Site';
        $mail->addAddress('rdleiria@gmail.com');
        $mail->addReplyTo($_POST['email'], $_POST['nome']);
        $mail->isHTML(true);
        // Set email format to HTML
        $mail->Subject = 'Solicitação de Liberação de Site';
        $mail->Body = "Foi solocitado a liberação do site: <a href='http://{$_GET['site']}'><b><i>{$_GET['site']}</i></b></a><br><br>";
        $mail->Body .= 'Detalhes da Requisição:<br>';
        $mail->Body .= '<ul>';
        $mail->Body .= "<li><b>Nome:</b> {$_POST['nome']}</li>";
        $mail->Body .= "<li><b>{$idtipo}:</b> {$_POST['id']}</li>";
        $mail->Body .= "<li><b>E-mail:</b> {$_POST['email']}</li>";
        $mail->Body .= "<li><b>Cargo:</b> {$_POST['cargo']}</li>";
        $mail->Body .= "<li><b>Lotação:</b> {$_POST['lotacao']}</li>";
        $mail->Body .= "<li><b>Local de Trabalho:</b> {$_POST['localdetrabalho']}</li>";
        $mail->Body .= "<li><b>Endereço IP:</b> {$_SERVER['REMOTE_ADDR']}</li>";
        $mail->Body .= '</ul>';
        $mail->Body .= '<fieldset><legend>Moivo da Requisição</legend>';
        $mail->Body .= $motivo;
        $mail->Body .= '</fieldset>';
        $mail->AltBody = "Foi solocitado a liberação do site: {$_POST['site']}. Por {$_POST['nome']}.";
        if (!$mail->send()) {
            $mensagem = 'Erro ao enviar requisição: ' . $mail->ErrorInfo;
        } else {
            $mensagem = 'Requisição enviada!';
        }
    }
} ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <style>
            * {
                box-sizing: border-box;
            }
            html {
                position: relative;
                min-height: 100%;
            }
            body {
                margin-bottom: 60px;
                margin: 0;
                font-family: "Helvetica Neue",Helvetica,Arial,sans-serif;
                font-size: 14px;
                line-height: 20px;
                color: #333333;
                background-color: #ffffff;
            }
            form.center {
                margin-left: auto;
                margin-right: auto;
                max-width: 320px;
            }
            hr {
                margin: 20px 0;
                border: 0;
                border-top: 1px solid #eeeeee;
                border-bottom: 1px solid #ffffff;
            }
            .vertical-center {
                min-height: 100%;
                min-height: 100vh;
                display: flex;
                align-items: center;
                padding-bottom: 60px;
                margin-bottom: -60px;
                padding-top: 40px;
                margin-top: -40px;
            }
            form {
                margin: 0 0 20px;
            }
            fieldset {
                padding: 0;
                margin: 0;
                border: 0;
            }
            .ui-widget {
                font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
                font-size: 13px;
            }
            .ui-corner-all {
                -webkit-border-radius: 4px;
                -moz-border-radius: 4px;
                -khtml-border-radius: 4px;
                border-radius: 4px;
            }
            .ui-state-error {
                position: relative;
                margin-bottom: 18px;
                color: #ffffff;
                border-width: 1px;
                border-style: solid;
                -webkit-border-radius: 4px;
                -moz-border-radius: 4px;
                border-radius: 4px;
                -webkit-box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.25);
                -moz-box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.25);
                box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.25);
                background-color: #c43c35;
                background-repeat: repeat-x;
                background-image: -khtml-gradient(linear, left top, left bottom, from(#ee5f5b), to(#c43c35));
                background-image: -moz-linear-gradient(top, #ee5f5b, #c43c35);
                background-image: -ms-linear-gradient(top, #ee5f5b, #c43c35);
                background-image: -webkit-gradient(linear, left top, left bottom, color-stop(0%, #ee5f5b), color-stop(100%, #c43c35));
                background-image: -webkit-linear-gradient(top, #ee5f5b, #c43c35);
                background-image: -o-linear-gradient(top, #ee5f5b, #c43c35);
                background-image: linear-gradient(top, #ee5f5b, #c43c35);
                filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#ee5f5b', endColorstr='#c43c35', GradientType=0);
                text-shadow: 0 -1px 0 rgba(0, 0, 0, 0.25);
                border-color: #c43c35 #c43c35 #882a25;
                border-color: rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);
            }
            .ui-state-error p {
                font-size: 13px;
                font-weight: normal;
                line-height: 18px;
                margin: 7px 15px 10px 15px;
            }
            legend {
                display: block;
                width: 100%;
                padding: 0;
                margin-bottom: 20px;
                font-size: 21px;
                line-height: 40px;
                color: #333333;
                border: 0;
                border-bottom: 1px solid #e5e5e5;
            }
            input, textarea {
                display: inline-block;
                padding: 4px;
                font-size: 13px;
                line-height: 18px;
                border: 1px solid #ccc;
                -webkit-border-radius: 3px;
                -moz-border-radius: 3px;
                border-radius: 3px;
                width: 100%;
            }
            .readonly {
                background-color: #eee;
                color: #000;
            }
            .control-group {
                margin-bottom: 5px;
            }
            .btn-primary {
                float: right;
                display: inline-block;
                padding: 4px 12px;
                font-size: 14px;
                line-height: 20px;
                color: #ffffff;
                text-shadow: 0 -1px 0 rgba(0, 0, 0, 0.25);
                background-color: #006dcc;
            }
            .btn-primary:hover, .btn-primary:active, .btn-primary.active {
                color: #ffffff;
                background-color: #0044cc;
            }
            .btn-primary.disabled, .btn-primary[disabled] {
                background-color: #7ab5d3;
            }
        </style>
        <title>Site Bloquado</title>
    </head>
    <body>
        <form id="requisicao" method="POST" class="form-horizontal vertical-center center">
            <fieldset>
                <legend>Requisitar Liberação de Site</legend>
                <div class="ui-widget">
                    <div class="ui-state-error ui-corner-all">
                        <p>
                            <strong>Erro:</strong> A página solicitada esta bloqueada! Se você necessida acessar essa página preencha o formulario de requisição de acesso.
                        </p>
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label" for="id">Matricula ou CPF</label>
                    <div class="controls">
                        <input id="id" name="id" type="text" class="input-xlarge" required>
                    </div>
                </div>
                <hr>
                <div class="control-group">
                    <label class="control-label" for="nome">Nome</label>
                    <div class="controls">
                        <input id="nome" name="nome" type="text" placeholder="" class="input-xlarge readonly" readonly="">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label" for="email">E-mail</label>
                    <div class="controls">
                        <input id="email" name="email" type="text" placeholder="email@dominio.com" class="input-xlarge">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label" for="cargo">Cargo</label>
                    <div class="controls">
                        <input id="cargo" name="cargo" type="text" class="input-xlarge readonly" readonly="">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label" for="lotacao">Lotação</label>
                    <div class="controls">
                        <input id="lotacao" name="lotacao" type="text" class="input-xlarge readonly" readonly="">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label" for="localdetrabalho">Local de Trabalho</label>
                    <div class="controls">
                        <input id="localdetrabalho" name="localdetrabalho" type="text" class="input-xlarge readonly" readonly="">
                    </div>
                </div>
                <div class="control-group">
                    <label class="control-label" for="motivo">Motivo da Requisição</label>
                    <div class="controls"><textarea id="motivo" name="motivo" style="margin: 0px; width: 328px; height: 86px;"></textarea>

                    </div>
                </div><div class="control-group">
                    <div class="controls">
                        <button class="btn-primary ui-corner-all" disabled>Requisitar</button>
                    </div>
                </div>
            </fieldset>
        </form>
        <script src="./js/jquery-1.11.1.min.js"></script>
        <script src="./js/jquery.inputmask.bundle.js"></script>
        <script type="text/javascript">
            $("#id").inputmask("9999[9][999999]");
            $("#id").keyup(function () {
                if ($(this).inputmask("isComplete")) {
                    $.get("./dados.php", {id: $("#id").inputmask('unmaskedvalue')}, function (data) {
                        var a = JSON.parse(data);
                        if (a.valid) {
                            $("#cargo").val(a.data.fun_cargo);
                            $("#email").val(a.data.fun_email);
                            $("#localdetrabalho").val(a.data.fun_localtrabalho);
                            $("#lotacao").val(a.data.fun_lotacao_nome);
                            $("#nome").val(a.data.fun_nome);
                            $(".btn-primary").removeAttr("disabled");
                        } else {
                            $("#cargo").val("");
                            $("#email").val("");
                            $("#localdetrabalho").val("");
                            $("#lotacao").val("");
                            $("#nome").val("");
                            $(".btn-primary").attr("disabled", "disabled");
                        }
                    });
                } else {
                    $("#cargo").val("");
                    $("#email").val("");
                    $("#localdetrabalho").val("");
                    $("#lotacao").val("");
                    $("#nome").val("");
                    $(".btn-primary").attr("disabled", "disabled");
                }
            });
            $("#requisicao").submit(function () {
                return confirm("Você confirma os seus dados e requisição?");
            });
            $("#id").focus();
<?php if (isset($mensagem)) {
    echo "alert('{$mensagem}');";
} ?>
        </script>
    </body>
</html><?php 