

/**
 * @file cellstate.h
 * @brief Header for the CellState class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef CELL_STATE_H
#define CELL_STATE_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <list> // std::list
#include <iostream> // std::cout

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "bindingsitehandler.h"
#include "chemicalhandler.h"
#include "reactionhandler.h"
#include "tablehandler.h"
#include "terminationsitehandler.h"

/**
 * @brief Class gathering cell components.
 *
 * CellState loads from a file all components contained
 * within a cell. Its content can be updated by Solver
 * instances to perform simulations.
 */
class CellState
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param filename File name to read cell components from.
   */
  CellState (const char* filename);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // CellState ( const CellState& other_cell_state );

  /**
   * @brief Destructor
   */
  virtual ~CellState (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Print chemicals in their current state to standard output.
   */
  void print_chemicals (void) const;

  /**
   * @brief Compute binding rate contributions for all binding sites.
   */
  void update_all_binding_rate_contributions (void);

  /**
   * @brief Compute binding rate contributions for binding sites on a specific sequence.
   * @param modified_sequence Sequence bearing binding sites whose contribution to binding should be updated.
   */
  void update_binding_rate_contributions (const ChemicalSequence& modified_sequence);

  

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return the list of reactions.
   * @return List of reactions.
   */
  const std::list< Reaction*>& reaction_list (void) const;

  /**
   * @brief Return chemical.
   * @param name Name of the chemical.
   * @return Reference to chemical.
   */
  const Chemical& chemical (const std::string& name) const;



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
  // CellState& operator= ( const CellState& other_cell_state );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Handler containing binding site information. */
  BindingSiteHandler _binding_site_handler;

  /** @brief Handler containing chemical information. */
  ChemicalHandler _chemical_handler;

  /** @brief Handler containing reaction information. */
  ReactionHandler _reaction_handler;

  /** @brief Handler containing information about various tables. */
  TableHandler _table_handler;

  /** @brief Handler containing termination site information. */
  TerminationSiteHandler _termination_site_handler;
  

  // =================
  //  Private Methods
  // =================
  //

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline const std::list< Reaction*>& CellState::reaction_list (void) const
{
  return _reaction_handler.reference_list();
}

inline const Chemical& CellState::chemical (const std::string& name) const
{
  return _chemical_handler.reference (name);
}

inline void CellState::print_chemicals (void) const
{
  std::cout << _chemical_handler;
}

inline void CellState::update_all_binding_rate_contributions (void)
{
  _binding_site_handler.update_all_binding_rate_contributions();
}

inline void CellState::update_binding_rate_contributions (const ChemicalSequence& modified_sequence)
{
  _binding_site_handler.update_binding_rate_contributions (modified_sequence);
}

#endif // CELL_STATE_H
