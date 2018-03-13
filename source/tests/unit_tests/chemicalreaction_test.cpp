
/**
 * @file chemicalreaction_test.cpp
 * @brief Unit testing for ChemicalReaction class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ChemicalReaction
#include <boost/test/unit_test.hpp>
// #include <boost/test/floating_point_comparison.hpp>

// ==================
//  Project Includes
// ==================
//
#include "chemicalreaction.h"
#include "freechemical.h"
#include "boundchemical.h"

class CRBuilderFixture
{
public:
  CRBuilderFixture (void) {}

  void add_reactant (FreeChemical* chemical, int stoichiometry, int order)
  {
    chemicals.push_back (chemical);
    stoichiometries.push_back (stoichiometry);
    orders.push_back (order);
  }

  std::vector <int> stoichiometries, orders;
  std::vector <FreeChemical*> chemicals;
};

BOOST_FIXTURE_TEST_SUITE (ConstructionTests, CRBuilderFixture)
 
BOOST_AUTO_TEST_CASE (constructor_reversibleNoBound_works)
{
  FreeChemical A, B, C, D;
  add_reactant (&A, 1, 1); add_reactant (&B, 1, 1);   
  add_reactant (&C, -1, 1); add_reactant (&D, -1, 1);
  ChemicalReaction cr (chemicals, stoichiometries, orders, 1, 1);
}

BOOST_AUTO_TEST_CASE (constructor_forwardOnlyNoBound_works)
{
  FreeChemical A, B, C, D;
  add_reactant (&A, 1, 1); add_reactant (&B, 1, 1);   
  add_reactant (&C, -1, 1); add_reactant (&D, -1, 1);
  ChemicalReaction cr (chemicals, stoichiometries, orders, 1, 0);
}

BOOST_AUTO_TEST_CASE (constructor_reversibleBoundReactantAndProduct_works)
{
  FreeChemical A, B, C, D;
  add_reactant (&A, 1, 1); add_reactant (&B, 1, 1);   
  add_reactant (&C, -1, 1); add_reactant (&D, -1, 1);
  BoundChemical bound_reactant, bound_product;
  ChemicalReaction cr (chemicals, stoichiometries, orders, 1, 1, 
		       &bound_reactant, &bound_product);
}

BOOST_AUTO_TEST_CASE (constructor_forwardOnlyBoundReactantNoBoundProduct_works)
{
  FreeChemical A, B, C, D;
  add_reactant (&A, 1, 1); add_reactant (&B, 1, 1);   
  add_reactant (&C, -1, 1); add_reactant (&D, -1, 1);
  BoundChemical bound_reactant;
  ChemicalReaction cr (chemicals, stoichiometries, orders, 1, 0, 
		       &bound_reactant);
}

BOOST_AUTO_TEST_CASE (constructor_backwardOnlyBoundProductNoBoundReactant_works)
{
  FreeChemical A, B, C, D;
  add_reactant (&A, 1, 1); add_reactant (&B, 1, 1);   
  add_reactant (&C, -1, 1); add_reactant (&D, -1, 1);
  BoundChemical bound_product;
  ChemicalReaction cr (chemicals, stoichiometries, orders, 0, 1, 
		       0, &bound_product);
}
 
BOOST_AUTO_TEST_SUITE_END()

