

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
   * @param  elongated_chemical
   *  Chemical that is elongated.
   * @param  step_size
   *  Number of bases processed at each elongation step.
   */
  Elongation ( ProcessiveChemical& processive_chemical, int step_size, double rate );

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
  void perform_forward( void );

  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  void perform_backward( void );
  

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns the forward reaction rate.
   * @return The forward reaction rate.
   */
  double forward_rate( void ) const;


  /**
   * @brief Returns the backward reaction rate.
   * @return The backward reaction rate.
   */
  double backward_rate( void ) const;


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
