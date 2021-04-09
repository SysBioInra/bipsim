
# global imports
import os
import pandas as pd

# local imports
import sbml_to_bipsim


def main():
    #convert_model("sbml/fixed_multistate.xml", "bipsim_models/multistate", "test_cases/multistate.csv")
    convert_model("sbml/fixed_multisite2.xml", "bipsim_models/multisite", "test_cases/multisite.csv")
    #convert_model("sbml/fixed_egfr_net_sbml_pSSAlib.xml", "bipsim_models/egfr", "test_cases/egfr.csv")
    #convert_model("sbml/fixed_BCR_sbml_pSSAlib.xml", "bipsim_models/bcr", "test_cases/bcr.csv")
    #convert_model("sbml/fceri_gamma2_sbml_pSSAlib.xml", "bipsim_models/fceri", "test_cases/fceri.csv")
    
def convert_model(input_file, model_dir, tc_file, algorithms = ["tree", "hybrid", "vector"]):    
    test_cases = pd.read_csv(tc_file)
    ic_species = list(test_cases.columns[5:])
    model = sbml_to_bipsim.SbmlData(input_file)
    
    if not os.path.exists(os.path.join(model_dir)):
        os.makedirs(os.path.join(model_dir))
    
    with open(os.path.join(model_dir, "model.in"), 'w') as f:
        f.write(model.to_bipsim(ic_species))
    
    with open(os.path.join(model_dir, "observables.txt"), 'w') as f:
        f.write("\n".join(model.assignments))
    
    for index, tc in test_cases.iterrows():
        sim_id = tc["simulation_id"]
        
        ic = sbml_to_bipsim.InitialConditions(dict(tc.iloc[5:]))
        with open(os.path.join(model_dir, "initial_conditions_" + str(sim_id) + ".in"), 'w') as f:
            f.write(ic.to_bipsim())
        
        params = sbml_to_bipsim.Params(sim_id, tc["end_time"], model.observables)
        for algo in algorithms:
            output_dir = os.path.join(model_dir, "output_" + str(sim_id) + "_" + algo)
            if not os.path.exists(os.path.join(output_dir)):
                os.makedirs(os.path.join(output_dir))
            with open(os.path.join(model_dir, "params_" + str(sim_id) + "_" + algo + ".in"), 'w') as f:
                f.write(params.to_bipsim(model_dir, output_dir, algo))                


if __name__ == "__main__":
    main()
