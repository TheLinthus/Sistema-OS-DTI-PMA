<?php if (isset($response['mensagem'])) { ?>
    <div class="ui-widget">
        <div class="ui-state-<?= $response['mensagem']['tipo'] ?> ui-corner-all">
            <p>
                <span class="ui-icon ui-icon-<?= $response['mensagem']['icone'] ?>" style="float: left; margin-right: .3em;"></span>
                <strong><?= $response['mensagem']['titulo'] ?> </strong><?= $response['mensagem']['texto'] ?>
            </p>
        </div>
    </div>
<?php } ?>