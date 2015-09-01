
/**
 * @file release.h
 * @brief Header for the Release class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RELEASE_H
#define RELEASE_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "chemicalreaction.h"

// ===================
//  Class Declaration
// ===================
//
/**
 * @brief Class for representing release reactions.
 *
 * A release reaction is defined by the release of a chemical that has been
 * synthesizing a template. By releasing it, its product is also released.
 * However, note that other components can take part in this reaction. In particular,
 * the reaction should be specify in what form the main chemical is released (it can be
 * a free form of the chemical, but it could also break down into several subparts).
 * This class inherits class Reaction.
 * @sa Reaction
 */
class Release: public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param unit_to_release Unit that is released from its template.
   * @param other_components
   *  Vector of other chemicals involved in the reaction.
   * @param stoichiometry
   *  Vector of stoichiometry of the other chemicals, positive for products, negative for
   *  reactants.
   * @param rate Reaction rate constant.
   * @sa ChemicalReaction
   */
  Release (BoundChemical& unit_to_release, std::vector<Chemical*>& other_components,
		    std::vector<int>& stoichiometry, double rate);
    

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Release (Release& other_release);

  /**
   * @brief Destructor
   */
  virtual ~Release (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //
  /**
   * @brief Print class content.
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
  // Release& operator= (Release& other_release);

  
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
  /** @brief Chemical element to release. */
  BoundChemical& _unit_to_release;

  /** @brief Side reaction representing other components that are involved in the release other than the unit to release. */
  ChemicalReaction _side_reaction;
  
  
  // =================
  //  Private Methods
  // =================
  //

};

// =====================
//  Inline Declarations
// =====================
//

#endif // RELEASE_H
