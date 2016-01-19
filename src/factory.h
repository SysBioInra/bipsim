

/**
 * @file factory.h
 * @brief Header for the Factory class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef FACTORY_H
#define FACTORY_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <sstream> // std::istringstream

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class used to create entities used by the simulator.
 *
 * Factory automatically recognizes which entity to create and stores it
 * in the appropriate object.
 */
class Factory
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //

  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Default constructor. */
  // Factory (void)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Factory (const Factory& other_factory);
  // /* @brief Assignment operator. */
  // Factory& operator= (const Factory& other_factory);
  /** @brief Destructor. */
  virtual ~Factory (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create entity from line.
   * @param line Text line to create entity from.
   * @return True if an entity was successfully created.
   */
  virtual bool handle (const std::string& line) = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Check if next word in stream corresponds to some defined tag.
   * @param line_stream Stream to read next word from.
   * @param tag Word to check against.
   * @return True if the next word corresponds to tag.
   */
  bool check_tag (std::istringstream& line_stream, std::string tag)
  {
    // check tag
    std::string tag_read;
    return ((line_stream >> tag_read) && (tag_read == tag));
  }


private:
  // ============
  //  Attributes
  // ============
  //
  

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//

#endif // FACTORY_H
