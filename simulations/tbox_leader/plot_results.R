
library(tidyverse)


main = function() {
    results = read_results() %>%
        select(!ends_with("TP")) %>%
        select(-protein_tracker, -rna_tracker)
    results = results %>%
        pivot_longer(-time, names_to = "species", values_to = "number")
    results %>%
        ggplot(aes(x = time, y = number+1, col = species)) +
        geom_line() +
        theme_bw() +
        scale_y_log10() +
        labs(x="Time (s)", y = "# Molecules + 1")
    ggsave("results.pdf")
}

read_results = function() {
    read_tsv("output/chemicals.out")
}

if (sys.nframe() == 0) {
    main()
}