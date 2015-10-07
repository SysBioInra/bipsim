

/**
 * @file simulationparams.h
 * @brief Header for the SimulationParams class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SIMULATION_PARAMS_H
#define SIMULATION_PARAMS_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <list> // std::list
#include <sstream> // std::istringstream

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class storing simulation parameters.
 *
 * SimulationParams reads and stores essential simulation parameters from a
 * file.
 */
class SimulationParams
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor from file.
   * @param filename Path to the file to read parameters from.
   */
  SimulationParams (const std::string& filename);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // SimulationParams (const SimulationParams& other_simulation_params);

  /**
   * @brief Destructor.
   */
  ~SimulationParams (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to initial simulation time.
   * @return Initial simulation time. 0 if none specified
   */
  double initial_time (void) const;

  /**
   * @brief Accessor to final simulation time.
   * @return Final simulation time. 1000 if none specified
   */
  double final_time (void) const;

  /**
   * @brief Accessor to input files.
   * @return List of input file names. Empty if none specified.
   */
  const std::list<std::string>& input_files (void) const;
  
  /**
   * @brief Accessor to output file.
   * @return Output file name. Empty if none specified.
   */
  const std::string& output_file (void) const;

  /**
   * @brief Accessor to output step.
   * @return Output step in number of reactions. 10000 if none specified
   */
  int output_step (void) const;
  
  /**
   * @brief Accessor to entities to output.
   * @return List of entity names. Empty if none specified.
   */
  const std::list<std::string>& output_entities (void) const;

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator.
  //  */
  // SimulationParams& operator= (const SimulationParams& other_simulation_params);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Initial simulation time. 0 if none specified. */
  double _initial_time;

  /** @brief Initial time tag. */
  static const std::string _initial_time_tag;

  /** @brief Final simulation time. 1000 if none specified. */
  double _final_time;

  /** @brief Final time tag. */
  static const std::string _final_time_tag;

  /** @brief List of input file names. Empty if none specified. */
  std::list<std::string> _input_files;
  
  /** @brief Input files tag. */
  static const std::string _input_files_tag;

  /** @brief Output file name. Empty if none specified. */
  std::string _output_file;

  /** @brief Output file tag. */
  static const std::string _output_file_tag;

  /** @brief Output step in number of reactions. 10000 if none specified. */
  int _output_step;
  
  /** @brief Initial time tag. */
  static const std::string _output_step_tag;

  /** @brief List of entity names. Empty if none specified. */
  std::list<std::string> _output_entities;
  
  /** @brief Initial time tag. */
  static const std::string _output_entities_tag;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Check whether line is empty or commented.
   * @param line Line to check.
   * @return True if line is empty or commented.
   */
  bool is_line_empty (const std::string& line);

  /**
   * @brief Check that next word in stream corresponds to desired tag.
   * @param line_stream Stream to extract next word from.
   * @param tag Tag to check against.
   * @param True if next word in stream is desired tag.
   */
  bool check_tag (std::istringstream& line_stream, std::string tag) const;

  /**
   * @brief Extract element from stream or display error if extraction failed.
   * @tparam T Type of element to extract.
   * @param line_stream Stream to extract element from.
   * @param element Reference where element should be stored.
   * @param line_tag Tag associated with line (used in error message).
   */
  template <typename T>
    bool read_or_error (std::istringstream& line_stream, T& element,
			const std::string& line_tag);
  
  /**
   * @brief Read initial simulation time.
   * @param line Line to read.
   * @return True if initial time was successfully read.
   */
  bool read_initial_time (const std::string& line);

  /**
   * @brief Read final simulation time.
   * @param line Line to read.
   * @return True if final time was successfully read.
   */
  bool read_final_time (const std::string& line);

  /**
   * @brief Read input files.
   * @param line Line to read.
   * @return True if input files were successfully read.
   */
  bool read_input_files (const std::string& line);
  
  /**
   * @brief Read output file.
   * @param line Line to read.
   * @return True if output file was successfully read.
   */
  bool read_output_file (const std::string& line);

  /**
   * @brief Read output step.
   * @param line Line to read.
   * @return True if output step was successfully read.
   */
  bool read_output_step (const std::string& line);
  
  /**
   * @brief Read entities to output.
   * @param line Line to read.
   * @return True if output entities were successfully read.
   */
  bool read_output_entities (const std::string& line);

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline bool SimulationParams::check_tag (std::istringstream& line_stream,
					 std::string tag) const
{
  // check tag
  std::string tag_read;
  return ((line_stream >> tag_read) && (tag_read == tag));
}

template <typename T>
inline bool SimulationParams::read_or_error (std::istringstream& line_stream,
					     T& element,
					     const std::string& line_tag)
{
  if (line_stream >> element) return true;
  std::cerr << "ERROR: could not read " << line_tag << ", invalid format.\n";
  return false;
}

inline double SimulationParams::initial_time (void) const
{
  return _initial_time;
}

inline double SimulationParams::final_time (void) const
{
  return _final_time;
}

inline const std::list<std::string>& SimulationParams::input_files (void) const
{
  return _input_files;
}

inline const std::string& SimulationParams::output_file (void) const
{
  return _output_file;
}

inline int SimulationParams::output_step (void) const
{
  return _output_step;
}

inline const std::list<std::string>& SimulationParams::output_entities (void) const
{
  return _output_entities;
}


#endif // SIMULATION_PARAMS_H
