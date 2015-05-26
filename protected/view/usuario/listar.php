<!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include "protected/view/headscripts.php"; ?>
        <title>Listagem de Usuários (<?= empty($response['data']) ? "Vazio" : $response['pageno'] . "/" . $response['lastpage'] ?>)</title>
    </head>
    <body>
        <?php include "protected/view/nav.php"; ?>
        <div class="container">
            <?php include "protected/view/header.php"; ?>
            <div id="content">
                <!---<div class="pagination"></div>-->
                <legend>Usuários</legend>
                <div class="control-group">
                    <label class="control-label" for="pesquisa">Pesquisar por</label>
                    <div class="controls">
                        <div class="input-append">
                            <input id="pesquisa" name="pesquisa" class="input-xxlarge" placeholder="palavras chave"
                            <?= isset($response['pesquisa']) ? "autofocus" : "" ?>
                                   value="<?= isset($response['pesquisa']) ? $response['pesquisa'] : "" ?>" type="text">
                            <div class="btn-group">
                                <button class="btn buscar-bt" data-mod="usuario">
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
                                    <th>IP de Rede</th>
                                    <th>E-mail</th>
                                    <th colspan="4">Ações</th>
                                </tr>
                            </thead>
                            <tbody>
                                <?php foreach ($response['data'] as $usuario) { ?>
                                    <tr>
                                        <td><?= $usuario['cpf'] ?></td>
                                        <td class="more-info" title="<?= $usuario['nome'] ?>"><?= $usuario['nome'] ?></td>
                                        <td><?= $usuario['ip'] ?></td>
                                        <td><?= $usuario['email'] ?></td>
                                        <td class="span1">
                                            <a href="/v/usuario/info/cpf/<?= $usuario['cpf'] ?>" class="ui-icon ui-icon-contact" title="Detalhes">Detalhes</a>
                                        </td>
                                        <?php if ($_SESSION['nivel'] == 3) { ?>
                                            <td class="span1">
                                                <a href="/v/usuario/remove/cpf/<?= $usuario['cpf'] ?>" class="ui-icon ui-icon-trash"
                                                   onclick="return confirm('Você realmente quer remover esse usuário?\nTodas as OS relacionadas irão ser removidas também!')"
                                                   title="Remover">Remover</a>
                                            </td>
                                            <td class="span1">
                                                <a href="/v/usuario/alterar/cpf/<?= $usuario['cpf'] ?>" class="ui-icon ui-icon-pencil" title="Alterar dados do Usuário">Alterar</a>
                                            </td>
                                            <td class="span1">
                                                <a href="/v/tecnico/cadastro/cpf/<?= $usuario['cpf'] ?>" class="ui-icon ui-icon-person" title="Definir usuário como técnico">Técnico</a>
                                            </td>
                                        <?php } ?>
                                    </tr>
                                <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } ?>
                <?php include "protected/view/paginacao.php"; ?>
            </div>
            <!---<div class="foot-pagination"></div>-->
        </div>
        <?php include "protected/view/footer.php"; ?>
        <?php include "protected/view/footscripts.php"; ?>
    </body>
</html>