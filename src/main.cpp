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
#include "bindingsitehandler.h"
#include "chemicalsequence.h"
#include "binding.h"
#include "bindingsite.h"
#include "boundchemical.h"

void print_test_message (  )
{
}

/**
 * @brief Program initiation.
 *
 * DETAILED DESCRIPTION OF PROGRAM
 */
int main ( )
{
  // create chemical sequences
  ChemicalSequence sample_rna;
  sample_rna.set_length ( 300 );
  sample_rna.add ( 3 );
  ChemicalSequence sample_rna2;
  sample_rna2.set_length ( 300 );
  sample_rna2.add ( 3 );

  // create binding sites
  BindingSiteHandler test;
  test.create_binding_site ( "RBS", sample_rna, 100, 5, 10e-4 );
  test.create_binding_site ( "RBS", sample_rna2, 100, 5, 10e-4 );
  test.create_binding_site ( "RBS", sample_rna, 200, 5, 10e-4 );
  test.create_binding_site ( "other binding site", sample_rna, 150, 10, 10e-4 );

  // create an element that can bind
  Chemical fun_chemical;
  fun_chemical.add( 10 );
  BoundChemical bound_fun_chemical;

  // create a binding reaction
  int RBS_id = test.retrieve_id ( "RBS" );
  Binding::set_binding_site_handler( test );
  Binding my_binding;
  my_binding.set_unit_to_bind ( fun_chemical );
  my_binding.set_binding_result ( bound_fun_chemical );
  my_binding.set_binding_site_family ( RBS_id );
  
  // perform some bindings
  for ( int i = 0; i < 9; i++ )
    {
      my_binding.perform_forward();
      std::cout << "fun_chemical has " << fun_chemical.number() << " elements." << std::endl;
      std::cout << "bound_fun_chemical has " << bound_fun_chemical.number() << " elements." << std::endl;
      test.print();
    }
  for ( int i = 0; i < 9; i++ )
    {
      my_binding.perform_backward();
      std::cout << "fun_chemical has " << fun_chemical.number() << " elements." << std::endl;
      std::cout << "bound_fun_chemical has " << bound_fun_chemical.number() << " elements." << std::endl;
      test.print();
    }

  return 0;
}
