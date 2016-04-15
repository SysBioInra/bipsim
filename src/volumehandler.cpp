

/**
 * @file volumehandler.cpp
 * @brief Implementation of the VolumeHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>
#include <limits> // std::numeric_limits

// ==================
//  Project Includes
// ==================
//
#include "volumehandler.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
const double VolumeHandler::INFINITY = 
  std::numeric_limits <double>::infinity();

VolumeHandler::VolumeHandler (double initial_volume)
  : _initial (initial_volume)
  , _next (initial_volume)
  , _next_time (INFINITY)
{
}

// Forbidden
// VolumeHandler::VolumeHandler (const VolumeHandler& other);
// VolumeHandler& VolumeHandler::operator= (const VolumeHandler& other);

// ===========================
//  Public Methods - Commands
// ===========================
//
void VolumeHandler::step (void)
{
  
}

// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
