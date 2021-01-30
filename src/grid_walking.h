#ifndef DUX_FILED_SRC_GRID_WALKING_H_
#define DUX_FILED_SRC_GRID_WALKING_H_

#include <vector>

#include "fixed_vec2.h"

namespace dux {

struct GridSize {
  int32_t width_;
  int32_t height_;
};

struct GridPosition {
  int32_t x_;
  int32_t y_;
  bool operator==(GridPosition const& other) {
    return x_ == other.x_ && y_ == other.y_;
  }
  bool operator!=(GridPosition const& other) {
    return x_ != other.x_ || y_ != other.y_;
  }
};

dux::GridPosition GridPositionFromFVec2(dux::FVec2 v);

// Returns a 4-connected line on a grid where each square is 64x64.
// Does not return positions outside of (0, 0) x (size.x_ - 1, size.y_ - 1).
// The grid is at most 2^15 wide.
std::vector<GridPosition> Walk(dux::FVec2 start, dux::FVec2 end, GridSize size);

}  // namespace dux

#endif  // DUX_FILED_SRC_GRID_WALKING_H_
