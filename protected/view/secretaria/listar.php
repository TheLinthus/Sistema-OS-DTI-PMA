<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>Listagem de Unidades</title>
    </head>
    <body>
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <legend>Secretarias</legend>
                <div class="control-group">
                    <label class="control-label" for="pesquisa">Pesquisar por</label>
                    <div class="controls">
                        <div class="input-append">
                            <input id="pesquisa" name="pesquisa" class="input-xxlarge" placeholder="palavras chave"
                            <?= isset($response['pesquisa']) ? "autofocus" : "" ?>
                                   value="<?= isset($response['pesquisa']) ? $response['pesquisa'] : "" ?>" type="text">
                            <div class="btn-group">
                                <button class="btn buscar-bt" data-mod="secretaria">
                                    Pesquisar
                                </button>
                            </div>
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
                                    <th>Unidade</th>
                                    <?php if ($_SESSION['nivel'] == 3) { ?>
                                        <th colspan="2">Ações</th>
                                    <?php } ?>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach ($response['data'] as $secretaria) { ?>
                                    <tr>
                                        <td><?= $secretaria['id'] ?></td>
                                        <td><?= $secretaria['secretaria'] ?></td>
                                        <?php if ($_SESSION['nivel'] == 3) { ?>
                                            <td class="span1">
                                                <a href="/v/secretaria/remove/id/<?= $secretaria['id'] ?>" class="ui-icon ui-icon-trash"
                                                   onclick="return confirm('Você realmente quer remover essa Secretaria?\nTodos os Setores relacionados serão removidos também!')"
                                                   title="Remover">Remover</a>
                                            </td>
                                            <td class="span1">
                                                <a href="#" data-id="<?= $secretaria['id'] ?>" data-nome="<?= $secretaria['secretaria'] ?>" class="ren-secretaria ui-icon ui-icon-pencil" title="Renomear Secretaria">Alterar</a>
                                            </td>
                                        <?php } ?>
                                    </tr>
                                <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } ?>
                <div id="secretaria-dialog" title="Secretaria">
                    <div class="control-group">
                        <label class="control-label" for="secretaria-id">ID</label>
                        <div class="controls">
                            <input id="secretaria-id" maxlength="4" name="secretaria-id" type="text" placeholder="#" class="input-mini" required="">
                        </div>
                        <label class="control-label" for="secretaria-nome">Nome da Secretaria</label>
                        <div class="controls">
                            <input id="secretaria-nome" name="secretaria-nome" type="text" placeholder="nome" class="input-xlarge">
                        </div>
                    </div>
                </div>
                <div class="control-group">
                    <div class="controls">
                        <a href="/v/secretaria/cadastro/" class="button new-secretaria"><span class="ui-icon ui-icon-plusthick pull-left"></span>Adicionar nova Secretaria</a>
                    </div>
                </div>
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <?php include "protected/view/footer.php"; ?>
        <?php include "protected/view/footscripts.php"; ?>
        <script src="/js/secretariaslista.js"></script>
    </body>
</html>