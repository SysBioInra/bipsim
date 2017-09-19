// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 


/**
 * @file randomhandler.cpp
 * @brief Implementation of the RandomHandler class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <numeric> // std::partial_sum

// ==================
//  Project Includes
// ==================
//
#include "randomhandler.h"
#include "biasedwheel.h"
#include "macros.h" // REQUIRE ENSURE

// ==========================
//  Constructors/Destructors
// ==========================
//
RandomHandler RandomHandler::_instance;

RandomHandler::RandomHandler (void)
{
#ifdef HAVE_BOOST
  _generator.seed (0);
#else
  srand (0);
#endif
}

// Not needed for this class (use of compiler-generated versions)
// RandomHandler::RandomHandler (const RandomHandler& other);
// RandomHandler& RandomHandler::operator= (RandomHandler& other);
// RandomHandler::~RandomHandler (void);

// ===========================
//  Public Methods - Commands
// ===========================
//

int RandomHandler::draw_index (const std::vector<int>& weights)
{
  /** @pre There must be at least one item. */
  REQUIRE (weights.size() > 0); 
  std::vector<int> cumulated_weights (weights.size());
  std::partial_sum (weights.begin(), weights.end(), cumulated_weights.begin());
  /** @pre Total weight must be strictly positive. */
  REQUIRE (cumulated_weights.back() > 0);

  // we draw a number in the weight distribution
  BiasedWheel<int> biased_wheel (cumulated_weights);
  int result = 
    biased_wheel.find_index (draw_uniform (1, cumulated_weights.back()));

  /** @post The weight associated to the drawn index must be positive. */
  ENSURE (weights[result] > 0);
  ENSURE ((result < weights.size()) && (result >= 0));
  return result;
}

int RandomHandler::draw_index (const std::vector<double>& weights)
{
  /** @pre There must be at least one item. */
  REQUIRE (weights.size() > 0); 
  std::vector<double> cumulated_weights (weights.size());
  std::partial_sum (weights.begin(), weights.end(), cumulated_weights.begin());
  /** @pre Total weight must be strictly positive. */
  REQUIRE (cumulated_weights.back() > 0);

  // we draw a number in the weight distribution
  BiasedWheel<double> biased_wheel (cumulated_weights);  
  int result = 
    biased_wheel.find_index (draw_uniform (cumulated_weights.back()*1e-16, 
					   cumulated_weights.back()));
  
  /** @post The weight associated to the drawn index must be positive. */
  ENSURE (weights[result] > 0);
  ENSURE ((result < weights.size()) && (result >= 0));
  return result;
}




// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
