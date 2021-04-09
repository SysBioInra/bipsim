
# BiPSim performance on benchmark of Gupta and Mendes.

This directory contains scripts used to compare BiPSim performance with other standard network-based and network-free SSA algorithms as implemented by [Gupta and Mendes](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC6013266/).

# How to run the benchmark

In the command line, use `Rscript` to run R scripts and `python` to run python scripts. In order run:
 - 01_make_test_cases.R: parse Gupta and Mendes results and export all test cases for a given SBML model (use cases are stored in test_cases/).
 - 02_convert_models.py: convert SMBL models into BiPSim models (stored in bipsim_models/).
 - 03_run_models.py: run BiPSim models (detailed results stored alongside models in bipsim_models/).
 - 04_gather_results.R: gather time used to run models and store in bipsim_results/.
 - 05_plot_results.R: plot benchmark results (BiPSim + orignal Gupta results)(stored in figs/).
 - 06_plot_concentrations.R: plot one example run for each benchmark (concentration of key compounds over time) to check overall behavior (stored in figs/).
 
# Subdirectories

Subdirectories store intermediate and final results of the benchmark. Note that only final results are stored on Github, intermediate results (including BiPSim models) can be generated by running the scripts listed above.

 - sbml: SBML models published by Gupta and Mendes in [Supplementary File 1](https://www.mdpi.com/2079-3197/6/1/9/s1)
 - gupta results: original results from the original Gupta and Mendes publication, as listed in [Supplementary File 1](https://www.mdpi.com/2079-3197/6/1/9/s1)
 - test_cases: Description of test cases implemented in Gupta and Mendes.
 - sbml_to_bipsim: Python package used to converst SBML models to BiPSim models.
 - bipsim_models: BiPSim models generated from SBML models.
 - bipsim_results: BiPSim timing results.
 - figs: fiures displaying results for all test cases 
 