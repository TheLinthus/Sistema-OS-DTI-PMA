<?php
if ($response['lastpage'] > 1) {
    $url = "/v/$mod/$act";
    foreach ($input['args'] as $key => $val) {
        if ($key !== "p") {
            $url .= "/$key/$val";
        }
    }
    ?>
    <div class='pages ui-buttonset'>
        <?php
        echo (
        "<a class='ui-button ui-corner-left' " .
        ($response['pageno'] == 1 ? "disabled" : "bt") . " " .
        ($response['pageno'] == 1 ? "" : "href='$url/" .
                ($response['pageno'] == 2 ? "" : "p/" . ($response['pageno'] - 1)) . "'") .
        ">Anterior</a>"
        );
        $i = $response['pageno'] > 4 ? $response['pageno'] - ($response['pageno'] < $response['lastpage'] - 3 ? 2 : (5 - ($response['lastpage'] - $response['pageno']))) : 1;
        $l = $response['pageno'] < $response['lastpage'] - 3 ? $response['pageno'] + ($response['pageno'] > 4 ? 2 : (6 - $response['pageno'])) : $response['lastpage'];
        if ($i > 1) {
            echo ("<a class='ui-button' href='$url/'>1</a>");
        }
        for ($i; $i <= $l; $i++) {
            if ($i == 1) {
                echo ("<a class='ui-button " . ($i == $response['pageno'] ? "ui-state-active" : "") . "' href='$url/'>1</a>");
            } else {
                echo ("<a class='ui-button " . ($i == $response['pageno'] ? "ui-state-active" : "") . "' href='$url/p/$i'>$i</a> ");
            }
        }
        if ($l < $response['lastpage']) {
            echo ("<a class='ui-button' href='$url/p/${response['lastpage']}'>${response['lastpage']}</a>");
        }
        echo (
        "<a class='ui-button ui-corner-right' " .
        ($response['pageno'] == $response['lastpage'] ? "disabled" : "") . " " .
        ($response['pageno'] == $response['lastpage'] ? "" : "href='$url/p/" . ($response['pageno'] + 1) . "'") .
        ">Próxima</a>"
        );
        ?>
    </div>
    <?php
}