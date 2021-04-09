
library(tidyverse)


main = function() {
    #gather_results("multistate")
    gather_results("multisite")
    #gather_results("egfr")
    #gather_results("bcr")
    #gather_results("fceri")
}

gather_results = function(model_name, algorithms = c("tree", "hybrid", "vector")) {
    model_dir = file.path("bipsim_models", model_name)
    output_file = file.path("bipsim_results", paste0(model_name, ".csv"))
    tc_file = file.path("test_cases", paste0(model_name, ".csv"))
    
    test_cases = read_csv(tc_file) %>%
        mutate(value = ifelse(type == "time", end_time, n_molecules)) %>%
        select(simulation_id, type, value, run)
    
    time = cross_df(list(simulation_id = test_cases$simulation_id,
                         algorithm = algorithms)) %>%
        group_by(simulation_id, algorithm) %>%
        summarize(time = read_time(model_dir, simulation_id, algorithm))
    
    result = left_join(test_cases, time, by = "simulation_id")
    write_csv(result, output_file)
}

read_time = function(model_dir, sim_id, algo) {
    output_dir = paste0("output_", sim_id, "_", algo)
    time_file = file.path(model_dir, output_dir, "time.txt")
    return(scan(time_file, quiet=TRUE))
}

if (sys.nframe() == 0) {
    main()
}