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
 * @file ratemanagerfactory.h
 * @brief Header for the RateManagerFactory, NaiveRateManagerFactory
 *  and DependencyRateManagerFactory classes.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_MANAGER_FACTORY_H
#define RATE_MANAGER_FACTORY_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class for RateManager factories.
 */
class RateManagerFactory
{  
 public:
  /** @brief Destructor (empty but virtual). */
  virtual ~RateManagerFactory (void) {};

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create a RateManager on heap (needs to be deleted by user).
   * @param params Simulation parameter.
   * @param reactions Vector of reactions that need to be handled by the rate 
   *  manager.
   * @return RateManager whose implementation depends on the factory used.
   */
  virtual RateManager* create (const SimulationParams& params,
			       const std::vector <Reaction*>& reactions) const = 0;
};

/**
 * @brief Factory class creating NaiveRateManager.
 */
class NaiveRateManagerFactory : public RateManagerFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  RateManager* create (const SimulationParams& params,
		       const std::vector <Reaction*>& reactions) const;
};

/**
 * @brief Factory class creating DependencyRateManager.
 */
class DependencyRateManagerFactory : public RateManagerFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  RateManager* create (const SimulationParams& params,
		       const std::vector <Reaction*>& reactions) const;
};

// ======================
//  Inline declarations
// ======================
//
#include "naiveratemanager.h"
#include "dependencyratemanager.h"

inline RateManager* 
NaiveRateManagerFactory::create (const SimulationParams& params,
				 const std::vector <Reaction*>& reactions) const
{
  return new NaiveRateManager (params, reactions);
}

inline RateManager* 
DependencyRateManagerFactory::create (const SimulationParams& params,
				      const std::vector <Reaction*>& reactions) const
{
  return new DependencyRateManager (params, reactions);
}


#endif // RATE_MANAGER_FACTORY_H
