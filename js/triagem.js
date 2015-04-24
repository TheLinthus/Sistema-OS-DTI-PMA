var vvv;
$(document).ready(function () {
    var notifyInterval = null;
    var verifyInterval = null;
    var trying = false;
    
    function clickChamado(v) {
        var janela = open("/v/chamado/ver/"+$(v).data("id")+"/atender");
        if (!janela) {
            alert("Janela bloquada, habilite popups neste site em seu navegador!");
        }
    }

    function onBlur() {
        $("body").addClass('blurred');
        $("body").removeClass('focused');
    }

    function onFocus() {
        $("body").addClass('focused');
        $("body").removeClass('blurred');
        clearInterval(notifyInterval);
        document.title = ((!$(".chamado-row").length ? "(vazio)" : "(" + $(".chamado-row").length + ")") + " Triagem de chamados");
    }

    if (/*@cc_on!@*/false) { // check for Internet Explorer
        document.onfocusin = onFocus;
        document.onfocusout = onBlur;
    } else {
        window.onfocus = onFocus;
        window.onblur = onBlur;
    }

    function verifyTriagem() {
        if (!trying) {
            trying = true;
            $.ajax({
                dataType: "json",
                url: "/v/chamado/triagemupdate/",
                success: function (data) {
                    if (data.md5 !== $("#chamados-table").data("md5")) {
                        $(".chamado-row").addClass("updating");
                        $.each(data.data, function (i, v) {
                            var cham;
                            if ($("#chamado-" + v.id).length) {
                                cham = $("#chamado-" + v.id);
                                cham.removeClass("updating");
                                cham.data("prioridade", v.prioridade);
                                var cells = cham.find("td");
                                if ($(cells[3]).html() !== (v.area + "")) {
                                    $(cells[3]).html(v.area);
                                    $(cells[3]).effect("highlight", 3000);
                                }
                                if ($(cells[4]).html() !== (v.prioridade + "%")) {
                                    $(cells[4]).html(v.prioridade + "%");
                                    $(cells[4]).effect("highlight", 3000);
                                }
                                if ($(cells[5]).html() !== (v.estados[0].estado)) {
                                    var icon = $("<span>").addClass("ui-icon").addClass("estado-"+v.estados[0].tipo).attr("title",v.estados[0].estado);
                                    $(cells[5]).html(v.estados[0].estado).preppend(icon);
                                    $(cells[5]).effect("highlight", 3000);
                                }
                            } else {
                                cham = $("<tr id='chamado-" + v.id + "' tabindex='0'>");
                                cham.addClass("chamado-row");
                                cham.data("id", v.id);
                                cham.data("prioridade", v.prioridade);
                                cham.append($("<td>").addClass("span1").html(v.id));
                                cham.append($("<td>").addClass("span3").html(v.estados[0].data));
                                cham.append($("<td>").addClass("more-info").attr("title",v.usuario).html(v.usuario).tooltip());
                                cham.append($("<td>").addClass("span1").html(v.area));
                                cham.append($("<td>").addClass("span1").html(v.prioridade + "%"));
                                var icon = $("<span>").addClass("ui-icon").addClass("estado-"+v.estados[0].tipo).attr("title",v.estados[0].estado);
                                cham.append($("<td>").addClass("span4").html(v.estados[0].estado).preppend(icon));
                                cham.holdToClick(400, clickChamado);
                                cham.find("td").effect("highlight", 3000);
                            }
                            if ($(".chamado-row").length) {
                                $(".chamado-row").each(function (j, x) {
                                    if ($(x).data("prioridade") <= v.prioridade && $(x).data("id") != v.id) {
                                        cham.detach();
                                        $(x).before(cham);
                                        return false;
                                    } else {
                                        $(x).after(cham);
                                    }
                                });
                            } else {
                                $("#chamados-table tbody").append(cham);
                            }
                        });
                        $("#chamados-table").data("md5", data.md5);
                        $(".updating").remove();

                        var val = true;
                        if (document.body.className === 'blurred') {
                            if ($(".chamado-row").length) {
                                clearInterval(notifyInterval);
                                notifyInterval = setInterval(function () {
                                    var title = ((!$(".chamado-row").length ? "(vazio)" : "(" + $(".chamado-row").length + ")") + " Triagem de chamados");
                                    document.title = val ? "Lista atualizada" : title;
                                    val = !val;
                                }, 1000);
                                $("#notification")[0].play();
                            } else {
                                document.title = "(vazio) Triagem de chamados";
                            }
                        }
                    }
                },
                complete: function () {
                    trying = false;
                }
            });
        }
    }

    $(".chamado-row").holdToClick(400, clickChamado);

    verifyInterval = setInterval(verifyTriagem, 2000);
});