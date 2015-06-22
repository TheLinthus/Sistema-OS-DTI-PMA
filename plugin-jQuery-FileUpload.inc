<?php  ?><span class="bt">
    <span>Adicionar Imagens</span>
    <input id="fileupload" type="file" name="files[]" data-url="/fileUpload.php" accept="image/*" multiple>
</span>
<div id="progress">
    <div class="bar" style="width: 0%;"></div>
</div>
<div id="files" class="galery-links"></div>
<script src="/js/vendor/jquery.ui.widget.js"></script>
<script src="/js/jquery.iframe-transport.js"></script>
<script src="/js/jquery.fileupload.js"></script>
<script>
    var submitForm = false;
    var fotos = 0;
    $(function() {
        $('#fileupload').fileupload({
            dataType: 'json',
            acceptFileTypes: /(\.|\/)(gif|jpe?g|png)$/i,
            done: function(e, data) {
                $.each(data.result.files, function(index, file) {
                    var imagem = $("<img src='" + file.thumbnailUrl + "' class='thumbnail' title='" + file.name + "'/>");
                    var del = $("<span class='delete-file' title='DELETE: "+window.location.protocol + file.deleteUrl.substring(5)+"'>X</span>");
                    var link = $("<a href='" + file.url + "'/>");
                    $(del).click(function(e) {
                        e.stopPropagation();
                        e.preventDefault();
                        $.ajax({
                            url: window.location.protocol + file.deleteUrl.substring(5),
                            type: file.deleteType,
                            success: function(result) {
                                $(del).parent().remove();
                                fotos--;
                            }
                        });
                    });
                    $(link).append(imagem);
                    $(link).append(del);
                    if (file.error) {
                        alert(file.name + "\nErro: " + file.error);
                    } else {
                        $("#files").append(link);
                    }
                });
            },
            always: function() {
                $("#progress").css("display", "none");
            },
            progressall: function(e, data) {
                var progress = parseInt(data.loaded / data.total * 100, 10);
                $('#progress .bar').css(
                        'width',
                        progress + '%'
                        );
            },
            start: function() {
                $("#progress").css("display", "block");
            },
            fail: function(e, data) {
                alert("Erro no Upload de Arquivo:\n" + data.errorThrown);
            },
            add: function(e, data) {
                if (fotos >= 3) {
                    alert("Não são permitidos upload de mais que 3 fotos!");
                } else {
                    fotos++;
                    data.submit();
                }
            }
        });
        $.ajax({
            url: $('#fileupload').fileupload('option', 'url'),
            dataType: 'json'
        }).done(function(result) {
            $.each(result.files, function(index, file) {
                fotos++;
                var imagem = $("<img src='" + file.thumbnailUrl + "' class='thumbnail' title='" + file.name + "'/>");
                var del = $("<span class='delete-file' title='DELETE: "+window.location.protocol + file.deleteUrl.substring(5)+"'>X</span>");
                var link = $("<a href='" + file.url + "'/>");
                $(del).click(function(e) {
                    e.stopPropagation();
                    e.preventDefault();
                    $.ajax({
                        url: window.location.protocol + file.deleteUrl.substring(5),
                        type: file.deleteType,
                        success: function(result) {
                            $(del).parent().remove();
                            fotos--;
                        }
                    });
                });
                $(link).append(imagem);
                $(link).append(del);
                if (file.error) {
                    alert(file.name + "\nErro: " + file.error);
                } else {
                    $("#files").append(link);
                }
            });
        });
        $("#submitForm").click(function(e) {
            submitForm = true;
        })
        $(window).bind('beforeunload', function() {
            if (!submitForm) { 
                $(".delete-file").click();
            }
        });
    });
</script>
<?php 