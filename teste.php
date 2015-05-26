<?php

function _normalise($path, $encoding = "UTF-8") {

    // Attempt to avoid path encoding problems.
    $path = iconv($encoding, "$encoding//IGNORE//TRANSLIT", $path);

    // Process the components
    $parts = explode('/', $path);
    $safe = array();
    foreach ($parts as $idx => $part) {
        if (empty($part) || ('.' == $part)) {
            continue;
        } elseif ('..' == $part) {
            array_pop($safe);
            continue;
        } else {
            $safe[] = $part;
        }
    }

    // Return the "clean" path
    $path = implode(DIRECTORY_SEPARATOR, $safe);
    return $path;
}
?>
<html>
    <head>
        <title>TESTES</title>
        <script src="/js/jquery-1.11.1.min.js"></script>
    </head>
    <body>
        <pre style="padding: 5px; background-color: lightgray; border: 1px solid gray;"><?php var_dump(realpath("./js/jquery-1.11.1.min.js")); ?></pre>
        <p id="t"></p>
	<span><?php echo  mysql_escape_string("Kae'l"); ?></span>
    </body>
    <script>$("#t").html("it works!");</script>
</html>
