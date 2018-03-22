// Copyright 2018, OpenCensus Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPENCENSUS_STATS_INTERNAL_MEASURE_DATA_H_
#define OPENCENSUS_STATS_INTERNAL_MEASURE_DATA_H_

#include <cstdint>
#include <vector>

#include "absl/types/span.h"
#include "opencensus/stats/bucket_boundaries.h"
#include "opencensus/stats/distribution.h"

namespace opencensus {
namespace stats {

// MeasureData tracks all aggregations for a single measure, including
// histograms for a number of different BucketBoundaries.
//
// MeasureData is thread-compatible.
class MeasureData final {
 public:
  MeasureData(absl::Span<const BucketBoundaries> boundaries);

  void Add(double value);

  uint64_t count() const { return count_; }
  double sum() const { return count_ * mean_; }

  // Adds this to 'distribution'. Requires that
  // distribution->bucket_boundaries() be in the set of boundaries passed to
  // this on construction.
  void AddToDistribution(Distribution* distribution) const;

 private:
  absl::Span<const BucketBoundaries> boundaries_;

  uint64_t count_ = 0;
  double mean_ = 0;
  double sum_of_squared_deviation_ = 0;
  double min_ = std::numeric_limits<double>::infinity();
  double max_ = -std::numeric_limits<double>::infinity();
  std::vector<std::vector<int64_t>> histograms_;
};

}  // namespace stats
}  // namespace opencensus

#endif  // OPENCENSUS_STATS_INTERNAL_MEASURE_DATA_H_
