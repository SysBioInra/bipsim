
/**
 * @file sequenceoccupation.cpp
 * @brief Implementation of the SequenceOccupation class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <algorithm> // std::find

// ==================
//  Project Includes
// ==================
//
#include "sequenceoccupation.h"
#include "bindingsite.h"
#include "sitegroup.h"
#include "partialstrand.h"
#include "macros.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
SequenceOccupation::SequenceOccupation (int length, int number)
  : _number_sequences (number)
  , _occupancy (length, 0)
  , _number_segments (length, 0)
{
}

// Forbidden
// SequenceOccupation::SequenceOccupation (const SequenceOccupation& other);
// SequenceOccupation& SequenceOccupation::operator= (const SequenceOccupation& other);

SequenceOccupation::~SequenceOccupation (void)
{
  for (int i = 0; i < _site_groups.size(); ++i)  { delete _site_groups[i]; }
  for (std::vector <PartialStrand*>::iterator strand_it 
	 = _partial_by_index.begin();
       strand_it != _partial_by_index.end(); ++strand_it)
    { delete *strand_it; }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void SequenceOccupation::add_element (int first, int last)
{
  /** @pre first must be within sequence bound. */
  REQUIRE ((first >= 0) && (first < _occupancy.size()));
  /** @pre last must be within sequence bound. */
  REQUIRE ((last >= 0) && (last < _occupancy.size()));
  /** @pre first must be smaller or equal to last. */
  REQUIRE (first <= last);

  // update occupancy status
  for (int i = first; i <= last; i++) { _occupancy [i] += 1; }

  // notify change
  notify_change (first, last);
}

void SequenceOccupation::remove_element (int first, int last)
{
  /** @pre first must be within sequence bound. */
  REQUIRE ((first >= 0) && (first < _occupancy.size()));
  /** @pre last must be within sequence bound. */
  REQUIRE ((last >= 0) && (last < _occupancy.size()));
  /** @pre first must be smaller or equal to last. */
  REQUIRE (first <= last);

  // update occupancy status
  for (int i = first; i <= last; i++) { _occupancy [i] -= 1; }  

  // notify change
  notify_change (first, last);
}

void SequenceOccupation::add_sequence (int quantity)
{
  /** @pre quantity must be positive. */
  REQUIRE (quantity >= 0);

  _number_sequences += quantity;
  notify_all_sites();
}

void SequenceOccupation::remove_sequence (int quantity)
{
  /** @pre quantity must be smaller than the current number of sequences. */
  REQUIRE (quantity <= _number_sequences);

  _number_sequences -= quantity;
  notify_all_sites();
}

int SequenceOccupation::start_segment (int position)
{
  /** @pre position must be within sequence bound. */
  REQUIRE ((position >= 0) && (position < _occupancy.size()));

  _occupancy [position] -= 1;
  _number_segments [position] += 1;
  int strand_id;
  std::list <int>::iterator strand_id_it = _partial_creation_order.begin();
  while (strand_id_it != _partial_creation_order.end())
    {
      strand_id = *strand_id_it;
      if (_partial_by_index [strand_id]->occupy (position)) { break; }
      ++strand_id_it;
    }
  if (strand_id_it == _partial_creation_order.end()) 
    { 
      strand_id = next_strand_id();
      _partial_by_index [strand_id] = new PartialStrand (_occupancy.size()); 
      _partial_creation_order.push_back (strand_id);
      _partial_by_index [strand_id]->occupy (position);
    }
  check_completion (strand_id);
  notify_change (position, position);
  /** @post strand_id must be in valid range. */
  ENSURE ((strand_id >= 0) && (strand_id < _partial_by_index.size()));
  return strand_id;
}

bool SequenceOccupation::extend_segment (int strand_id, int position)
{
  /** @pre position must be within sequence bound. */
  REQUIRE ((position >= 0) && (position < _occupancy.size()));
  /** @pre strand_id must be in valid range. */
  REQUIRE ((strand_id >= 0) && (strand_id < _partial_by_index.size()));

  if (_partial_by_index [strand_id]->occupy (position))
    {
      _occupancy [position] -= 1;
      _number_segments [position] += 1;
      check_completion (strand_id);
      notify_change (position, position);
      return true;
    }
  else { return false; }
}

