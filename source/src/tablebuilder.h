// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 


/**
 * @file tablebuilder.h
 * @brief Header for the CompositionTableBuilder, LoadingTableBuilder, 
 *  ProductTableBuilder, TransformationTableBuilder classes.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef TABLE_BUILDER_H
#define TABLE_BUILDER_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "builder.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class creating CompositionTable from text input.
 */
class CompositionTableBuilder : public Builder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  CompositionTableBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _header_format, _composition_format;
  std::string _name; 
  std::vector <std::string> _chemical_names;
  std::vector <char> _letters;
  std::vector <std::list <FreeChemical*> > _chemical_lists;
};

/**
 * @brief Class creating LoadingTable from text input.
 */
class LoadingTableBuilder : public Builder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  LoadingTableBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _header_format, _line_format;
  TagToken _comma_token;
  std::string _name, _template, _base_name, _occupied_name;
  double _rate;
  std::vector <std::string> _templates;
  std::vector <FreeChemical*> _bases;
  std::vector <BoundChemical*> _occupied;
  std::vector <double> _rates;
};

/**
 * @brief Class creating ProductTable from text input.
 */
class ProductTableBuilder : public Builder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  ProductTableBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _format;
  std::string _name, _table_name;
};

/**
 * @brief Class creating TransformationTable from text input.
 */
class TransformationTableBuilder : public Builder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  TransformationTableBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _header_format, _line_format;
  TagToken _comma_token;
  std::string _name, _input, _output;
};


#endif // TABLE_BUILDER_H
