

/**
 * @file compositiontable.h
 * @brief Header for the CompositionTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef COMPOSITION_TABLE_H
#define COMPOSITION_TABLE_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <map> // std::map
#include <list> // std::list

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h"
#include "simulatorinput.h"

/**
 * @brief Class storing letter to chemicals relations.
 *
 * CompositionTable stores relations indicating chemicals that are used to
 * polymerize a sequence, one or a list per letter of the sequence. These
 * chemicals are typically released during Degradation of the sequence.
 * @sa Degradation.
 */
class CompositionTable : public SimulatorInput
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Default constructor. */
  // CompositionTable (void);
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // CompositionTable (const CompositionTable& other_composition_table);
  // /* @brief Assignment operator. */
  // CompositionTable& operator= (const CompositionTable& other_table);  
  // /* @brief Destructor. */
  // ~CompositionTable (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add new composition rule.
   * @param letter Letter of the sequence.
   * @param composition List of chemicals implementing the letter who will
   *  be released during degradation.
   */
  void add_rule (char letter, const std::list<Chemical*>& composition);

  /**
   * @brief Get composition of a sequence according to registered rules.
   * @param sequence Sequence whose composition should be computed.
   * @return Map containing associating a reference to a chemical with the
   *  number of chemicals that would be released upon total degradation.
   */
  std::map <Chemical*, int> composition (const std::string& sequence) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Map containing the composition rules. */
  std::map <char, std::list <Chemical*> > _rules;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
inline void CompositionTable::add_rule (char letter,
					const std::list <Chemical*>& chemicals)
{
  _rules [letter] = chemicals;
}


#endif // COMPOSITION_TABLE_H
