<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Seletor de Usuário</title>
    </head>
    <body style="margin-bottom: 0;">
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <div class="control-group">
                    <label class="control-label" for="buscausuario">Buscar por Usuário</label>
                    <div class="controls">
                        <input id="buscausuario" name="buscausuario" type="text" placeholder="matricula, cpf, cgm ou nome do usuário" class="input-xxlarge">
                        <p class="help-block">Preencha o campo para a listagem</p>
                    </div>
                    <div class="controls">
                        <a class="button" id="nenhum">Nenhum</a>
                        <a class="button" id="cancelar">Cancelar</a>
                    </div>
                </div>
                <div class="table-responsive">
                    <table id="lista" class="table table-striped table-bordered table-hover" style="display: none;">
                        <thead>
                            <tr>
                                <th class="span2">Matricula</th>
                                <th class="span2">CGM</th>
                                <th>Nome</th>
                                <th class="span1">Selecionar</th>
                            </tr>
                        </thead>
                        <tbody>
                        </tbody>
                    </table>
                </div>
                <!---<div class="pagination"></div>-->
            </div>
        </div>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="/js/usuarioseletor.js"></script>
    </body>
</html><?php 