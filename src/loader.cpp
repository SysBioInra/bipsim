

/**
 * @file loader.cpp
 * @brief Implementation of the Loader class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "loader.h"
#include "decodingtable.h"
#include "randomhandler.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Loader::Loader (const DecodingTable& decoding_table)
  : _decoding_table (decoding_table)
  , _rate_validity (decoding_table.size())
  , _focused_template_index (Loader::UNKNOWN_TEMPLATE)
  , _loading_rates (decoding_table.size())
{
  // resize unit_map and leave it empty
  _unit_map.resize (decoding_table.size());

  // create an observer for every loadable base
  for (int i = 0; i < decoding_table.size(); ++i)
    { _rate_validity.add_observer (_decoding_table.chemical_to_load(i), i); }
}

// Not needed for this class (use of default copy constructor) !
// Loader::Loader ( const Loader& other_loader );

Loader::~Loader (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Loader::remove_focused_unit (void)
{
  // if unit was reading an identifed template
  if (_focused_template_index != Loader::UNKNOWN_TEMPLATE) 
    {
      // remove the unit from the map
      _unit_map [_focused_template_index].erase (_focused_unit);
      _rate_validity.update (_focused_template_index);
    }
  // else there is nothing to do, nothing was written into the table at creation

  // remove all other references to the deleted element by calling
  // the parent function
  BoundChemical::remove_focused_unit();
}

void Loader::focus_random_unit (void)
{
  // focus random unit by using the parent function
  BoundChemical::focus_random_unit();
  update_focused_template();
}

void Loader::focus_random_unit (int binding_site_family)
{
  // focus random unit by using the parent function
  BoundChemical::focus_random_unit (binding_site_family);
  update_focused_template();
}

void Loader::focus_random_unit_from_loading_rates (void)
{
  // draw a random chemical to load depending on loading rates
  update_rates();
  _loading_rates.update_cumulates();
  _focused_template_index = _loading_rates.random_index();

  // select a random unit that loads this chemical
  BoundUnitList& unit_list = _unit_map [_focused_template_index];
  _focused_unit = unit_list 
    [RandomHandler::instance().draw_uniform (0, unit_list.size()-1)];
}

void Loader::update_rates (void)
{
  /**
   * Loading rate is generally defined by k_on_i x [A_i] x [B_i], where [A_i] is
   * the concentration of Loader reading template i and [B_i] the
   * concentration of chemical to load on template i.
   * k_on_i may vary from one template to another. The latter
   * information is stored within the decoding table. The total rate is given by
   *   r_total = sum ( k_on_i x [A_i] x [B_i] )
   */
  while (_rate_validity.empty() == false)
    {
      int index = _rate_validity.front();
      _loading_rates.set_rate (_rate_validity.front(),
			       _decoding_table.loading_rate (index) // k_on_i
			       * _unit_map [index].size() // [A_i]
			       * _decoding_table.chemical_to_load(index).number()); // [B_i]
      _rate_validity.pop();
    }
}

// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// Loader& Loader::operator= ( const Loader& other_loader );


// =================
//  Private Methods
// =================
//
void Loader::add_unit (const BindingSite& binding_site, int position,
			   int reading_frame)
{
  // update quantity and references to focused unit
  BoundChemical::add_unit (binding_site, position, reading_frame);

  // retrieve template read by added unit
  ChemicalSequence& target_sequence = binding_site.location();
  std::string focused_template = target_sequence.sequence
    (reading_frame, reading_frame + _decoding_table.template_length() - 1);

  // decode the template
  _focused_template_index = _decoding_table.template_index (focused_template);
  if (_focused_template_index != DecodingTable::UNKNOWN_TEMPLATE) 
    {
      // update unit map
      _unit_map [_focused_template_index].insert (_focused_unit);
      _rate_validity.update (_focused_template_index);
    }
  else // unvalid template
    {
      // if the chemical reads an unknown template, it cannot load anything
      // there is nothing to update in the different vectors and maps
      // this unit will act as a plain bound_chemical
      _focused_template_index = Loader::UNKNOWN_TEMPLATE;
      std::cerr << "UNKNOWN TEMPLATE " << focused_template;
    }
}

void Loader::update_focused_template (void)
{
  // retrieve template read by focused unit
  ChemicalSequence& target_sequence = _focused_unit->binding_site().location();
  std::string focused_template = target_sequence.sequence
    (_focused_unit->reading_frame(),
     _focused_unit->reading_frame() + _decoding_table.template_length() - 1);
  
  // decode the template
  _focused_template_index = _decoding_table.template_index (focused_template);  
}

