
library(tidyverse)


main = function() {
    #plot_example("multistate", 85, "tree", multistate_names())
    #plot_example("multisite", 15, "vector", multisite_names())
    #plot_example("egfr", 75, "hybrid")
    plot_example("bcr", 81, "hybrid")
    #plot_example("fceri", 26, "hybrid")
}

plot_example = function(model_name, sim_id, algo_name, species_name=NULL) {
    result_file = file.path(
        "bipsim_models", model_name,
        paste0("output_", sim_id, "_", algo_name), "chemicals.out"
    )
    observable_file = file.path(
        "bipsim_models", model_name, "observables.txt"
    )
    output_file = file.path("figs", paste0(model_name, "_example.pdf"))
    
    results = read_delim(result_file, delim = "\t")
    species = colnames(results)
    species = species[species!="time"]
    observables = paste(readLines(observable_file, warn = FALSE), collapse=",")
    results = eval(parse(text = paste("mutate(results,", observables, ")")))
    results = results[, !(colnames(results) %in% species)] %>%
        pivot_longer(-time, names_to = "molecule", values_to = "number")
    
    if (!is.null(species_name)) {
        results$molecule = species_name[results$molecule]
    }
    
    results %>%
        ggplot(aes(x = time, y = number, col = molecule)) +
        geom_line() +
        theme_bw(base_size = 20) +
        labs(x = "Time (s)", y = "Number of molecules", col = "Species")
    ggsave(output_file)
}

multistate_names = function() {
    c(Group_A_bound_P = "A_bound_P", Group_A_P = "A_Phos",
      Group_A_unbound_P = "A_unbound_P", Group_RLA_P = "RLA_Phos")
}

multisite_names = function() {
    c(Group_Rfree = "R_free", Group_Lfree = "L_free", Group_A1P = "A_Phos")
}

if (sys.nframe() == 0) {
    main()
}