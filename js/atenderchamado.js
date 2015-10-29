var id = $("#id").val();
var prioridade = $("#prioridade").val();
var area = $("#area option:selected").val();
var modulo = $("#modulo option:selected").val();
var problema = $("#problema option:selected").val();
var files = [];
var excluir = [];

function updatePrioridade(v) {
    $("#prioridade").val(v);
    var text;
    if (v < 20) {
        text = "Baixa";
    } else if (v < 40) {
        text = "Média";
    } else if (v < 60) {
        text = "Alta";
    } else if (v < 80) {
        text = "Muito Alta";
    } else {
        text = "Urgente";
    }
    $("#prioridade-value").html(v + "% " + text);
    $("#prioridade-value").css("background-color", corPrioridade(v));
}

$("#prioridade-slider").slider({
    range: "min",
    min: 0,
    max: 100,
    value: prioridade,
    slide: function (event, ui) {
        updatePrioridade(ui.value);
    }
});
updatePrioridade($("#prioridade").val());

$("#area").change(function () {
    area = $("#area option:selected").val();
    $("#modulo option").not(".modulo-area-null").hide();
    $("#modulo option:selected").removeAttr("selected");
    $("#modulo option.modulo-area-null").attr("selected", true);
    if (area !== "null") {
        $("#modulo option.modulo-area-" + area).show();
        $("#modulo").removeAttr("readonly");
    } else {
        $("#modulo").attr("readonly", true);
    }
});

$("#modulo").change(function () {
    modulo = $("#modulo option:selected").val();
    $("#problema option").not(".problema-modulo-null").hide();
    $("#problema option:selected").removeAttr("selected");
    $("#problema option.problema-modulo-null").attr("selected", true);
    if (modulo !== "null") {
        $("#problema option.problema-modulo-" + modulo).show();
        $("#problema").removeAttr("readonly");
    } else {
        $("#problema").attr("readonly", true);
    }
});

$("#problema").change(function () {
    problema = $("#problema option:selected").val();
});

window.onmessage = function (e) {
    log = e;
    if (e.source.name === "patrimonio-seletor") {
        if (e.data !== undefined) {
            if (e.data.id === 0) {
                $("#placa-div").show();
            } else {
                $("#placa-div").hide();
            }
            $("#patrimonio-descricao").val(e.data.desc);
            $("#patrimonio").val(e.data.id);
        }
    }
};

$("#patrimonio-btn").click(function (e) {
    e.preventDefault();
    var midX, midY;
    midX = window.innerWidth / 2 - 250 + window.screenX;
    midY = window.innerHeight / 2 - 200 + window.screenY;
    window.open("v/patrimonio/seletor", "patrimonio-seletor", "height=370, width=500, location=0, scrollbars=0,resizable=0, top=" + midY + ", left=" + midX);
});

function loadForm() {
    var data = new FormData();
    data.append("id", id);
    data.append("area", area);
    data.append("modulo", modulo);
    data.append("problema", problema);
    data.append("prioridade", $("#prioridade").val());
    data.append("descricao", $("#descricao").val());
    data.append("solucao", $("#solucao").val());
    data.append("patrimonio", $("#patrimonio").val());
    data.append("excluir", excluir);
    $.each(files, function (key, value) {
        if (value !== undefined) {
            data.append("file-" + key, value);
        }
    });
    return data;
}

$("#save-chamado").click(function () {
    $("#atender-chamado fieldset").loading();
    var data = loadForm();
    $.ajax({
        type: "POST",
        url: "v/chamado/salvar",
        data: data,
        processData: false,
        contentType: false,
        dataType: "json",
        success: function (data) {
            if (data.ok) {
                alert("Salvo com sucesso");
                $(".input-file").val("");
                location.reload();
            } else {
                alert(data.mensagem);
            }
        }, error: function (jqXHR, textStatus, error) {
            alert(error);
        }, complete: function () {
            $("#atender-chamado fieldset").loading("finish");
        }
    });
});

