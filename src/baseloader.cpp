

/**
 * @file baseloader.cpp
 * @brief Implementation of the BaseLoader class.
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
#include "baseloader.h"
#include "decodingtable.h"
#include "randomhandler.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BaseLoader::BaseLoader (const DecodingTable& decoding_table)
  : _decoding_table (decoding_table)
  , _total_loading_rate (0)
  , _focused_template_index (BaseLoader::UNKNOWN_TEMPLATE)
{
  int number_templates = decoding_table.size();

  // initiate the loading rates to 0 (as there are no chemicals yet)
  _loading_rates.resize (number_templates, 0);

  // resize unit_map and leave it empty
  _unit_map.resize (number_templates);
}

// Not needed for this class (use of default copy constructor) !
// BaseLoader::BaseLoader ( const BaseLoader& other_base_loader );

BaseLoader::~BaseLoader (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BaseLoader::remove_focused_unit (void)
{
  if (_focused_template_index != BaseLoader::UNKNOWN_TEMPLATE) // if unit was reading an identifed template
    {
      // remove the unit from the map
      // find the unit to remove in the map
      std::list<BoundUnitList::iterator>::iterator candidate =_unit_map [_focused_template_index].begin();
      bool found = false;
      while (found == false)
	{
	  if (*candidate == _focused_unit)
	    {
	      found = true;
	      _unit_map [_focused_template_index].erase (candidate);
	    }
	  candidate++;
	}
    }
  // else there is nothing to do, nothing was written into the table at creation

  // remove all other references to the deleted element by calling
  // the parent function
  BoundChemical::remove_focused_unit();
}

void BaseLoader::focus_random_unit_from_loading_rates (void)
{
  // update loading rates
  update_loading_rates();

  // draw a random chemical to load depending on loading rates
  _focused_template_index = RandomHandler::instance().draw_index (_loading_rates);

  // select a random unit that loads this chemical
  std::list<BoundUnitList::iterator>& unit_list = _unit_map [_focused_template_index];
  int index_drawn = RandomHandler::instance().draw_uniform (0, unit_list.size()-1);
  int current_index = 0;
  // loop through units for the exact index
  std::list<BoundUnitList::iterator>::iterator unit = unit_list.begin();
  for (int index = current_index; index < index_drawn; index++)
    { 
      unit++; 
    }

  // focus unit
  _focused_unit = *unit;
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
// BaseLoader& BaseLoader::operator= ( const BaseLoader& other_base_loader );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool BaseLoader::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
void BaseLoader::add_unit (const BindingSite& binding_site, int position, int reading_frame)
{
  // update quantity and references to focused unit
  BoundChemical::add_unit (binding_site, position, reading_frame);

  // retrieve template read by added unit
  ChemicalSequence* target_sequence = dynamic_cast<ChemicalSequence*> (&(binding_site.location()));
  REQUIRE( target_sequence != 0); /** @pre Unit must be bound to a ChemicalSequence. */
  std::string focused_template =
    target_sequence->get_sequence (reading_frame, _decoding_table.template_length());

  // decode the template
  _focused_template_index = _decoding_table.template_index (focused_template);
  if (_focused_template_index != DecodingTable::UNKNOWN_TEMPLATE) // valid template
    {
      // update unit map
      _unit_map [_focused_template_index].push_back (_focused_unit);
    }
  else // unvalid template
    {
      // if the chemical reads an unknown template, it cannot load a base
      // there is nothing to update in the different vectors and maps
      // this unit will act as a plain bound_chemical
      _focused_template_index = BaseLoader::UNKNOWN_TEMPLATE;
      std::cerr << "UNKNOWN TEMPLATE " << focused_template;
    }
}

void BaseLoader::update_loading_rates (void)
{
  /**
   * Loading rate is generally defined by k_on_i x [A_i] x [B_i], where [A_i] is the
   * concentration of BaseLoader reading template i and [B_i] the concentration of
   * base i. k_on_i may vary from a base to another. The latter information is stored
   * within the decoding table. The total rate is given by
   *   r_total = sum ( k_on_i x [A_i] x [B_i] )
   */
  int number_templates = _decoding_table.size();
  _total_loading_rate = 0;
  
  for (int i = 0; i < number_templates; i++)
    {
      _loading_rates [i] = _decoding_table.loading_rate (i) // k_on_i
	* _unit_map [i].size() // [A_i]
	* _decoding_table.decode(i).number(); // [B_i]
      _total_loading_rate += _loading_rates [i];
    }
}
