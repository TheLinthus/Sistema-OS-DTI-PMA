<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>401 Não autorizado</title>
    </head>
    <body class="alert-error">
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <div class="vertical-center pagination-centered">
                    <div class="container">
                        <h1 id="error-code">401</h1>
                        <div class="ui-widget">
                            <div class="ui-state-error ui-corner-all">
                                <p>
                                    <span class="ui-icon ui-icon-alert" style="float: left; margin-right: .3em;"></span>
                                    <strong>Erro:</strong> Você não está logado com um usuário autorizado a acessar esta página.
                                </p>
                            </div>
                        </div>
                        <a class="button" href="javascript: history.back();">Voltar</a>
                        <a class="button ui-button-primary" href="/">Página Inicial</a>
                    </div>
                </div>
            </div>
        </div>
        <?php include "protected/view/footer.php"; ?>
    </body>
    <?php include "protected/view/footscripts.php"; ?>
</html>