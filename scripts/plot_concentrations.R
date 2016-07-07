
plot_concentrations = function (data, toplot) {
    colors = rainbow (length (toplot))
    plot_range = c(min(data[toplot]), max(data[toplot]))
    #plot_range = c(0,50)

    plot (t(data ['time']), t(data [toplot[1]]), xlab = "time", ylab = "Number of molecules", type = "l", col = colors[1], ylim = plot_range);

    if (length(toplot) > 1)
        for (i in 2:length(toplot))
            lines (t(data ['time']), t(data [toplot[i]]), col=colors[i]);

    legend ("topright", legend=toplot, col=colors, lty=1)
}
