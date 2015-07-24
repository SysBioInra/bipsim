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
#include "binding.h"
#include "bindingsitehandler.h"
#include "chemicalhandler.h"
#include "chemical.h"
#include "reactionhandler.h"
#include "tablehandler.h"
#include "terminationsitehandler.h"
#include "parser.h"
#include "solver.h"

/**
 * @brief Program initiation.
 *
 * DETAILED DESCRIPTION OF PROGRAM
 */
int main ( )
{
  // create handlers
  BindingSiteHandler binding_site_handler;
  Binding::set_binding_site_handler (binding_site_handler);
  ChemicalHandler chemical_handler;
  ReactionHandler reaction_handler;
  TableHandler table_handler;
  TerminationSiteHandler termination_site_handler;

  // open new parser and load file
  Parser parser (chemical_handler, reaction_handler, binding_site_handler, termination_site_handler, table_handler);
  parser.parse_units( "../data/test_input.txt" );
  parser.parse_reactions( "../data/test_input.txt" );

  // std::cout << chemical_handler;
  // std::cout << reaction_handler;
  // std::cout << table_handler;
  std::cin.get();
  
  // solve system
  Solver solver;
  solver.add_reaction_list (reaction_handler.reference_list());
  solver.compute_dependencies();
  
  while (solver.time() < 1000)
    {
      solver.go_to_next_reaction();
      //#define DETAILED_DISPLAY
#ifdef DETAILED_DISPLAY
      std::cout << "Next reaction (t=" << solver.time() << ")" << std::endl;
      std::cout << chemical_handler;
      //      std::cin.get();
#endif
    }
  const Chemical& protein = chemical_handler.reference( std::string("protein") );
  std::cout << chemical_handler;
  std::cout << "Proteins: " << protein << std::endl;
  std::cout << solver.number_reactions_performed() << " reactions occurred." << std::endl;
}
