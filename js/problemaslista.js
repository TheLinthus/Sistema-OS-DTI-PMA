var method = "";
$(document).ready(function () {
    $("#problema-dialog").dialog({
        autoOpen: false,
        resizable: false,
        width: 320,
        buttons: {
            "Salvar": function () {
                var id = $("#problema-id").val();
                var nome = $("#problema-nome").val();
                var dica = $("#problema-dica").val();
                var modulo = $("#problema-modulo").val();
                if (nome === "") {
                    alert("Nome não pode ficar em branco!");
                } else {
                    $("#problema-dialog").loading();
                    var query = (method === "cadastro") ? {problema: nome, modulo: modulo, dica: dica} : {problema: nome, modulo: modulo, dica: dica, id: id};
                    $.ajax({
                        type: "POST",
                        url: "/v/problema/" + method,
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
                            $("#problema-dialog").loading("finish");
                        }
                    });
                }
            },
            "Cancel": function () {
                $(this).dialog("close");
            }
        }
    });
    $(".new-problema").click(function (e) {
        method = "cadastro";
        $("#problema-nome").val("");
        $("#problema-dialog").dialog("open");
        e.preventDefault();
    });
    $(".ren-problema").click(function (e) {
        method = "alterar";
        $("#problema-id").val($(this).data("id"));
        $("#problema-nome").val($(this).data("nome"));
        $("#problema-dica").val($(this).data("dica"));
        $("#problema-dialog").dialog("open");
        e.preventDefault();
    });
});