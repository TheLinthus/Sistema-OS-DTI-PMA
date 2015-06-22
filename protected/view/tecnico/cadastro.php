<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Cadastrar Técnico</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <form id="form-cadastro-tecnico" method="POST" class="form-horizontal vertical-center center">
                    <fieldset>
                        <legend>Cadastro de Técnico</legend>
                        <?php include 'protected/view/mensagem.php'; ?>
                        <div class="control-group">
                            <label class="control-label" for="cpf">CPF</label>
                            <div class="controls">
                                <div class="input-append">
                                    <input id="cpf" name="cpf" type="text" 
                                           <?php echo isset($input['args']['cpf']) ? 'value="' . $input['args']['cpf'] . '"' : ''; ?>
                                           <?php echo isset($input['args']['cpf']) ? 'readonly' : ''; ?>
                                           placeholder="___.___.___-__" class="input-xlarge" required>
                                    <span class="add-on">
                                        <span class="ui-icon ui-icon-triangle-1-w"></span>
                                    </span>
                                </div>
                                <a href="/v/usuario/listar/">Procurar por Usuário</a>
                            </div>
                        </div>
                        <hr>
                        <div class="control-group">
                            <label class="control-label" for="telefone">Telefone</label>
                            <div class="controls">
                                <input id="telefone" name="telefone" type="text" placeholder="__ ________" class="input-xlarge">
                            </div>
                        </div>
                        <div class="control-group">
                            <div class="controls">
                                <button class="btn btn-primary row-fluid">Cadastrar</button>
                            </div>
                        </div>
                    </fieldset>
                </form>
            </div>
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="/js/acesso.js"></script>
    </body>
</html><?php 