
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

int ChemicalSequence::start_strand (int position)
{
  /** @pre Position must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (position, position) == false);
  return _occupation.start_segment (position);
}

bool ChemicalSequence::extend_strand (int strand_id, int position)
{
  /** @pre Position must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (position, position) == false);
  return _occupation.extend_segment (strand_id, position);
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

void ChemicalSequence::add_termination_site (const Site& termination_site)
{
  // as a first approximation, we consider that reaching any base of the
  // termination site sends a termination signal
  int first = termination_site.first();
  int last = termination_site.last();
  for (int i = first; i <= last; i++)
    { _termination_sites [i].push_back (&termination_site.family()); }
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
bool ChemicalSequence::is_termination_site 
(int position,
 const std::vector <const SiteFamily*>& termination_site_families) const
{
  /** @pre Position must be within sequence. */
  REQUIRE (is_out_of_bounds (position, position) == false); 

  // if there is no site to check or no termination site at the position 
  // to enquire
  if (termination_site_families.size() == 0) { return false; }
  const std::map <int, std::list <const SiteFamily*> >::const_iterator 
    local_sites = _termination_sites.find (position);
  if (local_sites == _termination_sites.end()) { return false; }
  
  // we loop through the vector of termination sites to inspect
  // we place ourselves at the beginnig of the vector
  std::vector <const SiteFamily*>::const_iterator
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
