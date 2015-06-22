<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Patrimônios (<?php echo empty($response['data']) ? 'Vazio' : $response['pageno'] . '/' . $response['lastpage']; ?>)</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <legend>Patrimonios</legend>
                <div class="control-group">
                    <label class="control-label" for="pesquisa">Pesquisar por</label>
                    <div class="controls">
                        <div class="input-append">
                            <input id="pesquisa" name="pesquisa" class="input-xxlarge" placeholder="palavras chave"
                            <?php echo isset($response['pesquisa']) ? 'autofocus' : ''; ?>
                                   value="<?php echo isset($response['pesquisa']) ? $response['pesquisa'] : ''; ?>" type="text">
                            <div class="btn-group">
                                <button class="btn buscar-bt" data-mod="patrimonio">
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
                                    <th>Placa</th>
                                    <th>Descrição</th>
                                    <th>Observações</th>
                                    <th colspan="3">Ações</th>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach ($response['data'] as $patrimonio) { ?>
                                    <tr>
                                        <td><?php echo $patrimonio['id']; ?></td>
                                        <td><?php echo $patrimonio['placa']; ?></td>
                                        <td class="more-info" title="<?php echo $patrimonio['descricao']; ?>"><?php echo $patrimonio['descricao']; ?></td>
                                        <td class="more-info" title="<?php echo $patrimonio['observacoes']; ?>"><?php echo $patrimonio['observacoes']; ?></td>
                                        <?php if ($_SESSION['nivel'] == 3) { ?>
                                            <td class="span1">
                                                <a href="/v/patrimonio/remove/id/<?php echo $patrimonio['id']; ?>"class="ui-icon ui-icon-trash"
                                                   onclick="return confirm('Você realmente quer excluir esse patrimônio da base local?/nPara isso não deve haver nenhuma Ordem de Serviço relacionada.')"
                                                   title="Excluir Patrimônico da Base Local">Remover Patrimônio</a>
                                            </td>
                                            <td class="span1">
                                                <a href="#"
                                                   data-id="<?php echo $patrimonio['id']; ?>"
                                                   data-placa="<?php echo $patrimonio['placa']; ?>"
                                                   data-descricao="<?php echo $patrimonio['descricao']; ?>"
                                                   data-observacoes="<?php echo $patrimonio['observacoes']; ?>"
                                                   class="ren-patrimonio ui-icon ui-icon-pencil" title="Alterar dados do Patrimônio">Alterar</a>
                                            </td>
                                        <?php } ?>
                                        <td class="span1">
                                            <a href="/v/chamado/listar/patrimonio/<?php echo $patrimonio['id']; ?>" class="ui-icon ui-icon-folder-open" title="Exibir chamados do Patrimônio">Chamados</a>
                                        </td>
                                    </tr>
                                <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } ?>
                <div id="patrimonio-dialog" title="Patrimônio">
                    <div class="control-group">
                        <input id="id" name="id" type="hidden">
                        <label class="control-label" for="placa">Placa</label>
                        <div class="controls">
                            <input id="placa" name="placa" type="text" placeholder="placa" class="input-xlarge">
                        </div>
                        <label class="control-label" for="descricao">Descrição</label>
                        <div class="controls">                     
                            <textarea id="descricao" name="descricao" class="input-xlarge"></textarea>
                        </div>
                        <label class="control-label" for="observacoes">Obeservações</label>
                        <div class="controls">                     
                            <textarea id="observacoes" name="observacoes" class="input-xlarge"></textarea>
                        </div>
                    </div>
                </div>
                <div class="control-group">
                    <div class="controls">
                        <a href="/v/patrimonio/cadastro/" class="button new-patrimonio"><span class="new-patrimonio ui-icon ui-icon-plusthick pull-left"></span>Adicionar novo patrimônio *</a>
                    </div>
                </div>
                <?php include 'protected/view/paginacao.php'; ?>
                <p>* Os patrimônios são inseridos de acordo com a criação de OS. A inserção manual deve ser feita apenas para patrimônios não existentes na base do e-cidade.</p>
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="/js/patrimonioslista.js"></script>
    </body>
</html><?php 