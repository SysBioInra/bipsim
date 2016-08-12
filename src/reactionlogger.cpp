
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
  , _values (reactions.size(), 0)
  , _previous (reactions.size(), 0)
  , _indices (reactions.size(), 0)
{
}

// Forbidden
// ReactionLogger::ReactionLogger (const ReactionLogger& logger);
// ReactionLogger& ReactionLogger::operator= (const ReactionLogger& other_logger);


// ===========================
//  Public Methods - Commands
// ===========================
//
class CompareValues
{
public:
  CompareValues (const std::vector <long long>& v) : _v (v) {}
  bool operator() (int i, int j) { return _v[i] > _v[j]; }
private:
  const std::vector <long long>& _v;
};

void ReactionLogger::log (double simulation_time)
{
  _output << simulation_time << "\n";

  // update number of reactions performed since last log
  for (int i = 0; i < _reactions.size(); ++i)
    {
      _values[i] = _reactions[i]->number_performed() - _previous[i];
      _previous[i] = _reactions[i]->number_performed();
    }

  // sort indices based on vector _values
  for (int i = 0; i < _indices.size(); ++i) { _indices [i] = i; }    
  std::sort(_indices.begin(), _indices.end(), CompareValues (_values));

  // print output
  for (int i = 1; i <= _log_number; ++i)
    { _output << "\"" << _reactions[_indices[i]]->name() << "\""
	      << "\t" << _values [_indices[i]] << "\n"; }
}

// =================
//  Private Methods
// =================
//
