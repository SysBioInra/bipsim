

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

 private:
  // Forbidden
  /** @brief Copy constructor. */
  SimulationParams (const SimulationParams& other_simulation_params);
  /** @brief Assignment operator. */
  SimulationParams& operator= (const SimulationParams& other_simulation_params);

 public:
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
   * @brief Accessor to random handler seed.
   * @return Random handler seed. 0 if none specified.
   */
  double seed (void) const;

  /**
   * @brief Accessor to initial simulation time.
   * @return Initial simulation time. 0 if none specified.
   */
  double initial_time (void) const;

  /**
   * @brief Accessor to final simulation time.
   * @return Final simulation time. 1000 if none specified.
   */
  double final_time (void) const;

  /**
   * @brief Accessor to input files.
   * @return List of input file names. Empty if none specified.
   */
  const std::list<std::string>& input_files (void) const;
  
  /**
   * @brief Accessor to name for concentration output file.
   * @return Output file name.
   */
  std::string concentration_file (void) const;

  /**
   * @brief Accessor to name for replication output file.
   * @return Output file name.
   */
  std::string replication_file (void) const;

  /**
   * @brief Accessor to name for reaction output file.
   * @return Output file name.
   */
  std::string reaction_file (void) const;

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

  /**
   * @brief Accessor to double strand to output.
   * @return Empty if none specified.
   */
  const std::string& output_double_strand (void) const;

  /**
   * @brief Accessor to default solver factory.
   * @return A factory creating the default type of solver to use.
   */
  const SolverFactory& solver_factory (void) const;

  /**
   * @brief Accessor to default rate manager factory.
   * @return A factory creating the default type of rate manager to use.
   */
  const RateManagerFactory& rate_manager_factory (void) const;

  /**
   * @brief Accessor to default rate container factory.
   * @return A factory creating the default type of rate container to use.
   */
  const RateContainerFactory& rate_container_factory (void) const;

  /**
   * @brief Accessor to base rate to use for hybrid method.
   * @return Base rate to use for hybrid method (1 by default).
   */
  double hybrid_base_rate (void) const;

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Current line read. */
  std::string _line;

  /** @brief Random handler seed. 0 if none specified. */
  int _seed;

  /** @brief Seed tag. */
  static const std::string _seed_tag;

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

  /** @brief Output dir name. Empty if none specified. */
  std::string _output_dir;

  /** @brief Output dir tag. */
  static const std::string _output_dir_tag;

  /** @brief Output file name for concentrations. */
  static const std::string _concentration_file;

  /** @brief Output file name for concentrations. */
  static const std::string _replication_file;

  /** @brief Output file name for reactions. */
  static const std::string _reaction_file;

  /** @brief Output file name for parameters.. */
  static const std::string _param_file;

  /** @brief Output step in number of reactions. 10000 if none specified. */
  int _output_step;
  
  /** @brief Initial time tag. */
  static const std::string _output_step_tag;

  /** @brief List of entity names. Empty if none specified. */
  std::list<std::string> _output_entities;

  /** @brief Double strand name. Empty if none specified. */
  std::string _output_double_strand;
  
  /** @brief Initial time tag. */
  static const std::string _output_entities_tag;

  /** @brief Drawing algorithm tag. */
  static const std::string _drawing_algorithm_tag;

  /** @brief Factory for default solver. */
  SolverFactory* _solver_factory;

  /** @brief Factory for default rate manager. */
  RateManagerFactory* _rate_manager_factory;

  /** @brief Factory for default rate container. */
  RateContainerFactory* _rate_container_factory;

  /** @brief Hybrid base rate tag. */
  static const std::string _hybrid_base_rate_tag;

  /** @brief Base rate for the HybridRateContainer. */
  double _hybrid_base_rate;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Check whether line is empty or commented.
   * @return True if line is empty or commented.
   */
  bool is_line_empty (void) const;

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
   * @brief Extract value from line if it holds a given tag.
   * @tparam T Type of element to extract.
   * @param line_tag Tag to check against.
   * @param element Reference where element should be stored.
   */
  template <typename T>
    inline bool decode_tag_value (const std::string& line_tag, T& element);
  
  /**
   * @brief Read input files.
   * @return True if input files were successfully read.
   */
  bool read_input_files (void);
  
  /**
   * @brief Read entities to output.
   * @return True if output entities were successfully read.
   */
  bool read_output_entities (void);

  /**
   * @brief Read drawing algorithm.
   * @return True if drawing algorithm was successfully read.
   */
  bool read_drawing_algorithm (void);

  /**
   * @brief Write relevant simulation parameters to a file for future reference.
   */
  void _write_params_out (void) const;
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

template <typename T>
inline bool SimulationParams::decode_tag_value (const std::string& line_tag,
						T& element)
{
  std::istringstream line_stream (_line);
  if (!check_tag (line_stream, line_tag)) return false;
  return read_or_error (line_stream, element, line_tag);
}

inline double SimulationParams::seed (void) const
{
  return _seed;
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

inline std::string SimulationParams::concentration_file (void) const
{
  return _output_dir + "/" + _concentration_file;
}

inline std::string SimulationParams::replication_file (void) const
{
  return _output_dir + "/" + _replication_file;
}

inline std::string SimulationParams::reaction_file (void) const
{
  return _output_dir + "/" + _reaction_file;
}

inline int SimulationParams::output_step (void) const
{
  return _output_step;
}

inline const std::list<std::string>& SimulationParams::output_entities (void) const
{
  return _output_entities;
}

inline const std::string& SimulationParams::output_double_strand (void) const
{
  return _output_double_strand;
}

inline const SolverFactory& SimulationParams::solver_factory (void) const
{
  return *_solver_factory;
}

inline const RateManagerFactory& SimulationParams::rate_manager_factory (void) const
{
  return *_rate_manager_factory;
}

inline const RateContainerFactory& SimulationParams::rate_container_factory (void) const
{
  return *_rate_container_factory;
}

inline double SimulationParams::hybrid_base_rate (void) const
{
  return _hybrid_base_rate;
}			       

#endif // SIMULATION_PARAMS_H
