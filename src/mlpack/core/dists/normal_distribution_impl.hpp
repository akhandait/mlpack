/**
 * @file normal_distribution.cpp
 * @author Atharva Khandait
 *
 * Implementation of Normal distribution class.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_CORE_DISTRIBUTIONS_NORMAL_DISTRIBUTION_IMPL_HPP
#define MLPACK_CORE_DISTRIBUTIONS_NORMAL_DISTRIBUTION_IMPL_HPP

#include "normal_distribution.hpp"

using namespace mlpack;
using namespace mlpack::distribution;

template<typename DataType>
NormalDistribution<DataType>::NormalDistribution(
    const DataType& mean,
    const DataType& stdDeviation) :
    mean(mean),
    stdDeviation(stdDeviation)
{
  if (mean.size() != stdDeviation.size())
  {
    Log::Fatal << "NormalDistribution<>::NormalDistribution(): The sizes of "
        << "the mean and the standard deviation should be equal." << std::endl;
  }
}

template<typename DataType>
NormalDistribution<DataType>::NormalDistribution(
    const DataType& input) :
    mean(input.submat(input.n_rows / 2, 0, input.n_rows - 1,
        input.n_cols - 1)),
    preStdDev(input.submat(0, 0, input.n_rows / 2 - 1, input.n_cols - 1))
{
  if (input.n_rows % 2 != 0)
  {
    Log::Fatal << "NormalDistribution<>::NormalDistribution(): The number of "
        << "rows of input matrix should be even." << std::endl;
  }

  // Apply softplus function.
  stdDeviation = preStdDev;
  stdDeviation.transform([](double val)
  {
    if (val < DBL_MAX)
      return val > -DBL_MAX ? std::log(1 + std::exp(val)) : 0;
    return 1.0;
  });
}

template<typename DataType>
DataType NormalDistribution<DataType>::Random() const
{
  return stdDeviation % arma::randn<DataType>(mean.n_rows, mean.n_cols) + mean;
}

template<typename DataType>
double NormalDistribution<DataType>::LogProbability(
    const DataType& observation) const
{
  return -0.5 * (arma::accu(2 * arma::log(stdDeviation) +
      arma::pow((mean - observation) / stdDeviation, 2) + log2pi));
}

template<typename DataType>
void NormalDistribution<DataType>::LogProbBackward(
    const DataType& observation, DataType& output) const
{
  if (preStdDev.is_empty())
    output = -0.5 * join_cols((2 / stdDeviation - 2 *
      arma::pow(mean - observation, 2) / arma::pow(stdDeviation, 3)), 2 *
      (mean - observation) / arma::pow(stdDeviation, 2));
  else
  {
  // Apply derivative of softplus function.
  output = 1.0 / (1 + arma::exp(-preStdDev));
  output = -0.5 * join_cols((2 / stdDeviation - 2 *
      arma::pow(mean - observation, 2) / arma::pow(stdDeviation, 3)) % output,
      2 * (mean - observation) / arma::pow(stdDeviation, 2));
  }
}

#endif
