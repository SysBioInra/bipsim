
# global imports
import os
import pandas as pd
import subprocess
import time
import sbml_to_bipsim


def main():
    make_model()
    run_model()
    
def make_model(algo = "hybrid"):
    tc_file = "test_cases/bcr.csv"
    model_dir = "bipsim_models/bcr"
    input_file = "sbml/fixed_BCR_sbml_pSSAlib.xml"
    sim_id = 81
    n_molecules = 750000
    final_time = 500
    
    test_cases = pd.read_csv(tc_file)
    ic_species = list(test_cases.columns[5:])
    model = sbml_to_bipsim.SbmlData(input_file)
                    
    ic = sbml_to_bipsim.InitialConditions(dict.fromkeys(ic_species, n_molecules))
    with open(os.path.join(model_dir, "initial_conditions_" + str(sim_id) + ".in"), 'w') as f:
        f.write(ic.to_bipsim())
        
    params = sbml_to_bipsim.Params(sim_id, final_time, model.observables)
    output_dir = os.path.join(model_dir, "output_" + str(sim_id) + "_" + algo)
    if not os.path.exists(os.path.join(output_dir)):
        os.makedirs(os.path.join(output_dir))
    with open(os.path.join(model_dir, "params_" + str(sim_id) + "_" + algo + ".in"), 'w') as f:
        f.write(params.to_bipsim(model_dir, output_dir, algo))
        

def run_model(algo = "hybrid"):
    model_name = "bcr"
    model_dir = os.path.join("bipsim_models", model_name)
    sim_id = 81
    
    print("Simulation ", sim_id)
    params_file = os.path.join(model_dir, "params_" + str(sim_id) + "_" + algo + ".in")
    output_dir = os.path.join(model_dir, "output_" + str(sim_id) + "_" + algo)

    start_time = time.time()
    result = subprocess.run(["bipsim", params_file], stdout=subprocess.PIPE) 
    end_time = time.time()
    total_time = end_time-start_time

    with open(os.path.join(output_dir, "time.txt"), "w") as f:
        f.write(str(total_time))
    with open(os.path.join(output_dir, "stdout.txt"), "wb") as f:
        f.write(result.stdout)
    if result.stderr:
        with open(os.path.join(output_dir, "stderr.txt"), "wb") as f:
            f.write(result.stderr)


if __name__ == "__main__":
    main()
