

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

// ==================
//  Project Includes
// ==================
//
#include "simulationparams.h"


// ===================
//  Static attributes
// ===================
//
const std::string SimulationParams::_initial_time_tag = "INITIAL_TIME";

const std::string SimulationParams::_final_time_tag = "FINAL_TIME";

const std::string SimulationParams::_input_files_tag = "INPUT_FILES";

const std::string SimulationParams::_output_file_tag = "OUTPUT_FILE";

const std::string SimulationParams::_output_step_tag = "OUTPUT_STEP";

const std::string SimulationParams::_output_entities_tag = "OUTPUT_ENTITIES";

// ==========================
//  Constructors/Destructors
// ==========================
//
SimulationParams::SimulationParams (const std::string& filename)
  : _initial_time (0)
  , _final_time (1000)
  , _output_step (10000)
{
  std::ifstream file (filename.c_str());
  if (file.fail())
    {
      std::cerr << "ERROR: could not open parameter file \"" << filename
		<<  "\".\n";
    }
  else
    {
      std::string line;
      while (getline (file, line))
	{
	  if (!(is_line_empty (line)
		|| read_initial_time (line)
		|| read_final_time (line)
		|| read_input_files (line)
		|| read_output_file (line)
		|| read_output_step (line)
		|| read_output_entities (line)))
	    {
	      std::cerr << "Warning: unrecognized line:\n"
			<< "\t" << line << "\n";
	    }
	}
    }
}

// Not needed for this class (use of default copy constructor) !
// SimulationParams::SimulationParams (const SimulationParams& other_simulation_params);

SimulationParams::~SimulationParams (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//


// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// SimulationParams& SimulationParams::operator= (const SimulationParams& other_simulation_params);


// =================
//  Private Methods
// =================
//
bool SimulationParams::is_line_empty (const std::string& line)
{
  std::istringstream line_stream (line);
  std::string first_word;
  if (!(line_stream >> first_word) // line is empty
      || (first_word [0] == '#')) // line is commented
    {
      return true;
    }

  return false;
}
 

bool SimulationParams::read_initial_time (const std::string& line)
{
  std::istringstream line_stream (line);
  if (!check_tag (line_stream, _initial_time_tag)) return false;
  return read_or_error (line_stream, _initial_time, _initial_time_tag);
}

bool SimulationParams::read_final_time (const std::string& line)
{
  std::istringstream line_stream (line);
  if (!check_tag (line_stream, _final_time_tag)) return false;
  return read_or_error (line_stream, _final_time, _final_time_tag);
}

bool SimulationParams::read_input_files (const std::string& line)
{
  std::istringstream line_stream (line);
  if (!check_tag (line_stream, _input_files_tag)) return false;

  std::string next_file;
  while (line_stream >> next_file)
    {
      _input_files.push_back (next_file);
    }
  return true;
}
  
bool SimulationParams::read_output_file (const std::string& line)
{
  std::istringstream line_stream (line);
  if (!check_tag (line_stream, _output_file_tag)) return false;
  return read_or_error (line_stream, _output_file, _output_file_tag);
}

bool SimulationParams::read_output_step (const std::string& line)
{
  std::istringstream line_stream (line);
  if (!check_tag (line_stream, _output_step_tag)) return false;
  return read_or_error (line_stream, _output_step, _output_step_tag);
}
  
bool SimulationParams::read_output_entities (const std::string& line)
{
  std::istringstream line_stream (line);
  if (!check_tag (line_stream, _output_entities_tag)) return false;

  std::string next_entity;
  while (line_stream >> next_entity)
    {
      _output_entities.push_back (next_entity);
    }
  return true;  
}
