<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Listagem de Problemas de <?php echo $response['modulo']['modulo']; ?></title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <legend>Problemas de <a href="/v/modulo/listar/area/<?php echo $response['modulo']['area']; ?>"><?php echo $response['modulo']['modulo']; ?></a></legend>
                <?php include 'protected/view/mensagem.php'; ?>
                <?php if (!empty($response['data'])) { ?>
                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover">
                            <thead>
                                <tr>
                                    <th class="span1">#</th>
                                    <th>Problema</th>
                                    <th>Dica</th>
                                    <th colspan="3">Ações</th>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach ($response['data'] as $problema) { ?>
                                    <tr>
                                        <td><?php echo $problema['id']; ?></td>
                                        <td><?php echo $problema['problema']; ?></td>
                                        <td class="span8 more-info" title="<?php echo $problema['dica']; ?>">
                                            <?php echo $problema['dica']; ?>
                                        </td>
                                        <td class="span1">
                                            <a href="/v/problema/remove/id/<?php echo $problema['id']; ?>" class="ui-icon ui-icon-trash"
                                               onclick="return confirm('Você realmente quer remover esse problema?')"
                                               title="Remover">Remover</a>
                                        </td>
                                        <td class="span1">
                                            <a href="#" data-id="<?php echo $problema['id']; ?>" data-nome="<?php echo $problema['problema']; ?>" data-modulo="<?php echo $problema['modulo']; ?>" data-dica="<?php echo $problema['dica']; ?>" class="ren-problema ui-icon ui-icon-pencil" title="Renomear Problema">Alterar</a>
                                        </td>
                                    </tr>
                                <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } ?>
                <div id="problema-dialog" title="Problema">
                    <div class="control-group">
                        <input id="problema-id" name="problema-id" type="hidden">
                        <input id="problema-modulo" name="problema-modulo" type="hidden" value="<?php echo $response['modulo']['id']; ?>">
                        <label class="control-label" for="problema-nome">Nome do Problema</label>
                        <div class="controls">
                            <input id="problema-nome" name="problema-nome" maxlength="254" type="text" placeholder="nome" class="input-xlarge">
                        </div>
                        <label class="control-label" for="problema-dica">Dica do Problema (opcional)</label>
                        <div class="controls">
                            <input id="problema-dica" name="problema-dica" maxlength="254" type="text" placeholder="uma breve dica de solução (ex. tente antes ...)" class="input-xlarge">
                        </div>
                    </div>
                </div>
                <a class="new-problema button"><span class="ui-icon ui-icon-plusthick pull-left"></span>Adicionar novo Problema</a>
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="/js/problemaslista.js"></script>
    </body>
</html><?php 