<?php ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Chamado <?php echo $response['data']['id']; ?></title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <form id="ver-chamado" class="form-horizontal vertical-center center">
                    <fieldset>
                        <legend>Informações do Chamado <?php echo $response['data']['id']; ?></legend>
                        <?php include 'protected/view/mensagem.php'; ?>
                        <?php if (isset($response['data'])) { ?>
                            <div class="control-group">
                                <label class="control-label" for="datacriacao">Data de Criação</label>
                                <div class="controls">
                                    <input id="datacriacao" type="text" value="<?php echo $response['data']['estado'][0]['data']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="secretaria">Secretaria</label>
                                <div class="controls">
                                    <input id="secretaria" type="text" value="<?php echo $response['data']['secretaria']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="setor">Setor</label>
                                <div class="controls">
                                    <input id="setor" type="text" value="<?php echo $response['data']['setor']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="iporigem">IP de origem</label>
                                <div class="controls">
                                    <input id="iporigem" type="text" value="<?php echo $response['data']['iporigem']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <hr>
                            <div class="control-group">
                                <label class="control-label" for="area">Área</label>
                                <div class="controls">
                                    <input id="area" type="text" value="<?php echo $response['data']['area']; ?>" class="input-xlarge natural" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="modulo">Modulo</label>
                                <div class="controls">
                                    <input id="modulo" type="text" value="<?php echo $response['data']['modulo']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="problema">Problema</label>
                                <div class="controls">
                                    <input id="problema" type="text" value="<?php echo $response['data']['problema']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="prioridade">Prioridade</label>
                                <div class="controls">
                                    <input id="prioridade" type="text" value="<?php echo $response['data']['nomeprioridade']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="descricao">Descrição</label>
                                <div class="controls">
                                    <textarea id="descricao" class="input-xlarge" disabled><?php echo $response['data']['descricao']; ?></textarea>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="solucao">Solução</label>
                                <div class="controls">
                                    <textarea id="solucao" class="input-xlarge" disabled><?php echo $response['data']['solucao']; ?></textarea>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="patrimonio">Patrimônio</label>
                                <div class="controls">
                                    <input id="patrimonio" type="text" value="<?php echo $response['data']['patrimonio']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <hr>
                            <div class="control-group">
                                <label class="control-label" for="usuario">Usuário</label>
                                <div class="controls">
                                    <input id="usuario" type="text" value="<?php echo $response['data']['usuario']['nome']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="ip">Endereço IP</label>
                                <div class="controls">
                                    <input id="ip" type="text" value="<?php echo $response['data']['usuario']['ip']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <hr>
                            <div class="control-group">
                                <label class="control-label">Histórico</label>
                                <div class="controls">
                                    <?php foreach ($response['data']['estado'] as $estado) { ?>
                                        <input type="text"
                                               value="(<?php echo $estado['data']; ?>) <?php echo $estado['estado']; ?>"
                                               class="input-xlarge" disabled/>
                                           <?php } ?>
                                </div>
                            </div>
                            <hr>
                            <div class="control-group">
                                <label class="control-label">Anexos</label>
                                <div class="controls attachments">
                                    <?php for ($i = 0; $i < 5; $i++) { ?>
                                        <?php if (isset($response['arquivos'][$i])) { ?>
                                            <div class="attachment file-<?php echo $response['arquivos'][$i]['tipo']; ?>"
                                                 title="<?php echo $response['arquivos'][$i]['nome']; ?>"
                                                 tabindex="0" data-id="<?php echo $response['arquivos'][$i]['id']; ?>"></div>
                                             <?php } else { ?>
                                            <div class="attachment" disabled></div>
                                        <?php } ?>
                                    <?php } ?>
                                </div>
                            </div>
                            <div>
                            <?php } ?>

                        </div>
                    </fieldset>
                </form>

            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="js/verchamado.js"></script>
    </body>
</html><?php 