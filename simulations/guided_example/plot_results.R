
library(tidyverse)


main = function() {
    plot_default("free_chemicals")
    ggsave("figs/free_chemicals.pdf")
    plot_default("sequences")
    ggsave("figs/sequences.pdf")
    plot_default("binding")
    ggsave("figs/binding.pdf")
    plot_default("polymerization", c("ATP", "CTP", "GTP", "UTP"))
    ggsave("figs/polymerization_a.pdf")
    plot_default("polymerization", c("RNAP_sigma", "bound_RNAP", "translocating_RNAP"))
    ggsave("figs/polymerization_b.pdf")
    plot_default("regulation")
    ggsave("figs/regulation.pdf")
    plot_default("annotations")
    ggsave("figs/annotations.pdf")
}

plot_default = function(output_dir, chemicals = NULL) {
    output_file= file.path("output", output_dir, "chemicals.out")
    results = read_delim(output_file, delim="\t") %>%
        pivot_longer(-time, names_to = "chemical", values_to = "number")
    if (!is.null(chemicals)) {
        results = filter(results, chemical %in% chemicals)
    }
    results %>%
        ggplot(aes(x = time, y = number, col = chemical)) +
        geom_line() +
        theme_bw(base_size = 20) +
        labs(x = "Simulation time (s)", y = "# molecules", col = NULL)
}

if (sys.nframe() == 0) {
    main()
}