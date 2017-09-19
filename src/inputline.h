
/**
 * @file inputline.h
 * @brief Header for the InputLine class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef INPUT_LINE_H
#define INPUT_LINE_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <string> // std::string
#include <sstream> // std::istringstream

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE ENSURE

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class representing text input as a vector of words.
 */
class InputLine
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor. 
   */
  InputLine (const std::string& line) 
    : _line (line)
    , _current_word (0)
  {
    std::istringstream line_stream (line);
    std::string word;
    while (line_stream >> word)
      {
	if (word [word.length()-1] == ',')
	  {
	    word.resize (word.length()-1);
	    _words.push_back (word);
	    _words.push_back (",");
	  }
	else { _words.push_back (word); }
      }    
  }

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // InputLine (const InputLine& other);
  // /* @brief Assignment operator. */
  // InputLine& operator= (const InputLine& other);
  // /* @brief Destructor. */
  // ~InputLine (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Move to next word in line.
   */
  void step (void) 
  { 
    /** @pre End of line must not have been reached already. */
    REQUIRE (end_of_line() == false);
    ++_current_word; 
  }

  /**
   * @brief Go to word corresponding to mark.
   * @param mark Integer mark corresponding to word.
   */
  void go_to (int mark)
  {
    /** @pre mark must match number of words in current line. */
    REQUIRE ((mark >= 0) && (mark <= _words.size()));
    _current_word = mark;
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to line.
   * @return Line being processed as a string.
   */
  const std::string& line (void) { return _line; }

  /**
   * @brief Accessor to current word.
   * @return Current word as a string.
   */
  const std::string& word (void) 
  {
    /** @pre current word must be valid. */
    REQUIRE (end_of_line() == false);
    return _words [_current_word]; 
  }

  /**
   * @brief Check whether end of line has been reached.
   * @return True if cursor moved past last word.
   */
  bool end_of_line (void) { return _current_word >= _words.size(); }

  /**
   * @brief Accessor to mark corresponding to current word.
   * @return Integer mark identifying current word or end of line.
   */
  int mark (void) { return _current_word; }

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  // 
  /** @brief Whole input line. */
  std::string _line;

  /** @brief Vector of words in line. */
  std::vector <std::string> _words;

  /** @brief Word currently read. */
  unsigned int _current_word;
};

// ======================
//  Inline declarations
// ======================
//

#endif // INPUT_LINE_H
