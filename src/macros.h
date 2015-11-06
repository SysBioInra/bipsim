

/**
 * @file macros.h
 * @brief Contains precompiler macros
 * @details Mainly used for macros linked with programming by contract.
 * @authors Marc Dinh, Stephan Fischer
 */

#ifndef MACROS_H
#define MACROS_H

#include <assert.h>
#include "config.h"

/**
 * @brief Precondition checking.
 *
 * If CHECK_PRECONDITIONS is defined, this macro will check the 
 * preconditions of all classes, else it is empty.
 */
#ifdef CHECK_PRECONDITIONS 
#define REQUIRE( precondition ) assert(precondition)
#else
#define REQUIRE( precondition ) // Empty macro: no check performed
#endif // CHECK_PRECONDITIONS

/**
 * @brief Postcondition checking.
 *
 * If CHECK_POSTCONDITIONS is defined, this macro will check the 
 * postconditions of all classes, else it is empty.
 */
#ifdef CHECK_POSTCONDITIONS 
#define ENSURE( postcondition ) assert(postcondition)
#else
#define ENSURE( postcondition ) // Empty macro: no check performed
#endif // CHECK POSTCONDITIONS



#endif // MACROS_H
