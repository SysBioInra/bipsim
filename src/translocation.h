

/**
 * @file translocation.h
 * @authors Marc Dinh, Stephan Fischer
 * @brief Header for the Translocation class.
 * 
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
   * @param step_size Number of bases processed at each translocation step.
   * @param rate Translocation rate (in step/s).
   */
  Translocation (ProcessiveChemical& processive_chemical,
		 BoundChemical& chemical_after_step,
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
  

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from Reaction
  bool is_reaction_possible (void) const;

 private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Polymerase that does the translocation. */
  ProcessiveChemical& _processive_chemical;

  /** @brief New polymerase from after stepping. */
  BoundChemical& _chemical_after_step;

  /** @brief Number of bases processed at each translocation step. */
  int _step_size;

  /** @brief Translocation rate (in s^-1). */
  double _rate;

  // =================
  //  Private Methods
  // =================
  //
  // Redefined from Reaction
  void do_reaction (void);
  double compute_rate (void) const;
  void print (std::ostream& output) const;
};

// ======================
//  Inline declarations
// ======================
//
#include "processivechemical.h"

inline bool Translocation::is_reaction_possible (void) const
{
  return (_processive_chemical.number() > 0);
}

#endif // TRANSLOCATION_H
