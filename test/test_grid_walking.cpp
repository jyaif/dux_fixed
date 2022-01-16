#include "test_grid_walking.h"

#include <algorithm>
#include <cmath>

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

    int x = floor(float_x / kGridSize);
    int y = floor(float_y / kGridSize);
    GridPosition p{x, y};
    if (x < 0 || y < 0) {
      continue;
    }
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
  WalkVerificationArgs FlipXAndY() const {
    WalkVerificationArgs args = *this;
    std::swap(args.start_.x_, args.start_.y_);
    std::swap(args.end_.x_, args.end_.y_);
    return args;
  }
  WalkVerificationArgs Translate() const {
    const int dx = 5;
    const int dy = 3;
    WalkVerificationArgs args = *this;
    dux::FVec2 offset(dx * kGridSize, dy * kGridSize);
    args.start_ += offset;
    args.end_ += offset;
    return args;
  }
  WalkVerificationArgs SwapStartAndEnding() const {
    const int dx = 5;
    const int dy = 3;
    WalkVerificationArgs args = *this;
    std::swap(args.start_, args.end_);
    return args;
  }

  WalkVerificationArgs MultiplyByMinusOne() const {
    WalkVerificationArgs args = *this;
    args.start_.x_ *= -1_fx;
    args.start_.y_ *= -1_fx;
    args.end_.x_ *= -1_fx;
    args.end_.y_ *= -1_fx;
    return args;
  }

  WalkVerificationArgs FlipVertically() const {
    WalkVerificationArgs args = *this;
    args.start_.y_ *= -1_fx;
    args.end_.y_ *= -1_fx;
    return args;
  }
};

void VerifyWalk(WalkVerificationArgs args) {
  auto result = Walk(args.start_, args.end_, {9999, 9999});

  double steps = 100;
  std::vector<GridPosition> computed_expected_results;
  do {
    computed_expected_results =
        CorrectAndSlowWalk(args.start_, args.end_, steps);
    steps *= 2;
  } while (steps < 100000 && computed_expected_results.size() != result.size());
  AssertVecEqual(result, computed_expected_results);
}

void VerifyWalkAllDirections(dux::FVec2 start, std::pair<int, int> end) {
  WalkVerificationArgs args;
  args.start_ = start;
  args.end_ = dux::FVec2(end.first, end.second);
  for (int i = 0; i < 3; i++) {
    VerifyWalk(args);
    VerifyWalk(args.FlipXAndY());
    VerifyWalk(args.Translate());
    VerifyWalk(args.FlipXAndY().Translate());
    VerifyWalk(args.Translate().FlipXAndY());

    VerifyWalk(args.MultiplyByMinusOne());
    VerifyWalk(args.FlipXAndY().MultiplyByMinusOne());
    VerifyWalk(args.Translate().MultiplyByMinusOne());
    VerifyWalk(args.FlipXAndY().Translate().MultiplyByMinusOne());
    VerifyWalk(args.Translate().FlipXAndY().MultiplyByMinusOne());

    if (i == 0) {
      args = args.SwapStartAndEnding();
    }
    if (i == 1) {
      args = args.FlipVertically();
    }
  }
}

}  // namespace

void TestGridWalking() {
  // resolution
  int r = 64;

  // Single square
  VerifyWalkAllDirections({0, 0}, {0, 0});
  VerifyWalkAllDirections({r - 1, 0}, {r - 1, 0});
  VerifyWalkAllDirections({r, 0}, {r, 0});
  VerifyWalkAllDirections({10 * r, 5 * r}, {10 * r, 5 * r});
  VerifyWalkAllDirections({10 * r - 1, 5 * r - 1}, {10 * r - 1, 5 * r - 1});

  // Axis-aligned lines
  VerifyWalkAllDirections({0, 0}, {r, 0});
  VerifyWalkAllDirections({0, 0}, {2 * r, 0});
  VerifyWalkAllDirections({0, 0}, {2 * r - 1, 0});
  VerifyWalkAllDirections({1, 1}, {3 * r, r - 1});
  VerifyWalkAllDirections({10 * r, r - 1}, {12 * r, 1});
  VerifyWalkAllDirections({10 * r, 5 * r - 1}, {12 * r, 4 * r + 1});

  dux::FInt offset = 10_fx;

  for (int x = 1; x < 20; x++) {
    for (int y = 1; y < 20; y++) {
      if (x != y) {
        dux::FVec2 start = RandFVec2(0_fx, 64_fx, 0_fx, 64_fx);
        VerifyWalkAllDirections(start, {x * r + 20, y * r + 31});
      }
    }
  }
}
