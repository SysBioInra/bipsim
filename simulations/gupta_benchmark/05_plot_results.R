
library(tidyverse)


main = function() {
    plot_all_results("Multistate")
    plot_all_results("Multisite")
    plot_all_results("EGFR")
    plot_all_results("BCR")
    plot_all_results("Fceri")
    
    plot_combined_results(c("Multistate", "Multisite", "EGFR", "BCR", "Fceri"))
}

plot_all_results = function(model_name) {
    output_prefix = file.path("figs", paste0(tolower(model_name), "_"))
    
    results = bind_rows(read_bipsim(model_name), read_gupta(model_name))
    results %>%
        filter(type == "time") %>%
        plot_results() +
        labs(x = "Simulation time (s)", y = "Execution time (s)")
    ggsave(paste0(output_prefix, "time.pdf"))

    results %>%
        filter(type == "molecule") %>%
        plot_results() +
        labs(x = "Number of molecules", y = "Execution time (s)")
    ggsave(paste0(output_prefix, "molecules.pdf"))
}

plot_results = function(results) {
    n_methods = length(unique(results$method))
    results %>%
        group_by(value, method, model_name) %>%
        summarize(mean_time = mean(time)) %>%
        filter(mean_time <= 3600) %>%
        ggplot(aes(x = value, y = mean_time, col = method, shape = method, linetype = !startsWith(method, "bipsim"))) +
        geom_line() +
        geom_point() +
        theme_bw(base_size = 20) +
        scale_y_log10() +
        guides(linetype=FALSE) +
        scale_shape_manual(values = rep(c(1:6), 4)[1:n_methods])
}

read_bipsim = function(model_name) {
    filename = file.path("bipsim_results", paste0(tolower(model_name), ".csv"))
    result = read_csv(filename, col_types = cols()) %>%
        mutate(method = paste0("bipsim_", algorithm)) %>%
        select(-simulation_id, -algorithm) %>%
        add_column(model_name = model_name)
    return(result)
}

read_gupta = function(model_name) {
    prefix = file.path("gupta_results", paste0(model_name, "_"))
    col_molecule = ifelse(model_name %in% c("Multistate", "Multisite"), "Endtime", "Molecules")
    
    time = read_csv(paste0(prefix, "results_time.csv"), col_types = cols()) %>%
        rename(value = Endtime, run = Run) %>%
        pivot_longer(c(-value, -run), names_to = "method", values_to = "time") %>%
        add_column(type = "time", .before = "value")
    molecule = read_csv(paste0(prefix, "results_molecules.csv"), col_types = cols()) %>%
        rename(value = .data[[col_molecule]], run = Run) %>%
        pivot_longer(c(-value, -run), names_to = "method", values_to = "time") %>%
        add_column(type = "molecule", .before = "value")
    result = bind_rows(time, molecule) %>%
        add_column(model_name = model_name)
    return(result)
}

plot_combined_results = function(model_names) {
    output_prefix = "figs/"
    
    results = lapply(model_names, function(n) {
        bind_rows(read_bipsim(n), read_gupta(n))
    })
    results = bind_rows(results) %>%
        mutate(model_name = factor(model_name, levels=model_names))
    
    results %>%
        filter(type == "time") %>%
        plot_results() +
        facet_wrap(~ model_name, scales = "free_x") +
        labs(x = "Simulation time (s)", y = "Execution time (s)", col = NULL, shape=NULL) +
        scale_x_continuous(n.breaks = 3) +
        theme_bw(base_size = 14) +
        theme(legend.text = element_text(size=8),
              axis.text.x = element_text(angle=45, hjust=1))
    ggsave(paste0(output_prefix, "time.pdf"))
        
    results %>%
        filter(type == "molecule") %>%
        plot_results() +
        facet_wrap(~ model_name, scales = "free_x") +
        labs(x = "Simulation time (s)", y = "Execution time (s)", col = NULL, shape=NULL) +
        scale_x_continuous(n.breaks = 3) +
        theme_bw(base_size = 14) +
        theme(legend.text = element_text(size=8),
              axis.text.x = element_text(angle=45, hjust=1))
    ggsave(paste0(output_prefix, "molecules.pdf"))
}

if (sys.nframe() == 0) {
    main()
}