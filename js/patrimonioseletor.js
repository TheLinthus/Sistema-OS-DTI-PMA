$(document).ready(function () {
    $("#buscapatrimonio").keyup(function () {
        $.get("v/patrimonio/json", {b: $(this).val()}, function (data) {
            $("tbody").html("");
            $.each(data,function (i,v) {
                var tr = $("<tr>");
                tr.append($("<td>").html(v.id));
                tr.append($("<td>").html(v.placa));
                tr.append($("<td>").html(v.descricao));
                var a = $("<a>");
                console.log(v);
                a.data("id",v.id);
                a.data("descricao",v.descricao);
                a.attr("href","#");
                a.addClass("patrimonio");
                a.addClass("ui-icon");
                a.addClass("ui-icon-check");
                tr.append($("<td>").append(a));
                $("tbody").append(tr);
            });
        }, "json");
    });
    $(document.body).on("click", ".patrimonio", function () {
        window.opener.postMessage({id:$(this).data("id"),desc:$(this).data("descricao")}, '*');
        window.close();
    });
    $("#cancelar").click(function () {
        window.close();
    });
    $("#nenhum").click(function () {
        window.opener.postMessage({id:"",desc:""}, '*');
        window.close();
    });
});