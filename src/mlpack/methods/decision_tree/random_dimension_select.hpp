/**
 * @file random_dimension_select.hpp
 * @author Ryan Curtin
 *
 * Selects one single random dimension to split on.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_DECISION_TREE_RANDOM_DIMENSION_SELECT_HPP
#define MLPACK_METHODS_DECISION_TREE_RANDOM_DIMENSION_SELECT_HPP

namespace mlpack {
namespace tree {

/**
 * This dimension selection policy only selects one single random dimension.
 */
class RandomDimensionSelect
{
 public:
  /**
   * Construct the RandomDimensionSelect object with the given number of
   * dimensions.
   */
  RandomDimensionSelect(const size_t dimensions) : dimensions(dimensions) { }

  /**
   * Get the first dimension to select from.
   */
  size_t Begin() const { return math::RandInt(dimensions); }

  /**
   * Get the last dimension to select from.
   */
  size_t End() const { return dimensions; }

  /**
   * Get the next (last) dimensions.  We only allow one dimension, so any 'next'
   * dimension is past our bounds.
   */
  size_t Next() const { return dimensions; }

 private:
  //! The number of dimensions to select from.
  const size_t dimensions;
};

} // namespace tree
} // namespace mlpack

#endif
