

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
			  int reading_frame /*= NO_READING_FRAME*/,
			  bool is_static /*= true */)
  : Site (family, location, first, last)
  , _k_on (k_on)
  , _k_off (k_off)
  , _reading_frame (reading_frame)
  , _update_id (DEFAULT_ID)
  , _static (is_static)
  , _last_availability (0)
{
  /** @pre If defined, reading frame must be within site. */
  REQUIRE ((reading_frame == NO_READING_FRAME)
	   || ((reading_frame >= first) && (reading_frame <= last)));

  // add binding site to family
  static_cast <BindingSiteFamily&> (_family).add (this);

  // demand availability notifications
  if (_static) { location.register_static_site (*this); }
  else { location.register_dynamic_site (*this); }
}

// Forbidden
// BindingSite::BindingSite (const BindingSite& other);
// BindingSite& BindingSite::operator= (const BindingSite& other);

BindingSite::~BindingSite (void)
{
  if (_static == false)
    {
      // remove binding site from family and from sequence
      static_cast <BindingSiteFamily&> (_family).remove (this);
      _location.deregister_dynamic_site (*this);
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BindingSite::update (void)
{
  // read availability
  int availability = _location.number_available_sites (first(), last());

  if (availability != _last_availability)
    {
      static_cast <BindingSiteFamily&> (_family).
	update (_update_id, _k_on * availability);
      _last_availability = availability;
    }
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
