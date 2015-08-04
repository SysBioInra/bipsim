

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
  Elongation (ProcessiveChemical& processive_chemical, BoundChemical& chemical_after_step, int step_size, double rate );

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
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  virtual void perform_forward( void );

  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  virtual void perform_backward( void );

  /**
   * @brief Update reaction rates.
   */
  virtual void update_rates ( void );
  

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @return Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;

  /**
   * @brief Returns whether there are enough chemicals to perform forward reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_forward_reaction_possible (void) const;

  /**
   * @brief Returns whether there are enough chemicals to perform backward reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_backward_reaction_possible (void) const;


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


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  bool check_invariant (void) const;


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

};

// ======================
//  Inline declarations
// ======================
//


#endif // ELONGATION_H
