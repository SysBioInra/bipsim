
from __future__ import absolute_import, division, print_function
import sys
import os
import subprocess

from src import test_case


def main():
    print('N I vector tree hybrid')
    for case in test_case.read_file(sys.argv[1]):
        vector_time = time_case(case.bipsim_directory("vector"))
        tree_time = time_case(case.bipsim_directory("tree"))
        hybrid_time = time_case(case.bipsim_directory("hybrid"))
        #copasi_time = time_case(case.copasi_directory())
        print('{} {} {} {} {}'.format(case._length, case._initial_value,
                                   vector_time, tree_time, hybrid_time))


def time_case(directory):
    current_dir = os.getcwd()
    os.chdir(directory)
    process = subprocess.Popen(['time sh run.sh'],
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell = True)
    time_output = process.communicate()[1]
    os.chdir(current_dir)
    return user_time_rugen_format(time_output)


def user_time_old_format(output):
    return float(output[:4])


def user_time_new_format(output):
    return float(output.strip().split()[2])

def user_time_rugen_format(output):
    return output.strip().split()[3]

if __name__ == '__main__':
    main()
