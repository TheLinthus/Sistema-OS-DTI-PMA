<?php if (isset($response['mensagem'])) { ?>
    <script>
        alert("<?php echo $response['mensagem']; ?>");
        history.back();
    </script>
<?php }