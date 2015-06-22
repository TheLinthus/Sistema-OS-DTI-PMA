<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Listagem de Técnicos (<?php echo empty($response['data']) ? 'Vazio' : $response['pageno'] . '/' . $response['lastpage']; ?>)</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <legend>Técnicos</legend>
                <div class="control-group">
                    <label class="control-label" for="pesquisa">Pesquisar por</label>
                    <div class="controls">
                        <div class="input-append">
                            <input id="pesquisa" name="pesquisa" class="input-xxlarge" placeholder="palavras chave"
                            <?php echo isset($response['pesquisa']) ? 'autofocus' : ''; ?>
                                   value="<?php echo isset($response['pesquisa']) ? $response['pesquisa'] : ''; ?>" type="text">
                            <div class="btn-group">
                                <button class="btn buscar-bt" data-mod="tecnico">
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
                                    <th>CPF</th>
                                    <th>Nome</th>
                                    <th>Matricula</th>
                                    <th>CGM</th>
                                    <th>E-mail</th>
                                    <th colspan="4">Ações</th>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach ($response['data'] as $usuario) { ?>
                                    <tr>
                                        <td><?php echo $usuario['cpf']; ?></td>
                                        <td class="more-info" title="<?php echo $usuario['nome']; ?>"><?php echo $usuario['nome']; ?></td>
                                        <td><?php echo $usuario['matricula']; ?></td>
                                        <td><?php echo $usuario['cgm']; ?></td>
                                        <td><?php echo $usuario['email']; ?></td>
                                        <td class="span1">
                                            <a href="/v/tecnico/info/cpf/<?php echo $usuario['cpf']; ?>"class="ui-icon ui-icon-contact" title="Detalhes">Exibir Dados do Usuário (Detalhes)</a>
                                        </td>
                                        <?php if ($_SESSION['nivel'] == 3) { ?>
                                            <td class="span1">
                                                <a href="/v/tecnico/remove/cpf/<?php echo $usuario['cpf']; ?>"class="ui-icon ui-icon-closethick" onclick="return confirm('Você realmente quer transoformar esse técnico em um usuário normal?')" title="Remover Privilégios de Técnico (Mudar para Usuário Comum)">Remover Privilégios de Técnico</a>
                                            </td>
                                            <td class="span1">
                                                <a href="/v/usuario/alterar/cpf/<?php echo $usuario['cpf']; ?>"class="ui-icon ui-icon-pencil" title="Alterar dados do Usuário">Alterar</a>
                                            </td>
                                            <td class="span1">
                                                <a href="/v/tecnico/areas/cpf/<?php echo $usuario['cpf']; ?>"class="ui-icon ui-icon-transferthick-e-w" title="Alterar Áreas do Técnico">Áreas</a>
                                            </td>
                                        <?php } ?>
                                    </tr>
                                <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } ?>
                <a href="/v/tecnico/cadastro/" class="button"><span class="ui-icon ui-icon-plusthick pull-left"></span>Adicionar novo Técnico</a>
                <?php include 'protected/view/paginacao.php'; ?>
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
    </body>
</html><?php 