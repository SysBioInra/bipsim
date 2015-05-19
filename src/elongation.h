

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
#include "forwarddeclarations"

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
   * @brief Default constructor
   */
  Elongation (void);

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


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Defines the chemical element responsible for the elongation.
   * @param  processive_chemical 
   *  A reference to the polymerase that does the elongation.
   */
  void set_processive_chemical (ProcessiveChemical *processive_chemical);

  /**
   * @brief Defines the chemical that is synthesized.
   * @param  elongated_chemical
   *  A reference to the chemical that is elongated.
   */
  void set_elongated_chemical (SequenceChemical *elongated_chemical);

  /**
   * @brief Defines by how many bases the polymerase advances along the tepmlate
   *  at every elongation step.
   * @param  step_size Number of bases processed at each elongation step.
   */
  void set_step_size (int step_size);


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
  bool check_invariant (void);


private:

  // ============
  //  Attributes
  // ============
  //
  

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
