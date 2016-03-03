

/**
 * @file chemicalsequence.cpp
 * @brief Implementation of the ChemicalSequence class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <algorithm> // std::fill

// ==================
//  Project Includes
// ==================
//
#include "chemicalsequence.h"
#include "bindingsite.h"
#include "boundchemical.h"
#include "processivechemical.h"
#include "freeendbindingsitefactory.h"
#include "freeendhandler.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ChemicalSequence::ChemicalSequence (const std::string& sequence,
				    int starting_position /*= 1*/)
  : _sequence (sequence)
  , _length (sequence.size())
  , _starting_position (starting_position)
  , _sequence_occupation (sequence.size(), 0, _free_end_handler)
  , _appariated_sequence (0)
  , _free_end_binding_site_factory (0)
{
}

// Not needed for this class (use of compiler-generated versions)
// ChemicalSequence::ChemicalSequence (ChemicalSequence& other_chemical);
// ChemicalSequence& ChemicalSequence::operator= (ChemicalSequence& other_chemical);

ChemicalSequence::~ChemicalSequence (void)
{
  if (_appariated_sequence) 
    { 
      _appariated_sequence->break_pairing(); 
      break_pairing();
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void ChemicalSequence::bind_unit (const BoundChemical& chemical_to_bind)
{
  /** @pre Unit positions must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (chemical_to_bind.focused_unit_first(),
			     chemical_to_bind.focused_unit_last()) == false); 

  _sequence_occupation.add_element
    (chemical_to_bind, chemical_to_bind.focused_unit_first(),
     chemical_to_bind.focused_unit_last());
}


void ChemicalSequence::unbind_unit ( const BoundChemical& chemical_to_unbind )
{
  /** @pre Unit positions must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (chemical_to_unbind.focused_unit_first(),
			     chemical_to_unbind.focused_unit_last()) == false); 
  
  _sequence_occupation.remove_element
    (chemical_to_unbind, chemical_to_unbind.focused_unit_first(),
     chemical_to_unbind.focused_unit_last());
}

void ChemicalSequence::replace_bound_unit (const BoundChemical& old_chemical,
					   const BoundChemical& new_chemical)
{
  /** @pre Unit positions must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (old_chemical.focused_unit_first(),
			     old_chemical.focused_unit_last()) == false); 
  REQUIRE (is_out_of_bounds (new_chemical.focused_unit_first(),
			     new_chemical.focused_unit_last()) == false); 

  _sequence_occupation.remove_element
    (old_chemical, old_chemical.focused_unit_first(),
     old_chemical.focused_unit_last());
  _sequence_occupation.add_element
    (new_chemical, new_chemical.focused_unit_first(),
     new_chemical.focused_unit_last());
}

void ChemicalSequence::move_bound_unit (ProcessiveChemical& chemical_to_move,
					int number_steps)
{
  /** @pre Position and length must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (chemical_to_move.focused_unit_first(),
			     chemical_to_move.focused_unit_last()) == false); 

  // update occupancy status
  int first = chemical_to_move.focused_unit_first();
  int last = chemical_to_move.focused_unit_last();
  _sequence_occupation.remove_element (chemical_to_move, first, last);
  _sequence_occupation.add_element (chemical_to_move, first+number_steps,
				    last+number_steps);
 }

void ChemicalSequence::extend_strand (int position)
{
  /** @pre Position must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (position, position) == false);
  _sequence_occupation.start_segment (position);
}
     
void ChemicalSequence::add (int quantity)
{
  /** @pre Quantity must be positive. */
  REQUIRE (quantity >= 0);

  Chemical::add (quantity);
  _sequence_occupation.add_sequence (quantity);
}

void ChemicalSequence::remove (int quantity)
{
  /** @pre Quantity must be positive. */
  REQUIRE (quantity > 0);

  Chemical::remove (quantity);
  _sequence_occupation.remove_sequence (quantity);
}

void ChemicalSequence::add_termination_site (const Site& termination_site)
{
  // as a first approximation, we consider that reaching any base of the
  // termination site sends a termination signal
  int first = termination_site.first();
  int last = termination_site.last();
  for (int i = first; i <= last; i++)
    { _termination_sites [i].push_back (&termination_site.family()); }
}

void ChemicalSequence::register_site (BindingSite& site)
{
  /** @pre Site must be on sequence. */
  REQUIRE (&site.location() == this);
  REQUIRE (is_out_of_bounds (site.first(), site.last()) == false); 
  
  _sequence_occupation.register_site (site);
}

void ChemicalSequence::set_appariated_sequence 
(ChemicalSequence& sequence, const FreeEndBindingSiteFactory& factory)
{
  /** @pre Sequences must have equal length. */
  REQUIRE (sequence._length == _length);
  /** @pre A pairing must have not been defined already. */
  REQUIRE (_appariated_sequence == 0);

  _appariated_sequence = &sequence;
  _free_end_binding_site_factory = &factory;
  _free_end_handler.set_opposite (sequence);
}

BindingSite& 
ChemicalSequence::create_left_end_binding_site (int opposite_position)
{
  /** @pre An free end binding site factory must have been set. */
  REQUIRE (_free_end_binding_site_factory != 0);
  /** @pre An appariated strand must have been set. */
  REQUIRE (_appariated_sequence != 0);

  BindingSite* site = _free_end_binding_site_factory->create_left 
    (_appariated_sequence->complementary (opposite_position));
  _free_end_binding_sites.push_back (site);
  _sequence_occupation.register_moving_site (*site);
  return *site;
}

BindingSite& 
ChemicalSequence::create_right_end_binding_site (int opposite_position)
{
  /** @pre An free end binding site factory must have been set. */
  REQUIRE (_free_end_binding_site_factory != 0);
  /** @pre An appariated strand must have been set. */
  REQUIRE (_appariated_sequence != 0);

  BindingSite* site = _free_end_binding_site_factory->create_right 
    (_appariated_sequence->complementary (opposite_position));  
  _free_end_binding_sites.push_back (site);
  _sequence_occupation.register_moving_site (*site);
  return *site;
}

void ChemicalSequence::remove_free_end_binding_site (BindingSite& site)
{
  _sequence_occupation.deregister_moving_site (site);
  _free_end_binding_sites.remove (&site);
  delete &site;
}

// ============================
//  Public Methods - Accessors
// ============================
//
bool ChemicalSequence::is_termination_site 
(int position,
 const std::list <const SiteFamily*>& termination_site_families) const
{
  /** @pre Position must be within sequence. */
  REQUIRE (is_out_of_bounds (position, position) == false); 

  // if there is no site to check or no termination site at the position 
  // to enquire
  const std::map <int, std::list <const SiteFamily*> >::const_iterator 
    local_sites = _termination_sites.find (position);
  if ((termination_site_families.size() == 0)
      || (local_sites == _termination_sites.end())) { return false; }
  
  // we loop through the list of termination sites to inspect
  // we place ourselves at the beginnig of the list
  std::list <const SiteFamily*>::const_iterator
    family_it = termination_site_families.begin();
  // we get start and end iterator to the list of sites at the current position
  // on the sequence
  std::list <const SiteFamily*>::const_iterator
    local_sites_begin_it = local_sites->second.begin();
  std::list <const SiteFamily*>::const_iterator
    local_sites_end_it = local_sites->second.end();
  // we check whether one of the local sites corresponds to one of the sites to 
  // inspect
  while (family_it != termination_site_families.end())
    {
      std::list <const SiteFamily*>::const_iterator 
	local_site = local_sites_begin_it;
      while (local_site != local_sites_end_it)
	{
	  if (*family_it == *local_site) { return true; }
	  ++local_site;
	}
      ++family_it;
    }

  // if we arrive here, all the tests were non conclusive
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

void ChemicalSequence::break_pairing (void)
{
  _appariated_sequence = 0;
  _free_end_binding_site_factory = 0;
  _free_end_handler.break_pairing();
  clean_free_end_binding_sites();
}

void ChemicalSequence::clean_free_end_binding_sites (void)
{
  for (std::list <BindingSite*>::iterator 
	 site_it = _free_end_binding_sites.begin();
       site_it != _free_end_binding_sites.end(); ++site_it)
    { delete *site_it; }
  _free_end_binding_sites.clear();
}
