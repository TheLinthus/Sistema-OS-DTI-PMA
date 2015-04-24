<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>Listagem de Setores</title>
    </head>
    <body>
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <div class="pagination"></div>
                <legend>Setores</legend>
                <div class="control-group">
                    <label class="control-label" for="pesquisa">Pesquisar por</label>
                    <div class="controls">
                        <div class="input-append">
                            <input id="pesquisa" name="pesquisa" class="input-xxlarge" placeholder="palavras chave"
                            <?= isset($response['pesquisa']) ? "autofocus" : "" ?>
                                   value="<?= isset($response['pesquisa']) ? $response['pesquisa'] : "" ?>" type="text">
                            <div class="btn-group">
                                <button class="btn buscar-bt" data-mod="setor">
                                    Pesquisar
                                </button>
                            </div>
                        </div>
                    </div>
                    <div class="control-group">
                        <label class="control-label" for="filtro">Filtrar por Secretaria</label>
                        <div class="controls">
                            <select id="filtro" name="filtro" class="input-xxlarge">
                                <option <?= isset($response['filtro']) ? "" : "selected" ?>>NENHUMA</option>
                                <optgroup label="Secretarias">
                                    <?php foreach ($response['secretarias'] as $id => $secretaria) { ?>
                                        <option 
                                        <?= isset($response['filtro']) && $response['filtro'] == $id ? "selected" : "" ?>
                                            value="<?= $id ?>" class="filtro">
                                                <?= $secretaria ?>
                                        </option>
                                    <?php } ?>
                                </optgroup>
                            </select>
                        </div>
                    </div>
                </div>
                <?php include 'protected/view/mensagem.php'; ?>
                <?php if (!empty($response['data'])) { ?>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover">
                            <thead>
                                <tr>
                                    <th class="span1">#</th>
                                    <th>Setor</th>
                                    <th class="span4">Secretaria</th>
                                    <th class="span1">Escola</th>
                                    <th class="span2">Telefone</th>
                                    <th class="span1 more-info" title="Prioridade">Pr.</th>
                                    <?php if ($_SESSION['nivel'] == 3) { ?>
                                        <th colspan="2">Ações</th>
                                    <?php } ?>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach ($response['data'] as $setor) { ?>
                                    <tr>
                                        <td><?= $setor['id'] ?></td>
                                        <td><?= $setor['setor'] ?></td>
                                        <td class="more-info" title="<?= $response['secretarias'][$setor['secretaria']] ?>">
                                            <?= $response['secretarias'][$setor['secretaria']] ?>
                                        </td>
                                        <td><?= $setor['escola'] ? "SIM" : "NÃO" ?></td>
                                        <td><?= $setor['telefone'] ?></td>
                                        <td><?= $setor['prioridade'] ?></td>
                                        <?php if ($_SESSION['nivel'] == 3) { ?>
                                            <td class="span1">
                                                <a href="/v/setor/remove/id/<?= $setor['id'] ?>" class="ui-icon ui-icon-trash"
                                                   onclick="return confirm('Você realmente quer remover esse setor?\nTodos os chamados perderão relação com o setor!')"
                                                   title="Remover">Remover</a>
                                            </td>
                                            <td class="span1">
                                                <a href="#"
                                                   data-id="<?= $setor['id'] ?>"
                                                   data-nome="<?= $setor['setor'] ?>"
                                                   data-secretaria="<?= $setor['secretaria'] ?>"
                                                   data-escola="<?= $setor['escola'] ?>"
                                                   data-telefone="<?= $setor['telefone'] ?>"
                                                   data-prioridade="<?= $setor['prioridade'] ?>"
                                                   class="ren-setor ui-icon ui-icon-pencil" title="Renomear e alterar valores do Setor">Alterar</a>
                                            </td>
                                        <?php } ?>
                                    </tr>
                                <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } ?>
                <div id="setor-dialog" title="Setor">
                    <div class="control-group">
                        <input id="setor-id" name="setor-id" type="hidden">
                        <div class="control-group">
                            <label class="control-label" for="secretaria-id">Secretaria</label>
                            <div class="controls">
                                <select id="secretaria-id" name="secretaria-id" class="input-xlarge">
                                    <?php foreach ($response['secretarias'] as $id => $secretaria) { ?>
                                        <option value="<?= $id ?>">
                                            <?= $secretaria ?>
                                        </option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <label class="control-label" for="setor-nome">Nome do Setor</label>
                        <div class="controls">
                            <input id="setor-nome" name="setor-nome" type="text" placeholder="nome" class="input-xlarge">
                        </div>
                        <label class="control-label" for="telefone">Telefone</label>
                        <div class="controls">
                            <input id="telefone" name="telefone" type="text" placeholder="__ ________" class="input-xlarge">
                        </div>
                        <label class="control-label" for="prioridade">Prioridade</label>
                        <div class="controls">
                            <input id="prioridade" name="prioridade" type="number" min="1" max="10" value="1" class="input-xlarge">
                        </div>
                        <label class="control-label" for="escola">Escola</label>
                        <div class="controls">
                            <label class="radio inline" for="escola-0">
                                <input type="radio" name="escola" id="escola-0" value="0" checked="checked">
                                Não
                            </label>
                            <label class="radio inline" for="escola-1">
                                <input type="radio" name="escola" id="escola-1" value="1">
                                Sim
                            </label>
                        </div>
                    </div>
                </div>
                <div class="control-group">
                    <div class="controls">
                        <a href="/v/setor/cadastro/" class="button new-setor"><span class="ui-icon ui-icon-plusthick pull-left"></span>Adicionar novo Setor</a>
                    </div>
                </div>
                <?php include "protected/view/paginacao.php"; ?>
            </div>
            <div class="foot-pagination"></div>
        </div>
        <?php include "protected/view/footer.php"; ?>
        <?php include "protected/view/footscripts.php"; ?>
        <script src="/js/setoreslista.js"></script>
    </body>
</html>