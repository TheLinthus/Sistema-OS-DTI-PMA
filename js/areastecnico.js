$(document).ready(function () {
    $("#add-area").click(function (e) {
        e.preventDefault();
        $("#areas option:selected").detach().appendTo("#tecnico-areas");
        $("#add-area").attr("disabled", "disabled");
        $("#rmv-area").removeAttr("disabled");

    });
    $("#rmv-area").click(function (e) {
        e.preventDefault();
        $("#tecnico-areas option:selected").detach().appendTo("#areas");
        $("#rmv-area").attr("disabled", "disabled");
        $("#add-area").removeAttr("disabled");
    });
    $("#tecnico-areas").change(function () {
        if ($("#tecnico-areas option:selected").length) {
            $("#areas option:selected").removeAttr("selected");
            $("#rmv-area").removeAttr("disabled");
            $("#add-area").attr("disabled", "disabled");
        } else {
            $("#rmv-area").attr("disabled", "disabled");
        }
    });
    $("#areas").change(function () {
        if ($("#areas option:selected").length) {
            $("#tecnico-areas option:selected").removeAttr("selected");
            $("#add-area").removeAttr("disabled");
            $("#rmv-area").attr("disabled", "disabled");
        } else {
            $("#add-area").attr("disabled", "disabled");
        }
    });
    $("#form-areas").submit(function (e) {
        e.preventDefault();
        $("#tecnico-areas option.new-area").attr("selected", "selected");
        $("#areas option.old-area").attr("selected", "selected");
        $.post("", $(this).serialize(), function (data) {
            alert("Para as alterações tomarem efeito, o usuário deve relogar");
            location.reload();
        });
    });
});