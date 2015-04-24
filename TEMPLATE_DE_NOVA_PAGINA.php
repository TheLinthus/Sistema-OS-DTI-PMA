<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>Titulo da Página</title> <!-- MODIFICAR TITULO DA PÁGINA AQUI -->
    </head>
    <body>
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <div class="pagination"></div>
                    
                    <!-- AQUI VAI OS DADOS PROCESSADO PELA CONTROLLER. Ex:
                    Nome: < ?= $response['data'][''] ?>
                    -->
                
                <?php // include "protected/view/paginacao.php"; ?> <!-- OPCIONAL! Para casos de listas com várias páginas -->
            </div>
            <div class="foot-pagination"></div>
        </div>
        <?php include "protected/view/footer.php"; ?>
        <?php include "protected/view/footscripts.php"; ?>
        <!--<script src="/js/abrirchamado.js"></script>-->  <!-- OPCIONAL! Se é necessário um script explusivo para essa página -->
    </body>
</html>