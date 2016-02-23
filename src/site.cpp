

/**
 * @file site.cpp
 * @brief Implementation of the Site class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "site.h"
#include "sitefamily.h"
#include "chemicalsequence.h"
#include "macros.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Site::Site (SiteFamily& family, ChemicalSequence& location, int first, int last)
  : _family (family)
  , _location (location)
  , _first (first)
  , _last (last)
{
  /** @pre First must be smaller than last.*/
  REQUIRE (first <= last);
  
  /** @pre Site must be within location.*/
  REQUIRE (location.is_out_of_bounds (first, last) == false);
}

// Not needed for this class (use of compiler generated versions)
// Site::Site (Site& other_site);
// Site& Site::operator= (Site& other_site);
// Site::~Site (void);


// ===========================
//  Public Methods - Commands
// ===========================
//
void Site::move (int step_size)
{
  /** @pre Site must stay within location bound. */
  REQUIRE (!_location.is_out_of_bounds (_first + step_size, _last + step_size));
  _first += step_size; _last += step_size;
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
