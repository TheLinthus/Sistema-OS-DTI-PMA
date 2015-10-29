<?php  ?><!DOCTYPE html>
<html lang="pt">
    <head>
        <?php include 'protected/view/headscripts.php'; ?>
        <title>Técnico -> Áreas</title>
    </head>
    <body>
        <?php include 'protected/view/nav.php'; ?>
        <div class="container">
            <?php include 'protected/view/header.php'; ?>
            <div id="content">
                <form id="form-areas" method="POST" class="form-horizontal vertical-center center">
                    <fieldset>
                        <legend>Técnico &#8633; Áreas</legend>
                        <?php include 'protected/view/mensagem.php'; ?>
                        <div class="control-group">
                            <label class="control-label" for="areas">Áreas Disponiveis</label>
                            <div class="controls">
                                <select id="areas" name="remover[]" class="input-xlarge" size="5" multiple>
                                    <?php foreach ($response['data']['areas'] as $id => $area) { ?>
                                        <option class="new-area" value="<?php echo $id; ?>"><?php echo $area['area']; ?>
 (<?php echo $area['nivel']; ?>)</option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="control-group">
                            <div class="controls">
                                <button id="add-area" class="btn row-fluid half-by-side" disabled>↧Adicionar↧</button>
                                <button id="rmv-area" class="btn row-fluid half-by-side" disabled>↥Remover↥</button>
                            </div>
                        </div>
                        <div class="control-group">
                            <label class="control-label" for="tecnico-areas">Áreas do Técnico</label>
                            <div class="controls">
                                <select id="tecnico-areas" name="areas[]" class="input-xlarge" size="5" multiple>
                                    <?php foreach ($response['data']['tecnico']['areas'] as $id => $area) { ?>
                                        <option class="old-area" value="<?php echo $id; ?>"><?php echo $area['area']; ?>
 (<?php echo $area['nivel']; ?>)</option>
                                    <?php } ?>
                                </select>
                            </div>
                        </div>
                        <div class="control-group">
                            <div class="controls">
                                <button id="salvar" class="btn btn-primary row-fluid">Salvar</button>
                            </div>
                        </div>
                        <div>
                            <div class="controls">
                                <a href="v/tecnico/listar/" class="btn row-fluid">Voltar</a>
                            </div>
                        </div>
                    </fieldset>
                </form>
            </div>
        </div>
        <?php include 'protected/view/footer.php'; ?>
        <?php include 'protected/view/footscripts.php'; ?>
        <script src="js/areastecnico.js"></script>
    </body>
</html><?php 