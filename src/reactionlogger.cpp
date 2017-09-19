
/**
 * @file reactionlogger.cpp
 * @brief Implementation of the ReactionLogger class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <algorithm> // std::sort

// ==================
//  Project Includes
// ==================
//
#include "reactionlogger.h"
#include "reaction.h"
#include "macros.h" // REQUIRE

// ==========================
//  Constructors/Destructors
// ==========================
//
ReactionLogger::ReactionLogger (const std::string& filename,
				const std::vector <const Reaction*>& reactions,
				bool overwrite /*= true*/)
  : Logger (filename, overwrite) 
  , _reactions (reactions)
  , _log_number (10)
  , _total_number_reactions (0)
  , _values (reactions.size(), 0)
  , _previous (reactions.size(), 0)
  , _indices (reactions.size(), 0)
{
  if (_log_number > reactions.size()) { _log_number = reactions.size(); }
  /** @post Number of reactions to log is smaller or equal to total number 
   *   of reactions */
  ENSURE (_log_number <= reactions.size());
}

// Forbidden
// ReactionLogger::ReactionLogger (const ReactionLogger& logger);
// ReactionLogger& ReactionLogger::operator= (const ReactionLogger& other_logger);


// ===========================
//  Public Methods - Commands
// ===========================
//
/**
 * @brief Convenience class used to sort vectors.
 */
class CompareValues
{
public:
  /**
   * @brief Constructor.
   * @param v Vector to sort.
   */
  CompareValues (const std::vector <long long>& v) : _v (v) {}
  /**
   * @brief Method used to sort.
   * @param i Index of first value.
   * @param j Index of second value.
   * @return True if value with index i is greater than value with index j.
   */
  bool operator() (int i, int j) { return _v[i] > _v[j]; }
private:
  const std::vector <long long>& _v;
};

void ReactionLogger::log (double simulation_time)
{
  // update number of reactions performed since last log
  long long int _previous_total = _total_number_reactions;
  _total_number_reactions = 0;
  for (std::size_t i = 0; i < _reactions.size(); ++i)
    {
      _total_number_reactions += _reactions[i]->number_performed();
      _values[i] = _reactions[i]->number_performed() - _previous[i];
      _previous[i] = _reactions[i]->number_performed();
    }

  // sort indices based on vector _values
  for (std::size_t i = 0; i < _indices.size(); ++i) { _indices [i] = i; }    
  std::sort(_indices.begin(), _indices.end(), CompareValues (_values));

  // print output
  long long int number_reactions = _total_number_reactions - _previous_total;
  _output << "\nt = " << simulation_time
	  << " (" << number_reactions << " reactions)\n";
  for (unsigned int i = 0; i < _log_number; ++i)
    { 
      if (_values [_indices[i]] == 0) { return; }
      _output << "\"" << _reactions[_indices[i]]->name() << "\""
	      << "\t" << _values[_indices[i]]
	      << " (" << 100 * _values[_indices[i]] / number_reactions
	      << "%%)\n"; 
    }
}

// =================
//  Private Methods
// =================
//
