

/**
 * @file partialstrand.h
 * @brief Header for the PartialStrand class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef PARTIAL_STRAND_H
#define PARTIAL_STRAND_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list

// ==================
//  Project Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class representing a strand composed of subsegments.
 */
class PartialStrand
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Constructor. */
  PartialStrand (int length, const FreeEndFactory& factory);


 private:
  // Forbidden
  /** @brief Copy constructor. */
  PartialStrand (const PartialStrand& other);
  /** @brief Assignment operator. */
  PartialStrand& operator= (const PartialStrand& other);
 public:
  
  /** @brief Destructor. */
  ~PartialStrand (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Start new segment at given position.
   * @param position Position where segment should be started.
   * @return True if segment was created, false if position was occupied.
   */
  bool start_segment (int position);

  /**
   * @brief Extend existing segment at given position.
   * @param position Position where extension should take place.
   * @return True if extension took place, False if position was occupied or
   *  if there was no segment spanning to given position.
   */
  bool extend_segment (int position);


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Check whether strand has been completed.
   * @return True if strand has been completed.
   */
  bool completed (void) const;

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Length of the strand. */
  int _length;

  /** @brief True if strand has been completed. */
  bool _complete;
  
  /** @brief Factory used to create free ends. */
  const FreeEndFactory& _free_end_factory;

  /** @brief List of sequence segments. */
  std::list <Segment*> _segments;

  /** @brief Last segment used. */
  std::list <Segment*>::iterator _segment_it;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Find segment immediately starting before position.
   * @param position Position used to select segment.
   * @return Segment immediately starting before position.
   */
  std::list <Segment*>::iterator _find (int position);
  
  /**
   * @brief Check whether last segment manipulated should be ligated to 
   *  segment that directly follows.
   */
  void _check_ligation (void);
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"
#include "segment.h"

inline bool PartialStrand::extend_segment (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  // A segment can be extended previous segment spans exactly until the given
  // position
  std::list <Segment*>::iterator result = _find (position);
  if (((*result)->last() != position)) { return false; }
  
  _segment_it = result;
  if (position < _length-1) { (*_segment_it)->extend_left(); }
  else { (*_segment_it)->set_last (Segment::END, _free_end_factory); }
  _check_ligation();
  return true;
}

inline bool PartialStrand::completed (void) const
{
  return _complete;
}

inline bool find_postcondition (std::list <Segment*>::iterator result, 
				int position)
{
  std::list <Segment*>::iterator next = result; ++next;
  return (((*result)->first() < position) 
	  && 
	  (((*result)->last() < position) || ((*next)->first() >= position)));
}

inline std::list <Segment*>::iterator PartialStrand::_find (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  std::list <Segment*>::iterator result = _segment_it;
  // this algorithm works thanks to the dummy segments
  if ((*result)->first() < position)
    { 
      if ((*result)->last() < position)
	{
	  ++result;
	  while ((*result)->first() < position) { ++result; } 
	  --result;
	}
    }
  else
    { while ((*result)->first() >= position) { --result; } }

  /**
   * @post The selected segment must meet two conditions. (i) starting position
   *  of the pointed segment must be smaller or equal to position. (ii) starting
   *  position of the following segment must be strictly greater than position.
   *  This is always true thanks to the dummy segments inserted at construction.
   */
  ENSURE (find_postcondition (result, position));
  return result;
}

inline void PartialStrand::_check_ligation (void)
{
  std::list <Segment*>::iterator next_it = _segment_it; ++next_it;

  // ligation occurs if end of last manipulated segments overlaps its follower
  if ((next_it != _segments.end()) 
      && (((*_segment_it)->last() > (*next_it)->first()) 
	  || ((*_segment_it)->last() == Segment::END)))
    { 
      (*_segment_it)->absorb_left (**next_it); 
      delete *next_it;
      _segments.erase (next_it); 

      // special case for circular strands
      // if we reached the last base of sequence, a free end appears at
      // the start (or it is implicitly ligated if there was a segment there).
      if ((_segment_it == (--_segments.end()))
	  && (_segments.front()->last() == Segment::START))
	{ _segments.front()->set_last (0, _free_end_factory); }

      // strand is complete when our two dummy segments finally meet <3
      if (((*_segment_it)->first() == Segment::START) 
	  && ((*_segment_it)->last() == Segment::END)) 
	{ _complete = true; }
    }  

  ENSURE ((_complete == true) || (_segments.size() > 1));
}


#endif // PARTIAL_STRAND_H