$("#forward-chamado").click(function () {
    if (area === "null") {
        alert("Você deve selecionar a Área antes de encaminhar!");
        $("#area").focus().parent().effect("bounce");
    } else {
        $("#atender-chamado fieldset").loading();
        var data = loadForm();
        $.ajax({
            type: "POST",
            url: "v/chamado/encaminhar",
            processData: false,
            contentType: false,
            data: data,
            dataType: "json",
            success: function (data) {
                if (data.ok) {
                    if (data.stay) {
                        location.reload();
                    } else {
                        alert("Encaminhado para a àrea designada!");
                        window.close();
                    }
                } else {
                    alert(data.mensagem);
                }
            }, error: function (jqXHR, textStatus, error) {
                alert(error);
            }, complete: function () {
                $("#atender-chamado fieldset").loading("finish");
            }
        });
    }
});

$("#finish-chamado").click(function () {
    if (!$("#solucao").val()) {
        alert("Você deve preencher a solução do chamado!");
        $("#solucao").focus().parent().effect("bounce");
    } else if (confirm("Você realmente deseja finalizar o chamado? Operação não pode ser desfeita!")) {
        $("#atender-chamado fieldset").loading();
        var data = loadForm();
        $.ajax({
            type: "POST",
            url: "v/chamado/finalizar",
            processData: false,
            contentType: false,
            data: data,
            dataType: "json",
            success: function (data) {
                if (data.ok) {
                    alert("Chamado finalizado!");
                    window.location = "v/chamado/gerarcomprovante/id/" + id;
                } else {
                    alert(data.mensagem);
                }
            }, error: function (jqXHR, textStatus, error) {
                alert(error);
            }, complete: function () {
                $("#atender-chamado fieldset").loading("finish");
            }
        });
    }
});



$("input[type=file]").on('change', function (event) {
    var i = $(this).data("i");
    files[i] = event.target.files[0];
    $("#file-" + i + "-label").removeClass(function (index, css) {
        return (css.match(/(^|\s)file-\S+/g) || []).join(' ');
    });
    if (files[i] !== undefined) {
        var filename = files[i].name.split(".");
        $("#file-" + i + "-label").addClass("file-" + filename[filename.length - 1]);
    }
});

$('#file-dialog').dialog({
    autoOpen: false,
    width: 300,
    buttons: {
        "Abrir": function () {
            window.location = "v/arquivos/download/id/" + $("#file-dialog").data("id");
            $(this).dialog("close");
        },
        "Modificar": function () {
            var i = $("#file-dialog").data("index");
            $("#file-" + i + "-label").removeAttr("data-id");
            $("#file-" + i + "-label").removeData("id");
            $("#file-" + i).val("");
            $("#file-" + i + "-label").removeClass(function (index, css) {
                return (css.match(/(^|\s)file-\S+/g) || []).join(' ');
            });
            $("#file-" + i).click();
            excluir.push($("#file-dialog").data("id"));
            $(this).dialog("close");
        },
        "Excluir": function () {
            var i = $("#file-dialog").data("index");
            $("#file-" + i + "-label").removeAttr("data-id");
            $("#file-" + i + "-label").removeData("id");
            $("#file-" + i).val("");
            $("#file-" + i + "-label").removeClass(function (index, css) {
                return (css.match(/(^|\s)file-\S+/g) || []).join(' ');
            });
            excluir.push($("#file-dialog").data("id"));
            $(this).dialog("close");
        }
    }
});

$(".attachment").click(function () {
    if ($(this).data("id") !== undefined) {
        var id = $(this).data("id");
        var index = $(this).data("index");
        $("#file-dialog").attr("title", $(this).attr("title"));
        $("#file-dialog").data("id", id);
        $("#file-dialog").data("index", index);
        $("#file-dialog").dialog("open");
        return false;
    }
});

$("#modulo option").not(".modulo-area-null, .modulo-area-" + area).hide();
$("#problema option").not(".problema-modulo-null, .problema-modulo-" + modulo).hide();