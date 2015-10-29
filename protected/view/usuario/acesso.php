<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Acesso de Usuário</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <form action="v/usuario/acesso" id="form-acesso-usuario" data-base="local" method="POST" class="form-horizontal vertical-center center">
                    <fieldset>
                        <legend>Acesso Usuário</legend>
                        <?php include 'protected/view/mensagem.php'; ?>
                        <div class="control-group">
                            <label class="control-label" for="matricula">Matricula</label>
                            <div class="controls">
                                <div class="input-append">
                                    <input id="matricula" name="matricula" type="text" placeholder="Nº da matricula ou CGM" class="input-xlarge">
                                    <span class="add-on">
                                        <span class="ui-icon ui-icon-triangle-1-w"></span>
                                    </span>
                                </div>
                            </div>
                        </div>
                        <div class="control-group">
                            <label class="control-label" for="cgm">ou CGM</label>
                            <div class="controls">
                                <div class="input-append">
                                    <input id="cgm" name="cgm" type="text" placeholder="CGM ou nº da matricula" class="input-xlarge natural">
                                    <span class="add-on">
                                        <span class="ui-icon ui-icon-triangle-1-w"></span>
                                    </span>
                                </div>
                            </div>
                        </div>
                        <div class="control-group">
                            <label class="control-label" for="cpf">CPF</label>
                            <div class="controls">
                                <div class="input-append">
                                    <input id="cpf" name="cpf" type="text" placeholder="___.___.___-__" class="input-xlarge">
                                    <span class="add-on">
                                        <span class="ui-icon ui-icon-triangle-1-w"></span>
                                    </span>
                                </div>
                            </div>
                        </div>
                        <div class="control-group">
                            <div class="controls">
                                <button class="btn btn-primary row-fluid">Acessar</button>
                            </div>
                        </div>
                        <div class="control-group">
                            <div class="controls">
                                <a href="v/usuario/cadastro" class="btn row-fluid">Cadastrar</a>
                            </div>
                        </div>
                    </fieldset>
                </form>
            </div>
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="js/acesso.js"></script>
        <div id="acesso_dialog" class="ui-dialog-content ui-widget-content" style="width: auto; min-height: 19px; max-height: none; height: auto;"></div>
    </body>
</html><?php 