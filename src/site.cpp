

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

// ============================
//  Public Methods - Accessors
// ============================
//
// =================
//  Private Methods
// =================
//
