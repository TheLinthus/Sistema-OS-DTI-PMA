<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>Abrir Chamado</title>
    </head>
    <body>
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <form action="/v/chamado/confirmar" id="form-abrir-chamado" method="POST" enctype="multipart/form-data" class="form-horizontal">
                    <div id="step-box" class="etapa1">
                        <fieldset id="step-1" data-etapa="1" class="step active">
                            <legend>Abrir Chamado - Selecionar Secretaria</legend>
                            <div class="flex-container">
                                <div class="flex-grid">
                                    <input id="secretaria" type="hidden" name="secretaria" value="<?= $response['sec'] ?>">
                                    <?php foreach ($response['secretarias'] as $secretaria) { ?>
                                        <a class="item secretaria next-step <?= $response['sec'] == $secretaria['id'] ? "active" : "" ?>"
                                           data-id="<?= $secretaria['id'] ?>"
                                           href="/v/chamado/cadastro/sec/<?= $secretaria['id'] ?>">
                                               <?= $secretaria['secretaria'] ?>
                                        </a>
                                    <?php } ?>
                                    <!-- Os div's seguintes são para ajustar tamanho dos últimos itens do flex-grid -->
                                    <div></div>
                                    <div></div>
                                    <div></div>
                                    <div></div>
                                    <div></div>
                                </div>
                            </div>
                            <?php if (empty($response['secretarias'])) { ?>
                                <div class="ui-widget row-fluid">
                                    <div class="ui-state-erro ui-corner-all">
                                        <p>
                                            <span class="ui-icon ui-icon-alert" style="float: left; margin-right: .3em;"></span>
                                            <strong>Vazio </strong>A lista de secretarias está vazia, erro interno. Por favor contate o DTI.
                                        </p>
                                    </div>
                                </div>
                            <?php } ?>
                            <footer style="display: none;">
                                <div class="ui-widget pull-right">
                                    <div class="ui-state-highlight ui-corner-all">
                                        <p>
                                            <span class="ui-icon ui-icon-help" style="float: left; margin-right: .3em"></span>
                                            Selecione sua SECRETARIA para Continuar
                                        </p>
                                    </div>
                                </div>
                            </footer>
                        </fieldset>
                        <fieldset id="step-2" data-etapa="2" class="step <?= $response['etapa'] < 2 ? "hide" : "active"; ?>">
                            <legend>Abrir Chamado - Selecionar Setor</legend>
                            <div class="flex-container">
                                <div class="flex-grid">
                                    <input id="setor" type="hidden" name="setor" value="<?= $response['set'] ?>">
                                    <?php foreach ($response['setores'] as $setor) { ?>
                                        <a class="item setor next-step <?= $response['set'] == $setor['id'] ? "active" : "" ?>"
                                           data-id="<?= $setor['id'] ?>"
                                           data-secretaria="<?= $setor['secretaria'] ?>"
                                           href="/v/chamado/cadastro/sec/<?= $setor['secretaria'] ?>/set/<?= $setor['id'] ?>">
                                               <?= $setor['setor'] ?>
                                        </a>
                                    <?php } ?>
                                    <!-- Os div's seguintes são para ajustar tamanho dos últimos itens do flex-grid -->
                                    <div></div>
                                    <div></div>
                                    <div></div>
                                    <div></div>
                                    <div></div>
                                </div>
                            </div>
                            <?php if (empty($response['setores'])) { ?>
                                <div class="warn ui-widget row-fluid">
                                    <div class="ui-state-highlight ui-corner-all">
                                        <p>
                                            <span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>
                                            Primeiramente selecione a secretaria.
                                        </p>
                                    </div>
                                </div>
                            <?php } ?>
                            <footer style="display: none;">
                                <a href="/v/chamado/cadastro" class="button prev-step input-large">Voltar</a>
                                <div class="ui-widget pull-right">
                                    <div class="ui-state-highlight ui-corner-all">
                                        <p>
                                            <span class="ui-icon ui-icon-help" style="float: left; margin-right: .3em"></span>
                                            Selecione seu SETOR para Continuar
                                        </p>
                                    </div>
                                </div>
                            </footer>
                        </fieldset>
                        <fieldset id="step-3" data-etapa="3" class="step <?= $response['etapa'] < 3 ? "hide" : "active"; ?>">
                            <legend>Abrir Chamado - Preencha o formulário</legend>
                            <div class="flex-container">
                                <div class="control-group">
                                    <label class="control-label" for="area">Área Responsavel</label>
                                    <div class="controls">
                                        <span id="areas">
                                            <?php foreach ($response['areas'] as $area) { ?>
                                                <?php if ($area['nivel'] == 2) { // Exibe apenas áreas de nível 2  ?>
                                                    <input 
                                                        type="radio" id="area-<?= $area['id'] ?>"
                                                        value="<?= $area['id'] ?>" name="area"
                                                        <?= $response['area'] == $area['id'] ? "checked" : "" ?>
                                                        class="ui-helper-hidden-accessible"/>
                                                    <label for="area-<?= $area['id'] ?>">
                                                        <a 
                                                            data-id="<?= $area['id'] ?>"
                                                            href="/v/chamado/cadastro/sec/<?= $response['sec'] ?>/set/<?= $response['set'] ?>/area/<?= $area['id'] ?>">
                                                                <?= $area['area'] ?>
                                                        </a>
                                                    </label>
                                                <?php } ?>
                                            <?php } ?>
                                            <input
                                                type="radio" id="unknow-area" value="-1"
                                                name="area" <?= $response['area'] == -1 ? "checked" : "" ?>
                                                class="ui-helper-hidden-accessible"/>
                                            <label for="unknow-area" style="display: none;">
                                                Não informar
                                            </label>
                                        </span>
                                        <p class="help-block">Selecione a área do problema ou não informe caso não tenha certeza</p>
                                    </div>
                                </div>
                                <div id="modulo-control"
                                     class="control-group"
                                     <?= $response['area'] < 0 ? "style='display: none;'" : "" ?>>
                                    <label class="control-label" for="modulo">Modulo</label>
                                    <div class="controls">
                                        <span id="modulos">
                                            <?php foreach ($response['modulos'] as $modulo) { ?>
                                                <input 
                                                    type="radio" id="modulo-<?= $modulo['id'] ?>"
                                                    value="<?= $modulo['id'] ?>" name="modulo"
                                                    <?= $response['mod'] == $modulo['id'] ? "checked" : "" ?>
                                                    class="ui-helper-hidden-accessible" data-area="<?= $modulo['area'] ?>"
                                                    <?= $modulo['area'] != $response['area'] ? "style='display: none;'" : "" ?>/>
                                                <label for="modulo-<?= $modulo['id'] ?>" data-area="<?= $modulo['area'] ?>"
                                                       <?= $modulo['area'] != $response['area'] ? "style='display: none;'" : "" ?>>
                                                    <a 
                                                        data-id="<?= $modulo['id'] ?>"
                                                        href="/v/chamado/cadastro/sec/<?= $response['sec'] ?>/set/<?= $response['set'] ?>/area/<?= $modulo['area'] ?>/prob/<?= $modulo['id'] ?>">
                                                            <?= $modulo['modulo'] ?>
                                                    </a>
                                                </label>
                                            <?php } ?>
                                            <input
                                                type="radio" id="unknow-modulo" value="-1"
                                                name="modulo" <?= $response['mod'] == -1 ? "checked" : "" ?>
                                                class="ui-helper-hidden-accessible"/>
                                            <label for="unknow-modulo" style="display: none;">
                                                Não informar
                                            </label>
                                        </span>
                                        <p class="help-block">Selecione o modulo da lista ou não informe caso não tenha certeza</p>
                                    </div>
                                </div>
                                <div id="problema-control"
                                     class="control-group"
                                     <?= $response['mod'] < 0 ? "style='display: none;'" : "" ?>>
                                    <label class="control-label" for="problema">Problema</label>
                                    <div class="controls">
                                        <span id="problemas">
                                            <?php foreach ($response['problemas'] as $problema) { ?>
                                                <input 
                                                    type="radio" id="problema-<?= $problema['id'] ?>"
                                                    value="<?= $problema['id'] ?>" name="problema"
                                                    <?= $response['problema'] == $problema['id'] ? "checked" : "" ?>
                                                    class="ui-helper-hidden-accessible" data-modulo="<?= $problema['modulo'] ?>"
                                                    <?= $problema['modulo'] != $response['mod'] ? "style='display: none;'" : "" ?>
                                                    title="<?= $problema['problema'] ?>" data-dica="<?= $problema['dica'] ?>"/>
                                                <label for="problema-<?= $problema['id'] ?>" data-modulo="<?= $problema['modulo'] ?>"
                                                       title="<?= $problema['dica'] ?>"
                                                       <?= $problema['modulo'] != $response['mod'] ? "style='display: none;'" : "" ?>>
                                                           <?php if (!empty($problema['dica'])) { ?>
                                                        <span class="ui-icon ui-icon-lightbulb pull-left"></span>
                                                    <?php } ?>
                                                    <a 
                                                        data-id="<?= $problema['id'] ?>"
                                                        href="/v/chamado/cadastro/sec/<?= $response['sec'] ?>/set/<?= $response['set'] ?>/area/<?= $response['area'] ?>/mod/<?= $problema['mod'] ?>/prob/<?= $problema['id'] ?>">
                                                            <?= $problema['problema'] ?>
                                                    </a>
                                                </label>
                                            <?php } ?>
                                            <input
                                                type="radio" id="unknow-problema" value="-1"
                                                name="problema" <?= $response['problema'] == -1 ? "checked" : "" ?>
                                                class="ui-helper-hidden-accessible"/>
                                            <label for="unknow-problema" style="display: none;">
                                                Outro
                                            </label>
                                        </span>
                                        <p class="help-block">Selecione o problema da lista ou/e descreva outro problema</p>
                                    </div>
                                </div>
                                <div class="control-group">
                                    <label class="control-label" for="descricao">Descrição do Problema (mín. 10 caracteres)</label>
                                    <div class="controls">                     
                                        <textarea id="descricao" name="descricao" class="input-xxlarge"></textarea>
                                    </div>
                                </div>
                                <div class="control-group">
                                    <label class="control-label" for="file-1">Arquivo 1</label>
                                    <div class="controls">
                                        <input id="file-1" name="file-1" class="input-file" type="file">
                                    </div>
                                    <label class="control-label" for="file-2">Arquivo 2</label>
                                    <div class="controls">
                                        <input id="file-2" name="file-2" class="input-file" type="file">
                                    </div>
                                    <label class="control-label" for="file-3">Arquivo 3</label>
                                    <div class="controls">
                                        <input id="file-3" name="file-3" class="input-file" type="file">
                                    </div>
                                    <label class="control-label" for="file-4">Arquivo 4</label>
                                    <div class="controls">
                                        <input id="file-4" name="file-4" class="input-file" type="file">
                                    </div>
                                    <label class="control-label" for="file-5">Arquivo 5</label>
                                    <div class="controls">
                                        <input id="file-5" name="file-5" class="input-file" type="file">
                                    </div>
                                </div>
                                <div class="control-group">
                                    <label class="control-label" for="patrimonio">Patrimônio</label>
                                    <div class="controls">
                                        <div class="input-append">
                                            <input id="patrimonio-descricao" name="patrimonio-descricao" class="input-xlarge" placeholder="(opcional) patrimônio relacionado" type="text" readonly>
                                            <input id="patrimonio" name="patrimonio"type="hidden">
                                            <div class="btn-group">
                                                <button id="patrimonio-btn" class="btn" data-mod="usuario">
                                                    Selecionar
                                                </button>
                                            </div>
                                        </div>
                                        <p class="help-block">Selecione o patrimônio com problema, caso seja um problema em um patrimônio.</p>
                                    </div>
                                </div>
                                <div id="placa-div" class="control-group" style="display: none;">
                                    <label class="control-label" for="placa">Placa</label>
                                    <div class="controls">
                                        <input id="placa" name="placa" type="text" placeholder="" class="input-xlarge">
                                        <p class="help-block">Informe a placa do patrimônio, se houver</p>
                                    </div>
                                </div>
                                <div id="prioridade-control"
                                     class="control-group">
                                    <label class="control-label" for="prioridade">Prioridade</label>
                                    <div class="controls">
                                        <span id="prioridades">
                                            <input id="prioridade-baixa"
                                                   type="radio" value="1"
                                                   name="prioridade" checked
                                                   class="ui-helper-hidden-accessible"/>
                                            <label for="prioridade-baixa">
                                                Baixa
                                            </label>
                                            <input id="prioridade-media"
                                                   type="radio" value="2"
                                                   name="prioridade"
                                                   class="ui-helper-hidden-accessible"/>
                                            <label for="prioridade-media">
                                                Média
                                            </label>
                                            <input id="prioridade-alta"
                                                   type="radio" value="3"
                                                   name="prioridade"
                                                   class="ui-helper-hidden-accessible"/>
                                            <label for="prioridade-alta">
                                                Alta
                                            </label>
                                        </span>
                                        <p class="help-block">Selecione a prioridade, selecione opções da alta prioridade somente se necessario</p>
                                    </div>
                                </div>
                            </div>
                            <footer>
                                <a href="#" class="button prev-step input-large" style="display: none;">Voltar</a>
                                <input type="submit" id="enviar-chamado"
                                       class="button btn-primary next-step input-large ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only"
                                       value="Enviar"/>
                            </footer>
                        </fieldset>
                    </div>
                </form>
            </div>
        </div>
        <?php include "protected/view/footer.php"; ?>
        <?php include "protected/view/footscripts.php"; ?>
        <script src="/js/abrirchamado.js"></script>
    </body>
</html>