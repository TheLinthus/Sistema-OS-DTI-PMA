<?php 

if (isset($response['mensagem'])) { ?>
    <div class="ui-widget">
        <div class="ui-state-<?php echo $response['mensagem']['tipo']; ?>
 ui-corner-all">
            <p>
                <span class="ui-icon ui-icon-<?php echo $response['mensagem']['icone']; ?>" style="float: left; margin-right: .3em;"></span>
                <strong><?php echo $response['mensagem']['titulo']; ?>
 </strong><?php echo $response['mensagem']['texto']; ?>
            </p>
        </div>
    </div>
<?php }