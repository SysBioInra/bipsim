
save_concentrations = function (data, toplot, filename) {
    pdf (filename, width=7, height=5);
    plot_concentrations (data, toplot);
    dev.off();
}
