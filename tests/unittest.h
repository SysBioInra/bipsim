

/**
 * @file unittest.h
 * @brief Header for the UnitTest class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef UNIT_TEST_H
#define UNIT_TEST_H

// ==================
//  General Includes
// ==================
//
#include <stdexcept> // std::runtime_error

/**
 * @brief BRIEF DESCRIPTION OF UNIT
 */
class UnitTest
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Default constructor. */
  // UnitTest (void);
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // UnitTest (const UnitTest& other_unit_test);
  // /* @brief Assignment operator. */
  // UnitTest& operator= (const UnitTest& other_unit_test);
  // /* @brief Destructor. */
  // ~UnitTest (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //


  // ============================
  //  Public Methods - Accessors
  // ============================
  //

 protected:
  void test (bool condition, const std::string& msg)
  {
    if (condition == false) { throw std::runtime_error (msg); }
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

#endif // UNIT_TEST_H
