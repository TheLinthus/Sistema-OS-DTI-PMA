<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>Listagem de Modulos</title>
    </head>
    <body>
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <div class="pagination"></div>
                <legend>Áreas</legend>
                <?php include 'protected/view/mensagem.php'; ?>
                <?php if (!empty($response['data'])) { ?>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover">
                            <thead>
                                <tr>
                                    <th class="span1">#</th>
                                    <th>Área</th>
                                    <th class="span1">Nível</th>
                                    <?php if ($_SESSION['nivel'] >= 2) { ?>
                                        <th colspan="3">Ações</th>
                                    <?php } ?>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach ($response['data'] as $area) { ?>
                                    <tr>
                                        <td style="background-color: <?= isset($_SESSION['areas'][$area['id']]) ? "#eeffee" : "#ffeeee" ?>;"><?= $area['id'] ?></td>
                                        <td><?= $area['area'] ?></td>
                                        <td><?= $area['nivel'] ?></td>
                                        <?php if ($_SESSION['nivel'] == 3) { ?>
                                            <td class="span1">
                                                <a href="/v/area/remove/id/<?= $area['id'] ?>" class="ui-icon ui-icon-trash"
                                                   onclick="return confirm('Você realmente quer remover essa área?\nTodos os técnicos perderão relação com a área selecionada!')"
                                                   title="Remover">Remover</a>
                                            </td>
                                            <td class="span1">
                                                <a href="#" data-id="<?= $area['id'] ?>" data-nome="<?= $area['area'] ?>" data-nivel="<?= $area['nivel'] ?>" class="ren-area ui-icon ui-icon-pencil" title="Renomear e alterar nível da Área">Alterar</a>
                                            </td>
                                        <?php } ?>
                                        <?php if ($_SESSION['nivel'] >= 2) { ?>
                                            <td class="span1">
                                                <?php if (isset($_SESSION['areas'][$area['id']])) { ?>
                                                    <a href="/v/modulo/listar/area/<?= $area['id'] ?>" data-id="<?= $area['id'] ?>" class="ui-icon ui-icon-folder-open" title="Cadastrar e Editar Modulos">Cadastrar Modulos</a>
                                                <?php } ?>
                                            </td>
                                        <?php } ?>
                                    </tr>
                                <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } ?>
                <div id="area-dialog" title="Área">
                    <div class="control-group">
                        <input id="area-id" name="area-id" type="hidden" class="input-large">
                        <label class="control-label" for="area-nome">Nome da Área</label>
                        <div class="controls">
                            <input id="area-nome" name="area-nome" type="text" placeholder="nome" class="input-large">
                        </div>
                        <label class="control-label" for="area-nivel">Nível</label>
                        <div class="controls">
                            <input id="area-nivel" name="area-nivel" type="text" class="input-large">
                        </div>
                    </div>
                </div>
                <a class="new-area button"><span class="ui-icon ui-icon-plusthick pull-left"></span>Adicionar nova Área</a>
            </div>
            <div class="foot-pagination"></div>
        </div>
        <?php include "protected/view/footer.php"; ?>
        <?php include "protected/view/footscripts.php"; ?>
        <script src="/js/areaslista.js"></script>
    </body>
</html>