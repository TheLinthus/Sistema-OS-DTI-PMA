<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Informações do Técnico</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <form class="form-horizontal vertical-center center">
                    <fieldset>
                        <legend>Informações do Usuário</legend>
                        <?php include 'protected/view/mensagem.php'; ?>
                        <?php if (isset($response['data'])) { ?>
                            <div class="control-group">
                                <label class="control-label" for="matricula">Matricula</label>
                                <div class="controls">
                                    <input id="matricula" name="matricula" type="text" value="<?php echo $response['data']['matricula']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="cgm">ou CGM</label>
                                <div class="controls">
                                    <input id="cgm" name="cgm" type="text" value="<?php echo $response['data']['cgm']; ?>" class="input-xlarge natural" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="cpf">CPF</label>
                                <div class="controls">
                                    <input id="cpf" name="cpf" type="text" value="<?php echo $response['data']['cpf']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <hr>
                            <div class="control-group">
                                <label class="control-label" for="nome">Nome</label>
                                <div class="controls">
                                    <input id="nome" name="nome" type="text" value="<?php echo $response['data']['nome']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="email">E-mail</label>
                                <div class="controls">
                                    <input id="email" name="email" type="text" value="<?php echo $response['data']['email']; ?>" placeholder="email@dominio.com" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="telefone">Telefone</label>
                                <div class="controls">
                                    <input id="telefone" name="telefone" type="text" value="<?php echo $response['data']['telefone']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="cargo">Cargo</label>
                                <div class="controls">
                                    <input id="cargo" name="cargo" type="text" value="<?php echo $response['data']['cargo']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="lotacao">Lotação</label>
                                <div class="controls">
                                    <input id="lotacao" name="lotacao" type="text" value="<?php echo $response['data']['lotacao']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="localdetrabalho">Local de Trabalho</label>
                                <div class="controls">
                                    <input id="localdetrabalho" name="localdetrabalho" type="text" value="<?php echo $response['data']['localdetrabalho']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="ip">IP de Rede</label>
                                <div class="controls">
                                    <input id="ip" name="ip" type="text" value="<?php echo $response['data']['ip']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="areas">Areas</label>
                                <div class="controls">
                                    <select id="areas" name="areas" class="input-xlarge" multiple="multiple" readonly>
                                        <?php foreach ($response['data']['areas'] as $id => $area) { ?>
                                            <option><?php echo $area['area']; ?>
 (<?php echo $area['nivel']; ?>)</option>
                                        <?php } ?>
                                    </select>
                                </div>
                            </div>
                        <?php } ?>
                    </fieldset>
                </form>
            </div>
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="/js/acesso.js"></script>
    </body>
</html><?php 