$(document).ready(function () {
    $("#buscausuario").keyup(function () {
        $.get("/v/usuario/json", {b: $(this).val()}, function (data) {
            $("#lista tbody tr").remove();
            if (data.length == 0) {
                $("#lista").hide();
            } else {
                $("#lista").show();
            }
            $.each(data, function (i, v) {
                var tr = $("<tr>");
                tr.append($("<td>").html(v.matricula));
                tr.append($("<td>").html(v.cgm));
                var td = $("<td>");
                td.html(v.nome);
                td.attr("title", v.nome);
                td.addClass("more-info");
                td.tooltip();
                tr.append(td);
                var a = $("<a>");
                console.log(v);
                a.data("cgm", v.cgm);
                a.data("nome", v.nome);
                a.attr("href", "#");
                a.addClass("usuario");
                a.addClass("ui-icon");
                a.addClass("ui-icon-check");
                tr.append($("<td>").append(a));
                $("#lista tbody").append(tr);
            });
        }, "json");
    });
    $(document.body).on("click", ".usuario", function () {
        window.opener.postMessage({cgm: $(this).data("cgm"), nome: $(this).data("nome")}, '*');
        window.close();
    });
    $("#cancelar").click(function () {
        window.close();
    });
    $("#nenhum").click(function () {
        window.opener.postMessage({nome: "", matricula: ""}, '*');
        window.close();
    });
});