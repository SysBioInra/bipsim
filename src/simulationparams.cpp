

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
const std::string SimulationParams::_param_file = "params.out";
const std::string SimulationParams::_reaction_file = "reactions.out";
const std::string SimulationParams::_output_step_tag = "OUTPUT_STEP";
const std::string SimulationParams::_output_entities_tag = "OUTPUT_ENTITIES";
const std::string SimulationParams::_drawing_algorithm_tag = "DRAWING_ALGORITHM";
const std::string SimulationParams::_hybrid_base_rate_tag = "HYBRID_BASE_RATE";

// ==========================
//  Constructors/Destructors
// ==========================
//
SimulationParams::SimulationParams (const std::string& filename)
  : _seed (0)
  , _initial_time (0)
  , _final_time (1000)
  , _output_step (10000)
  , _solver_factory (new NaiveSolverFactory())
  , _rate_manager_factory (new DependencyRateManagerFactory())
  , _rate_container_factory (new HybridRateContainerFactory())
  , _hybrid_base_rate (1)
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

  bool failure = false;
  int line_number = 0;
  while (getline (file, _line))
    {
      ++line_number;
      if (!(is_line_empty()
	    || decode_tag_value (_seed_tag, _seed)
	    || decode_tag_value (_initial_time_tag, _initial_time)
	    || decode_tag_value (_final_time_tag, _final_time)
	    || read_input_files()
	    || decode_tag_value (_output_dir_tag, _output_dir)
	    || decode_tag_value (_output_step_tag, _output_step)
	    || read_output_entities()
	    || read_drawing_algorithm()
	    || decode_tag_value (_hybrid_base_rate_tag, _hybrid_base_rate)))
	{
	  std::cerr << "Warning: unrecognized line (line " << line_number
		    << "):\n\t" << _line << "\n";
	  failure = true;
	}
    }
  if (failure)
    {
      std::ostringstream message;
      message << "Please check invalid lines in parameter file " << filename;
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
 
bool SimulationParams::read_input_files (void)
{
  std::istringstream line_stream (_line);
  if (!check_tag (line_stream, _input_files_tag)) return false;

  std::string next_file;
  while (line_stream >> next_file)
    {
      _input_files.push_back (next_file);
    }
  return true;
}
bool SimulationParams::read_output_entities (void)
{
  std::istringstream line_stream (_line);
  if (!check_tag (line_stream, _output_entities_tag)) return false;

  std::string next_entity;
  while (line_stream >> next_entity)
    {
      _output_entities.push_back (next_entity);
    }
  return true;  
}

bool SimulationParams::read_drawing_algorithm (void)
{
  std::istringstream line_stream (_line);
  if (!check_tag (line_stream, _drawing_algorithm_tag)) return false;
  std::string algorithm_tag;
  if (!read_or_error (line_stream, algorithm_tag, _drawing_algorithm_tag))
    { return false; }
  if (algorithm_tag == "vector")
    { 
      delete _rate_container_factory;
      _rate_container_factory = new RateVectorFactory;
    }
  else if (algorithm_tag == "tree")
    { 
      delete _rate_container_factory;
      _rate_container_factory = new RateTreeFactory;
    }
  else if (algorithm_tag == "hybrid")
    { 
      delete _rate_container_factory;
      _rate_container_factory = new HybridRateContainerFactory;
    }
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
  for (std::list<std::string>::const_iterator file_it = _input_files.begin();
       file_it != _input_files.end(); ++file_it)
    { file << "\n\t" << *file_it; }
  file << "\n";
  file << _drawing_algorithm_tag << ": " << *_rate_container_factory << "\n";
  file << _hybrid_base_rate_tag << ": " << _hybrid_base_rate << "\n";
}
