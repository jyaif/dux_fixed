#include "grid_walking.h"

#include <algorithm>

namespace {

constexpr int kGridShift = 6;

std::vector<dux::GridPosition> AxisAlignedWalk(dux::GridPosition start,
                                               dux::GridPosition end) {
  std::vector<dux::GridPosition> positions;
  if (start.x_ != end.x_) {
    int32_t dx = end.x_ > start.x_ ? 1 : -1;
    positions.push_back(start);
    while (start != end) {
      start.x_ += dx;
      positions.push_back(start);
    }
  } else {
    int32_t dy = end.y_ > start.y_ ? 1 : -1;
    positions.push_back(start);
    while (start != end) {
      start.y_ += dy;
      positions.push_back(start);
    }
  }
  return positions;
}

}  // namespace

namespace dux {

dux::GridPosition GridPositionFromFVec2(dux::FVec2 v) {
  dux::GridPosition p;
  p.x_ =
      static_cast<int32_t>(v.x_.raw_value_ >> (kGridShift + dux::FInt::kShift));
  p.y_ =
      static_cast<int32_t>(v.y_.raw_value_ >> (kGridShift + dux::FInt::kShift));
  return p;
}

std::vector<GridPosition> Walk(dux::FVec2 start, dux::FVec2 end) {
  dux::GridPosition grid_start = GridPositionFromFVec2(start);
  dux::GridPosition grid_end = GridPositionFromFVec2(end);
  if (grid_start.x_ == grid_end.x_ || grid_start.y_ == grid_end.y_) {
    return AxisAlignedWalk(grid_start, grid_end);
  }

  bool swap = false;

  if (end < start || (start.x_ > end.x_ && start.y_ < end.y_)) {
    swap = true;
    std::swap(start, end);
    std::swap(grid_start, grid_end);
  }

  std::vector<GridPosition> v;

  int32_t iterations =
      abs(grid_end.x_ - grid_start.x_) + abs(grid_end.y_ - grid_start.y_);
  dux::FVec2 delta = end - start;

  if (start < end) {
    // From bottom-left to top-right
    dux::FInt Δx = dux::FInt::FromInt(64) -
                   start.x_.EuclideanDivisionRemainder(dux::FInt::FromInt(64));
    dux::FInt Δy = dux::FInt::FromInt(64) -
                   start.y_.EuclideanDivisionRemainder(dux::FInt::FromInt(64));
    dux::FInt error = delta.x_ * Δy - delta.y_ * Δx;
    delta *= dux::FInt::FromInt(64);
    for (int i = 0; i < iterations; i++) {
      v.push_back(grid_start);
      if (error < dux::FInt::FromInt(0)) {
        error = error + delta.x_;
        grid_start.y_++;
      } else {
        error = error - delta.y_;
        grid_start.x_++;
      }
    }
    v.push_back(grid_end);
  } else {
    assert(start.x_ < end.x_ && start.y_ > end.y_);
    // From top-left to bottom-right
    dux::FInt Δx =
        dux::FInt::FromInt(64) -
        (start.x_.EuclideanDivisionRemainder(dux::FInt::FromInt(64)));
    dux::FInt Δy = start.y_.EuclideanDivisionRemainder(dux::FInt::FromInt(64));
    dux::FInt error = delta.x_ * Δy + delta.y_ * Δx;
    delta *= dux::FInt::FromInt(64);
    for (int i = 0; i < iterations; i++) {
      v.push_back(grid_start);
      if (error < dux::FInt::FromInt(0)) {
        error = error + delta.x_;
        grid_start.y_--;
      } else {
        error = error + delta.y_;
        grid_start.x_++;
      }
    }
    v.push_back(grid_end);
  }

  if (swap) {
    std::reverse(v.begin(), v.end());
  }

  return v;
}

}  // namespace dux
