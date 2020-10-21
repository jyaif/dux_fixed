#include "grid_walking.h"

namespace {

constexpr int kGridShift = 6;

dux::GridPosition GridPositionFromFVec2(dux::FVec2 v) {
  dux::GridPosition p;
  p.x_ = static_cast<int32_t>(
      v.x_.raw_value_ >> (kGridShift + dux::FInt::kShift));
  p.y_ = static_cast<int32_t>(
      v.y_.raw_value_ >> (kGridShift + dux::FInt::kShift));
  return p;
}

std::vector<dux::GridPosition> AxisAlignedWalk(dux::GridPosition start, dux::GridPosition end) {
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

std::vector<GridPosition> Walk(dux::FVec2 start, dux::FVec2 end) {
  dux::GridPosition grid_start = GridPositionFromFVec2(start);
  dux::GridPosition grid_end = GridPositionFromFVec2(end);
  if (grid_start.x_ == grid_end.x_ || grid_start.y_ == grid_end.y_) {
    return AxisAlignedWalk(grid_start, grid_end);
  }
    
  std::vector<GridPosition> v;

  bool swap = false;
    
  if (end < start) {
    swap = true;
    std::swap(start, end);
    std::swap(grid_start, grid_end);
  }
  
  int32_t iterations = abs(grid_end.x_ - grid_start.x_) + abs(grid_end.y_ - grid_start.y_);
  dux::FVec2 delta = end - start;

  if (start < end) {
    constexpr dux::FInt edge = dux::FInt::FromInt(64);
//    constexpr dux::FInt center = (edge  - dux::FInt::FromRawValue(1)) * dux::FInt::FromFraction(1, 2);
    constexpr dux::FInt center = (edge * dux::FInt::FromFraction(1, 2)) - dux::FInt::FromRawValue(1);

//    dux::FInt x_error = center - (start.x_ % edge);
//    dux::FInt y_error = center - (start.y_ % edge);
//    dux::FInt error = (y_error * delta.y_ - x_error * delta.x_);
//    error *= dux::FInt::FromFraction(1, 64);

    dux::FInt x_error = (start.x_ % edge) - center;
//    dux::FInt y_error = (start.y_ % edge) - center;
//    dux::FInt error = y_error * delta.x_;
    dux::FInt error = -x_error * delta.y_;
    error *= dux::FInt::FromFraction(1, 64);

 
//    delta.y_ *= dux::FInt::FromInt(64);
//    delta.x_ *= dux::FInt::FromInt(64);

    printf("error:%f\n", error.DoubleValue());
    printf("➡️:%f ⬆️:%f\n", delta.y_.DoubleValue(), -delta.x_.DoubleValue());
    printf("\nstart of the loop:\n\n");
    for (int32_t i = 0; i < iterations; i++) {
      printf("%i %i\n", grid_start.x_, grid_start.y_);
      v.push_back(grid_start);
      dux::FInt error1 = error + delta.y_;// * dux::FInt::FromInt(64);
      dux::FInt error2 = error - delta.x_;// * dux::FInt::FromInt(64);
      // Keep the error that is the closest to 0.
      printf("error:%f\n", error.DoubleValue());
      printf("new errors:   ➡️%f    ⬆️%f\n", error1.DoubleValue(), error2.DoubleValue());
      if (error1.Abs() < error2.Abs()) {
        grid_start.x_++;
        printf("➡️\n");
        error = error1;
      } else {
        grid_start.y_++;
        printf("⬆️\n");
        error = error2;
      }
    }
  } else {
    assert(false);
  }
  v.push_back(grid_end);

  if (swap) {
    std::reverse(v.begin(), v.end());
  }

  return v;

}

}  // namespace dux
