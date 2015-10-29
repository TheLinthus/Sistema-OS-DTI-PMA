var typingTimer;
var validMatricula = false;
var validCGM = false;

function testaCPF(strCPF) {
    var Soma;
    var Resto;
    Soma = 0;
    if (strCPF === "00000000000")
        return false;
    for (i = 1; i <= 9; i++)
        Soma = Soma + parseInt(strCPF.substring(i - 1, i)) * (11 - i);
    Resto = (Soma * 10) % 11;
    if ((Resto === 10) || (Resto === 11))
        Resto = 0;
    if (Resto !== parseInt(strCPF.substring(9, 10)))
        return false;
    Soma = 0;
    for (i = 1; i <= 10; i++)
        Soma = Soma + parseInt(strCPF.substring(i - 1, i)) * (12 - i);
    Resto = (Soma * 10) % 11;
    if ((Resto === 10) || (Resto === 11))
        Resto = 0;
    if (Resto !== parseInt(strCPF.substring(10, 11)))
        return false;
    return true;
}

function testaMatricula() {
    var matricula = $("#matricula").inputmask('unmaskedvalue');
    var cpf = $("#cpf").inputmask('unmaskedvalue');
    var base = $("#form-acesso-usuario").data("base");
    
    if (cpf === "" || matricula === "") {
        return;
    }

    validMatricula = false;
    $.post("v/usuario/matriculavalidate/", {matricula: matricula, cpf: cpf, base: base}, function (data) {
        validMatricula = data.valid;
        console.log(data);

        if (validMatricula) {
            switch (base) {
                case "local":
                    $("#cgm").val(data.data.cgm);
                    break;
                case "ecidade":
                    $("#matricula").val(data.data.fun_matricula);
                    $("#cgm").val(data.data.fun_cgm);
                    $("#cargo").val(data.data.fun_cargo);
                    $("#email").val(data.data.fun_email);
                    $("#localdetrabalho").val(data.data.fun_localtrabalho);
                    $("#lotacao").val(data.data.fun_lotacao_nome);
                    $("#nome").val(data.data.fun_nome);
                    break;
            }
            $("#matricula").next().css("background-color", "#96C94F").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-cancel").removeClass("ui-icon-refresh").addClass("ui-icon-check");
        } else {
            $("#matricula").next().css("background-color", "#f7a3a3").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-check").removeClass("ui-icon-refresh").addClass("ui-icon-cancel");
        }
    }, "json");
}

function testaCGM() {
    var cgm = $("#cgm").inputmask('unmaskedvalue');
    var cpf = $("#cpf").inputmask('unmaskedvalue');
    var base = $("#form-acesso-usuario").data("base");
    
    if (cpf === "" || cgm === "") {
        return;
    }

    validCGM = false;
    $.post("v/usuario/cgmvalidate/", {cgm: cgm, cpf: cpf, base: base}, function (data) {
        validCGM = data.valid;

        if (validCGM) {
            switch (base) {
                case "local":
                    $("#matricula").val(data.data.matricula);
                    break;
                case "ecidade":
                    $("#matricula").val(data.data.fun_matricula);
                    $("#cgm").val(data.data.fun_cgm);
                    $("#cargo").val(data.data.fun_cargo);
                    $("#email").val(data.data.fun_email);
                    $("#localdetrabalho").val(data.data.fun_localtrabalho);
                    $("#lotacao").val(data.data.fun_lotacao_nome);
                    $("#nome").val(data.data.fun_nome);
                    break;
            }
            $("#matricula").val(data.data.matricula);
            $("#cgm").next().css("background-color", "#96C94F").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-cancel").removeClass("ui-icon-refresh").addClass("ui-icon-check");
        } else {
            $("#cgm").next().css("background-color", "#f7a3a3").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-check").removeClass("ui-icon-refresh").addClass("ui-icon-cancel");
        }
    }, "json");
}

