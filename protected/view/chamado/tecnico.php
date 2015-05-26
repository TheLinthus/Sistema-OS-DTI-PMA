<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title><?= empty($response['data']) ? "(vazio)" : "(" . sizeof($response['data']) . ")" ?> Chamados designados (Técnico)</title>
    </head>
    <body>
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <legend>Chamados designados (Técnico)</legend>
                <?php include 'protected/view/mensagem.php'; ?>
                <div class="table-responsive">
                    <table id="chamados-table" data-md5="<?= $response['md5'] ?>" class="table table-striped table-bordered table-hover">
                        <thead>
                            <tr>
                                <th>ID</th>
                                <th>Data</th>
                                <th>Usuário</th>
                                <th>Área</th>
                                <th>Prioridade</th>
                                <th class="more-info">Descrição (Problema)</th>
                                <th>Estado</th>
                            </tr>
                        </thead>
                        <tbody>
                            <?php foreach ($response['data'] as $chamado) { ?>
                                <tr id="chamado-<?= $chamado['id'] ?>" class="chamado-row"
                                    data-id="<?= $chamado['id'] ?>"
                                    data-prioridade="<?= $chamado['prioridade'] ?>"
                                    data-estado="<?= $chamado['estados'][0]['estado'] ?>"
                                    data-area="<?= $chamado['area'] ?>"
                                    data-usuario="<?= $chamado['usuario'] ?>"
                                    data-data="<?= $chamado['estados'][0]['data'] ?>"
                                    data-descricao="<?= $chamado['descricao'] ?> (<?= $chamado['problema'] ?>)"
                                    tabindex="0">
                                    <td class="span1"><?= $chamado['id'] ?></td>
                                    <td class="span1 more-info" title="<?= $chamado['estados'][0]['data'] ?>"><?= $chamado['estados'][0]['data'] ?></td>
                                    <td class="more-info" title="<?= $chamado['usuario'] ?>"><?= $chamado['usuario'] ?></td>
                                    <td class="span1"><?= $chamado['area'] ?></td>
                                    <td class="span1">
                                        <span class="ui-icon" style="background: <?= $chamado['corprioridade'] ?>"></span>
                                        <?= $chamado['nomeprioridade'] ?>
                                    </td>
                                    <td class="span4 more-info" title="<?= $chamado['descricao'] ?> (<?= $chamado['problema'] ?>)"><?= $chamado['descricao'] ?> (<?= $chamado['problema'] ?>)</td>
                                    <td class="span3 more-info" data-tipo="<?= $chamado['estados'][0]['tipo'] ?>" title="<?= $chamado['estados'][0]['estado'] ?>">
                                        <span class="ui-icon estado-<?= $chamado['estados'][0]['tipo'] ?>" title="<?= $chamado['estados'][0]['estado'] ?>"></span>
                                        <?= $chamado['estados'][0]['estado'] ?>
                                    </td>
                                </tr>
                            <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <audio id="notification"><source src="/sounds/served.mp3"></audio>
            <?php include "protected/view/footer.php"; ?>
            <?php include "protected/view/footscripts.php"; ?>
        <script src="/js/tecnico.js"></script>
    </body>
</html>