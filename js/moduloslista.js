var method = "";
$(document).ready(function () {
    $("#modulo-dialog").dialog({
        autoOpen: false,
        resizable: false,
        width: 270,
        buttons: {
            "Salvar": function () {
                var id = $("#modulo-id").val();
                var nome = $("#modulo-nome").val();
                var area = $("#modulo-area").val();
                if (nome === "") {
                    alert("Nome não pode ficar em branco!");
                } else {
                    $("#modulo-dialog").loading();
                    var query = (method === "cadastro") ? {modulo: nome, area: area} : {modulo: nome, area: area, id: id};
                    $.ajax({
                        type: "POST",
                        url: "/v/modulo/" + method,
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
                            $("#modulo-dialog").loading("finish");
                        }
                    });
                }
            },
            "Cancel": function () {
                $(this).dialog("close");
            }
        }
    });
    $(".new-modulo").click(function (e) {
        method = "cadastro";
        $("#modulo-nome").val("");
        $("#modulo-dialog").dialog("open");
        e.preventDefault();
    });
    $(".ren-modulo").click(function (e) {
        method = "alterar";
        $("#modulo-id").val($(this).data("id"));
        $("#modulo-nome").val($(this).data("nome"));
        $("#modulo-dialog").dialog("open");
        e.preventDefault();
    });
});