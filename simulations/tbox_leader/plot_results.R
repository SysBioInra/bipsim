
library(tidyverse)
theme_set(theme_classic(base_size = 20))


main = function() {
    results = read_results() %>%
        select(!ends_with("TP")) %>%
        select(-protein_tracker, -rna_tracker)
    results = results %>%
        pivot_longer(-time, names_to = "species", values_to = "number")
    panel_a = c("EtRNA_Y_ca", "tRNA_Y", "Y")
    results %>%
        filter(species %in% panel_a) %>%
        ggplot(aes(x = time, y = number, col = species)) +
        geom_line(size=2) +
        geom_vline(xintercept=200, linetype="dashed") +
        geom_vline(xintercept=600, linetype="dashed") +
        labs(x="Time (s)", y = "# Molecules")
    ggsave("results_a.pdf")

    panel_b = c("aborted_U2301", "aborted_U2303", "U2301", "U2303", "tyrS_BSU29670")
    results %>%
        filter(species %in% panel_b) %>%
        ggplot(aes(x = time, y = number, col = species)) +
        geom_line(size=2) +
        geom_vline(xintercept=200, linetype="dashed") +
        geom_vline(xintercept=600, linetype="dashed") +
        labs(x="Time (s)", y = "# Molecules")
    ggsave("results_b.pdf")
}

read_results = function() {
    read_tsv("output/chemicals.out")
}

if (sys.nframe() == 0) {
    main()
}