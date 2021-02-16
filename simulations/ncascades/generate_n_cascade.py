"""Generate n cascades."""

import sys
import os

from src import test_case


def main():
    case_file = "test_cases.txt"
    for case in test_case.read_file(case_file):
        n_cascade = case.generate_n_cascade()
        for method in ["ssa", "nf"]:
            os.makedirs(case.bionetgen_directory(method), exist_ok=True)
            n_cascade.to_bionetgen(case.bionetgen_directory(method), method)
        for method in ["vector", "tree", "hybrid"]:
            os.makedirs(case.bipsim_directory(method), exist_ok=True)
            n_cascade.to_bipsim(case.bipsim_directory(method), method)
        #os.makedirs(case.copasi_directory(), exist_ok=True)
        #n_cascade.to_copasi(case.copasi_directory())


if __name__ == '__main__':
    main()