void SequenceOccupation::watch_site (BindingSite& site)
{
  // look if site should be added to an existing group
  bool new_group = true;
  int ind_g = position_to_group (site.first());
  if (ind_g < _site_groups.size())
    {
      SiteGroup* g = _site_groups [ind_g];
      if (site.overlaps (g->first(), g->last()))
	{
	  // add to group and look whether it overlaps with the following groups
	  g->add_site (site);
	  fuse_groups (ind_g);
	  new_group = false;
	}
    }
  if (new_group == true)
    {
      // create and insert new group
      SiteGroup* g = new SiteGroup;
      g->add_site (site);
      if (ind_g < _site_groups.size())
	{ _site_groups.insert (_site_groups.begin()+ind_g, g); }
      else
	{ _site_groups.push_back (g); }
    }

  // send first notification about site availability
  site.update();
}

// ============================
//  Public Methods - Accessors
// ============================
//
int SequenceOccupation::number_sites (int first, int last) const
{
  /** @pre first must be within sequence bound. */
  REQUIRE ((first >= 0) && (first < _occupancy.size()));
  /** @pre last must be within sequence bound. */
  REQUIRE ((last >= 0) && (last < _occupancy.size()));
  /** @pre first must be smaller or equal to last. */
  REQUIRE (first <= last);

  int min = _number_segments [first];
  for (int i = first+1; i <= last; ++i)
    {
      if (_number_segments [i] < min) { min = _number_segments [i]; }
    }
  return _number_sequences + min;
}

int SequenceOccupation::number_available_sites (int first, int last) const
{
  /** @pre first must be within sequence bound. */
  REQUIRE ((first >= 0) && (first < _occupancy.size()));
  /** @pre last must be within sequence bound. */
  REQUIRE ((last >= 0) && (last < _occupancy.size()));
  /** @pre first must be smaller or equal to last. */
  REQUIRE (first <= last);

  int max_occupied = _occupancy [first];
  for (int i = first+1; i <= last; ++i)
    {
      if (_occupancy [i] > max_occupied) { max_occupied = _occupancy [i]; }
    }
  int result = _number_sequences - max_occupied;
  if (result < 0) { return 0; }
  return result;
}

std::list <std::vector <int> > SequenceOccupation::partial_strands (void) const
{
  std::list <std::vector <int> > result;
  for (std::list <int>::const_iterator it = _partial_creation_order.begin();
       it != _partial_creation_order.end(); ++it)
    {
      result.push_back (_partial_by_index [*it]->segments());
    }
  return result;
}

// =================
//  Private Methods
// =================
//
void SequenceOccupation::fuse_groups (int index)
{
  // if group is last in vector no fusion with a follower is possible
  if (index >= _site_groups.size()-1) return;

  // fuse if group overlaps with the following one
  if (_site_groups [index]->last() < _site_groups [index+1]->first())
    { return; }
  _site_groups [index]->absorb (*_site_groups [index+1]);
  delete _site_groups [index+1];
  _site_groups.erase (_site_groups.begin() + index+1);
  fuse_groups (index);
}

int SequenceOccupation::position_to_group (int position) const
{
  if (_site_groups.size() == 0) { return 1; }
  if (_site_groups[0]->last() >= position) { return 0; }

  // we look for the largest i such that
  //  _site_groups[i].last() < position
  int i_min = 0; int i_max = _site_groups.size()-1;
  int i;
  while (i_min < i_max)
    {
      i = (i_min + i_max + 1)/2;
      if (_site_groups[i]->last() >= position) { i_max = i-1; }
      else { i_min = i; }
    }

  // we return the next group that may contain position, i.e. i+1
  return i_min+1;
}

void SequenceOccupation::notify_change (int a, int b) const
{
  if (_site_groups.size() > 0)
    {
      int last_group = position_to_group (b);
      if (last_group == _site_groups.size()) { --last_group; }
      for (int i = position_to_group (a); i <= last_group; ++i)
	{ _site_groups [i]->update (a, b); }
    }
}

void SequenceOccupation::notify_all_sites (void) const
{
  for (int i = 0; i < _site_groups.size(); ++i)
    { _site_groups [i]->update_all(); }
}

void SequenceOccupation::check_completion (int strand_id)
{
  if (_partial_by_index [strand_id]->completed()) 
    {
      delete _partial_by_index [strand_id];
      _partial_by_index [strand_id] = 0;
      _partial_creation_order.remove (strand_id);
      ++_number_sequences;
      for (int i = 0; i < _occupancy.size(); ++i) 
	{ ++_occupancy [i]; _number_segments [i] -= 1; }
    }
}

int SequenceOccupation::next_strand_id (void)
{
  for (int i = 0; i < _partial_by_index.size(); ++i)
    { 
      if (_partial_by_index [i] == 0) { return i; }
    }
  int result = _partial_by_index.size();
  _partial_by_index.resize (result+1, 0);
  return result;
}
