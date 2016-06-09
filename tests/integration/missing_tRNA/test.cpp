
// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tRNAConsumption
#include <boost/test/unit_test.hpp>
// #include <boost/test/floating_point_comparison.hpp>

// ==================
//  Project Includes
// ==================
//
#include "simulation.h"

class SimulationFixture
{
public:
  SimulationFixture (void)
    : simulation ("input/params.in")
  {
    simulation.run();
  }

  Simulation simulation;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, SimulationFixture)
 
BOOST_AUTO_TEST_CASE (RibosomeProteinNumber_MustBeZero)
{
  FreeChemical& ribosome = simulation.cell_state().fetch <FreeChemical> ("ribosome");
  FreeChemical& protein = simulation.cell_state().fetch <FreeChemical> ("protein");
  BOOST_CHECK_EQUAL (ribosome.number(), 0);     
  BOOST_CHECK_EQUAL (protein.number(), 0);
}
 
BOOST_AUTO_TEST_SUITE_END()
