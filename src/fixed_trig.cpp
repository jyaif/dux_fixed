#include "fixed_trig.h"

#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

namespace {

std::array<int16_t, 512> kCosTable = {
    4096,  4095,  4094,  4093,  4091,  4088,  4084,  4080,  4076,  4071,  4065,
    4058,  4051,  4043,  4035,  4026,  4017,  4007,  3996,  3985,  3973,  3960,
    3947,  3933,  3919,  3904,  3889,  3873,  3856,  3839,  3821,  3803,  3784,
    3764,  3744,  3723,  3702,  3680,  3658,  3635,  3612,  3588,  3563,  3538,
    3513,  3487,  3460,  3433,  3405,  3377,  3348,  3319,  3289,  3259,  3229,
    3197,  3166,  3134,  3101,  3068,  3034,  3000,  2966,  2931,  2896,  2860,
    2824,  2787,  2750,  2713,  2675,  2637,  2598,  2559,  2519,  2480,  2439,
    2399,  2358,  2317,  2275,  2233,  2191,  2148,  2105,  2062,  2018,  1975,
    1930,  1886,  1841,  1796,  1751,  1705,  1659,  1613,  1567,  1520,  1474,
    1427,  1379,  1332,  1284,  1237,  1189,  1140,  1092,  1043,  995,   946,
    897,   848,   799,   749,   700,   650,   601,   551,   501,   451,   401,
    351,   301,   251,   200,   150,   100,   50,    0,     -50,   -100,  -150,
    -200,  -251,  -301,  -351,  -401,  -451,  -501,  -551,  -601,  -650,  -700,
    -749,  -799,  -848,  -897,  -946,  -995,  -1043, -1092, -1140, -1189, -1237,
    -1284, -1332, -1379, -1427, -1474, -1520, -1567, -1613, -1659, -1705, -1751,
    -1796, -1841, -1886, -1930, -1975, -2018, -2062, -2105, -2148, -2191, -2233,
    -2275, -2317, -2358, -2399, -2439, -2480, -2519, -2559, -2598, -2637, -2675,
    -2713, -2750, -2787, -2824, -2860, -2896, -2931, -2966, -3000, -3034, -3068,
    -3101, -3134, -3166, -3197, -3229, -3259, -3289, -3319, -3348, -3377, -3405,
    -3433, -3460, -3487, -3513, -3538, -3563, -3588, -3612, -3635, -3658, -3680,
    -3702, -3723, -3744, -3764, -3784, -3803, -3821, -3839, -3856, -3873, -3889,
    -3904, -3919, -3933, -3947, -3960, -3973, -3985, -3996, -4007, -4017, -4026,
    -4035, -4043, -4051, -4058, -4065, -4071, -4076, -4080, -4084, -4088, -4091,
    -4093, -4094, -4095, -4095, -4095, -4094, -4093, -4091, -4088, -4084, -4080,
    -4076, -4071, -4065, -4058, -4051, -4043, -4035, -4026, -4017, -4007, -3996,
    -3985, -3973, -3960, -3947, -3933, -3919, -3904, -3889, -3873, -3856, -3839,
    -3821, -3803, -3784, -3764, -3744, -3723, -3702, -3680, -3658, -3635, -3612,
    -3588, -3563, -3538, -3513, -3487, -3460, -3433, -3405, -3377, -3348, -3319,
    -3289, -3259, -3229, -3197, -3166, -3134, -3101, -3068, -3034, -3000, -2966,
    -2931, -2896, -2860, -2824, -2787, -2750, -2713, -2675, -2637, -2598, -2559,
    -2519, -2480, -2439, -2399, -2358, -2317, -2275, -2233, -2191, -2148, -2105,
    -2062, -2018, -1975, -1930, -1886, -1841, -1796, -1751, -1705, -1659, -1613,
    -1567, -1520, -1474, -1427, -1379, -1332, -1284, -1237, -1189, -1140, -1092,
    -1043, -995,  -946,  -897,  -848,  -799,  -749,  -700,  -650,  -601,  -551,
    -501,  -451,  -401,  -351,  -301,  -251,  -200,  -150,  -100,  -50,   0,
    50,    100,   150,   200,   251,   301,   351,   401,   451,   501,   551,
    601,   650,   700,   749,   799,   848,   897,   946,   995,   1043,  1092,
    1140,  1189,  1237,  1284,  1332,  1379,  1427,  1474,  1520,  1567,  1613,
    1659,  1705,  1751,  1796,  1841,  1886,  1930,  1975,  2018,  2062,  2105,
    2148,  2191,  2233,  2275,  2317,  2358,  2399,  2439,  2480,  2519,  2559,
    2598,  2637,  2675,  2713,  2750,  2787,  2824,  2860,  2896,  2931,  2966,
    3000,  3034,  3068,  3101,  3134,  3166,  3197,  3229,  3259,  3289,  3319,
    3348,  3377,  3405,  3433,  3460,  3487,  3513,  3538,  3563,  3588,  3612,
    3635,  3658,  3680,  3702,  3723,  3744,  3764,  3784,  3803,  3821,  3839,
    3856,  3873,  3889,  3904,  3919,  3933,  3947,  3960,  3973,  3985,  3996,
    4007,  4017,  4026,  4035,  4043,  4051,  4058,  4065,  4071,  4076,  4080,
    4084,  4088,  4091,  4093,  4094,  4095,
};

std::array<int16_t, 512> kSinTable = {
    0,     50,    100,   150,   200,   251,   301,   351,   401,   451,   501,
    551,   601,   650,   700,   749,   799,   848,   897,   946,   995,   1043,
    1092,  1140,  1189,  1237,  1284,  1332,  1379,  1427,  1474,  1520,  1567,
    1613,  1659,  1705,  1751,  1796,  1841,  1886,  1930,  1975,  2018,  2062,
    2105,  2148,  2191,  2233,  2275,  2317,  2358,  2399,  2439,  2480,  2519,
    2559,  2598,  2637,  2675,  2713,  2750,  2787,  2824,  2860,  2896,  2931,
    2966,  3000,  3034,  3068,  3101,  3134,  3166,  3197,  3229,  3259,  3289,
    3319,  3348,  3377,  3405,  3433,  3460,  3487,  3513,  3538,  3563,  3588,
    3612,  3635,  3658,  3680,  3702,  3723,  3744,  3764,  3784,  3803,  3821,
    3839,  3856,  3873,  3889,  3904,  3919,  3933,  3947,  3960,  3973,  3985,
    3996,  4007,  4017,  4026,  4035,  4043,  4051,  4058,  4065,  4071,  4076,
    4080,  4084,  4088,  4091,  4093,  4094,  4095,  4095,  4095,  4094,  4093,
    4091,  4088,  4084,  4080,  4076,  4071,  4065,  4058,  4051,  4043,  4035,
    4026,  4017,  4007,  3996,  3985,  3973,  3960,  3947,  3933,  3919,  3904,
    3889,  3873,  3856,  3839,  3821,  3803,  3784,  3764,  3744,  3723,  3702,
    3680,  3658,  3635,  3612,  3588,  3563,  3538,  3513,  3487,  3460,  3433,
    3405,  3377,  3348,  3319,  3289,  3259,  3229,  3197,  3166,  3134,  3101,
    3068,  3034,  3000,  2966,  2931,  2896,  2860,  2824,  2787,  2750,  2713,
    2675,  2637,  2598,  2559,  2519,  2480,  2439,  2399,  2358,  2317,  2275,
    2233,  2191,  2148,  2105,  2062,  2018,  1975,  1930,  1886,  1841,  1796,
    1751,  1705,  1659,  1613,  1567,  1520,  1474,  1427,  1379,  1332,  1284,
    1237,  1189,  1140,  1092,  1043,  995,   946,   897,   848,   799,   749,
    700,   650,   601,   551,   501,   451,   401,   351,   301,   251,   200,
    150,   100,   50,    0,     -50,   -100,  -150,  -200,  -251,  -301,  -351,
    -401,  -451,  -501,  -551,  -601,  -650,  -700,  -749,  -799,  -848,  -897,
    -946,  -995,  -1043, -1092, -1140, -1189, -1237, -1284, -1332, -1379, -1427,
    -1474, -1520, -1567, -1613, -1659, -1705, -1751, -1796, -1841, -1886, -1930,
    -1975, -2018, -2062, -2105, -2148, -2191, -2233, -2275, -2317, -2358, -2399,
    -2439, -2480, -2519, -2559, -2598, -2637, -2675, -2713, -2750, -2787, -2824,
    -2860, -2896, -2931, -2966, -3000, -3034, -3068, -3101, -3134, -3166, -3197,
    -3229, -3259, -3289, -3319, -3348, -3377, -3405, -3433, -3460, -3487, -3513,
    -3538, -3563, -3588, -3612, -3635, -3658, -3680, -3702, -3723, -3744, -3764,
    -3784, -3803, -3821, -3839, -3856, -3873, -3889, -3904, -3919, -3933, -3947,
    -3960, -3973, -3985, -3996, -4007, -4017, -4026, -4035, -4043, -4051, -4058,
    -4065, -4071, -4076, -4080, -4084, -4088, -4091, -4093, -4094, -4095, -4095,
    -4095, -4094, -4093, -4091, -4088, -4084, -4080, -4076, -4071, -4065, -4058,
    -4051, -4043, -4035, -4026, -4017, -4007, -3996, -3985, -3973, -3960, -3947,
    -3933, -3919, -3904, -3889, -3873, -3856, -3839, -3821, -3803, -3784, -3764,
    -3744, -3723, -3702, -3680, -3658, -3635, -3612, -3588, -3563, -3538, -3513,
    -3487, -3460, -3433, -3405, -3377, -3348, -3319, -3289, -3259, -3229, -3197,
    -3166, -3134, -3101, -3068, -3034, -3000, -2966, -2931, -2896, -2860, -2824,
    -2787, -2750, -2713, -2675, -2637, -2598, -2559, -2519, -2480, -2439, -2399,
    -2358, -2317, -2275, -2233, -2191, -2148, -2105, -2062, -2018, -1975, -1930,
    -1886, -1841, -1796, -1751, -1705, -1659, -1613, -1567, -1520, -1474, -1427,
    -1379, -1332, -1284, -1237, -1189, -1140, -1092, -1043, -995,  -946,  -897,
    -848,  -799,  -749,  -700,  -650,  -601,  -551,  -501,  -451,  -401,  -351,
    -301,  -251,  -200,  -150,  -100,  -50,
};

std::array<int32_t, 512> kTanTable = {
    0,      12,     25,     37,     50,     62,     75,     87,      100,
    113,    125,    138,    150,    163,    176,    188,    201,     213,
    226,    239,    251,    264,    276,    289,    302,    314,     327,
    340,    352,    365,    378,    390,    403,    416,    428,     441,
    454,    466,    479,    492,    505,    517,    530,    543,     556,
    569,    581,    594,    607,    620,    633,    646,    659,     671,
    684,    697,    710,    723,    736,    749,    762,    775,     788,
    801,    814,    827,    840,    854,    867,    880,    893,     906,
    919,    932,    946,    959,    972,    986,    999,    1012,    1025,
    1039,   1052,   1066,   1079,   1093,   1106,   1119,   1133,    1147,
    1160,   1174,   1187,   1201,   1215,   1228,   1242,   1256,    1270,
    1283,   1297,   1311,   1325,   1339,   1353,   1367,   1381,    1395,
    1409,   1423,   1437,   1451,   1465,   1479,   1493,   1508,    1522,
    1536,   1551,   1565,   1580,   1594,   1608,   1623,   1638,    1652,
    1667,   1681,   1696,   1711,   1726,   1740,   1755,   1770,    1785,
    1800,   1815,   1830,   1845,   1860,   1876,   1891,   1906,    1921,
    1937,   1952,   1968,   1983,   1999,   2014,   2030,   2046,    2061,
    2077,   2093,   2109,   2125,   2141,   2157,   2173,   2189,    2205,
    2221,   2238,   2254,   2270,   2287,   2303,   2320,   2336,    2353,
    2370,   2387,   2404,   2421,   2437,   2455,   2472,   2489,    2506,
    2523,   2541,   2558,   2576,   2593,   2611,   2629,   2646,    2664,
    2682,   2700,   2718,   2736,   2755,   2773,   2791,   2810,    2828,
    2847,   2865,   2884,   2903,   2922,   2941,   2960,   2979,    2999,
    3018,   3037,   3057,   3076,   3096,   3116,   3136,   3156,    3176,
    3196,   3216,   3237,   3257,   3278,   3298,   3319,   3340,    3361,
    3382,   3403,   3425,   3446,   3467,   3489,   3511,   3533,    3555,
    3577,   3599,   3621,   3644,   3666,   3689,   3712,   3735,    3758,
    3781,   3804,   3828,   3852,   3875,   3899,   3923,   3947,    3972,
    3996,   4021,   4046,   4070,   4096,   4121,   4146,   4172,    4197,
    4223,   4249,   4275,   4302,   4328,   4355,   4382,   4409,    4436,
    4463,   4491,   4519,   4547,   4575,   4603,   4632,   4660,    4689,
    4719,   4748,   4777,   4807,   4837,   4867,   4898,   4929,    4959,
    4990,   5022,   5053,   5085,   5117,   5150,   5182,   5215,    5248,
    5281,   5315,   5349,   5383,   5417,   5452,   5487,   5522,    5558,
    5594,   5630,   5666,   5703,   5740,   5778,   5815,   5853,    5892,
    5931,   5970,   6009,   6049,   6089,   6130,   6170,   6212,    6253,
    6296,   6338,   6381,   6424,   6468,   6512,   6556,   6601,    6647,
    6693,   6739,   6786,   6833,   6881,   6929,   6978,   7027,    7077,
    7128,   7179,   7230,   7282,   7335,   7388,   7441,   7496,    7551,
    7606,   7663,   7719,   7777,   7835,   7894,   7954,   8014,    8075,
    8137,   8199,   8263,   8327,   8392,   8457,   8524,   8591,    8660,
    8729,   8799,   8870,   8942,   9015,   9089,   9164,   9240,    9317,
    9395,   9474,   9555,   9636,   9719,   9803,   9888,   9975,    10062,
    10151,  10242,  10334,  10427,  10522,  10618,  10716,  10815,   10916,
    11019,  11123,  11229,  11337,  11447,  11559,  11672,  11788,   11906,
    12026,  12147,  12272,  12398,  12527,  12659,  12792,  12929,   13068,
    13210,  13355,  13502,  13653,  13807,  13964,  14124,  14288,   14455,
    14626,  14801,  14979,  15162,  15349,  15540,  15736,  15936,   16141,
    16352,  16567,  16788,  17015,  17247,  17485,  17730,  17982,   18240,
    18505,  18778,  19059,  19347,  19645,  19951,  20266,  20591,   20927,
    21273,  21630,  21999,  22380,  22775,  23183,  23605,  24043,   24497,
    24967,  25456,  25964,  26491,  27041,  27612,  28208,  28830,   29479,
    30158,  30868,  31611,  32391,  33209,  34069,  34974,  35928,   36934,
    37998,  39124,  40318,  41587,  42937,  44377,  45916,  47564,   49334,
    51240,  53298,  55528,  57950,  60593,  63487,  66670,  70188,   74096,
    78463,  83376,  88943,  95304,  102644, 111207, 121325, 133466,  148305,
    166852, 190697, 222489, 266996, 333755, 445017, 667538, 1335099,
};

int32_t SearchValueInTanTable(int32_t value) {
  int32_t lowerBound = 0;
  int32_t higherBound = kTanTable.size() - 1;
  assert(value >= 0);
  if (value <= kTanTable[lowerBound])
    return lowerBound;
  if (value >= kTanTable[higherBound])
    return higherBound;
  while (higherBound - lowerBound > 1) {
    int32_t index = (higherBound + lowerBound) / 2;
    int32_t valueInTheCenter = kTanTable[index];
    if (valueInTheCenter > value) {
      higherBound = index;
    } else {
      if (valueInTheCenter < value) {
        lowerBound = index;
      } else {
        return index;
      }
    }
  }
  return lowerBound;
}

}  // namespace

