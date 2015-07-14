<?php
require_once './protected/controller/login.inc';
if (!isset($_SESSION)) {
    session_start();
}
?><div id="nav-menu">
    <ul>
        <li>
            <a href="/">Página Inicial</a>
        </li>
        <?php if (Login::isLogedIn()) { ?>
            <li><a>Chamados</a>
                <ul>
                    <li><a href="/v/chamado/cadastro/">Abrir Novo Chamado</a></li>
                    <?php if (Login::isTriagem()) { ?>
                        <li><a href="/v/chamado/triagem/">Triagem</a></li>
                        <?php } if ($_SESSION['nivel'] > 0) { ?>
                        <li><a href="/v/chamado/tecnico/">Chamados Designados</a></li>
                    <?php } ?>
                    <li><a href="/v/chamado/historico/">Histórico de Chamados</a></li>
                    <li><a href="/">Listar Todos os Chamados</a></li>
                </ul>
            </li>
            <?php if (Login::isDti()) { ?>
                <li>
                    <a>Base Local</a>
                    <ul>
                        <li><a href="/v/area/listar/">Áreas</a></li>
                        <li><a href="/v/patrimonio/listar/">Patrimônios</a></li>
                        <li><a href="/v/secretaria/listar/">Secretarias</a></li>
                        <li><a href="/v/setor/listar/">Setores</a></li>
                        <li><a href="/v/tecnico/listar/">Técnicos</a></li>
                        <li><a href="/v/usuario/listar/">Usuários</a></li>
                    </ul>
                </li>
                <li><a href="/v/pesquisa/">Pesquisar</a></li>
            <?php } ?>
            <li class="pull-right">
                <a href="/v/<?php echo $_SESSION['nivel'] > 0 ? 'tecnico' : 'usuario'; ?>/info/">
                    <?php if (Login::isDti()) { ?>
                        (Nível <?php echo $_SESSION['nivel']; ?>)
                    <?php } else { ?>
                        (Usuário)
                    <?php } ?>
                    <?php echo ($tmp = explode(' ', $_SESSION['nome'])) ? $tmp[0] : $tmp[0]; ?>
                </a>
                <ul>
                    <?php if (Login::isDti()) { ?>
                        <li><a href="/v/tecnico/senha">Alterar Senha</a></li>
                    <?php } ?>
                    <li><a href="/v/usuario/sair" onclick="return confirm('Você realmente quer sair?')">Sair</a></li>
                </ul>
            </li>
        <?php } else { ?>
            <li class="pull-right"><a href="/v/usuario/acesso">Entrar</a>
                <ul>
                    <li><a href="/v/usuario/acesso">Usuário</a>
                    <li><a href="/v/tecnico/acesso">Técnico</a></li>
                    <li><a href="/v/usuario/cadastro">Cadastro</a></li>
                </ul>
            </li>
        <?php } ?>
    </ul>
</div>