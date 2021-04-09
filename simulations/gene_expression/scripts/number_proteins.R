
library(tidyverse)
theme_set(theme_classic(base_size = 20))


main = function() {
    experiments = c(constant = "../detailed_constant", stacked = "../detailed_stacked",
                    detailed = "../detailed_detailed", hybrid = "../hybrid_constant/",
                    aggregated = "../aggregated_constant")
    proteins = bind_rows(lapply(experiments, load_proteins), .id = "experiment")
    print(cor.test(proteins$n_rnas, proteins$final_number))
    print(kruskal.test(proteins$final_number / proteins$n_rnas, proteins$experiment))
    
    ggsave("proteins_vs_rna.pdf", plot_proteins_vs_rna(proteins))
    ggsave("proteins_per_rna.pdf", plot_proteins_per_rna(proteins))
    ggsave("stacked_vs_detailed.pdf", plot_correlation(proteins, "detailed", "stacked"))
    ggsave("constant_vs_detailed.pdf", plot_correlation(proteins, "detailed", "constant"))
    ggsave("constant_vs_stacked.pdf", plot_correlation(proteins, "stacked", "constant"))
    ggsave("detailed_vs_hybrid.pdf", plot_correlation(proteins, "constant", "hybrid") %>% add_experiment_labs("ge_detailed", "ge_hybrid"))
    ggsave("detailed_vs_aggregated.pdf", plot_correlation(proteins, "constant", "aggregated") %>% add_experiment_labs("ge_detailed", "ge_aggregated"))
}

load_proteins = function(simulation_path) {
    output_dir = file.path(simulation_path, "output/full_output")
    input_dir = file.path(simulation_path, "input")
    proteins = read_proteins(file.path(input_dir, "proteins.in"))
    chemicals = read.table(file.path(output_dir, "chemicals.out"), header = TRUE, stringsAsFactors = FALSE)
    proteins = add_column(proteins, final_number = as.integer(chemicals[nrow(chemicals), proteins$protein]))
    return(proteins)
}

read_proteins = function(filename) {
     matches = stringr::str_match_all(
         readr::read_file(filename),
         "ChemicalSequence (\\w+) product_of (\\w+) [0-9]+ [0-9]+ proteins"
    )
    result = table(matches[[1]][,2]) %>%
        c %>%
        enframe("protein", "n_rnas")
    return(result)
}

plot_proteins_vs_rna = function(proteins) {
    proteins %>%
        sample_frac %>%
        ggplot(aes(x = n_rnas, y = final_number, col = experiment)) +
            geom_jitter() +
            labs(x = "Number of distinct RNAs carrying protein", y = "Final number of proteins")
}

plot_proteins_per_rna = function(proteins) {
    proteins %>%
        ggplot(aes(x = experiment, y = final_number / n_rnas)) +
            geom_violin(aes(fill = experiment)) +
            geom_boxplot(width = 0.1) +
            labs(x = "Experiment", y = "Final number of proteins (per distinct RNA carrying protein)")
}

plot_correlation = function(proteins, experiment_a, experiment_b) {
    spread_proteins = proteins %>% spread(experiment, final_number)
    cor_label = get_r2_string(cor(spread_proteins[[experiment_a]], spread_proteins[[experiment_b]])**2)
    xcor = 0.25*max(spread_proteins[[experiment_a]])
    ycor = 0.75*max(spread_proteins[[experiment_b]])
    spread_proteins %>%
        ggplot(aes_string(x = experiment_a, y = experiment_b)) +
            geom_point() +
            annotate("text", x=xcor, y=ycor, label=cor_label, parse=TRUE, size = 8) +
            geom_abline(slope = 1, linetype = "dashed") +
            labs(x = paste0("Final number of proteins (", experiment_a, " metabolism)"),
                 y = paste0("Final number of proteins (", experiment_b, " metabolism)"))
}

add_experiment_labs = function(my_plot, experiment_a, experiment_b) {
    my_plot +
        labs(x = paste0("Final number of proteins (", experiment_a, ")"),
             y = paste0("Final number of proteins (", experiment_b, ")"))
}

get_r2_string = function(correlation){
    eq = substitute(italic(R)^2~"="~r2, list(r2 = format(correlation, digits = 3)))
    as.character(as.expression(eq))
}

if (!interactive()) {
    main()
}