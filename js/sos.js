var notifyInterval = null;
var title = document.title;
var clickChamado = function (v) {
    window.location = "/v/chamado/ver/id/" + $(v).data("id");
}

function onBlur() {
    $("body").addClass('blurred');
    $("body").removeClass('focused');
}

function onFocus() {
    $("body").addClass('focused');
    $("body").removeClass('blurred');
    clearInterval(notifyInterval);
    document.title = title;
}

if (/*@cc_on!@*/false) { // check for Internet Explorer
    document.onfocusin = onFocus;
    document.onfocusout = onBlur;
} else {
    window.onfocus = onFocus;
    window.onblur = onBlur;
}

function corPrioridade(v) {
    var g = 305 - v;
    var r = 205 + v;
    var b = 205;
    if (g > 255) {
        g = 255;
    }
    if (r > 255) {
        r = 255;
    }
    var R = ("0" + r.toString(16)).substr(-2);
    var G = ("0" + g.toString(16)).substr(-2);
    var B = ("0" + b.toString(16)).substr(-2);
    return "#" + R + G + B;
}

function updateTable(data) {
    $(".chamado-row").addClass("updating");
    if (data.debug) {
        console.log(data.debug);
    }
    $.each(data.data, function (i, v) {
        var cham;
        var desc = v.descricao + " (" + v.problema + ")";
        if ($("#chamado-" + v.id).length) {
            cham = $("#chamado-" + v.id);
            cham.removeClass("updating");
            var cells = cham.find("td");
            if (cham.data("area") !== v.area) {
                cham.data("area", v.area);
                $(cells[3]).html(v.area);
                $(cells[3]).effect("highlight", 3000);
            }
            if (cham.data("prioridade") != parseInt(v.prioridade)) {
                cham.data("prioridade", v.prioridade);
                var icon = $("<span>").addClass("ui-icon").css("background", v.corprioridade);
                $(cells[4]).html(v.nomeprioridade).prepend(icon);
                $(cells[4]).effect("highlight", 3000);
            }
            if (cham.data("estado") != v.estados[0].estado) {
                cham.data("estado", v.estados[0].id);
                var icon = $("<span>").addClass("ui-icon").addClass("estado-" + v.estados[0].tipo).attr("title", v.estados[0].estado);
                $(cells[6]).html(v.estados[0].estado).prepend(icon).attr("title", v.estados[0].estado);
                $(cells[6]).effect("highlight", 3000);
            }
            if (cham.data("data") != v.estados[0].data) {
                cham.data("data", v.estados[0].data);
                $(cells[1]).html(v.estados[0].data);
                $(cells[1]).effect("highlight", 3000);
            }
            if (cham.data("descricao") != desc) {
                console.log(v.id);
                console.log(cham.data("descricao"));
                console.log(desc);
                cham.data("descricao", desc);
                $(cells[5]).html(desc).attr("title", desc);
                $(cells[5]).effect("highlight", 3000);

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
            cham.data("descricao", desc);
            cham.attr("tabindex", 0);
            cham.append($("<td>").addClass("span1").html(v.id));
            cham.append($("<td>").addClass("span1").addClass("more-info").attr("title", v.estados[0].data).html(v.estados[0].data).tooltip());
            cham.append($("<td>").addClass("more-info").attr("title", v.usuario).html(v.usuario).tooltip());
            cham.append($("<td>").addClass("span1").html(v.area));
            var icon = $("<span>").addClass("ui-icon").css("background", v.corprioridade);
            cham.append($("<td>").addClass("span1").html(v.nomeprioridade).prepend(icon));
            cham.append($("<td>").addClass("span3").addClass("more-info").html(desc).attr("title", desc).tooltip());
            icon = $("<span>").addClass("ui-icon").addClass("estado-" + v.estados[0].tipo).attr("title", v.estados[0].estado);
            cham.append($("<td>").addClass("span5").addClass("more-info").attr("title", v.estados[0].estado).html(v.estados[0].estado).tooltip().prepend(icon));
            cham.holdToClick(400, clickChamado);
            cham.find("td").effect("highlight", 3000);
        }
        if ($(".chamado-row").length) {
            $(".chamado-row").each(function (j, x) {
                var test = $(x).data("prioridade") <= v.prioridade;
                var sort = $(".sorted-desc").length ? ($(".sorted-desc").data("column") + "-desc") : ($(".sorted-asc").data("column") + "-asc");
                switch (sort) {
                    case "prioridade-desc" :
                        test = parseInt($(x).data("prioridade")) <= v.prioridade;
                        break;
                    case "prioridade-asc" :
                        test = parseInt($(x).data("prioridade")) > v.prioridade;
                        break;
                    case "id-desc" :
                        test = parseInt($(x).data("id")) <= v.id;
                        break;
                    case "id-asc" :
                        test = parseInt($(x).data("id")) > v.id;
                        break;
                    case "usuario-desc" :
                        test = $(x).data("usuario") <= v.usuario;
                        break;
                    case "usuario-asc" :
                        test = $(x).data("usuario") > v.usuario;
                        break;
                    case "area-desc" :
                        test = $(x).data("area") <= v.area;
                        break;
                    case "area-asc" :
                        test = $(x).data("area") > v.area;
                        break;
                    case "estado-desc" :
                        test = $(x).data("estado") <= v.estados[0].estado;
                        break;
                    case "estado-asc" :
                        test = $(x).data("estado") > v.estados[0].estado;
                        break;
                    case "data-desc" :
                        test = $(x).data("data") <= v.estados[0].data;
                        break;
                    case "data-asc" :
                        test = $(x).data("data") > v.estados[0].data;
                        break;
                }
                if (test && $(x).data("id") != v.id) {
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
}

function updateTitle(text) {
    title = ((!$(".chamado-row").length ? "(vazio)" : "(" + $(".chamado-row").length + ")") + " " + text);
    document.title = title;
    var val = false;

    if (document.body.className === 'blurred') {
        if ($(".chamado-row").length) {
            clearInterval(notifyInterval);
            notifyInterval = setInterval(function () {
                title = ((!$(".chamado-row").length ? "(vazio)" : "(" + $(".chamado-row").length + ")") + " " + text);
                document.title = val ? "Lista atualizada" : title;
                val = !val;
            }, 1000);
            $("#notification")[0].play();
        } else {
            document.title = "(vazio) " + text;
        }
    }
}

(function ($) {

    $.fn.closestToOffset = function (offset) {
        var el = null, elOffset, x = offset.left, y = offset.top, distance, dx, dxR, dy, dyB, minDistance;
        this.each(function () {
            elOffset = $(this).offset();

            if (
                    (x >= elOffset.left) && (x <= elOffset.right) &&
                    (y >= elOffset.top) && (y <= elOffset.bottom)
                    ) {
                el = $(this);
                return false;
            }

            var offsets = [[elOffset.left, elOffset.top], [elOffset.right, elOffset.top], [elOffset.left, elOffset.bottom], [elOffset.right, elOffset.bottom]];
            for (off in offsets) {
                dx = offsets[off][0] - x;
                dxR = dx + $(this).width();
                dx = dx * dx > dxR * dxR ? dxR : dx;
                dy = offsets[off][1] - y;
                dyB = dy + $(this).height();
                dy = dy * dy > dyB * dyB ? dyB : dy;
                distance = Math.sqrt((dx * dx) + (dy * dy));
                if (minDistance === undefined || distance < minDistance) {
                    minDistance = distance;
                    el = $(this);
                }
            }
        });
        return el;
    };

    $.fn.roundflexcorners = function () {
        var flex = $(this);
        var cw = flex.width(); // container width
        var ch = flex.height(); // container height

        var tl = flex.find(".ui-widget:visible").first();
        var tr = flex.find(".ui-widget:visible").closestToOffset({left: flex.offset().left + cw, top: 0});
        var bl = flex.find(".ui-widget:visible").closestToOffset({left: 0, top: flex.offset().top + ch});
        var br = flex.find(".ui-widget:visible").last();

        flex.find("[class*='ui-corner']").removeClass(function (index, css) {
            return (css.match(/\ui-corner-\S+/g) || []).join(' ');
        });
        console.log(tl, tr, bl, br);

        tl.addClass("ui-corner-tl");
        tr.addClass("ui-corner-tr");
        bl.addClass("ui-corner-bl");
        br.addClass("ui-corner-br");
        return flex;
    };
    $.fn.titlecase = function () {
        return this.each(function () {
            var el = $(this);
            el.html(el.html().replace(".", ". ").replace(/\w\S*/g, function (txt) {
                return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase();
            }));
        });
    };
    $.fn.loading = function (action) {
        action = action || "start";
        if (action === "start") {
            var loading = $("<div>");
            loading.addClass("loading");
            for (var i = 1; i <= 7; i++) {
                var rect = $("<div>");
                rect.addClass("rect" + i);
                rect.appendTo(loading);
            }
            return this.each(function () {
                if ($(this).find(".loading").length === 0) {
                    $(this).append(loading);
                }
            });
        } else if (action === "finish") {
            return this.each(function () {
                $(this).find(".loading").hide('slow').remove();
            });
        }
    };
    $.fn.menumaker = function (options) {

        var cssmenu = $(this), settings = $.extend({
            title: "Menu",
            format: "dropdown",
            sticky: false
        }, options);

        return this.each(function () {
            cssmenu.prepend('<div id="menu-button">' + settings.title + '</div>');
            $(this).find("#menu-button").on('click', function () {
                $(this).toggleClass('menu-opened');
                var mainmenu = $(this).next('ul');
                if (mainmenu.hasClass('open')) {
                    mainmenu.hide().removeClass('open');
                }
                else {
                    mainmenu.show().addClass('open');
                    if (settings.format === "dropdown") {
                        mainmenu.find('ul').show();
                    }
                }
            });

            cssmenu.find('li ul').parent().addClass('has-sub');

            cssmenu.find('a').focus(function () {
                $(this).parents("li.has-sub").addClass('hover');
            }).blur(function () {
                $(this).parents("li.has-sub").removeClass('hover');
            });

            multiTg = function () {
                cssmenu.find(".has-sub").prepend('<span class="submenu-button"></span>');
                cssmenu.find('.submenu-button').on('click', function () {
                    $(this).toggleClass('submenu-opened');
                    if ($(this).siblings('ul').hasClass('open')) {
                        $(this).siblings('ul').removeClass('open').hide();
                    }
                    else {
                        $(this).siblings('ul').addClass('open').show();
                    }
                });
            };

            if (settings.format === 'multitoggle')
                multiTg();
            else
                cssmenu.addClass('dropdown');

            if (settings.sticky === true)
                cssmenu.css('position', 'fixed');

            resizeFix = function () {
                if ($(window).width() > 768) {
                    cssmenu.find('ul').show();
                }

                if ($(window).width() <= 768) {
                    cssmenu.find('ul').hide().removeClass('open');
                }
            };
            resizeFix();
            return $(window).on('resize', resizeFix);

        });
    };
    $.fn.holdToClick = function (delay, action) {
        delay = delay || 1000;
        var time = 33 / delay;
        var canvas = $("#hold-cav").length ? $("#hold-cav") : $("<canvas id='hold-cav' width='40' height='40'>");
        var follower = $("#follower");
        if (!follower.length) {
            follower = $("<div id='follower'>");
            follower.append(canvas);
            $("body").append(follower);
        }

        $(document).mousemove(function (event) {
            follower.css({left: event.pageX - 20, top: event.pageY - 15});
        });

        var circle = new ProgressCircle({
            canvas: canvas[0],
            minRadius: 1,
            arcWidth: 18,
        });

        var target, timeHeld = 0, canClick = false;

        $(this).mousedown(function (event) {
            if (event.button === 0) {
                target = this;
                canClick = false;
                circle.start(33);
                timeHeld = 0;
            }
        });
        $(this).mouseup(function (event) {
            target = null;
            circle.stop();
            circle._clear();
            circle.circles[0].fillColor = "rgba(255, 180, 0, 0.5)";
            circle.circles[0].outlineColor = "rgba(255, 180, 0, 1)";
        });
        $(this).mouseout(function (event) {
            target = null;
            circle.stop();
            circle._clear();
            circle.circles[0].fillColor = "rgba(255, 180, 0, 0.5)";
            circle.circles[0].outlineColor = "rgba(255, 180, 0, 1)";
        });
        $(this).click(function (event) {
            if (canClick) {
                target = null;
                circle.stop();
                circle._clear();
                circle.circles[0].fillColor = "rgba(255, 180, 0, 0.5)";
                circle.circles[0].outlineColor = "rgba(255, 180, 0, 1)";
            } else {
                event.preventDefault();
            }
        });

        circle.addEntry({
            fillColor: 'rgba(255, 180, 0, 0.5)',
            outlineColor: 'rgba(255, 180, 0, 1)',
            progressListener: function () {
                if (timeHeld >= 0.990) {
                    timeHeld = 1;
                    canClick = true;
                    action(target);
                    circle.stop();
                    circle.circles[0].fillColor = "rgba(0, 180, 0, 0.5)";
                    circle.circles[0].outlineColor = "rgba(0, 180, 0, 1)";
                } else {
                    timeHeld += time;
                }
                return timeHeld;
            },
        });

        return $(this);
    };
})(jQuery);

$(document).ready(function () {

    $("#nav-menu").menumaker({
        title: "SOS - Sistema Ordem de Serviço",
        format: "multitoggle"
    });
    
    $(".hidder").click(function(e) {
        e.preventDefault();
        $(this).parent().toggleClass("collapsed retracted");
        $(this).siblings(".hidding-content").toggle(400);
    });

    $("a.button").button();

    $("#error-code").fitText("0.4");
    $("#info-code").fitText("1");

    $("#pesquisa").keyup(function (e) {
        if (e.which == 13) {
            $(this).parent().find("button").click();
        }
    });
    $(".buscar-bt").click(function () {
        var mod = $(this).data("mod");
        var act = $(this).data("act") || "listar";
        var newlocation = "/v/" + mod + "/" + act; // define caminho para redirecionamento
        newlocation += $("#pesquisa").val() !== "" ? "/b/" + encodeURIComponent($("#pesquisa").val()) : ""; // adiciona argumentos de busca
        newlocation += $(".filtro:selected").length ? "/f/" + $(".filtro:selected").val() : ""; // adciona argumentos de filtro
        newlocation += $("#dataA").length ? "/da/" + $("#dataA").val() : "";
        newlocation += $("#dataB").length ? "/db/" + $("#dataB").val() : "";
        window.location = newlocation;
    });
    $("#filtro").change(function () {
        $(".buscar-bt").click();
    });

    $.validator.addMethod("greaterThanPrevious", function (value, element) {
        var prev = $(element).prevAll("input");
        return prev.valid() && prev.val() <= $(element).val();
    }, "Datas devem ser validas no formato dia/mês/ano e segunda data deve ser maior que a anterior");

    $("#filtros").validate({
        rules: {
            dataA: {
                date: true
            },
            dataB: {
                date: true,
                greaterThanPrevious: true
            }
        },
        onsubmit: false
    });

    $(".ui-icon, .more-info").tooltip();

    //$(".item").titlecase();

    $("textarea").autogrow();
    $("textarea:disabled").each(function (i, e) {
        var s = $(e)[0].scrollHeight;
        $(e).animate({height: s});
    });

    $(".attachment").button();
    $(".retracted .hidding-content").delay(1000).hide(400);
}
);