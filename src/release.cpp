

/**
 * @file release.cpp
 * @brief Implementation of the Release class.
 * 
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
Release::Release (BoundChemical& unit_to_release,
		  std::vector<FreeChemical*>& other_components,
		  std::vector<int>& stoichiometry, 
		  std::vector<int>& orders, 
		  double rate,
		  ProductTable* product_table /*= 0*/)
  : _side_reaction (other_components, stoichiometry, orders, rate, 0)
  , _unit_to_release (unit_to_release)
  , _product_table (product_table)
{
  /** @pre Rate must be strictly positive. */
  REQUIRE (rate > 0);

  _reactants.push_back (&_unit_to_release);

  // copy components from side reaction into parent component list
  _reactants.insert (_reactants.end(),
		     _side_reaction.forward_reactants().begin(),
		     _side_reaction.forward_reactants().end());
  _products.insert (_products.end(),
		    _side_reaction.backward_reactants().begin(),
		    _side_reaction.backward_reactants().end());

  // TODO add products from the product table
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
  return ((_side_reaction.is_forward_reaction_possible())
	  && (_unit_to_release.number() > 0));
}

// ===================
//  Protected Methods
// ===================
//
void Release::do_reaction (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_reaction_possible());

  BoundUnit& unit = _unit_to_release.random_unit();
  if (_product_table != 0)
    {
      ChemicalSequence* product = _product_table->product 
	(unit.location(),
	 unit.binding_site().reading_frame(),
	 unit.reading_frame() - 1);

      if (product != 0) { product->add (1); }
      else
	{
	  std::cerr << "Unknown product ("
		    << unit.binding_site().reading_frame() << ", "
		    << unit.reading_frame() - 1 << ")";
	}
    }

  _unit_to_release.remove (unit);
  BoundUnitFactory::instance().free (unit);
  _side_reaction.perform_forward();
}

double Release::compute_rate (void) const
{
  /**
   * Forward rate is simply defined by r = k_1 x product ( [reactant_i] ).
   * It is 0 if there are not enough reactants.
   */
  return _unit_to_release.number()*_side_reaction.forward_rate();
}

void Release::print (std::ostream& output) const
{
  output << "Release reaction.";
}


// =================
//  Private Methods
// =================
//
