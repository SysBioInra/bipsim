

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
   * @param number_components Number of chemicals involved in the reaction.
   * @param components
   *  Vector of chemicals involved in the reaction.
   * @param stoichiometry
   *  Vector of stoichiometry of the chemicals, positive for products, negative for
   *  reactants.
   * @sa ChemicalReaction
   */
  void create_chemical_reaction (std::vector<Chemical*>& components,
				 std::vector<int>& stoichiometry, double forward_rate_constant,
				 double backward_rate_constant);

  /**
   * @brief Add new binding reaction to list.
   * @param unit_to_bind
   *  Chemical element that can potentially bind to a binding site.
   * @param binding_result
   *  A bound chemical element that corresponds to the original chemical in its
   *  bound form.
   * @param  binding_site_type
   *  The type (not a specific instance) of binding sites the chemical can bind
   *  onto.
   * @sa Binding
   */
  void create_binding (Chemical& unit_to_bind, BoundChemical& binding_result, int binding_site_family);

  /**
   * @brief Add new complexation reaction to list.
   * @param  component_a Reference to the first chemical involved.
   * @param  component_b Reference to the second chemical involved.
   * @param  complex Reference to the complex.
   * @sa Complexation
   */
  void create_complexation (Chemical& component_a, Chemical& component_b, Chemical& complex, double k_on, double k_off);

  /**
   * @brief Add new elongation reaction to list.
   * @param  processive_chemical 
   *  Polymerase that does the elongation.
   * @param  elongated_chemical
   *  Chemical that is elongated.
   * @param  step_size
   *  Number of bases processed at each elongation step.
   * @sa Elongation
   */
  void create_elongation (ProcessiveChemical& processive_chemical, int step_size, double rate);



  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return the list of reactions.
   * @return List of reactions.
   */
  std::list< Reaction*>& reference_list (void);

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
   * @return True if class invariant is preserved
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

  /** @brief List of reactions. */
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
inline std::list< Reaction*>& ReactionHandler::reference_list (void)
{
  return _references;
}

inline Reaction& ReactionHandler::last_reference_created (void) const
{
  return *_last_reference;
}

#endif // REACTIONHANDLER_H
