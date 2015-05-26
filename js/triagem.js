var vvv;
$(document).ready(function () {
    var verifyInterval = null;
    var trying = false;
    
    clickChamado = function(v) {
        var janela = open("/v/chamado/atender/id/" + $(v).data("id"));
        if (!janela) {
            alert("Janela bloquada, habilite popups neste site em seu navegador!");
        }
    }

    function verifyTriagem() {
        if (!trying) {
            trying = true;
            $.ajax({
                dataType: "json",
                url: "/v/chamado/triagemupdate/",
                success: function (data) {
                    if (data.md5 !== $("#chamados-table").data("md5")) {
                        updateTable(data);
                        
                        updateTitle("Triagem de chamados");
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