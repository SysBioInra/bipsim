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
 * @file doublestrandrecruitment.h
 * @brief Header for the DoubleStrandRecruitment class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef DOUBLE_STRAND_RECRUITMENT_H
#define DOUBLE_STRAND_RECRUITMENT_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "reaction.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class representing recruitment reactions on a double strand.
 */
class DoubleStrandRecruitment : public Reaction
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor. 
   * @param recruiter BoundChemical facilitating binding of recruit on 
   *  opposite strand.
   * @param recruit FreeChemical able to bind on strand opposite of recruiter.
   * @param bound_recruit Bound form of the recruit.
   * @param rate Reaction rate constant.
   */
  DoubleStrandRecruitment (BoundChemical& recruiter, 
			   FreeChemical& recruit,
			   BoundChemical& bound_recruit, 
			   double rate);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // DoubleStrandRecruitment (const DoubleStrandRecruitment& other);
  // /* @brief Assignment operator. */
  // DoubleStrandRecruitment& operator= (const DoubleStrandRecruitment& other);
  // /* @brief Destructor. */
  // ~DoubleStrandRecruitment (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from Reaction
  void handle_volume_change (double volume);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from Reaction
  bool is_reaction_possible (void) const;

 private:  
  // =================
  //  Private Methods
  // =================
  //
  // Redefined from Reaction.
  void do_reaction (void);
  double compute_rate (void) const;
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief Chemical facilitating binding of recruit on opposite strand. */
  BoundChemical& _recruiter;
  /** @brief Chemical able to bind on strand opposite of recruiter. */
  FreeChemical& _recruit;
  /** @brief Bound form of the recruit. */
  BoundChemical& _bound_recruit;
  /** @brief Rate constant. */
  double _rate;
  /** @brief Volume constant incorporating rate constant. */
  double _volume_constant;
};

// ======================
//  Inline declarations
// ======================
//
inline void DoubleStrandRecruitment::handle_volume_change (double volume)
{
  _volume_constant = _rate / volume;
}

#endif // DOUBLE_STRAND_RECRUITMENT_H
