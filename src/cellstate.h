

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
#include "handler.h"

/**
 * @brief Class gathering cell components and reactions.
 *
 * CellState is used to store and access all cell parts. It acts as
 * a mediator between users and more specified handlers, yielding
 * a simplified API.
 */
class CellState
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Default constructor. */
  // CellState ();
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor */
  // CellState ( const CellState& other_cell_state );
  // /* @brief Assignment operator */
  // CellState& operator= ( const CellState& other_cell_state );
  // /* @brief Destructor */
  // ~CellState (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Store element created on the heap.
   * @param element Pointer to the element to store.
   * @param name Name of element (optional).
   */
  void store (SimulatorInput* element, const std::string& name = "");
  

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Access pointer to element by name.
   * @tparam T Class of element to find.
   * @param name Name of element.
   * @return Pointer to element or 0 if name not found.
   */
  template <class T>
    T* find (const std::string& name) const;
  
  /**
   * @brief Return the list of reactions.
   * @return List of reactions.
   */
  const std::vector <Reaction*>& reactions (void) const;

  /**
   * @brief Accessor to number of chemicals.
   * @return Number of chemicals in the cell.
   */
  int number_chemicals (void) const;

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Handler containing site information. */
  Handler <Site> _site_handler;

  /** @brief Handler containing site family information. */
  Handler <SiteFamily> _site_family_handler;

  /** @brief Handler containing chemical information. */
  Handler <Chemical> _chemical_handler;

  /** @brief Handler containing reaction information. */
  Handler <Reaction> _reaction_handler;

  /** @brief Handler containing two-way reaction information. */
  Handler <BidirectionalReaction> _bireaction_handler;

  /** @brief Handler containing information about composition tables. */
  Handler <CompositionTable> _composition_table_handler;

  /** @brief Handler containing information about loading tables. */
  Handler <LoadingTable> _loading_table_handler;

  /** @brief Handler containing information about prdouct tables. */
  Handler <ProductTable> _product_table_handler;
  
  /** @brief Handler containing information about transformation tables. */
  Handler <TransformationTable> _transformation_table_handler;
};

// ======================
//  Inline declarations
// ======================
//
#include <typeinfo>

#include "site.h"
#include "sitefamily.h"

#include "chemical.h"

#include "compositiontable.h"
#include "loadingtable.h"
#include "producttable.h"
#include "transformationtable.h"

#include "reaction.h"
#include "bidirectionalreaction.h"

inline const std::vector <Reaction*>& CellState::reactions (void) const
{
  return _reaction_handler.references();
}

inline int CellState::number_chemicals (void) const
{
  return _chemical_handler.references().size();
}


template <class T>
inline T* CellState::find (const std::string& name) const
{
  Site* s = _site_handler.find (name);
  if (s != 0) { return dynamic_cast <T*> (s); }

  SiteFamily* sf = _site_family_handler.find (name);
  if (sf != 0) { return dynamic_cast <T*> (sf); }

  Chemical* c = _chemical_handler.find (name);
  if (c != 0) { return dynamic_cast <T*> (c); }

  Reaction* r = _reaction_handler.find (name);
  if (r != 0) { return dynamic_cast <T*> (r); }

  BidirectionalReaction* br = _bireaction_handler.find (name);
  if (r != 0) { return dynamic_cast <T*> (br); }
  
  CompositionTable* ct = _composition_table_handler.find (name);
  if (ct != 0) { return dynamic_cast <T*> (ct); }

  LoadingTable* dt = _loading_table_handler.find (name);
  if (dt != 0) { return dynamic_cast <T*> (dt); }

  ProductTable* pt = _product_table_handler.find (name);
  if (pt != 0) { return dynamic_cast <T*> (pt); }

  TransformationTable* tt = _transformation_table_handler.find (name);
  if (tt != 0) { return dynamic_cast <T*> (tt); }

  return 0;
}

#endif // CELL_STATE_H
