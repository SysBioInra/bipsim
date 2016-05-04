

/**
 * @file simulationparams.cpp
 * @brief Implementation of the SimulationParams class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout std::cerr
#include <fstream> // std::ifstream
#include <sstream> // std::istringstream
#include <stdexcept>

// ==================
//  Project Includes
// ==================
//
#include "simulationparams.h"
#include "interpreter.h"
#include "solverfactory.h"
#include "ratemanagerfactory.h"
#include "ratecontainerfactory.h"

// ===================
//  Static attributes
// ===================
//
const std::string SimulationParams::_seed_tag = "SEED";
const std::string SimulationParams::_initial_time_tag = "INITIAL_TIME";
const std::string SimulationParams::_final_time_tag = "FINAL_TIME";
const std::string SimulationParams::_input_files_tag = "INPUT_FILES";
const std::string SimulationParams::_output_dir_tag = "OUTPUT_DIR";
const std::string SimulationParams::_concentration_file = "concentrations.out";
const std::string SimulationParams::_replication_file = "replication.out";
const std::string SimulationParams::_param_file = "params.out";
const std::string SimulationParams::_reaction_file = "reactions.out";
const std::string SimulationParams::_output_step_tag = "OUTPUT_STEP";
const std::string SimulationParams::_output_entities_tag = "OUTPUT_ENTITIES";
const std::string SimulationParams::_drawing_algorithm_tag = "DRAWING_ALGORITHM";
const std::string SimulationParams::_hybrid_base_rate_tag = "HYBRID_BASE_RATE";
const std::string SimulationParams::_base_volume_tag = "BASE_VOLUME";
const std::string SimulationParams::_volume_modifier_tag = "VOLUME_MODIFIER";
const std::string SimulationParams::_volume_step_tag = "VOLUME_STEP";

// ==========================
//  Constructors/Destructors
// ==========================
//
SimulationParams::SimulationParams (const std::string& filename)
  : _seed (0)
  , _initial_time (0)
  , _final_time (1000)
  , _output_step (1)
  , _output_double_strand ("chromosome")
  , _solver_factory (new NaiveSolverFactory())
  , _rate_manager_factory (new DependencyRateManagerFactory())
  , _hybrid_base_rate (1)
  , _base_volume (1)
  , _volume_step (1)
{
  std::cout << "Reading parameter file " << filename << "..." << std::endl;
  std::ifstream file (filename.c_str());
  if (file.fail())
    {
      std::ostringstream message;
      message << "Could not open parameter file \"" << filename
	      <<  "\".\n";
      throw std::runtime_error (message.str());
    }
  
  std::vector <Rule> rules;
  rules.push_back (TagToken (_seed_tag) + IntToken (_seed));
  rules.push_back (TagToken (_initial_time_tag) + DblToken (_initial_time));
  rules.push_back (TagToken (_final_time_tag) + DblToken (_final_time));
  rules.push_back (TagToken (_input_files_tag) 
		   + Iteration (MemToken <std::string> (_input_files)));
  rules.push_back (TagToken (_output_dir_tag) + StrToken (_output_dir));
  rules.push_back (TagToken (_output_step_tag) + DblToken (_output_step));
  rules.push_back (TagToken (_output_entities_tag) 
		   + Iteration (MemToken <std::string> (_output_entities)));
  std::string rate_container ("hybrid");
  rules.push_back (TagToken (_drawing_algorithm_tag) 
		    + StrToken (rate_container));
  rules.push_back (TagToken (_hybrid_base_rate_tag) 
		   + DblToken (_hybrid_base_rate));
  rules.push_back (TagToken (_base_volume_tag) + DblToken (_base_volume));
  rules.push_back (TagToken (_volume_step_tag) + DblToken (_volume_step));
  rules.push_back (TagToken (_volume_modifier_tag)
		   + Iteration (MemToken <std::string> (_volume_modifiers)
				+ MemToken <double> (_volume_weights)));
  
  bool failure = false;
  int line_number = 0;
  while (getline (file, _line))
    {
      ++line_number;
      if (!is_line_empty()) 
	{
	  InputLine input (_line);
	  std::vector <Rule>::iterator it = rules.begin();
	  while ((it != rules.end()) && (!(it->match (input)))) { ++it; }
	  if (it == rules.end())
	    {
	      std::cerr << "Unrecognized line (line " << line_number
			<< "):\n\t" << _line << "\n";
	      failure = true;
	    }
	}
    }
  if (!interpret_drawing_algorithm (rate_container)) { failure = true; }
  if (failure)
    {
      std::ostringstream message;
      message << "Please check invalid lines in parameter "
	      << "file " << filename;
      throw std::runtime_error (message.str());
    }
  _write_params_out();
}

// Forbidden
// SimulationParams::SimulationParams (const SimulationParams& other_simulation_params);
// SimulationParams& SimulationParams::operator= (const SimulationParams& other_simulation_params);


SimulationParams::~SimulationParams (void)
{
  delete _solver_factory;
  delete _rate_manager_factory;
  delete _rate_container_factory;
}

// ===========================
//  Public Methods - Commands
// ===========================
//

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
bool SimulationParams::is_line_empty (void) const
{
  std::istringstream line_stream (_line);
  std::string first_word;
  if (!(line_stream >> first_word) // line is empty
      || (first_word [0] == '#')) // line is commented
    {
      return true;
    }

  return false;
}
 
bool SimulationParams::interpret_drawing_algorithm (const std::string& tag)
{
  if (tag == "vector") 
    { _rate_container_factory = new RateVectorFactory;  }
  else if (tag == "tree") 
    { _rate_container_factory = new RateTreeFactory; }
  else if (tag == "hybrid") 
    { _rate_container_factory = new HybridRateContainerFactory;  }
  else
    {
      std::cerr << "ERROR: could not read " 
		<< _drawing_algorithm_tag << ", unknown algorithm.\n";
      return false;
    }
  return true;
}

void SimulationParams::_write_params_out (void) const
{
  std::ofstream file ((_output_dir + "/" + _param_file).c_str());
  if (file.fail())
    {      
      throw std::runtime_error ("Could not create output file. Output "
				"directory " + _output_dir + " must be a "
				"valid path to an existing directory");
    }

  file << _seed_tag << ": " << _seed << "\n";
  file << _initial_time_tag << ": " << _initial_time << "\n";
  file << _final_time_tag << ": " << _final_time << "\n";
  file << _input_files_tag << ":";
  for (std::vector <std::string>::const_iterator file_it = _input_files.begin();
       file_it != _input_files.end(); ++file_it)
    { file << "\n\t" << *file_it; }
  file << "\n";
  file << _drawing_algorithm_tag << ": " << *_rate_container_factory << "\n";
  file << _hybrid_base_rate_tag << ": " << _hybrid_base_rate << "\n";
  file << _base_volume_tag << ": " << _base_volume << "\n";
  file << _volume_step_tag << ": " << _volume_step << "\n";
  file << _volume_modifier_tag << ":";
  for (int i = 0; i < _volume_modifiers.size(); ++i)
    { file << "\n\t" << _volume_modifiers [i] << " " << _volume_weights [i]; }
  file << "\n";
}
