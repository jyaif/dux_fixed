#include "test_grid_walking.h"

#include <algorithm>

#include "grid_walking.h"
#include "utils.h"

using namespace dux;
using namespace dux_test_utils;

namespace {

constexpr int kGridSize = 64;

std::vector<GridPosition> CorrectAndSlowWalk(dux::FVec2 start,
                                             dux::FVec2 end,
                                             double steps) {
  double x0 = start.x_.DoubleValue();
  double y0 = start.y_.DoubleValue();
  double x1 = end.x_.DoubleValue();
  double y1 = end.y_.DoubleValue();

  double dx = x1 - x0;
  double dy = y1 - y0;

  std::vector<GridPosition> v;
  for (int i = 0; i <= steps; i++) {
    double float_x = (x0 * steps + (dx * i)) / steps;
    double float_y = (y0 * steps + (dy * i)) / steps;

    int x = float_x;
    x /= kGridSize;
    int y = float_y;
    y /= kGridSize;
    GridPosition p{x, y};
    if (v.empty()) {
      v.push_back(p);
    } else {
      if (p != v.back()) {
        v.push_back(p);
      }
    }
  }
  return v;
}

void AssertVecEqual(std::vector<GridPosition> a, std::vector<GridPosition> b) {
  assert(a.size() == b.size());
  for (size_t i = 0; i < a.size(); i++) {
    assert(a[i] == b[i]);
  }
}

struct WalkVerificationArgs {
  dux::FVec2 start_;
  dux::FVec2 end_;
  std::vector<GridPosition> expected_result_;
  WalkVerificationArgs FlipXAndY() const {
    WalkVerificationArgs args = *this;
    std::swap(args.start_.x_, args.start_.y_);
    std::swap(args.end_.x_, args.end_.y_);
    for (int i = 0; i < expected_result_.size(); i++) {
      std::swap(args.expected_result_[i].x_, args.expected_result_[i].y_);
    }
    return args;
  }
  WalkVerificationArgs Translate() const {
    const int dx = 5;
    const int dy = 3;
    WalkVerificationArgs args = *this;
    dux::FVec2 offset(dx * kGridSize, dy * kGridSize);
    args.start_ += offset;
    args.end_ += offset;
    for (int i = 0; i < expected_result_.size(); i++) {
      args.expected_result_[i].x_ += dx;
      args.expected_result_[i].y_ += dy;
    }
    return args;
  }
  WalkVerificationArgs SwapStartAndEnding() const {
    const int dx = 5;
    const int dy = 3;
    WalkVerificationArgs args = *this;
    std::swap(args.start_, args.end_);
    std::reverse(args.expected_result_.begin(), args.expected_result_.end());
    return args;
  }
};

void VerifyWalk(WalkVerificationArgs args) {
  auto result = Walk(args.start_, args.end_);

  if (!args.expected_result_.empty()) {
    AssertVecEqual(result, args.expected_result_);
  }

  double steps = 100;
  std::vector<GridPosition> computed_expected_results;
  do {
    computed_expected_results =
        CorrectAndSlowWalk(args.start_, args.end_, steps);
    steps *= 2;
  } while (steps < 100000 && computed_expected_results.size() != result.size());

  AssertVecEqual(result, computed_expected_results);
}

void VerifyWalkAllDirections(dux::FVec2 start,
                             std::pair<int, int> end,
                             std::vector<GridPosition> expected_result) {
  WalkVerificationArgs args;
  args.start_ = start;
  args.end_ = dux::FVec2(end.first, end.second);
  args.expected_result_ = expected_result;

  for (int i = 0; i < 2; i++) {
    auto args2 = args.FlipXAndY();
    auto args3 = args.Translate();
    auto args4 = args.FlipXAndY().Translate();
    auto args5 = args.Translate().FlipXAndY();
    VerifyWalk(args);
    VerifyWalk(args2);
    VerifyWalk(args3);
    VerifyWalk(args4);
    VerifyWalk(args5);
    args = args.SwapStartAndEnding();
  }
}

}  // namespace

void TestGridWalking() {
  // resolution
  int r = 64;

  // Single square
  VerifyWalkAllDirections({0, 0}, {0, 0}, {{0, 0}});
  VerifyWalkAllDirections({r - 1, 0}, {r - 1, 0}, {{0, 0}});
  VerifyWalkAllDirections({r, 0}, {r, 0}, {{1, 0}});
  VerifyWalkAllDirections({10 * r, 5 * r}, {10 * r, 5 * r}, {{10, 5}});
  VerifyWalkAllDirections({10 * r - 1, 5 * r - 1}, {10 * r - 1, 5 * r - 1},
                          {{9, 4}});

  // Axis-aligned lines
  VerifyWalkAllDirections({0, 0}, {r, 0}, {{0, 0}, {1, 0}});
  VerifyWalkAllDirections({0, 0}, {2 * r, 0}, {{0, 0}, {1, 0}, {2, 0}});
  VerifyWalkAllDirections({0, 0}, {2 * r - 1, 0}, {{0, 0}, {1, 0}});
  VerifyWalkAllDirections({0, 0}, {r, r - 1}, {{0, 0}, {1, 0}});
  VerifyWalkAllDirections({10 * r, r - 1}, {12 * r, 0},
                          {{10, 0}, {11, 0}, {12, 0}});
  VerifyWalkAllDirections({10 * r, 5 * r - 1}, {12 * r, 4 * r},
                          {{10, 4}, {11, 4}, {12, 4}});

  dux::FRandGenerator rng;
  dux::FInt offset = dux::FInt::FromInt(10);

  for (int x = 1; x < 20; x++) {
    for (int y = 1; y < 20; y++) {
      if (x != y) {
        dux::FVec2 start =
            rng.RandFVec2(dux::FInt::FromInt(0), dux::FInt::FromInt(64),
                          dux::FInt::FromInt(0), dux::FInt::FromInt(64));
        VerifyWalkAllDirections(start, {x * r + 20, y * r + 31}, {});
      }
    }
  }
}
