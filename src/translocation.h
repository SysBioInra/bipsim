

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
   * @param  processive_chemical 
   *  Polymerase that does the translocation.
   * @param  chemical_after_step
   *  Chemical after translocation.
   * @param  step_size
   *  Number of bases processed at each translocation step.
   * @param rate Translocation rate (in steps/s).
   */
  Translocation (ProcessiveChemical& processive_chemical,
		 BoundChemical& chemical_after_step,
		 int step_size, double rate);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Translocation (Translocation& other_translocation);

  /**
   * @brief Destructor
   */
  virtual ~Translocation (void);

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
  // Translocation& operator= (Translocation& other_translocation);

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


#endif // TRANSLOCATION_H
