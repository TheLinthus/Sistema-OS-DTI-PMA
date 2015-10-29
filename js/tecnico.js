var vvv;
$(document).ready(function () {
    var verifyInterval = null;
    var trying = false;

    clickChamado = function(v) {
        var janela = open("v/chamado/atender/id/" + $(v).data("id"));
        if (!janela) {
            alert("Janela bloquada, habilite popups neste site em seu navegador!");
        }
    };

    function verifyChamados() {
        if (!trying) {
            trying = true;
            $.ajax({
                dataType: "json",
                data: {md5: $("#chamados-table").data("md5")},
                url: "v/chamado/tecnicoupdate/",
                success: function (data) {
                    if (data.md5 !== $("#chamados-table").data("md5")) {
                        updateTable(data);
                        
                        updateTitle("Chamados designados (Técnico)");
                    }
                },
                complete: function () {
                    trying = false;
                }
            });
        }
    }

    $(".chamado-row").holdToClick(400, clickChamado);

    verifyInterval = setInterval(verifyChamados, 2000);
});