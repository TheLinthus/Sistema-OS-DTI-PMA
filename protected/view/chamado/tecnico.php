<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title><?php echo empty($response['data']) ? '(vazio)' : '(' . sizeof($response['data']) . ')'; ?>
 Chamados designados (Técnico)</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <legend>Chamados designados (Técnico)</legend>
                <?php include 'protected/view/mensagem.php'; ?>
                <div class="table-responsive">
                    <table id="chamados-table" data-md5="<?php echo $response['md5']; ?>" class="table table-striped table-bordered table-hover">
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
                                <tr id="chamado-<?php echo $chamado['id']; ?>" class="chamado-row"
                                    data-id="<?php echo $chamado['id']; ?>"
                                    data-prioridade="<?php echo $chamado['prioridade']; ?>"
                                    data-estado="<?php echo $chamado['estados'][0]['estado']; ?>"
                                    data-area="<?php echo $chamado['area']; ?>"
                                    data-usuario="<?php echo $chamado['usuario']; ?>"
                                    data-data="<?php echo $chamado['estados'][0]['data']; ?>"
                                    data-descricao="<?php echo $chamado['descricao']; ?>
 (<?php echo $chamado['problema']; ?>)"
                                    tabindex="0">
                                    <td class="span1"><?php echo $chamado['id']; ?></td>
                                    <td class="span1 more-info" title="<?php echo $chamado['estados'][0]['data']; ?>"><?php echo $chamado['estados'][0]['data']; ?></td>
                                    <td class="more-info" title="<?php echo $chamado['usuario']; ?>"><?php echo $chamado['usuario']; ?></td>
                                    <td class="span1"><?php echo $chamado['area']; ?></td>
                                    <td class="span1">
                                        <span class="ui-icon" style="background: <?php echo $chamado['corprioridade']; ?>"></span>
                                        <?php echo $chamado['nomeprioridade']; ?>
                                    </td>
                                    <td class="span3 more-info" title="<?php echo $chamado['descricao']; ?>
 (<?php echo $chamado['problema']; ?>)"><?php echo $chamado['descricao']; ?>
 (<?php echo $chamado['problema']; ?>)</td>
                                    <td class="span5 more-info" data-tipo="<?php echo $chamado['estados'][0]['tipo']; ?>" title="<?php echo $chamado['estados'][0]['estado']; ?>">
                                        <span class="ui-icon estado-<?php echo $chamado['estados'][0]['tipo']; ?>" title="<?php echo $chamado['estados'][0]['estado']; ?>"></span>
                                        <?php echo $chamado['estados'][0]['estado']; ?>
                                    </td>
                                </tr>
                            <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <audio id="notification"><source src="sounds/served.mp3"></audio>
            <?php include 'protected/view/footer.php'; ?>
            <?php include 'protected/view/footscripts.php'; ?>
        <script src="js/tecnico.js"></script>
    </body>
</html><?php 