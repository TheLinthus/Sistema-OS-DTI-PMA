var method = "";
$(document).ready(function () {
    $("#setor-dialog").dialog({
        autoOpen: false,
        resizable: false,
        width: 315,
        buttons: {
            "Salvar": function () {
                var id = $("#setor-id").val();
                var nome = $("#setor-nome").val();
                var escola = $("input[name=escola]:checked").val();
                var secretaria = $("#secretaria-id option:selected").val()
                var telefone = $("#telefone").val();
                var prioridade = $("#prioridade").val();
                if (nome === "") {
                    alert("Nome não pode ficar em branco!");
                } else {
                    $("#setor-dialog").loading();
                    var query = {
                        setor: nome,
                        secretaria: secretaria,
                        escola: escola,
                        telefone: telefone,
                        prioridade: prioridade
                    };
                    if (method === "alterar") {
                        query.id = id;
                    }
                    $.ajax({
                        type: "POST",
                        url: "v/setor/" + method,
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
                            $("#setor-dialog").loading("finish");
                        }
                    });
                }
            },
            "Cancel": function () {
                $(this).dialog("close");
            }
        }
    });
    $(".new-setor").click(function (e) {
        method = "cadastro";
        $("#setor-id").val(0);
        $("#setor-nome").val("");
        if ($("#filtro .filtro:selected").length) {
            $("#secretaria-id option[value=" + $("#filtro .filtro:selected").val() + "]").attr("selected", "selected");
        } else {
            $("#secretaria-id option").first().attr("selected", "selected");
        }
        $("#escola-0").attr("checked", "checked");
        $("#telefone").val("");
        $("#prioridade").val(1);
        $("#setor-dialog").dialog("open");
        e.preventDefault();
    });
    $(".ren-setor").click(function (e) {
        method = "alterar";
        $("#setor-id").val($(this).data("id"));
        $("#setor-nome").val($(this).data("nome"));
        $("#secretaria-id option[value=" + $(this).data("secretaria") + "]").attr("selected", "selected");
        $("#escola-" + $(this).data("escola")).attr("checked", "checked");
        $("#telefone").val($(this).data("telefone"));
        $("#prioridade").val($(this).data("prioridade"));
        $("#setor-dialog").dialog("open");
        e.preventDefault();
    });
    $("#setor-id").inputmask("integer", {allowMinus: false});
    $("#telefone").inputmask({mask: "99 9{8,9}", greedy: false});
});