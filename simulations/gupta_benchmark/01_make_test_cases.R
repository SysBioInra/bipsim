
library(tidyverse)


main = function() {
    #make_multistate()
    make_multisite()
    #make_egfr()
    #make_bcr()
    #make_fceri()
}

make_multistate = function() {
    # For molecules, "Endtime" seems to correspond to S1
    result = bind_rows(
        make_time("Multistate", n_molecules=5000, S1=5000, S2=1000, S3=5000),
        make_molecule("Multistate", S1=n_molecules, S2=S1/5, S3=S1)
    ) %>%
        mutate(simulation_id = 1:n(), .before = "type")
    write_csv(result, "test_cases/multistate.csv")
}

make_time = function(prefix, ...) {
    filename = file.path("gupta_results", paste0(prefix, "_results_time.csv"))
    result = read_csv(filename) %>%
        select(end_time = Endtime, run = Run) %>%
        arrange(end_time, run) %>%
        add_column(...) %>%
        add_column(type = "time") %>%
        select(type, end_time, n_molecules, run, everything())    
}

make_molecule = function(prefix, ..., col_molecules = "Endtime") {
    filename = file.path("gupta_results", paste0(prefix, "_results_molecules.csv"))
    result = read_csv(filename) %>%
        select(n_molecules = .data[[col_molecules]], run = Run) %>%
        arrange(n_molecules, run) %>%
        mutate(...) %>%
        add_column(end_time=100, type = "molecule") %>%
        select(type, end_time, n_molecules, run, everything())
}

make_multisite = function() {
    # For molecules, "Endtime" seems to correspond to S1+S3
    result = bind_rows(
        make_time("Multisite", n_molecules=5000, S1=5000, S2=1000, S3=5000),
        make_molecule("Multisite", S1=n_molecules/2, S2=S1/5, S3=S1)
    ) %>%
        mutate(simulation_id = 1:n(), .before = "type")
    write_csv(result, "test_cases/multisite.csv")
}

make_egfr = function() {
    # Result table and Sup. Table A1 disagree!
    # In the results, the number of molecules is the *total* number,
    # but counts are off: 683 should be 12000+1800+1500+2700+130+490=18,620.
    # Sup. Table A1 is mostly correct, except for the largest test case which has 3.10^6 EGF molecules (not 6.10^6)
    result = bind_rows(
        make_time("EGFR", n_molecules=6.83e4, S1=1.2e6, S5=1.8e5, S2=1e5, S3=2.7e5, S4=1.3e4, S6=4.9e4),
        make_molecule("EGFR", S1=round(n_molecules/6.83*120), S5=round(S1/12*1.8), S2=round(S1/12),
                      S3=round(S1/12*2.7), S4=round(S1/120*1.3), S6=round(S1/120*4.9), col_molecules="Molecules")
    ) %>%
        mutate(simulation_id = 1:n(), .before = "type")
    write_csv(result, "test_cases/egfr.csv")
}

make_bcr = function() {
    result = bind_rows(
        make_time("BCR", n_molecules=18e4, S1=3e4, S2=S1, S3=S1, S4=S1, S5=S1, S6=S1),
        make_molecule("BCR", S1=n_molecules/6, S2=S1, S3=S1, S4=S1, S5=S1, S6=S1, col_molecules="Molecules")
    ) %>%
        mutate(simulation_id = 1:n(), .before = "type")
    write_csv(result, "test_cases/bcr.csv")
}

make_fceri = function() {
    # Result table and Sup. Table A1 disagree!
    # In the results, the number of molecules is the *total* number,
    # but counts are off, e.g. 723 should be 600+40+3+40=683
    # Sup. Table A1 is mostly correct, except for the smallest test case which has 3 Lyn molecules (not 30)
    # There is a chance that there was a mix-up with BCR, which has a 683 molecule counts.
    result = bind_rows(
        make_time("Fceri", n_molecules=6.83e4, S1=6e4, S2=300, S3=4e3, S4=4e3),
        make_molecule("Fceri", S1=round(n_molecules/723*600), S2=round(S1/600*3),
                      S3=round(S1/60*4), S4=S3, col_molecules="Molecules")
    ) %>%
        mutate(simulation_id = 1:n(), .before = "type")
    write_csv(result, "test_cases/fceri.csv")
}

if (sys.nframe() == 0) {
    main()
}