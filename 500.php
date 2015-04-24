<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>500 Erro Interno no Servidor</title>
    </head>
    <body class="alert-error">
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <div class="vertical-center pagination-centered">
                    <div class="container">
                        <h1 id="error-code">500</h1>
                        <div class="ui-widget">
                            <div class="ui-state-error ui-corner-all">
                                <p>
                                    <span class="ui-icon ui-icon-alert" style="float: left; margin-right: .3em;"></span>
                                    <strong>Erro:</strong> Ocorreu um erro interno no servidor, por favor tente novamente ou contate o DTI.
                                    <?php if (isset($error) && $error !== "") { ?>
                                        <br><?= $error ?>
                                    <?php } ?>
                                </p>
                            </div>
                        </div>
                        <a class="button" href="javascript: history.back();">Voltar</a>
                        <a class="button" href="javascript: location.reload();">Recarregar</a>
                        <a class="button ui-button-primary" href="/">Página Inicial</a>
                    </div>
                </div>
            </div>
        </div>
        <?php include "protected/view/footer.php"; ?>
    </body>
    <?php include "protected/view/footscripts.php"; ?>
</html>