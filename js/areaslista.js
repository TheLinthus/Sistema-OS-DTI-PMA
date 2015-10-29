var method = "";
$(document).ready(function () {
    $("#area-dialog").dialog({
        autoOpen: false,
        resizable: false,
        width: 270,
        buttons: {
            "Salvar": function () {
                var id = $("#area-id").val();
                var nome = $("#area-nome").val();
                var nivel = $("#area-nivel").val();
                if (nome === "") {
                    alert("Nome não pode ficar em branco!");
                } else if (nivel < 1 || nivel > 3) {
                    alert("O nível da área deve ser entre 1 à 3");
                } else {
                    $("#area-dialog").loading();
                    var query = (method === "cadastro") ? {area: nome, nivel: nivel} : {area: nome, nivel: nivel, id: id};
                    $.ajax({
                        type: "POST",
                        url: "v/area/" + method,
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
                            $("#area-dialog").loading("finish");
                        }
                    });
                }
            },
            "Cancel": function () {
                $(this).dialog("close");
            }
        }
    });
    $(".new-area").click(function (e) {
        method = "cadastro";
        $("#area-nome").val("");
        $("#area-nivel").val(2);
        $("#area-dialog").dialog("open");
        e.preventDefault();
    });
    $(".ren-area").click(function (e) {
        method = "alterar";
        $("#area-id").val($(this).data("id"));
        $("#area-nome").val($(this).data("nome"));
        $("#area-nivel").val($(this).data("nivel"));
        $("#area-dialog").dialog("open");
        e.preventDefault();
    });
    $("#area-nivel").inputmask("9");
});