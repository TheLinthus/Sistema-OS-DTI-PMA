
$(document).ready(function () {
    var verifyInterval = null;
    var trying = false;

    function verifyChamados() {
        if (!trying) {
            try {
                trying = true;
                var post = {};
                post.sort = $(".sorted-desc").length ? ($(".sorted-desc").data("column") + "-desc") : ($(".sorted-asc").data("column") + "-asc");
                post.tipo = [];
                post.pesquisa = $("#pesquisa").val();
                post.dataA = $("#dataA").val();
                post.dataB = $("#dataB").val();
                post.md5 = $("#chamados-table").data("md5");
                $("#filtro-estado + div input:checked").each(function () {
                    post.tipo.push($(this).val());
                });
                $.ajax({
                    method: "POST",
                    data: post,
                    dataType: "json",
                    url: "v/chamado/indexupdate/",
                    success: function (data) {
                        if (data.md5 !== $("#chamados-table").data("md5")) {
                            updateTable(data);

                            updateTitle("Chamados");
                        }
                    },
                    complete: function () {
                        trying = false;
                    }
                });
                return true;
            } catch (ex) {
                trying = false;
                return false;
            }
        } else {
            return false;
        }
    }

    $(document).on("click", ".chamado-row", function () {
        clickChamado(this);
    });

    $(document).on("change", "#filtro-estado + div input", verifyChamados);

    $(document).on("click", ".sorted-desc", function () {
        $(this).removeClass("sorted-desc");
        $(this).addClass("sorted-asc");
        while (!verifyChamados())
            ;
    });

    $(document).on("click", ".sorted-asc", function () {
        $(this).addClass("sorted-desc");
        $(this).removeClass("sorted-asc");
        while (!verifyChamados())
            ;
    });

    $(document).on("click", "#chamados-table th:not([class*='sorted'])", function () {
        $(".sorted-desc").removeClass("sorted-desc");
        $(".sorted-asc").removeClass("sorted-asc");
        $(this).addClass("sorted-desc");
        while (!verifyChamados())
            ;
    });

    $(".data-range input").keypress(function (e) {
        if (e.which == 13) {
            e.preventDefault();
            while (!verifyChamados())
                ;
        }
    });

    verifyInterval = setInterval(verifyChamados, 3000);

    if (!$("#pesquisa").val()) {
        history.replaceState({}, "", "");
    }
});