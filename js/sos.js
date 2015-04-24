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

    $("a.button").button();

    $("#error-code").fitText("0.4");
    $("#info-code").fitText("1");

    $("#pesquisa").keyup(function (e) {
        if (e.which == 13) {
            $(this).parent().find("button").click();
        }
    });
    $(".buscar-bt").click(function () {
        var newlocation = "/v/" + $(this).data("mod") + "/listar"; // define caminho para redirecionamento
        newlocation += $("#pesquisa").val() !== "" ? "/b/" + encodeURIComponent($("#pesquisa").val()) : ""; // adiciona argumentos de busca
        newlocation += $(".filtro:selected").length ? "/f/" + $(".filtro:selected").val() : ""; // adciona argumentos de filtro
        window.location = newlocation;
    });
    $("#filtro").change(function () {
        $(".buscar-bt").click();
    });
    
    $(".ui-icon, .more-info").tooltip();

    //$(".item").titlecase();

    $("textarea").autogrow();
});