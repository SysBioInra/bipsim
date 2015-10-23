
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

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "bidirectionalreaction.h"

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
class Binding: public BidirectionalReaction
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
   * @brief Update chemical quantities according to the forward reaction.
   * @return Current forward rate.
   */
  virtual void perform_forward (void);
  
  /**
   * @brief Update chemical quantities according to the backward reaction.
   * @return Current backward rate.
   */
  virtual void perform_backward (void);

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
   * @brief Compute current forward rate.
   * @return Current forward rate.
   */
  virtual double compute_forward_rate (void) const;

  /**
   * @brief Compute current backward rate.
   * @return Current backward rate.
   */
  virtual double compute_backward_rate (void) const;

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
