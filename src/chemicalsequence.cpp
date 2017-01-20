
/**
 * @file chemicalsequence.cpp
 * @brief Implementation of the ChemicalSequence class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <algorithm> // std::fill

// ==================
//  Project Includes
// ==================
//
#include "chemicalsequence.h"
#include "bindingsite.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ChemicalSequence::ChemicalSequence (const std::string& sequence,
				    int starting_position /*= 1*/,
				    bool is_circular /* = false */)
  : _sequence (sequence)
  , _length (sequence.size())
  , _starting_position (starting_position)
  , _is_circular (is_circular)
  , _occupation (sequence.size())
  , _appariated_sequence (0)
{
}

// Not needed for this class (use of compiler-generated versions)
// ChemicalSequence::ChemicalSequence (ChemicalSequence& other_chemical);
// ChemicalSequence& ChemicalSequence::operator= (ChemicalSequence& other_chemical);
// ChemicalSequence::~ChemicalSequence (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void ChemicalSequence::bind_unit (int first, int last)
{
  /** @pre first must be smaller than last. */
  REQUIRE (first <= last);
  /** @pre Unit positions must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (first, last) == false); 

  _occupation.add_element (first, last);
}


void ChemicalSequence::unbind_unit (int first, int last)
{
  /** @pre first must be smaller than last. */
  REQUIRE (first <= last);
  /** @pre Unit positions must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (first, last) == false); 
  
  _occupation.remove_element (first, last);
}

bool ChemicalSequence::extend_strand (int strand_id, int position)
{
  /** @pre Position must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (position, position) == false);

  if (!_occupation.extend_strand (strand_id, position)) { return false; }
  // extension worked: check whether partial strand was completed
  if (_occupation.strand_completed (strand_id))
    {
      Chemical::add (1);
      if (!_appariated_sequence)
	{
	  _occupation.release_strand_id (strand_id);
	}
      else if (_appariated_sequence->_occupation.strand_completed (strand_id))
	{
	  _occupation.release_strand_id (strand_id);
	  _appariated_sequence->_occupation.release_strand_id (strand_id);
	}
    }
  return true;
}
     
void ChemicalSequence::add (int quantity)
{
  /** @pre Quantity must be positive. */
  REQUIRE (quantity >= 0);

  Chemical::add (quantity);
  _occupation.add_sequence (quantity);
}

void ChemicalSequence::remove (int quantity)
{
  /** @pre Quantity must be positive. */
  REQUIRE (quantity > 0);

  Chemical::remove (quantity);
  _occupation.remove_sequence (quantity);
}

void ChemicalSequence::add_switch_site (int position, int identifier)
{
  _switch_sites[position].push_back (identifier);
}

void ChemicalSequence::watch_site (BindingSite& site)
{
  /** @pre Site must be on sequence. */
  REQUIRE (&site.location() == this);
  REQUIRE (is_out_of_bounds (site.first(), site.last()) == false); 
  
  _occupation.watch_site (site);
}

void ChemicalSequence::set_appariated_sequence (ChemicalSequence& sequence)
{
  /** @pre Sequences must have equal length. */
  REQUIRE (sequence._length == _length);
  /** @pre A pairing must have not been defined already. */
  REQUIRE (_appariated_sequence == 0);
  _appariated_sequence = &sequence;
}

// ============================
//  Public Methods - Accessors
// ============================
//
bool ChemicalSequence::is_switch_site (int position, int identifier) const
{
  /** @pre Position must be within sequence. */
  REQUIRE (is_out_of_bounds (position, position) == false); 
  
  const std::map <int, std::list<int> >::const_iterator 
    local_sites = _switch_sites.find (position);

  // no switch site on current position
  if (local_sites == _switch_sites.end()) { return false; }

  // loop through switch sites on current position and check id
  const std::list<int>& id_list = local_sites->second;
  for (std::list<int>::const_iterator id_it = id_list.begin();
       id_it != id_list.end(); id_it++)
    {
      if (*id_it == identifier) { return true; }
    }
  return false;
}

// =================
//  Private Methods
// =================
//
void ChemicalSequence::print (std::ostream& output) const
{
  output << "Chemical sequence of length " << _length << " with currently "
	 << number() << " molecules.";
}
