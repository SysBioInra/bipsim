

/**
 * @file templatefilter.cpp
 * @brief Implementation of the TemplateFilter class.
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
#include "templatefilter.h"
#include "loadingtable.h"
#include "freechemical.h"
#include "boundunit.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
TemplateFilter::TemplateFilter (const LoadingTable& table)
  : _table (table)
  , _rate_validity (table.size())
  , _loading_rates (table.size())
{
  // resize unit_map and leave it empty
  _unit_map.resize (table.size());

  // create an observer for every loadable base
  for (int i = 0; i < table.size(); ++i)
    { _rate_validity.add_observer (_table.chemical_to_load(i), i); }
}


// Not needed for this class (use of compiler-generated versions)
// TemplateFilter::TemplateFilter (const TemplateFilter& other);
// TemplateFilter& TemplateFilter::operator= (const TemplateFilter& other);
// TemplateFilter::~TemplateFilter (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void TemplateFilter::add (BoundUnit& unit)
{
  int template_index =  _table.template_index 
    (unit.location().sequence().substr 
     (unit.reading_frame(), _table.template_length()));

  if (template_index != LoadingTable::UNKNOWN_TEMPLATE) 
    {
      // update unit map
      _unit_map [template_index].add (&unit);
      _rate_validity.update (template_index);
    }
  else // invalid template
    {
      // if the chemical reads an unknown template, it cannot load anything
      // there is nothing to update in the different vectors and maps
      // this unit will act as a plain bound_chemical
      std::cerr << "UNKNOWN TEMPLATE " << unit.location().sequence().substr 
	(unit.reading_frame(), _table.template_length());
    }
}

void TemplateFilter::remove (BoundUnit& unit)
{
  int template_index =  _table.template_index 
    (unit.location().sequence().substr 
     (unit.reading_frame(), _table.template_length()));

  // if unit was reading an identifed template
  if (template_index != LoadingTable::UNKNOWN_TEMPLATE) 
    {
      // remove the unit from the map
      _unit_map [template_index].remove (&unit);
      _rate_validity.update (template_index);
    }
  // else there is nothing to do, nothing was written into the table at creation
}

// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
void TemplateFilter::update_rates (void) const
{
  /**
   * Loading rate is generally defined by k_on_i x [A_i] x [B_i], where [A_i] is
   * the concentration of Loader reading template i and [B_i] the
   * concentration of chemical to load on template i.
   * k_on_i may vary from one template to another. The latter
   * information is stored within the loading table. The total rate is given by
   *   r_total = sum ( k_on_i x [A_i] x [B_i] )
   */
  while (_rate_validity.empty() == false)
    {
      int index = _rate_validity.front();
      _loading_rates.set_rate 
	(index, _table.loading_rate (index) // k_on_i
	 * _unit_map [index].size() // [A_i]
	 * _table.chemical_to_load (index).number()); // [B_i]
      _rate_validity.pop();
    }
}
