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
 * @file translocation.h
 * @authors Marc Dinh, Stephan Fischer
 * @brief Header for the Translocation class.
 */

// Multiple include protection
//
#ifndef TRANSLOCATION_H
#define TRANSLOCATION_H


// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "reaction.h"

/**
 * @brief Class that represents translocation of a polymerase.
 *
 * The polymerase is supposed to process along a sequence and to generate a
 * a product for which the sequence is a template. Translocation corresponds to
 * the movement along the sequence and synthesis of the product.
 */
class Translocation : public Reaction
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param processive_chemical Polymerase that does the translocation.
   * @param chemical_after_step Chemical after translocation.
   * @param stalled_form BoundChemical that results when the translocating 
   *  chemical encounters end of sequence.
   * @param step_size Number of bases processed at each translocation step.
   * @param rate Translocation rate (in step/s).
   */
  Translocation (BoundChemical& processive_chemical,
		 BoundChemical& chemical_after_step,
		 BoundChemical& stalled_form,
		 int step_size, double rate);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Translocation (Translocation& other_translocation);
  // /* @brief Assignment operator. */
  // Translocation& operator= (Translocation& other_translocation);
  // /* @brief Destructor. */
  // ~Translocation (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from reaction
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
  // Redefined from Reaction
  void do_reaction (void);
  double compute_rate (void) const;
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief Form that translocates. */
  BoundChemical& _processive_chemical;
  /** @brief Form after translocation. */
  BoundChemical& _chemical_after_step;
  /** @brief Form if translocation reached end of sequence. */
  BoundChemical& _stalled_form;

  /** @brief Number of bases processed at each translocation step. */
  int _step_size;
  /** @brief Translocation rate (in s^-1). */
  double _rate;
  /** @brief Volume constant. */
  double _volume_constant;
};

// ======================
//  Inline declarations
// ======================
//
#include "boundchemical.h"

inline bool Translocation::is_reaction_possible (void) const
{
  return (_processive_chemical.number() > 0);
}

inline void Translocation::handle_volume_change (double volume)
{
  // no volume dependency: _volume_constant = _rate;
}

#endif // TRANSLOCATION_H
