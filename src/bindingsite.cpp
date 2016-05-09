

/**
 * @file bindingsite.cpp
 * @brief Implementation of the ClassName class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout

// ==================
//  Project Includes
// ==================
//
#include "bindingsite.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSite::BindingSite (BindingSiteFamily& family, 
			  ChemicalSequence& location, 
			  int first, int last, 
			  double k_on, double k_off,
			  int reading_frame /*= NO_READING_FRAME*/)
  : Site (family, location, first, last)
  , _k_on (k_on)
  , _k_off (k_off)
  , _reading_frame (reading_frame)
  , _update_id (DEFAULT_ID)
{
  /** @pre If defined, reading frame must be within site. */
  REQUIRE ((reading_frame == NO_READING_FRAME)
	   || ((reading_frame >= first) && (reading_frame <= last)));

  // add binding site to family
  family.add (this);
}

// Forbidden
// BindingSite::BindingSite (const BindingSite& other);
// BindingSite& BindingSite::operator= (const BindingSite& other);
// BindingSite::~BindingSite (void)

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
