

/**
 * @file boundunit.cpp
 * @brief Implementation of the BoundUnit class.
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
#include "boundunit.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BoundUnit::BoundUnit (const BindingSite& binding_site)
  : _binding_site (&binding_site)
  , _first (binding_site.first())
  , _last (binding_site.last())
  , _reading_frame (binding_site.reading_frame())
  , _location (&(binding_site.location()))
  , _strand (NO_STRAND)
{
}

// BoundUnit::BoundUnit (const BoundUnit& other);
// BoundUnit& BoundUnit::operator= (const BoundUnit& other);
// BoundUnit::~BoundUnit (void)

// ===========================
//  Public Methods - Commands
// ===========================
//
void BoundUnit::rebind (const BindingSite& binding_site)
{
  _binding_site = &binding_site;
  _first = binding_site.first();
  _last = binding_site.last();
  _reading_frame = binding_site.reading_frame();
  _location = &(binding_site.location());
  _strand = NO_STRAND;
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
