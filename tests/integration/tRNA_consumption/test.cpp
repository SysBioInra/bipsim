
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
 
BOOST_AUTO_TEST_CASE (tRNAConsumptionIs10)
{
  FreeChemical& tRNA = 
    simulation.cell_state().fetch <FreeChemical> ("tRNA_F");     
  BOOST_CHECK_EQUAL (tRNA.number(),  90);
}
 
BOOST_AUTO_TEST_SUITE_END()
