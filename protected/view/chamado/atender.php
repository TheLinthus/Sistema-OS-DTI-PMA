<?php ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Chamado <?php echo $input['args']['id']; ?></title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <form id="atender-chamado" class="form-horizontal vertical-center center">
                    <?php include 'protected/view/mensagem.php'; ?>
                    <fieldset>
                        <?php if (isset($response['data'])) { ?>
                            <legend>Atendimento: Chamado <?php echo $response['data']['id']; ?></legend>
                            <input id="id" type="hidden" value="<?php echo $response['data']['id']; ?>"/>
                            <div class="control-group no-margin-bottom">
                                <label class="control-label" for="datacriacao">Data de Criação</label>
                                <div class="controls">
                                    <input id="datacriacao" type="text" value="<?php echo $response['data']['estado'][0]['data']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group no-margin-bottom">
                                <label class="control-label" for="secretaria">Secretaria</label>
                                <div class="controls">
                                    <input id="secretaria" type="text" value="<?php echo $response['data']['secretaria']; ?>" class="input-xlarge"  disabled>
                                </div>
                            </div>
                            <div class="control-group no-margin-bottom">
                                <label class="control-label" for="setor">Setor</label>
                                <div class="controls">
                                    <input id="setor" type="text" value="<?php echo $response['data']['setor']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group no-margin-bottom">
                                <label class="control-label" for="iporigem">IP de origem</label>
                                <div class="controls">
                                    <input id="iporigem" type="text" value="<?php echo $response['data']['iporigem']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <hr>
                            <div class="control-group">
                                <label class="control-label" for="area">Área</label>
                                <div class="controls">
                                    <select id="area" name="area" class="input-xlarge" <?php echo (($tmp = end($response['data']['estado'])) ? $tmp['nivel'] : $tmp['nivel']) > 1 ? 'disabled' : ''; ?>>
                                        <option value="null">Selecione</option>
                                        <?php foreach ($response['areas'] as $area) { ?>
                                            <?php if ($area['nivel'] == '2') { ?>
                                                <option value="<?php echo $area['id']; ?>" <?php echo $area['id'] == $response['data']['area']['id'] ? 'selected' : ''; ?>>
                                                    <?php echo $area['area']; ?>
                                                </option>
                                            <?php } ?>
                                        <?php } ?>
                                    </select>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="modulo">Modulo</label>
                                <div class="controls">
                                    <select id="modulo" name="modulo" class="input-xlarge" <?php echo $response['data']['modulo']['id'] == -1 ? 'readonly' : ''; ?>>
                                        <option class="modulo-area-null" value="null">Selecione</option>
                                        <?php foreach ($response['modulos'] as $modulo) { ?>
                                            <option value="<?php echo $modulo['id']; ?>"
                                                    class="modulo-area-<?php echo $modulo['area']; ?>"
                                                    <?php echo $modulo['id'] == $response['data']['modulo']['id'] ? 'selected' : ''; ?>>
                                                        <?php echo $modulo['modulo']; ?>
                                            </option>
                                        <?php } ?>
                                    </select>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="problema">Problema</label>
                                <div class="controls">
                                    <select id="problema" name="problema" class="input-xlarge" <?php echo $response['data']['problema']['id'] == -1 ? 'readonly' : ''; ?>>
                                        <option class="problema-modulo-null" value="null">Selecione</option>
                                        <?php foreach ($response['problemas'] as $problema) { ?>
                                            <option value="<?php echo $problema['id']; ?>"
                                                    class="problema-modulo-<?php echo $problema['modulo']; ?>"
                                                    <?php echo $problema['id'] == $response['data']['problema']['id'] ? 'selected' : ''; ?>>
                                                        <?php echo $problema['problema']; ?>
                                            </option>
                                        <?php } ?>
                                    </select>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="prioridade">Prioridade</label>
                                <div class="controls">
                                    <div class="input-append">
                                        <div id="prioridade-slider" class="input-xlarge"></div>
                                        <span id="prioridade-value" class="add-on"><?php echo $response['data']['prioridade']; ?>%</span>
                                        <input id="prioridade" name="prioridade" type="hidden" value="<?php echo $response['data']['prioridade']; ?>">
                                    </div>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="descricao">Descrição</label>
                                <div class="controls">
                                    <textarea id="descricao" name="descricao" class="input-xlarge"><?php echo $response['data']['descricao']; ?></textarea>
                                </div>
                            </div>
                            <div class="control-group">
                                <label class="control-label" for="solucao">Solução</label>
                                <div class="controls">
                                    <textarea id="solucao" name="solucao" class="input-xlarge"><?php echo $response['data']['solucao']; ?></textarea>
                                </div>
                            </div>
                            <div class="control-group"><label class="control-label" for="patrimonio">Patrimônio</label>
                                <div class="controls">
                                    <div class="input-append">
                                        <input id="patrimonio-descricao" name="patrimonio-descricao" class="input-xlarge"
                                               placeholder="(opcional) patrimônio relacionado" type="text"
                                               value="<?php echo $response['data']['patrimonio']['descricao']; ?>" readonly>
                                        <input id="patrimonio" name="patrimonio"type="hidden">
                                        <div class="btn-group">
                                            <button id="patrimonio-btn" class="btn" data-mod="usuario">
                                                Selecionar
                                            </button>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <hr>
                            <div class="control-group no-margin-bottom">
                                <label class="control-label" for="usuario">Usuário</label>
                                <div class="controls">
                                    <input id="usuario" type="text" value="<?php echo $response['data']['usuario']['nome']; ?>" class="input-xlarge" disabled>
                                </div>
                            </div>
                            <div class="control-group no-margin-bottom">
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
                                            <label id="file-<?php echo $i; ?>-label" for="file-<?php echo $i; ?>" class="attachment file-<?php echo $response['arquivos'][$i]['tipo']; ?>"
                                                   title="<?php echo $response['arquivos'][$i]['nome']; ?>" tabindex="0"
                                                   data-id="<?php echo $response['arquivos'][$i]['id']; ?>" data-index="<?php echo $i; ?>"></label>
                                               <?php } else { ?>
                                            <label id="file-<?php echo $i; ?>-label" for="file-<?php echo $i; ?>" class="attachment" tabindex="0"></label>
                                        <?php } ?>
                                        <input id="file-<?php echo $i; ?>" data-i="<?php echo $i; ?>" name="file-<?php echo $i; ?>" class="input-file customfile-input" type="file">
                                    <?php } ?>
                                </div>
                            </div>
                            <div class="control-group">
                                <a id="save-chamado" class="button">
                                    <span class="ui-icon ui-icon-disk pull-left"></span>
                                    Salvar Alterações
                                </a>
                                <a id="forward-chamado" class="button">
                                    <span class="ui-icon ui-icon-seek-next pull-left"></span>
                                    <?php echo (($tmp = end($response['data']['estado'])) ? $tmp['tipo'] : $tmp['tipo']) == 5 ? 'Atender' : 'Encaminhar'; ?>
                                </a>
                                <?php if ((($tmp = end($response['data']['estado'])) ? $tmp['tipo'] : $tmp['tipo']) != 5) { ?>
                                    <a id="finish-chamado" class="button">
                                        <span class="ui-icon ui-icon-check pull-left"></span>
                                        Finalizar Chamado
                                    </a>
                                <?php } ?>
                            </div>
                        <?php } ?>
                    </fieldset>
                </form>
            </div>
            <div id="file-dialog" title="Arquivo">
                Escolha uma ação:
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="/js/atenderchamado.js"></script>
    </body>
</html><?php 