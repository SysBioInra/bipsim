

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
  /**
   * @brief Default constructor.
   */
  CellState ();

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
   * @brief Access pointer to element by name.
   * @param name Name of element.
   * @return Pointer to element or 0 if name not found.
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
   * @brief Acess identifier attributed to element by name.
   * @param name Name of element.
   * @return Element identifier or CellState::NOT_FOUND if name not found.
   */
  int find_id (const std::string& name) const;
  
  /**
   * @brief Return the list of reactions.
   * @return List of reactions.
   */
  const std::vector <Reaction*>& reactions (void) const;


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

  // ==================
  //  Public Constants
  // ==================
  //
  static const int NOT_FOUND = Handler <Chemical>::NOT_FOUND;

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Handler containing site information. */
  Handler <Site> _site_handler;

  /** @brief Handler containing binding site family information. */
  Handler <BindingSiteFamily> _binding_site_family_handler;

  /** @brief Handler containing other site family information. */
  Handler <SiteFamily> _site_family_handler;

  /** @brief Handler containing chemical information. */
  Handler <Chemical> _chemical_handler;

  /** @brief Handler containing reaction information. */
  Handler <Reaction> _reaction_handler;

  /** @brief Handler containing information about various tables. */
  Handler <DecodingTable> _table_handler;
  

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
#include <typeinfo>

#include "site.h"
#include "bindingsite.h"
#include "bindingsitefamily.h"
#include "sitefamily.h"

#include "chemical.h"
#include "boundchemical.h"
#include "processivechemical.h"
#include "baseloader.h"
#include "chemicalsequence.h"

#include "decodingtable.h"

#include "reaction.h"
#include "chemicalreaction.h"
#include "complexation.h"
#include "binding.h"
#include "baseloading.h"
#include "release.h"
#include "elongation.h"


inline const std::vector <Reaction*>& CellState::reactions (void) const
{
  return _reaction_handler.references();
}


template <class T>
inline T* CellState::find (const std::string& name) const
{
  Site* s = _site_handler.find (name);
  if (s != 0) { return dynamic_cast <T*> (s); }

  BindingSiteFamily* bsf = _binding_site_family_handler.find (name);
  if (bsf != 0) { return dynamic_cast <T*> (bsf); }

  SiteFamily* sf = _site_family_handler.find (name);
  if (sf != 0) { return dynamic_cast <T*> (sf); }

  Chemical* c = _chemical_handler.find (name);
  if (c != 0) { return dynamic_cast <T*> (c); }

  Reaction* r = _reaction_handler.find (name);
  if (r != 0) { return dynamic_cast <T*> (r); }
  
  DecodingTable* t = _table_handler.find (name);
  if (t != 0) { return dynamic_cast <T*> (t); }

  return 0;
}

#endif // CELL_STATE_H
