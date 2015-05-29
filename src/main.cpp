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
#include "bindingsite.h"
#include "bindingsitehandler.h"
#include "boundchemical.h"
#include "chemicalsequence.h"
#include "complexation.h"
#include "elongation.h"
#include "processivechemical.h"
#include "terminationsitehandler.h"
#include "solver.h"

/**
 * @brief Program initiation.
 *
 * DETAILED DESCRIPTION OF PROGRAM
 */
int main ( )
{
  // create chemical sequences
  ChemicalSequence rna;
  rna.set_length ( 300 );
  rna.add ( 3 );
  ChemicalSequence rna2;
  rna2.set_length ( 300 );
  rna2.add ( 3 );

  // create binding sites
  BindingSiteHandler binding_sites;
  double k_on = 10;
  double k_off = 1;
  binding_sites.create_site ( "RBS", rna, 100, 5, k_on, k_off );
  binding_sites.create_site ( "RBS", rna, 200, 5, k_on, k_off );
  binding_sites.create_site ( "RBS", rna2, 100, 5, k_on, k_off );
  binding_sites.create_site ( "other binding site", rna, 150, 10, k_on, k_off );

  // create termination sites
  TerminationSiteHandler termination_sites;
  termination_sites.create_site ( "hairpin", rna, 142, 9 );
  termination_sites.create_site ( "hairpin", rna, 251, 9 );
  termination_sites.create_site ( "hairpin", rna2, 199, 9 );

  // create an element that can bind
  int hairpin_id = termination_sites.retrieve_id ( "hairpin" );
  Chemical free_30S;
  Chemical free_50S;
  free_30S.add( 10 );
  free_50S.add( 10 );
  BoundChemical stalled_ribosome;
  BoundChemical bound_ribosome;
  ProcessiveChemical moving_ribosome ( stalled_ribosome );
  moving_ribosome.add_recognized_termination_site ( hairpin_id );

  // create some reactions
  int RBS_id = binding_sites.retrieve_id ( "RBS" );
  Binding::set_binding_site_handler( binding_sites );
  Binding ribosome_binding ( free_30S, bound_ribosome, RBS_id );
  Complexation ribosome_assembly ( bound_ribosome, free_50S, moving_ribosome, 0.01, 0 );
  Elongation ribosome_elongation ( moving_ribosome, 3, 10 );

  
  // solve system
  Solver solver;
  solver.add_reaction (ribosome_binding);
  solver.add_reaction (ribosome_assembly);
  solver.add_reaction (ribosome_elongation);

  while ( solver.time() < 100 )
    {
      solver.go_to_next_reaction ();

      std::cout << "There are " << free_30S.number() << " free 30S." << std::endl;
      std::cout << "There are " << free_50S.number() << " free 50S." << std::endl;
      std::cout << "There are " << bound_ribosome.number() << " bound ribosomes." << std::endl;
      std::cout << "There are " << moving_ribosome.number() << " moving ribosomes." << std::endl;
      std::cout << "There are " << stalled_ribosome.number() << " stalled ribosomes." << std::endl;
      binding_sites.print();
      std::cout << "BOUND RIBOSOMES:" << std::endl;
      bound_ribosome.print();
      moving_ribosome.print();
      stalled_ribosome.print();
      std::cin.get();
    }
}
