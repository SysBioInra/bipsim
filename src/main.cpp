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
#include "cellstate.h"
#include "chemical.h"
#include "naivesolver.h"
#include "manualdispatchsolver.h"
#include "reactionclassification.h"

/**
 * @brief Program initiation.
 *
 * DETAILED DESCRIPTION OF PROGRAM
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


  // read cell state from file
  CellState cell_state ("../data/test_input.txt");

  // solve system
  // ReactionClassification classification;
  // int class_id = classification.create_new_class (1);
  // classification.add_reaction_list_to_class (class_id, cell_state.reaction_list());
  NaiveSolver solver (0, cell_state);

  std::cout << "Solving from t = 0 to t = " << simulation_time << "..." << std::endl;
  while (solver.time() < simulation_time)
    {
      solver.go_to_next_reaction();
      // std::cout << "Next reaction (t=" << solver.time() << ")" << std::endl;
      // cell_state.print_chemicals;
      // std::cin.get();
    }

  std::ofstream output ("output.txt");
  //std::ostream& output = std::cout;
  const Chemical& protein = cell_state.chemical ("protein");
  cell_state.print_chemicals (output);
  output << "Proteins: " << protein << std::endl;
  output << solver.number_reactions_performed() << " reactions occurred." << std::endl;
}
