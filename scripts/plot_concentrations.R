
plot_concentrations = function (data, toplot, filename = "", leg = TRUE) {
    if (filename != "") { pdf (filename, width=7, height=5); }

    colors = rainbow (length (toplot))
    plot_range = c(min(data[toplot]), max(data[toplot]))

    plot (t(data ['time']), t(data [toplot[1]]), type = "l", col = colors[1], ylim = plot_range, xlab = "time", ylab = "Number of molecules", );

    if (length(toplot) > 1)
        for (i in 2:length(toplot))
            lines (t(data ['time']), t(data [toplot[i]]), col=colors[i]);

    if (leg == TRUE) { legend ("topright", legend=toplot, col=colors, lty=1) }
    if (filename != "") { dev.off(); }
}
