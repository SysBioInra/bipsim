

/**
 * @file reactionhandler.h
 * @brief Header for the ReactionHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTIONHANDLER_H
#define REACTIONHANDLER_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "identifiedlist.h"
#include "macros.h" // REQUIRE

/**
 * @brief This class creates and organizes chemical reactions.
 *
 * Chemical reactions are created and added to solvers (?).
 */
class ReactionHandler
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  ReactionHandler (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ReactionHandler (ReactionHandler& other_reaction_handler);

  /**
   * @brief Destructor
   */
  virtual ~ReactionHandler (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add new chemical reaction to list.
   * @param components
   *  Vector of chemicals involved in the reaction.
   * @param stoichiometry
   *  Vector of stoichiometry of the chemicals, positive for products, negative for
   *  reactants.
   * @param forward_rate_constant Forward rate constant.
   * @param backward_rate_constant Backward rate constant.
   * @sa ChemicalReaction
   */
  void create_chemical_reaction (std::vector<Chemical*>& components,
				 std::vector<int>& stoichiometry, double forward_rate_constant,
				 double backward_rate_constant);

  /**
   * @brief Add new base loading reaction to list.
   * @param base_loader
   *  Chemical that matches templates with bases.
   * @sa BaseLoading
   */
  void create_base_loading (BaseLoader& base_loader);

  /**
   * @brief Add new binding reaction to list.
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
   * @sa Binding
   */
  void create_binding (Chemical& unit_to_bind, BoundChemical& binding_result, BindingSiteFamily& binding_site_family, int binding_site_family_id);

  /**
   * @brief Add new complexation reaction to list.
   * @param  component_a Reference to the first chemical involved.
   * @param  component_b Reference to the second chemical involved.
   * @param  complex Reference to the complex.
   * @param k_on Association constant.
   * @param k_off Dissociation constant.
   * @sa Complexation
   */
  void create_complexation (Chemical& component_a, Chemical& component_b, Chemical& complex, double k_on, double k_off);

  /**
   * @brief Add new elongation reaction to list.
   * @param  processive_chemical 
   *  Polymerase that does the elongation.
   * @param  chemical_after_step
   *  Chemical after elongation.
   * @param  step_size
   *  Number of bases processed at each elongation step.
   * @param rate Elongation rate (in steps/s).
   * @sa Elongation
   */
  void create_elongation (ProcessiveChemical& processive_chemical, BoundChemical& chemical_after_step, int step_size, double rate);

  /**
   * @brief Add new release reaction to list.
   * @param unit_to_release Unit that is released from its template.
   * @param components
   *  Vector of chemicals involved in the reaction.
   * @param stoichiometry
   *  Vector of stoichiometry of the chemicals, positive for products, negative for
   *  reactants.
   * @param rate Reaction rate constant.
   * @sa Release
   */
  void create_release (BoundChemical& unit_to_release, std::vector<Chemical*>& components,
		       std::vector<int>& stoichiometry, double rate);


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return the list of reactions.
   * @return List of reactions.
   */
  const std::list< Reaction*>& reference_list (void) const;

  /**
   * @brief Return the last reaction added.
   * @return Reference to the last reaction added.
   */
  Reaction& last_reference_created (void) const;

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
  // ReactionHandler& operator= (ReactionHandler& other_reaction_handler);

  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param reaction_handler Reference to the reaction handler whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output,const ReactionHandler& reaction_handler);
  
  
  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check if invariant conditions are fulfilled.
   * @return True if class invariant is preserved.
   */
  virtual bool check_invariant (void) const;

  // ==================
  //  Public Constants
  // ==================
  //


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief List of reactions. */
  std::list< Reaction* > _references;

  /** @brief Reference to the last reaction that has been created. */
  Reaction* _last_reference;
    

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline const std::list< Reaction*>& ReactionHandler::reference_list (void) const
{
  return _references;
}

inline Reaction& ReactionHandler::last_reference_created (void) const
{
  return *_last_reference;
}


#endif // REACTIONHANDLER_H
