<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Pesquisar em Base Local</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <legend>Pesquisar em Base Local</legend>
                <?php include 'protected/view/mensagem.php'; ?>
                <div class="control-group">
                    <label class="control-label" for="pesquisa">Pesquisar por</label>
                    <div class="controls">
                        <div class="input-append">
                            <input id="pesquisa" name="pesquisa" class="input-xlarge" placeholder="palavras chave"
                                   value="<?php echo isset($response['pesquisa']) ? $response['pesquisa'] : ''; ?>" type="text">
                            <div class="btn-group">
                                <button class="btn dropdown-toggle" data-toggle="dropdown">
                                    Buscar
                                    <span class="caret"></span>
                                </button>
                                <ul class="dropdown-menu">
                                    <li><a class="buscar-bt" data-mod="patrimonio">Patrimônio</a></li>
                                    <li><a class="buscar-bt" data-mod="setor">Setor</a></li>
                                    <li><a class="buscar-bt" data-mod="tecnico">Técnico</a></li>
                                    <li><a class="buscar-bt" data-mod="secretaria">Secretaria</a></li>
                                    <li><a class="buscar-bt" data-mod="usuario">Usuário</a></li>
                                    <li><a class="buscar-bt" data-mod="chamado" data-act="index">Ordem de Serviço</a></li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
    </body>
</html><?php 