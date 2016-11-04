
plot_concentrations = function(time, concentrations, colors = c(), filename = "") {
    if (filename != "") { pdf(filename, width=7, height=5); }

    time = t(time); concentrations = t(concentrations);
    nb_lines = dim(concentrations)[1];
    if (length(colors)==0) { colors = rainbow(nb_lines) }
    plot_range = c(min(concentrations), max(concentrations))

    plot(time, concentrations[1,], type = "l", col = colors[1], ylim = plot_range, xlab = "time", ylab = "Number of molecules");

    if (nb_lines> 1) {
        for (i in 2:nb_lines)
        { lines(time, concentrations[i,], col=colors[i]); }
    }

    if (filename != "") { dev.off(); }
}
