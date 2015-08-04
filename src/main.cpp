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
int main ( )
{ 
  // read cell state from file
  CellState cell_state ("../data/test_input.txt");

  // solve system
  // ReactionClassification classification;
  // int class_id = classification.create_new_class (1);
  // classification.add_reaction_list_to_class (class_id, cell_state.reaction_list());
  NaiveSolver solver (0, cell_state.reaction_list());
  
  while (solver.time() < 2000)
    {
      solver.go_to_next_reaction();
      // std::cout << "Next reaction (t=" << solver.time() << ")" << std::endl;
      // cell_state.print_chemicals;
      // std::cin.get();
    }
  const Chemical& protein = cell_state.chemical ("protein");
  cell_state.print_chemicals();
  std::cout << "Proteins: " << protein << std::endl;
  std::cout << solver.number_reactions_performed() << " reactions occurred." << std::endl;
}
