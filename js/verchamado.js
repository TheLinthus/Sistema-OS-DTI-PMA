$(".attachment").click(function () {
    if ($(this).data("id") !== undefined) {
        var id = $(this).data("id");
        console.log(id);
        window.location = "v/arquivos/download/id/" + id;
    }
});