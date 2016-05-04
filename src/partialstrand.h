

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
#include <vector> // std::vector

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
  PartialStrand (int length, const FreeEndHandler& free_end_handler);


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

  /**
   * @brief Positon of segments composing partial strand.
   * @return Vector containing positions of segments. Numbers are to be read
   *  two by two, each pair representing a new segment. E.g. [1, 10, 15, 20]
   *  means that the strand is composed of segment [1 10] and [15 20].
   */
  std::vector <int> segments (void) const;

private:
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Extend focused segment to the left, possibly joining previous 
   *  segment.
   */
  void extend_left (void);

  /**
   * @brief Extend focused segment to the right, possibly joining next
   *  segment.
   */
  void extend_right (void);

  /**
   * @brief Join focused segment to segment at its right if there is a one base
   *  gap between them.
   * @return True if joining succeeded.
   */
  bool join_right (void);

  /**
   * @brief Join focused segment to segment at its left if there is a one base
   *  gap between them.
   * @return True if joining succeeded.
   */
  bool join_left (void);

  /**
   * @brief Find segment containing or immediately starting before position.
   * @param position Position used to select segment.
   * @return Segment containing or immediately starting before position.
   */
  std::list <Segment*>::iterator find (int position);
  
  /**
   * @brief Check whether last segment manipulated should be ligated to 
   *  segment that directly follows.
   */
  void check_completeness (void);

  // ============
  //  Attributes
  // ============
  //
  /** @brief Length of the strand. */
  int _length;

  /** @brief True if strand has been completed. */
  bool _complete;
  
  /** @brief Handler used to generate free end binding sites. */
  const FreeEndHandler& _free_end_handler;

  /** @brief List of sequence segments. */
  std::list <Segment*> _segments;

  /** @brief Last segment used. */
  std::list <Segment*>::iterator _segment_it;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"
#include "segment.h"

inline bool PartialStrand::completed (void) const
{
  return _complete;
}

inline bool PartialStrand::extend_segment (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  // A segment can be extended previous segment spans exactly until the given
  // position
  std::list <Segment*>::iterator result = find (position);
  if (((*result)->first() == position))
    { _segment_it = result; extend_left(); }
  else if (((*result)->last() == position))
    { _segment_it = result; extend_right(); }
  else { return false; }
  return true;
}

inline void PartialStrand::extend_left (void)
{
  // check whether segment is about to reach dummy (START START) segment
  // at origin, i.e. it has form (0 L)
  if (((*_segment_it)->first() == 0) 
      && (_segments.front()->last() == Segment::START)) 
    {
      // we modify dummy segment to (START 0) so it will apply the junction
      // (START 0) + (0 L) = (START L)
      _segments.front()->set_last (0);
      // a free end appears on other end so it becomes (length-1 END)
      _segments.back()->set_first (_length-1);
    }
  // join to previous segment or extend
  if (!join_left()) { (*_segment_it)->extend_left(); }
  check_completeness();
}

inline void PartialStrand::extend_right (void)
{
  // check whether segment is about to reach dummy (END END) segment
  // at origin, i.e. it has form (L length-1)
  if (((*_segment_it)->last() == _length-1) 
      && (_segments.back()->first() == Segment::END)) 
    { 
      // we modify segment to (length-1 END) so it will apply the junction
      // (L length-1) + (length-1 END) = (L END)
      _segments.back()->set_first (_length-1);
      // a free end appears on other end so it becomes (START 0)
      _segments.front()->set_last (0);
    }
  // join to following segment or extend
  if (!join_right()) { (*_segment_it)->extend_right(); }
  check_completeness();
 }


inline bool find_postcondition (std::list <Segment*>::iterator result, 
				int position)
{
  std::list <Segment*>::iterator next = result; ++next;
  return (((*result)->first() <= position) 
	  && 
	  (((*result)->last() >= position) || ((*next)->first() > position)));
}

inline std::list <Segment*>::iterator PartialStrand::find (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  std::list <Segment*>::iterator result = _segment_it;
  if ((*result)->first() < position)
    { 
      if ((*result)->last() < position)
	{
	  ++result;
	  while ((*result)->first() <= position) { ++result; } 
	  --result;
	}
    }
  else
    { while ((*result)->first() > position) { --result; } }

  /**
   * @post The selected segment (a,b) must meet two conditions. 
   *  (i) a must be smaller or equal to position.
   *  (iia) b is larger or equal to position
   *  OR (iib) a of the following segment is strictly larger than position.
   */
  ENSURE (find_postcondition (result, position));
  return result;
}

inline void PartialStrand::check_completeness (void)
{
  // strand is complete when our two dummy segments finally meet <3
  _complete = ((*_segment_it)->first() == Segment::START) 
    && ((*_segment_it)->last() == Segment::END);
  ENSURE ((_complete == true) || (_segments.size() > 1));
}  

inline bool PartialStrand::join_left (void)
{
  REQUIRE (_segment_it != _segments.begin());      
  REQUIRE (_segment_it != _segments.end());      

  --_segment_it;
  if (join_right()) { return true; }
  else { ++_segment_it; return false; }
}

inline bool PartialStrand::join_right (void)
{
  REQUIRE (_segment_it != _segments.end());
  std::list <Segment*>::iterator next_it = _segment_it; ++next_it;
  REQUIRE (next_it != _segments.end());
  // junction rule: (a b) + (b c) = (a c)
  if ((*_segment_it)->last() == (*next_it)->first())
    { 
      (*_segment_it)->absorb_right (**next_it); 
      delete *next_it;
      _segments.erase (next_it);
      return true;
    }  
  else { return false; }
}


#endif // PARTIAL_STRAND_H
