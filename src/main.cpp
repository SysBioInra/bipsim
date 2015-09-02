/**
 * @file main.cpp
 * @brief Contains the main routine of the program.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */

// #define CLASSIFICATION

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
  double simulation_time = 1000;
  if (argc > 1)
    {
      std::string argument(argv[1]);
      std::istringstream word_stream (argument);
      int time;
      if (word_stream >> time)
       	{
      	  simulation_time = time;
      	}
      else
       	{
       	  std::cerr << "Input makes no sense, using default simulation time (" << simulation_time << ") instead." << std::endl;
      	}
    }

  Simulation sim ("../data/test_input.txt");
  sim.run (simulation_time);
}