$(document).ready(function () {

    $("#matricula").inputmask({
        mask: "9999[9]",
        greedy: false,
        "oncomplete": function () {
            clearTimeout(typingTimer);
            typingTimer = setTimeout(testaMatricula, 2000);
            $(this).next().css("background-color", "#F7E85C").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-cancel").removeClass("ui-icon-check").addClass("ui-icon-refresh");
        },
        "oncleared": function () {
            clearTimeout(typingTimer);
            $(this).next().css("background-color", "").children().removeClass("ui-icon-check").removeClass("ui-icon-cancel").addClass("ui-icon-triangle-1-w");
        }
    });
    $("#matricula").blur(function () {
        clearTimeout(typingTimer);
        testaMatricula();
    });
    $("#cgm").inputmask({
        mask: "9{1,20}",
        greedy: false,
        "oncomplete": function () {
            clearTimeout(typingTimer);
            typingTimer = setTimeout(testaCGM, 2000);
            $(this).next().css("background-color", "#F7E85C").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-cancel").removeClass("ui-icon-check").addClass("ui-icon-refresh");
        },
        "oncleared": function () {
            $(this).next().css("background-color", "").children().removeClass("ui-icon-check").removeClass("ui-icon-cancel").addClass("ui-icon-triangle-1-w");
        }
    });
    $("#cgm").blur(function () {
        clearTimeout(typingTimer);
        testaCGM();
    });
    $("#cpf").inputmask({
        mask: "999.999.999-99",
        "oncomplete": function () {
            if (testaCPF($(this).inputmask('unmaskedvalue'))) {
                $(this).next().css("background-color", "#96C94F").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-cancel").addClass("ui-icon-check");
                if ($("#cgm").length && $("#cgm").inputmask('unmaskedvalue') !== "") {
                    $("#cgm").next().css("background-color", "#F7E85C").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-cancel").removeClass("ui-icon-check").addClass("ui-icon-refresh");
                    testaCGM();
                }
                if ($("#matricula").length && $("#matricula").inputmask('unmaskedvalue') !== "") {
                    $("#matricula").next().css("background-color", "#F7E85C").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-cancel").removeClass("ui-icon-check").addClass("ui-icon-refresh");
                    testaMatricula();
                }
            } else {
                $(this).next().css("background-color", "#f7a3a3").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-check").addClass("ui-icon-cancel");
            }
        },
        "oncleared": function () {
            $(this).next().css("background-color", "").children().removeClass("ui-icon-check").removeClass("ui-icon-cancel").addClass("ui-icon-triangle-1-w");
        }
    });
    $("#matricula, #cgm, #cpf").keyup(function () {
        if (!$(this).inputmask("isComplete") && $(this).inputmask('unmaskedvalue') !== "") {
            $(this).next().css("background-color", "#f7a3a3").children().removeClass("ui-icon-triangle-1-w").removeClass("ui-icon-check").addClass("ui-icon-cancel");
        }
    });
    $("#email").inputmask({
        mask: "*{1,20}[.*{1,20}][.*{1,20}][.*{1,20}]@*{1,20}[.*{2,6}][.*{1,2}]",
        greedy: false,
        onBeforePaste: function (pastedValue, opts) {
            pastedValue = pastedValue.toLowerCase();
            return pastedValue.replace("mailto:", "");
        },
        definitions: {
            '*': {
                validator: "[0-9A-Za-z!#$%&'*+/=?^_`{|}~\-]",
                cardinality: 1,
                casting: "lower"
            }
        }//,"oncomplete": function(){ alert('email complete'); }
    });
    $("#telefone").inputmask({ mask: "99 9{8,9}", greedy: false });
    $("#ip").inputmask("ip");

    $("#acesso_dialog").dialog({
        autoOpen: false,
        modal: true,
        resizable: false,
        title: 'Erro',
        buttons: {
            Ok: function () {
                $(this).dialog('close');
            }
        }
    });

    $("#form-acesso-usuario").submit(function (event) {
        if (!validCGM && !validMatricula) {
            event.preventDefault();
            $("#acesso_dialog").html("<p>Não foi possivel validar CGM ou Matricula com o CPF informado.</p><p>Certifique-se que está cadastrado ou clique no botão Cadastrar</p>");
            $("#acesso_dialog").dialog('open');
        }
    });
});