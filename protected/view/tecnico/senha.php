<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Alteração de Senha</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <form method="POST" class="form-horizontal vertical-center center">
                    <fieldset>
                        <legend>Alteração de Senha</legend>
                        <?php include 'protected/view/mensagem.php'; ?>
                        <div class="control-group">
                            <label class="control-label" for="antiga">Senha Antiga*</label>
                            <div class="controls">
                                <input id="antiga" name="antiga" type="password" placeholder="sua senha atual" class="input-xlarge" required="">
                            </div>
                        </div>
                        <div class="control-group">
                            <label class="control-label" for="nova1">Nova Senha*</label>
                            <div class="controls">
                                <input id="nova1" name="nova1" type="password" placeholder="sua nova senha" class="input-xlarge" required="">
                            </div>
                        </div>
                        <div class="control-group">
                            <label class="control-label" for="nova2">Confirmar Senha*</label>
                            <div class="controls">
                                <input id="nova2" name="nova2" type="password" placeholder="Repita a nova senha" class="input-xlarge" required="">
                            </div>
                        </div>
                        <div class="control-group">
                            <div class="controls">
                                <button class="btn btn-primary row-fluid">Alterar</button>
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