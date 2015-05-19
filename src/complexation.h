

/**
 * @file complexation.h
 * @authors Marc Dinh, Stephan Fischer
 * @brief Header for the Complexation class.
 * 
 */


// Multiple include protection
//
#ifndef COMPLEXATION_H
#define COMPLEXATION_H

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
 * @brief This class represents complexation reactions.
 *
 * A complexation is defined by two chemical elements and an affinity between
 * them. They can combine into a new entity.
 */
class Complexation : public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  Complexation (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Complexation (Complexation& other_complexation);

  /**
   * @brief Destructor
   */
  virtual ~Complexation (void);

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
   * @brief Defines the two chemicals that can form a complex.
   * @param  component_a A reference to the first chemical involved.
   * @param  component_b A reference to the second chemical involved.
   */
  void set_components (Chemical *component_a, Chemical *component_b);

  /**
   * @brief Defines the entity that represents the complexed form.
   * @param  complex A reference to the complex.
   */
  void set_complex (Chemical *complex);

  /**
   * @brief Defines the affinity between the two complex components.
   * @param  affinity Affinity value.
   */
  void set_affinity (double affinity);

  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator
  //  */
  // Complexation& operator= (Complexation& other_complexation);


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


#endif // COMPLEXATION_H
