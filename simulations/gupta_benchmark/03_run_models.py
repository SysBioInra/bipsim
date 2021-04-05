
# global imports
import os
import pandas as pd
import subprocess
import time


def main():
    #run_models("multistate")
    run_models("multisite")
    #run_models("egfr")
    #run_models("bcr")
    #run_models("fceri")

def run_models(model_name, algorithms = ["tree", "hybrid", "vector"]):
    model_dir = os.path.join("bipsim_models", model_name)
    tc_file = os.path.join("test_cases", model_name + ".csv")
    test_cases = pd.read_csv(tc_file)
    
    for index, tc in test_cases.iterrows():
        sim_id = tc["simulation_id"]
        print("Simulation ", sim_id)
        for algo in algorithms:    
            params_file = os.path.join(model_dir, "params_" + str(sim_id) + "_" + algo + ".in")
            output_dir = os.path.join(model_dir, "output_" + str(sim_id) + "_" + algo)

            start_time = time.time()
            result = subprocess.run(["bipsim", params_file], capture_output=True) 
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
