<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Seletor de Patrimônio</title>
    </head>
    <body>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <div class="control-group">
                    <label class="control-label" for="buscapatrimonio">Buscar por Patrimônio</label>
                    <div class="controls">
                        <input id="buscapatrimonio" name="buscapatrimonio" type="text" placeholder="placa ou identificação do patrimônio" class="input-xlarge">
                    </div>
                    <div class="controls">
                        <a class="button" id="nenhum">Nenhum</a>
                        <a class="button" id="cancelar">Cancelar</a>
                    </div>
                </div>
                <div class="table-responsive">
                    <table class="table table-striped table-bordered table-hover">
                        <thead>
                            <tr>
                                <th class="span1">#</th>
                                <th>Placa</th>
                                <th>Descrição</th>
                                <th class="span1">Selecionar</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td>0</td>
                                <td>????</td>
                                <td>Não cadastrado</td>
                                <td><a href="#" class="patrimonio ui-icon ui-icon-check" data-id="0" data-descricao="Não cadastrado"></a></td>
                            </tr>
                        </tbody>
                    </table>
                </div>
                <!---<div class="pagination"></div>-->
            </div>
        </div>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="/js/patrimonioseletor.js"></script>
    </body>
</html><?php 