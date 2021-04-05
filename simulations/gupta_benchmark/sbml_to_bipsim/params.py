
import numpy.random


class Params(object):

    def __init__(self, simulation_id, final_time, observables):
        self.simulation_id = simulation_id
        self.observables = observables
        self.final_time = final_time
        self.output_step = self.final_time/1000
        
    def to_bipsim(self, model_dir, output_dir, algorithm):
        seed = numpy.random.randint(10)
        output_entities = " ".join(self.observables)
        result = """
################################################################################
# SEED
# Random handler seed. Format is  
#   SEED <integer value>
################################################################################

SEED {}

################################################################################
# INITIAL TIME 
# Time at the beginning of simulation. Format is  
#   INITIAL_TIME <real value>
################################################################################

INITIAL_TIME 0

################################################################################
# FINAL TIME 
# Time at which simulation should stop. Format is  
#   FINAL_TIME <real value>
################################################################################

FINAL_TIME {}

################################################################################
# INPUT FILES 
# Path to input files relative to binary. Any number of files can be
# specified. Format is 
#   INPUT_FILES path/to/file1 path/to/file2 etc.
################################################################################

INPUT_FILES {}/model.in {}/initial_conditions_{}.in

################################################################################
# OUTPUT DIRECTORY
# Path to output directory relative to binary. Format is 
#   OUTPUT_DIR path/to/output/directory
################################################################################

OUTPUT_DIR {}

################################################################################
# OUTPUT STEP
# Time step at which concentrations should be logged.
# Format is
#   OUTPUT_STEP <real value>
################################################################################

OUTPUT_STEP {}

################################################################################
# OUTPUT ENTITIES
# Entities which should be logged in the output file. Format is
#   OUTPUT_ENTITIES name1 name2 etc.
################################################################################

OUTPUT_ENTITIES {}

################################################################################
# DRAWING_ALGORITHM
# Algorithm to use to determine next reaction to perform. Format is
#   DRAWING_ALGORITHM <algorithm>
#   HYBRID_BASE_RATE <real value>
#
# where <algorithm> is picked among
#  - vector (rates stored/drawn from a vector - complexity O(N))
#  - tree (rates stored/drawn from a tree - complexity O(logN))
#  - hybrid (group structure using a rejection method - complexity O(1))
#
# HYBRID_BASE_RATE is only relevant if "hybrid" was chosen.
################################################################################

DRAWING_ALGORITHM {}

HYBRID_BASE_RATE 1e-12
        """.format(seed, self.final_time, model_dir, model_dir,
                   self.simulation_id, output_dir, self.output_step,
                   output_entities, algorithm)
        return result
