
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
#include "reaction.h"
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
 * However, note that other components can take part in this reaction. In
 * particular, the reaction should be specify in what form the main chemical is
 * released (it can bea free form of the chemical, but it could also break down
 * into several subparts).
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
   * @param other_components Vector of other chemicals involved in the reaction.
   * @param stoichiometry Vector of stoichiometry of the other chemicals, 
   *  positive for products, negative for reactants.
   * @param orders Vector of orders of the other chemicals.
   * @param rate Reaction rate constant.
   * @param product_table If bound chemical releases a product, table listing
   *  molecules it may produce depending on its binding and release sites.
   * @sa ChemicalReaction
   */
  Release (BoundChemical& unit_to_release,
	   std::vector<FreeChemical*>& other_components,
	   std::vector<int>& stoichiometry, 
	   std::vector<int>& orders, double rate,
	   ProductTable* product_table = 0);
    

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Release (Release& other_release);
  // /* @brief Assignment operator. */
  // Release& operator= (Release& other_release);
  // /* @brief Destructor */
  // ~Release (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from Reaction
  void handle_volume_change (double volume);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from Reaction
  bool is_reaction_possible (void) const;

 private:  
  // =================
  //  Private Methods
  // =================
  //
  // Redefined from Reaction.
  void do_reaction (void);
  double compute_rate (void) const;
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief Chemical element to release. */
  BoundChemical& _unit_to_release;

  /** @brief Side reaction representing other components involved. */
  ChemicalReaction _side_reaction;
  
  /** @brief Table of molecules that the released chemical may have produced. */
  ProductTable* _product_table;

  /** @brief Volume constant. */
  double _volume_constant;
};

// =====================
//  Inline Declarations
// =====================
//
inline void Release::handle_volume_change (double volume)
{
  _volume_constant = 1 / volume;
}

#endif // RELEASE_H
