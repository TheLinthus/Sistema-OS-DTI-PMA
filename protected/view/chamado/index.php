<?php ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title><?php echo empty($response['data']) ? '(vazio)' : '(' . sizeof($response['data']) . ')'; ?>
            Chamados</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <legend>Chamados</legend>
                <form id="filtros">
                    <div class="control-group">
                        <label class="control-label" for="pesquisa">Pesquisar por</label>
                        <div class="controls">
                            <div class="input-append">
                                <input id="pesquisa" name="pesquisa" class="input-xxlarge" placeholder="(id, descrição, ip de origem, dados do usuário, patrimônio)"
                                <?php echo isset($response['pesquisa']) ? 'autofocus' : ''; ?>
                                       value="<?php echo isset($response['pesquisa']) ? $response['pesquisa'] : ''; ?>" type="text">
                                <div class="btn-group">
                                    <button class="btn buscar-bt" data-mod="chamado" data-act="index">
                                        Pesquisar
                                    </button>
                                </div>
                            </div>
                        </div>
<!--                        <label for="dataA">Periodo</label>
                        <div class="controls">
                            <input id="dataA" name="dataA" type="date" class="input-medium"
                                   value="<?php echo isset($response['dataA']) ? $response['dataA'] : date("Y-m-d", time() - 3600 * 24 * 30); ?>">
                            <span>até</span>
                            <input id="dataB" name="dataB" type="date" class="input-medium"
                                   value="<?php echo isset($response['dataB']) ? $response['dataB'] : date("Y-m-d"); ?>">
                        </div>-->
                        <label class="control-label" id="filtro-estado">Estado</label>
                        <div class="controls">
                            <label class="checkbox inline" for="filtro-estado-1">
                                <input type="checkbox" id="filtro-estado-1" value="1" checked="true">
                                Aberto
                            </label>
                            <label class="checkbox inline" for="filtro-estado-2">
                                <input type="checkbox" id="filtro-estado-2" value="2" checked="true">
                                Atendimento
                            </label>
                            <label class="checkbox inline" for="filtro-estado-5">
                                <input type="checkbox" id="filtro-estado-5" value="5"  checked="true">
                                Terceirizado
                            </label>
                            <label class="checkbox inline" for="filtro-estado-3">
                                <input type="checkbox" id="filtro-estado-3" value="3">
                                Pronto/Fechado
                            </label>
                            <label class="checkbox inline" for="filtro-estado-4">
                                <input type="checkbox" id="filtro-estado-4" value="4">
                                Baixa
                            </label>
                        </div>
                    </div>
                </form>
                <?php include 'protected/view/mensagem.php'; ?>
                <div class="table-responsive">
                    <table id="chamados-table" data-md5="<?php echo $response['md5']; ?>" class="table table-striped table-bordered table-hover">
                        <thead>
                            <tr>
                                <th id="id" data-column="id"><span>ID</span></th>
                                <th id="data" data-column="id"><span>Data</span></th>
                                <th id="usuario" data-column="usuario_cpf"><span>Usuário</span></th>
                                <th id="area" data-column="area"><span>Área</span></th>
                                <th id="prioridade" class="sorted-desc" data-column="prioridade"><span>Prioridade</span></th>
                                <th class="more-info">Descrição (Problema)</th>
                                <th id="estado" data-column="id"><span>Estado</span></th>
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
                                    (<?php echo $chamado['problema']; ?>
                                    )"
                                    tabindex="0">
                                    <td class="span1"><?php echo $chamado['id']; ?>
                                    </td>
                                    <td class="span1 more-info" title="<?php echo $chamado['estados'][0]['data']; ?>"><?php echo $chamado['estados'][0]['data']; ?>
                                    </td>
                                    <td class="more-info" title="<?php echo $chamado['usuario']; ?>"><?php echo $chamado['usuario']; ?>
                                    </td>
                                    <td class="span1"><?php echo $chamado['area']; ?>
                                    </td>
                                    <td class="span1">
                                        <span class="ui-icon" style="background: <?php echo $chamado['corprioridade']; ?>"></span>
                                        <?php echo $chamado['nomeprioridade']; ?>
                                    </td>
                                    <td class="span3 more-info" title="<?php echo $chamado['descricao']; ?> (<?php echo $chamado['problema']; ?>)">
                                        <?php echo $chamado['descricao']; ?> (<?php echo $chamado['problema']; ?> )</td>
                                    <td class = "span5 more-info" 
                                        data-tipo = "<?php echo $chamado['estados'][0]['tipo']; ?>" 
                                        title = "<?php echo $chamado['estados'][0]['estado']; ?>">
                                        <span class = "ui-icon estado-<?php echo $chamado['estados'][0]['tipo']; ?>" title = "<?php echo $chamado['estados'][0]['estado']; ?>"></span>
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
        <audio id="notification"><source src="/sounds/served.mp3"></audio>
            <?php include 'protected/view/footer.php'; ?>
            <?php include 'protected/view/footscripts.php'; ?>
        <script src="/js/chamados.js"></script>
    </body>
</html><?php 