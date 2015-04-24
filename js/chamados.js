var vvv;
$(document).ready(function () {
    var notifyInterval = null;
    var verifyInterval = null;
    var trying = false;

    function clickChamado(v) {
        var janela = open("/v/chamado/ver/" + $(v).data("id"));
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

    function verifyChamados() {
        if (!trying) {
            trying = true;
            var sort = $(".sorted-desc").length ? ($(".sorted-desc").data("column") + "-desc") : ($(".sorted-asc").data("column") + "-asc");
            $.ajax({
                method: "POST",
                data: {
                    sort: sort
                },
                dataType: "json",
                url: "/v/chamado/indexupdate/",
                success: function (data) {
                    if (data.md5 !== $("#chamados-table").data("md5")) {
                        $(".chamado-row").addClass("updating");
                        $.each(data.data, function (i, v) {
                            var cham;
                            if ($("#chamado-" + v.id).length) {
                                cham = $("#chamado-" + v.id);
                                cham.removeClass("updating");
                                var cells = cham.find("td");
                                if (cham.data("area") !== v.area) {
                                    cham.data("area", v.area);
                                    $(cells[3]).html(v.area);
                                    $(cells[3]).effect("highlight", 3000);
                                }
                                if (cham.data("prioridade") !== parseInt(v.prioridade)) {
                                    cham.data("prioridade", v.prioridade);
                                    $(cells[4]).html(v.prioridade + "%");
                                    $(cells[4]).effect("highlight", 3000);
                                }
                                if (cham.data("estado") !== v.estados[0].estado) {
                                    cham.data("estado", v.estados[0].id);
                                    var icon = $("<span>").addClass("ui-icon").addClass("estado-" + v.estados[0].tipo).attr("title", v.estados[0].estado);
                                    $(cells[5]).html(v.estados[0].estado).prepend(icon).attr("title", v.estados[0].estado);
                                    $(cells[5]).effect("highlight", 3000);
                                }
                                if (cham.data("data") !== v.estados[0].data) {
                                    cham.data("data", v.estados[0].data);
                                    $(cells[1]).html(v.estados[0].data);
                                    $(cells[1]).effect("highlight", 3000);
                                }
                            } else {
                                cham = $("<tr id='chamado-" + v.id + "' tabindex='0'>");
                                cham.addClass("chamado-row");
                                cham.data("id", v.id);
                                cham.data("prioridade", v.prioridade);
                                cham.data("estado", v.estados[0].id);
                                cham.data("area", v.area);
                                cham.data("usuario", v.usuario);
                                cham.data("data", v.estados[0].data);
                                cham.attr("tabindex", 0);
                                cham.append($("<td>").addClass("span1").html(v.id));
                                cham.append($("<td>").addClass("span3").addClass("more-info").attr("title", v.estados[0].data).html(v.estados[0].data).tooltip());
                                cham.append($("<td>").addClass("more-info").attr("title", v.usuario).html(v.usuario).tooltip());
                                cham.append($("<td>").addClass("span1").html(v.area));
                                cham.append($("<td>").addClass("span1").html(v.prioridade + "%"));
                                var icon = $("<span>").addClass("ui-icon").addClass("estado-" + v.estados[0].tipo).attr("title", v.estados[0].estado);
                                cham.append($("<td>").addClass("span4").addClass("more-info").attr("title", v.estados[0].estado).html(v.estados[0].estado).tooltip().prepend(icon));
                                cham.holdToClick(400, clickChamado);
                                cham.find("td").effect("highlight", 3000);
                            }
                            if ($(".chamado-row").length) {
                                $(".chamado-row").each(function (j, x) {
                                    var test = $(x).data("prioridade") <= v.prioridade && $(x).data("id") != v.id;
                                    switch (sort) {
                                        case "prioridade-desc" :
                                            test = parseInt($(x).data("prioridade")) <= v.prioridade && $(x).data("id") != v.id;
                                            break;
                                        case "prioridade-asc" :
                                            test = parseInt($(x).data("prioridade")) > v.prioridade && $(x).data("id") != v.id;
                                            break;
                                        case "id-desc" :
                                            test = parseInt($(x).data("id")) <= v.id;
                                            break;
                                        case "id-asc" :
                                            test = parseInt($(x).data("id")) > v.id;
                                            break;
                                        case "usuario-desc" :
                                            test = $(x).data("usuario") <= v.usuario && $(x).data("id") != v.id;
                                            break;
                                        case "usuario-asc" :
                                            test = $(x).data("usuario") > v.usuario && $(x).data("id") != v.id;
                                            break;
                                        case "area-desc" :
                                            test = $(x).data("area") <= v.area && $(x).data("id") != v.id;
                                            break;
                                        case "area-asc" :
                                            test = $(x).data("area") > v.area && $(x).data("id") != v.id;
                                            break;
                                        case "estado-desc" :
                                            test = $(x).data("estado") <= v.estados[0].estado && $(x).data("id") != v.id;
                                            break;
                                        case "estado-asc" :
                                            test = $(x).data("estado") > v.estados[0].estado && $(x).data("id") != v.id;
                                            break;
                                        case "data-desc" :
                                            test = $(x).data("data") <= v.estados[0].data && $(x).data("id") != v.id;
                                            break;
                                        case "data-asc" :
                                            test = $(x).data("data") > v.estados[0].data && $(x).data("id") != v.id;
                                            break;
                                    }
                                    if (test) {
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
            return true;
        } else {
            return false;
        }
    }

    $(document).on("click", ".chamado-row", clickChamado);

    $(document).on("click", ".sorted-desc", function () {
        console.log("clicou em sorted-desc > mudar para sorted-asc");
        $(this).removeClass("sorted-desc");
        $(this).addClass("sorted-asc");
        while (!verifyChamados());
    });

    $(document).on("click", ".sorted-asc", function () {
        console.log("clicou em sorted-asc > mudar para sorted-desc");
        $(this).addClass("sorted-desc");
        $(this).removeClass("sorted-asc");
        while (!verifyChamados());
    });

    $(document).on("click", "#chamados-table th:not([class*='sorted'])", function () {
        $(".sorted-desc").removeClass("sorted-desc");
        $(".sorted-asc").removeClass("sorted-asc");
        $(this).addClass("sorted-desc");
        while (!verifyChamados());
    });

    verifyInterval = setInterval(verifyChamados, 2000);
});