
/**
 * @file release.cpp
 * @brief Implementation of the Release class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE
#include "config.h" // DISPLAY_WARNINGS

#include "release.h"
#include "chemical.h"
#include "boundchemical.h"
#include "boundunit.h"
#include "boundunitfactory.h"
#include "producttable.h"


// ==========================
//  Constructors/Destructors
// ==========================
//
Release::Release (BoundChemical& releasing_polymerase, 
		  BoundChemical& empty_polymerase,
		  BoundChemical& fail_polymerase,
		  const ProductTable& product_table, double rate)
  : _releasing_polymerase (releasing_polymerase)
  , _empty_polymerase (empty_polymerase)
  , _fail_polymerase (fail_polymerase)
  , _product_table (product_table)
  , _rate (rate)
  , _volume_constant (rate)
{
  /** @pre Rate must be strictly positive. */
  REQUIRE (rate > 0);

  _reactants.push_back (&_releasing_polymerase);

  _products.push_back (&_empty_polymerase);
  _products.push_back (&_fail_polymerase);
  _products.insert (_products.end(),
		    _product_table.products().begin(),
		    _product_table.products().end());
}

// Not needed for this class (use of compiler generated versions)
// Release::Release (const Release& other_release);
// Release& Release::operator= (const Release& other_release);
// Release::~Release (void);

// ===========================
//  Public Methods - Commands
// ===========================
//

// ============================
//  Public Methods - Accessors
// ============================
//
bool Release::is_reaction_possible (void) const
{
  return (_releasing_polymerase.number() > 0);
}

// ===================
//  Protected Methods
// ===================
//
void Release::do_reaction (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_reaction_possible());

  BoundUnit& unit = _releasing_polymerase.random_unit();
  ChemicalSequence* product = _product_table.product 
    (unit.location(), unit.initial_reading_frame(), unit.reading_frame()-1);

  if (product != 0) 
    { 
      _releasing_polymerase.remove (unit);
      _empty_polymerase.add (unit);
      product->add (1); 
    }
  else
    {
#ifdef DISPLAY_WARNINGS
      std::cerr << "Warning: Unknown product ("
		<< unit.initial_reading_frame() << ", "
		<< unit.reading_frame() - 1 << "). ";
#endif
      _releasing_polymerase.remove (unit);
      _fail_polymerase.add (unit);
    }
}

double Release::compute_rate (void) const
{
  return _volume_constant * _releasing_polymerase.number();
}

void Release::print (std::ostream& output) const
{
  output << "Release reaction.";
}


// =================
//  Private Methods
// =================
//
