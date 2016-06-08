/**
 * @file main.cpp
 * @brief Contains the main routine of the program.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <iostream> // std::cout
#include <sstream> // std::istringstream
#include <ctime> // clock()
#include <cstdlib> // EXIT_FAILURE
#include <exception>

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
      clock_t t = clock();
      try 
	{
	  Simulation simulation (param_file);
	  simulation.run();
	  t = clock() - t;
	  std::cout << "CPU runtime: " << t << " clicks ("
		    << ((float)t)/CLOCKS_PER_SEC << " seconds).\n";
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Interrupting execution: " << e.what() << ".\n";
	  return EXIT_FAILURE;
	}
    }
  else
    {
      std::cerr << "Input makes no sense, please provide valid path to a "
		<< "parameter file (relative to program)." << std::endl;
    }
}
