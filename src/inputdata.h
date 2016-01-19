

/**
 * @file inputdata.h
 * @brief Header for the InputData class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef INPUT_DATA_H
#define INPUT_DATA_H

// ==================
//  General Includes
// ==================
//
#include <iostream> // std::ostream
#include <fstream> // std::ifstream
#include <string> // std::string
#include <vector> // std::vector
#include <list> // std::list

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class simplifying access to set of input files.
 *
 * InputData gives access to several input files as if it were a single file.
 * It offers classical file operations as well as a marking system that allows
 * treated lines to be "removed".
 */
class InputData
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor from file names.
   * @param input_file_names List of the files to open.
   */
  InputData (const std::list <std::string>& input_file_names);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  InputData (const InputData& other_input_data);
  /** @brief Assignment operator. */
  InputData& operator= (const InputData& other_input_data);

 public:
  /**
   * @brief Destructor.
   */
  ~InputData (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Move to next line that has is not empty and has not been treated.
   */
  void go_next (void);

  /**
   * @brief Mark current line as treated.
   */
  void mark_line_as_treated (void);

  /**
   * @brief Go back to first input line.
   */
  void rewind (void);

  /**
   * @brief Write warnings about untreated lines.
   * @param output Stream to write warnings to.
   */
  void write_warnings (std::ostream& output);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns whether end of file has been reached.
   * @return True if object proceeded past last input line.
   */
  bool is_eof (void);

  /**
   * @brief Returns current line.
   * @return Current line content as a string.
   */
  std::string line (void);

  /**
   * @brief Returns current file name.
   * @return Name of file containing current line.
   */
  std::string file_name (void);

  /**
   * @brief Returns current line number within file.
   * @return Position of current line within its file.
   */
  int line_number (void);

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Input files. */
  std::list <std::ifstream*> _files;

  /** @brief Index (in the _files vector) of file being currently read. */
  std::list <std::ifstream*>::iterator _file;
 
  /** @brief Input file names. */
  std::list <std::string> _file_names;

  /** @brief Input file name of file being currently read. */
  std::list <std::string>::iterator _file_name;

  /** @brief Number of line being read (summed over all files). */
  int _line_number;

  /** @brief Number of line being read (relative to its file). */
  int _file_line_number;

  /** @brief Line being read. */
  std::string _line;

  /** @brief List of lines that have been left untreated. */
  std::vector <bool> _line_treated;

  /** @brief Flag indicating whether end of file was reached. */
  bool _eof;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Returns whether current line has already been treated.
   * @return True if current line was already treated.
   */
  bool is_line_treated (void);

  /**
   * @brief Returns whether current line is empty or commented.
   * @return True if current line is empty or commented.
   */
  bool is_line_empty (void);

  /**
   * @brief Move to the next file line (possibly in the next file).
   */
  void go_to_next_line (void);
};

// ======================
//  Inline declarations
// ======================
//

inline bool InputData::is_line_treated (void)
{
  return _line_treated [_line_number-1];
}

inline std::string InputData::line (void)
{
  return _line;
}

inline std::string InputData::file_name (void)
{
  return *_file_name;
}

inline int InputData::line_number (void)
{
  return _file_line_number;
}

inline void InputData::mark_line_as_treated (void)
{
  _line_treated [_line_number-1] = true;
}

inline bool InputData::is_eof (void)
{
  return _eof;
}

#endif // INPUT_DATA_H

