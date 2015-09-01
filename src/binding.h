
/**
 * @file binding.h
 * @brief Header for the Binding class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BINDING_H
#define BINDING_H

// ==================
//  General Includes
// ==================
//
#include <string>

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "reaction.h"

// ===================
//  Class Declaration
// ===================
//
/**
 * @brief Class for representing binding reactions.
 *
 * The Binding class can represent the binding of chemical elements onto
 * binding sites. A binding reaction is represented by an element to bind, the
 * same element in its bound form and the type of binding sites it can bind
 * onto.
 * This class inherits class Reaction.
 * @sa Reaction
 */
class Binding: public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param unit_to_bind
   *  Chemical element that can potentially bind to a binding site.
   * @param binding_result
   *  A bound chemical element that corresponds to the original chemical in its
   *  bound form.
   * @param  binding_site_family
   *  The family of binding sites the chemical can bind
   *  onto.
   * @param  binding_site_family_id
   *  The family identfier of binding sites the chemical can bind
   *  onto.
   * @sa BindingSite
   */
  Binding (Chemical& unit_to_bind, BoundChemical& binding_result,
	   BindingSiteFamily& binding_site_family, int binding_site_family_id);
    

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Binding (Binding& other_binding);

  /**
   * @brief Destructor
   */
  virtual ~Binding (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //
  /**
   * @brief Update reaction rates.
   */
  virtual void update_rates (void);

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
  // Binding& operator= (Binding& other_binding);

  
  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  virtual void do_forward_reaction (void);
  
  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  virtual void do_backward_reaction (void);
  

 private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Chemical element that can potentially bind to a binding site. */
  Chemical& _unit_to_bind;
  
  /**
   * @brief A bound chemical element that corresponds to the original chemical
   * in its bound form.
   */
  BoundChemical& _binding_result;
  
  /**
   * @brief Binding sites the chemical can bind onto.
   * @sa BindingSiteFamily
   */
  BindingSiteFamily& _binding_site_family;

  /**
   * @brief Identifier of the binding site family the chemical can bind onto.
   * @sa BindingSiteHandler
   */
  int _binding_site_family_id;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;

};

// =====================
//  Inline Declarations
// =====================
//

#endif // BINDING_H
