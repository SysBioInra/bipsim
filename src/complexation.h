

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
#include "forwarddeclarations.h"
#include "reaction.h"

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
   * @param  component_a Reference to the first chemical involved.
   * @param  component_b Reference to the second chemical involved.
   * @param  complex Reference to the complex.
   * @param k_on Association constant.
   * @param k_off Dissociation constant.
   */
  Complexation (Chemical& component_a, Chemical& component_b, Chemical& complex, double k_on, double k_off);

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
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  virtual void perform_forward ( void );

  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  virtual void perform_backward ( void );

  /**
   * @return Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;

  /**
   * @brief Update reaction rates.
   */
  virtual void update_rates ( void );


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
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
  // Complexation& operator= (Complexation& other_complexation);


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
  /** @brief Reference to first component of the complex. */
  Chemical& _component_a;

  /** @brief Reference to second component of the complex. */
  Chemical& _component_b;

  /** @brief Reference to the complex. */
  Chemical& _complex;

  /** @brief Indicates which component is bound (0 if none, 1 if a, 2 if b). */
  int _bound_component;
  
  /** @brief on-rate of complexation reaction. */
  double _k_on;
  
  /** @brief off-rate of complexation reaction. */
  double _k_off;
  
  // =================
  //  Private Methods
  // =================
  //
  /** 
   * @brief Update the _bound_component index and performs a number of checks.
   *
   * Checks include the fact that both components cannot be bound and if one of 
   * the components is bound, then the complex also needs to be bound.
   */
  void look_for_bound_components (void);

};

// ======================
//  Inline declarations
// ======================
//


#endif // COMPLEXATION_H
