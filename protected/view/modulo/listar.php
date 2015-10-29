<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Listagem de Módulos de <?php echo $response['area']['area']; ?></title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <legend>Módulos de <a href="v/area/listar"><?php echo $response['area']['area']; ?></a></legend>
                <?php include 'protected/view/mensagem.php'; ?>
                <?php if (!empty($response['data'])) { ?>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover">
                            <thead>
                                <tr>
                                    <th class="span1">#</th>
                                    <th>Módulo</th>
                                    <th colspan="3">Ações</th>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach ($response['data'] as $modulo) { ?>
                                    <tr>
                                        <td><?php echo $modulo['id']; ?></td>
                                        <td><?php echo $modulo['modulo']; ?></td>
                                        <td class="span1">
                                            <a href="v/modulo/remove/id/<?php echo $modulo['id']; ?>" class="ui-icon ui-icon-trash"
                                               onclick="return confirm('Você realmente quer remover esse modulo?')"
                                               title="Remover">Remover</a>
                                        </td>
                                        <td class="span1">
                                            <a href="#" data-id="<?php echo $modulo['id']; ?>" data-nome="<?php echo $modulo['modulo']; ?>" data-area="<?php echo $modulo['area']; ?>" class="ren-modulo ui-icon ui-icon-pencil" title="Renomear Módulo">Alterar</a>
                                        </td>
                                        <td class="span1">
                                            <a href="v/problema/listar/modulo/<?php echo $modulo['id']; ?>" data-id="<?php echo $modulo['id']; ?>" class="ui-icon ui-icon-folder-open" title="Cadastrar e Editar Problemas">Cadastrar Problemas</a>
                                        </td>
                                    </tr>
                                <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } ?>
                <div id="modulo-dialog" title="Módulo">
                    <div class="control-group">
                        <input id="modulo-id" name="modulo-id" type="hidden" class="input-large">
                        <input id="modulo-area" name="modulo-area" type="hidden" value="<?php echo $response['area']['id']; ?>" class="input-large">
                        <label class="control-label" for="modulo-nome">Nome do Módulo</label>
                        <div class="controls">
                            <input id="modulo-nome" name="modulo-nome" type="text" placeholder="nome" class="input-large">
                        </div>
                    </div>
                </div>
                <a class="new-modulo button"><span class="ui-icon ui-icon-plusthick pull-left"></span>Adicionar novo Módulo</a>
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="js/moduloslista.js"></script>
    </body>
</html><?php 