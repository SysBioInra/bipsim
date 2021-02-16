
import sys
import os
import subprocess
import time

from src import test_case


def main():
    case_file = "test_cases.txt"
    
    for case in test_case.read_file(case_file):
        for method in ["ssa", "nf"]:
            time_case(case.bionetgen_directory(method))
        #for method in ["vector", "tree", "hybrid"]:
        #    time_case(case.bipsim_directory(method))
        #copasi_time = time_case(case.copasi_directory())


def time_case(directory):
    current_dir = os.getcwd()
    os.chdir(directory)
    
    start_time = time.time()
    try:
        result = subprocess.run(['sh', 'run.sh'], timeout = 7200)
    except subprocess.TimeoutExpired:
        print(directory + " timed out.")
    end_time = time.time()
    total_time = end_time-start_time
    with open("time.txt", "w") as f:
        f.write(str(total_time))
    os.chdir(current_dir)

                    
if __name__ == '__main__':
    main()
