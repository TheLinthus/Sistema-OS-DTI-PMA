<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>Site em construção</title>
    </head>
    <body class="alert-working">
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <div class="vertical-center pagination-centered">
                    <div class="container">
                        <img src="/img/men-at-work.png"/>
                        <h1 id="info-code">Em construção</h1>
                        <div class="ui-widget">
                            <div class="ui-state-default ui-corner-all">
                                <p>
                                    <span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>
                                    Site está em construção, por favor utilize somente as funções implementadas (Encontradas na barra de navegação superior)
                                </p>
                            </div>
                            <?php include "protected/view/tecnologia.php"; ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </body>
    <?php include "protected/view/footscripts.php"; ?>
</html>