namespace dux {

namespace trig {

FInt Cos(FInt angle) {
  int32_t index = ((angle * FInt(512)) / FInt::kTwoPi).Int32();
  index %= 512;
  if (angle.raw_value_ >= 0) {
    return FInt::FromRawValue(kCosTable[index]);
  } else {
    return FInt::FromRawValue(kCosTable[-index]);
  }
}

FInt Sin(FInt angle) {
  int32_t index = ((angle * FInt(512)) / FInt::kTwoPi).Int32();
  index %= 512;
  if (angle.raw_value_ >= 0) {
    return FInt::FromRawValue(kSinTable[index]);
  } else {
    return FInt::FromRawValue(-kSinTable[-index]);
  }
}

FInt Atan2(FInt y, FInt x) {
  if (x.raw_value_ == 0) {
    if (y.raw_value_ > 0) {
      return FInt::kHalfPi;
    } else {
      return -FInt::kHalfPi;
    }
  }
  int32_t d = static_cast<uint32_t>((y / x).raw_value_);
  d = std::abs(d);
  FInt angle = FInt::kHalfPi * FInt(SearchValueInTanTable(d));
  angle >>= 9;
  if (y.raw_value_ > 0) {
    if (x.raw_value_ > 0) {
      return angle;
    } else {
      return FInt::kPi - angle;
    }
  } else {
    if (x.raw_value_ > 0) {
      return -angle;
    } else {
      return -FInt::kPi + angle;
    }
  }
  return FInt::FromRawValue(0);
}

FInt ToDegree(FInt angle) {
  return FInt(0);
  //  if (angle.raw_value_ >= (360 << FInt::kShift))
  //    angle.raw_value_ %= (360 << FInt::kShift);
  //  if (angle.raw_value_ <= -(360 << FInt::kShift))
  //    angle.raw_value_ = - (-angle.raw_value_ % (360 << FInt::kShift));
  //  // Precision of conversion can be improved.
  //  // 57 = 1 / ((Pi * 2) / 360)
  //  return FInt::FromRawValue(angle.raw_value_ / 57);
}

void GenerateLookupTables() {
  std::vector<FInt> cosTable;
  std::vector<FInt> sinTable;
  std::vector<FInt> tanTable;
  for (int i = 0; i < 512; i++) {
    float angle = i * (2 * M_PI / 512);
    cosTable.push_back(FInt(std::cos(angle)));
    sinTable.push_back(FInt(std::sin(angle)));
    tanTable.push_back(FInt(std::tan(angle / 4)));
  }
  printf("std::array<int16_t, 512> kCosTable = {\n");
  for (FInt& v : cosTable) {
    printf("%lli,\n", v.raw_value_);
  }
  printf("};\n");

  printf("std::array<int16_t, 512> kSinTable = {\n");
  for (FInt& v : sinTable) {
    printf("%lli,\n", v.raw_value_);
  }
  printf("};\n");

  printf("std::array<int32_t, 512> kTanTable = {\n");
  for (FInt& v : tanTable) {
    printf("%lli,\n", v.raw_value_);
  }
  printf("};\n");
}

}  // namespace trig

}  // namespace dux
