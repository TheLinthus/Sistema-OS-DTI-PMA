var method = "";
$(document).ready(function () {
    $("#patrimonio-dialog").dialog({
        autoOpen: false,
        resizable: false,
        width: 315,
        buttons: {
            "Salvar": function () {
                $("#patrimonio-dialog").loading();
                var query = {
                    placa: $("#placa").val(),
                    descricao: $("#descricao").val(),
                    observacoes: $("#observacoes").val()
                };
                if (method === "alterar") {
                    query.id = $("#id").val();
                }
                $.ajax({
                    type: "POST",
                    url: "/v/patrimonio/" + method,
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
                        $("#patrimonio-dialog").loading("finish");
                    }
                });
            },
            "Cancel": function () {
                $(this).dialog("close");
            }
        }
    });
    $(".new-patrimonio").click(function (e) {
        method = "cadastro";
        $("#id").val(0);
        $("#placa").val("");
        $("#descricao").html("");
        $("#observacoes").html("");
        $("#patrimonio-dialog").dialog("open");
        e.preventDefault();
    });
    $(".ren-patrimonio").click(function (e) {
        method = "alterar";
        $("#id").val($(this).data("id"));
        $("#placa").val($(this).data("placa"));
        $("#descricao").html($(this).data("descricao"));
        $("#observacoes").html($(this).data("observacoes"));
        $("#patrimonio-dialog").dialog("open");
        e.preventDefault();
    });
});