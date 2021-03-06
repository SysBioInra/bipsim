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
 * @file ratemanager.h
 * @brief Header for the RateManager class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_MANAGER_H
#define RATE_MANAGER_H

// ==================
//  General Includes
// ==================
//
#include <iostream> // std::ostream
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class for handling updates of reaction rates.
 *
 * RateManager is an abstract class that stores reaction rates and
 * provides an interface for rate updating that has to be implemented
 * by inheriting classes. Objects are built from vectors of reactions
 * and are responsible for updating rates when prompted by user. Rates
 * are made available in a vector that of same size as reaction vector.
 */
class RateManager
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param params Simulation parameters.
   * @param reactions Vector of reactions whose rates need to be stored and
   *  updated.
   */
  RateManager (const SimulationParams& params,
	       const std::vector <Reaction*>& reactions);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  RateManager (const RateManager& other_rate_manager);
  /** @brief Assignment operator. */
  RateManager& operator= (const RateManager& other_rate_manager);

 public:
  /**
   * @brief Destructor.
   */
  virtual ~RateManager (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update rates according to current chemical levels.
   */
  virtual void update_rates (void) = 0;

  /**
   * @brief Recompute rates for all reactions and refill whole _rates vector.
   */
  void compute_all_rates (void);

  /**
   * @brief Draw a random reaction according to current rates.
   * @return Reaction obtained from multinomial drawing.
   */
  Reaction& random_reaction (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to total reaction rate.
   * @return Total reaction rate (as of last update).
   */
  double total_rate (void) const;

  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param manager Reference to the manager whose information should be 
   *  written.
   */
  friend std::ostream& operator<< (std::ostream& output,
				   const RateManager& manager);

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Update rates for reaction at a specific index.
   * @param reaction_index Index of the reaction in the reaction vector.
   */
  void update_reaction (int reaction_index);

  /**
   * @brief Cumulate reaction rates.
   */
  void cumulate_rates (void);

  /**
   * @brief Accessor to reaction vector.
   */
  const std::vector <Reaction*>& reactions (void) const;
  

 private:
  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Vector of reactions currently handled by the manager.
   */
  std::vector <Reaction*> _reactions;

  /** @brief Container storing reaction rates. */
  RateContainer* _rates;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
#include <numeric> // std::partial_sum
#include "reaction.h"
#include "ratecontainer.h"

inline Reaction& RateManager::random_reaction (void)
{
  return *(_reactions [_rates->random_index()]);
}

inline void RateManager::compute_all_rates (void)
{
  for (std::size_t i = 0; i < _reactions.size(); ++i) { update_reaction (i); }
}

inline void RateManager::cumulate_rates (void)
{
  _rates->update_cumulates();
}

inline void RateManager::update_reaction (int reaction_index)
{
  Reaction* reaction_to_update = _reactions [reaction_index];
  reaction_to_update->update_rate();
  _rates->set_rate (reaction_index, reaction_to_update->rate());
}

inline double RateManager::total_rate (void) const
{
  return _rates->total_rate();
}

inline const std::vector<Reaction*>& RateManager::reactions (void) const
{
  return _reactions;
}

inline std::ostream& operator<< (std::ostream& output,
				 const RateManager& manager)
{
  output << *(manager._rates);
  return output;
}

#endif // RATE_MANAGER_H
