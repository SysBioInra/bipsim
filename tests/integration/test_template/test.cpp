
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
 
BOOST_AUTO_TEST_CASE (TestName)
{
  // Use any basic boost test such as BOOST_TEST_EQUAL
  // or write any information to file to be used by run_test.sh
}
 
BOOST_AUTO_TEST_SUITE_END()
