var method = "";
$(document).ready(function () {
    $("#secretaria-dialog").dialog({
        autoOpen: false,
        resizable: false,
        width: 315,
        buttons: {
            "Salvar": function () {
                var id = $("#secretaria-id").val();
                var nome = $("#secretaria-nome").val();
                if (nome === "") {
                    alert("Nome não pode ficar em branco!");
                } else if (!parseInt(id) || parseInt(id) <= 0) {
                    alert("O id deve ser um número inteiro maior que zero!");
                } else {
                    $("#secretaria-dialog").loading();
                    var query = {
                        secretaria: nome
                    };
                    if (method === "alterar") {
                        query.id = id;
                    }
                    $.ajax({
                        type: "POST",
                        url: "/v/secretaria/" + method,
                        data: query,
                        dataType: "json",
                        success: function (data) {
                            if (data.ok) {
                                alert("Salvo com sucesso");
                                location.reload();
                            } else {
                                alert(data.mensagem);
                            }
                        }, error: function (jqXHR, textStatus, error) {
                            alert(error);
                        }, complete: function () {
                            $("#secretaria-dialog").loading("finish");
                        }
                    });
                }
            },
            "Cancel": function () {
                $(this).dialog("close");
            }
        }
    });
    $(".new-secretaria").click(function (e) {
        method = "cadastro";
        $("#secretaria-id").val(0);
        $("#secretaria-id").removeAttr("readonly");
        $("#secretaria-nome").val("");
        $("#secretaria-dialog").dialog("open");
        e.preventDefault();
    });
    $(".ren-secretaria").click(function (e) {
        method = "alterar";
        $("#secretaria-id").val($(this).data("id"));
        $("#secretaria-id").attr("readonly", "readonly");
        $("#secretaria-nome").val($(this).data("nome"));
        $("#secretaria-dialog").dialog("open");
        e.preventDefault();
    });
    $("#secretaria-id").inputmask("integer", {allowMinus: false});
});