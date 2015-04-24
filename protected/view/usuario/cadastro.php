<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>Acesso de Usuário</title>
    </head>
    <body>
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <form id="form-acesso-usuario" method="POST" data-base="ecidade" class="form-horizontal vertical-center center">
                    <fieldset>
                        <legend>Cadastro de Usuário</legend>
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
                                    <input id="cpf" name="cpf" type="text" placeholder="___.___.___-__" class="input-xlarge" required>
                                    <span class="add-on">
                                        <span class="ui-icon ui-icon-triangle-1-w"></span>
                                    </span>
                                </div>
                            </div>
                        </div>
                        <hr>
                        <div class="control-group">
                            <label class="control-label" for="nome">Nome</label>
                            <div class="controls">
                                <input id="nome" name="nome" type="text" placeholder="" class="input-xlarge" readonly>
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
                                <input id="cargo" name="cargo" type="text" class="input-xlarge" readonly>
                            </div>
                        </div>
                        <div class="control-group">
                            <label class="control-label" for="lotacao">Lotação</label>
                            <div class="controls">
                                <input id="lotacao" name="lotacao" type="text" class="input-xlarge" readonly>
                            </div>
                        </div>
                        <div class="control-group">
                            <label class="control-label" for="localdetrabalho">Local de Trabalho</label>
                            <div class="controls">
                                <input id="localdetrabalho" name="localdetrabalho" type="text" class="input-xlarge" readonly>
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
        <?php include "protected/view/footer.php"; ?>
        <?php include "protected/view/footscripts.php"; ?>
        <script src="/js/acesso.js"></script>
        <div id="acesso_dialog" class="ui-dialog-content ui-widget-content" style="width: auto; min-height: 19px; max-height: none; height: auto;"></div>
    </body>
</html>