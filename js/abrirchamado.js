var etapa, url, sec, set, area, modulo, problema, log;
$(document).ready(function () {
    console.log(location.href);
    etapa = $("#step-box fieldset.active").last().data("etapa");
    url = "/v/chamado/cadastro";

    if (etapa >= 2) {
        sec = "/sec/" + $(".secretaria.active").data("id");
    } else {
        sec = "";
    }
    if (etapa >= 3) {
        set = "/set/" + $(".setor.active").data("id");
    } else {
        set = "";
    }
    if ($("#areas input[checked]").val() >= 0) {
        area = "/area/" + $("#areas input[checked]").val();
    } else {
        area = "";
    }
    console.log($("#modulos input[checked]").val());
    if ($("#modulos input[checked]").val() >= 0) {
        modulo = "/mod/" + $("#modulos input[checked]").val();
    } else {
        modulo = "";
    }
    if ($("#problemas input[checked]").val() >= 0) {
        problema = "/prob/" + $("#problemas input[checked]").val();
    } else {
        problema = "";
    }

    history.replaceState({etapa: etapa}, "Abrir Chamado - Etapa " + etapa, url + sec + set + area + modulo + problema);

    if (window.history && window.history.pushState) {
        $(window).on('popstate', function (e) {
            if (history.state !== null && history.state.etapa !== undefined) {
                etapa = history.state.etapa;
                $(".step").not("#step-" + etapa).hide(500);
                $("#step-" + etapa).show(500);
            }
        });
    }

    $("#form-abrir-chamado").addClass("step-form");

    $("#step-box").addClass("step-box");
    $("#step-box fieldset").hide();
    $("#step-box fieldset.active").last().show();
    $(".step .flex-container").addClass("flex-scroller");
    $(".next-step, .prev-step, .step footer").show();
    $("#areas, #modulos, #problemas, #prioridades").buttonset();
    $("input[type='file']").customFileInput({
        button_position: 'right',
        feedback_text: '(opcional) arquivo selecionado...',
        button_text: 'Selecionar um arquivo',
        button_change_text: 'Alterar o arquivo'
    });
    $("#problemas").find("[class*='ui-corner']").removeClass(function (index, css) {
        return (css.match(/\ui-corner-\S+/g) || []).join(' ');
    });
//    $("#areas, #problemas").roundflexcorners();

    $(document.body).on("click", ".secretaria", function (e) {
        if (!$(this).hasClass("active")) {
            $("#step-2 .flex-grid > a").remove();
            $("#step-2 .warn").hide();
            $("#step-2").loading();
            $.get("/v/setor/json", {sec: $(this).data("id")}, function (data) {
                if (data.setores === undefined || data.setores === null || data.setores.lenght === 0) {
                    alert("Erro, não foram encontradas setores para a secretaria selecionada\nPor favor volte e tente novamente. Se o problema persistir contate o DTI");
                } else {
                    $.each(data.setores, function (i, e) {
                        var a = $("<a>");
                        a.addClass("item");
                        a.addClass("setor");
                        a.addClass("next-step");
                        a.data("id", e.id);
                        a.data("secretaria", e.secretaria);
                        a.attr("href", "/v/chamado/cadastro/sec/" + e.secretaria + "/set/" + e.id);
                        a.html(e.setor);
                        //a.titlecase();
                        a.insertBefore($("#step-2 .flex-grid div").first());
                    });
                }
                $("#step-2").loading("finish");
            }, 'json');
            $("#secretaria").val($(this).data("id"));
            $(this).siblings(".active").removeClass("active");
            $(this).addClass("active");
        }
        sec = "/sec/" + $(this).data("id");
        history.pushState({etapa: 2}, "Abrir Chamado - Etapa 2", url + sec);
    });
    $(document.body).on("click", ".setor", function (e) {
        set = "/set/" + $(this).data("id");
        history.pushState({etapa: 3}, "Abrir Chamado - Etapa 3", url + sec + set);
        $("#setor").val($(this).data("id"));
        $(this).siblings(".active").removeClass("active");
        $(this).addClass("active");
    });

    $("#areas a, #modulos a, #problemas a").click(function (e) {
        e.preventDefault();
        $(this).closest("label").click();
    });

    $("#unknow-area+label, #unknow-modulo+label,#unknow-problema+label").show();

    $("#areas input").change(function (e) {
        $("#unknow-modulo").click();
        if ($(this).val() >= 0) {
            area = "/area/" + $(this).val();
            $("#unknow-modulo").attr("href", url + sec + set + area + modulo + problema);
            $("#modulo-control").show(500);
            $("#modulos > *[data-area='" + $(this).val() + "']").show('fast');
            $("#modulos > *:not([data-area='" + $(this).val() + "'], #unknow-modulo, [for='unknow-modulo'])").hide('fast');
//            $("#modulos").roundflexcorners();
        } else {
            area = "";
            modulo = "";
            problema = "";
            $("#modulo-control").hide(500);
        }
        history.replaceState({etapa: etapa}, "Abrir Chamado - Etapa " + etapa, url + sec + set + area + modulo + problema);
    });

    $("#modulos input").change(function (e) {
        $("#unknow-problema").click();
        if ($(this).val() >= 0) {
            modulo = "/mod/" + $(this).val();
            $("#unknow-problema").attr("href", url + sec + set + area + modulo + problema);
            $("#problema-control").show(500);
            $("#problemas > *[data-modulo='" + $(this).val() + "']").show('fast');
            $("#problemas > *:not([data-modulo='" + $(this).val() + "'], #unknow-problema, [for='unknow-problema'])").hide('fast');
//            $("#problemas").roundflexcorners();
        } else {
            modulo = "";
            problema = "";
            $("#problema-control").hide(500);
        }
        history.replaceState({etapa: etapa}, "Abrir Chamado - Etapa " + etapa, url + sec + set + area + modulo + problema);
    });

    $("#problemas input").change(function (e) {
        if ($(this).val() >= 0) {
            problema = "/prob/" + $(this).val();
        } else {
            problema = "";
        }
        history.replaceState({etapa: etapa}, "Abrir Chamado - Etapa " + etapa, url + sec + set + area + modulo + problema);
    });

    $("#patrimonio-btn").click(function (e) {
        e.preventDefault();
        var midX, midY;
        midX = window.innerWidth / 2 - 250 + window.screenX;
        midY = window.innerHeight / 2 - 200 + window.screenY;
        window.open("/v/patrimonio/seletor", "patrimonio-seletor", "height=370, width=500, location=0, scrollbars=0,resizable=0, top=" + midY + ", left=" + midX);
    });

    $("#usuario-btn").click(function (e) {
        e.preventDefault();
        var midX, midY;
        midX = window.innerWidth / 2 - 400 + window.screenX;
        midY = window.innerHeight / 2 - 225 + window.screenY;
        window.open("/v/usuario/seletor", "usuario-seletor", "height=500, width=800, location=0, scrollbars=0,resizable=0, top=" + midY + ", left=" + midX);
    });

    window.onmessage = function (e) {
        log = e;
        console.log(e.data);
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
        if (e.source.name === "usuario-seletor") {
            if (e.data !== undefined) {
                $("#usuario-nome").val(e.data.nome);
                $("#usuario-cgm").val(e.data.cgm);
            }
        }
    };

    $(document.body).on("click", ".next-step", function (e) {
        e.preventDefault();
        $(this).closest(".step").next(".step").show(500).addClass("active");
        $(this).closest(".step").hide(500);
        $("#areas a").each(function () {
            $(this).attr("href", url + sec + set + "/area/" + $(this).data("id"));
        });
        etapa++;
    });
    $(document.body).on("click", ".prev-step", function (e) {
        e.preventDefault();
        history.back();
    });

    $("#enviar-chamado").click(function (e) {
        e.preventDefault();
        if ($("#descricao").val().length < 10) {
            alert('A descrição está muito curta (no mínimo 10 caracteres)!');
            e.stopPropagation();
            return false;
        }
        if ($("#problemas input:checked").data("dica")) {
            if (!confirm("Primeiramente tente solucionar o problema com a seguinte dica:\n\n" +
                    $("#problemas input:checked").data("dica") +
                    "\n\nApós tentar solucionar o problema, você ainda quer enviar a solicitação de chamado?")) {
                e.stopPropagation();
                return false;
            }
        }
        $("#step-box").loading();
        $("#form-abrir-chamado").submit();
        history.pushState({etapa: 4}, "Abrir Chamado - Carregando", url + sec + set + area + modulo + problema);
    });
});