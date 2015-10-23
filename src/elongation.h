

/**
 * @file elongation.h
 * @authors Marc Dinh, Stephan Fischer
 * @brief Header for the Elongation class.
 * 
 */


// Multiple include protection
//
#ifndef ELONGATION_H
#define ELONGATION_H


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
 * @brief Class that represents elongation of a polymerase.
 *
 * The polymerase is supposed to process along a sequence and to generate a
 * a product for which the sequence is a template. Elongation corresponds to
 * the movement along the sequence and synthesis of the product.
 */
class Elongation : public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param  processive_chemical 
   *  Polymerase that does the elongation.
   * @param  chemical_after_step
   *  Chemical after elongation.
   * @param  step_size
   *  Number of bases processed at each elongation step.
   * @param rate Elongation rate (in steps/s).
   */
  Elongation (ProcessiveChemical& processive_chemical,
	      BoundChemical& chemical_after_step, int step_size, double rate);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Elongation (Elongation& other_elongation);

  /**
   * @brief Destructor
   */
  virtual ~Elongation (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns whether there are enough chemicals to perform reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_reaction_possible (void) const;


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator
  //  */
  // Elongation& operator= (Elongation& other_elongation);

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //

 private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Polymerase that does the elongation. */
  ProcessiveChemical& _processive_chemical;

  /** @brief New polymerase from after stepping. */
  BoundChemical& _chemical_after_step;

  /** @brief Number of bases processed at each elongation step. */
  int _step_size;

  /** @brief Elongation rate (in s^-1). */
  double _rate;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Update chemical quantities according to the reaction.
   */
  virtual void do_reaction (void);
  
  /**
   * @brief Compute current reaction rates.
   * @return Current reaction rate.
   */
  virtual double compute_rate (void) const;

  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;
};

// ======================
//  Inline declarations
// ======================
//


#endif // ELONGATION_H
