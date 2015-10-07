/**
 * @file main.cpp
 * @brief Contains the main routine of the program.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <iostream> // std::cout
#include <fstream> // std::ofstream
#include <sstream> // std::istringstream

// ==================
//  Project Includes
// ==================
//
#include "simulation.h"

/**
 * @brief Program initiation.
 *
 * Initialize simulatior and run.
 */
int main (int argc, char *argv[])
{ 
  // read parameters
  std::string param_file;
  if (argc > 1)
    {
      std::string argument(argv[1]);
      std::istringstream word_stream (argument);
      word_stream >> param_file;
    }

  if (param_file != "")
    {
      Simulation simulation (param_file);
      simulation.run();
    }
  else
    {
      std::cerr << "Input makes no sense, please provide valid path to a parameter file (relative to program)." << std::endl;
    }
}
