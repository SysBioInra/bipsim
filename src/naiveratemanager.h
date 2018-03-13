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
 * @file naiveratemanager.h
 * @brief Header for the NaiveRateManager class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef NAIVE_RATE_MANAGER_H
#define NAIVE_RATE_MANAGER_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "ratemanager.h"

/**
 * @brief Class handling updates of reaction rates in a naive way.
 *
 * NaiveRateManager inherits and implements the interface provided by 
 * RateManager. It handles its updating task by a brute force approach: every 
 * time it is supposed  to update rates, it recomputes every single rate it 
 * knows of.
 */
class NaiveRateManager : public RateManager
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param params Simulation parameters.
   * @param reactions Vector of reactions whose rates need to be stored and updated.
   */
  NaiveRateManager (const SimulationParams& params,
		    const std::vector <Reaction*>& reactions);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // NaiveRateManager (const NaiveRateManager& other_manager);
  // /* @brief Assignment operator. */
  // NaiveRateManager& operator= (const NaiveRateManager& other_manager);
  // /* @brief Destructor. */
  // virtual ~NaiveRateManager (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from RateManager.
  /**
   * @brief Update rates according to current chemical levels.
   *
   * This manager uses a brute force approach: when prompted to update, it 
   * recomputes every single rate under its control.
   */
  virtual void update_rates (void);


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
private:

  // ============
  //  Attributes
  // ============
  //

  // ================
  //  Pivate Methods
  // ================
  //
};

// ======================
//  Inline declarations
// ======================
//

#endif // NAIVE_RATE_MANAGER_H
