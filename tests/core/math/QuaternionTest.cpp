#include <unittest++/UnitTest++.h>
#include <engine/core/math/Math.hpp>
#include <engine/core/math/Quaternion.hpp>

using namespace glPortal;
using namespace std;

struct QuaternionTestFixtures {
  Quaternion quat;

  QuaternionTestFixtures() {
  }
  
  ~QuaternionTestFixtures() {}
  
};

bool fuzzyEq(float f1, float f2) {
  return std::abs(f1-f2) <= 0.001f;
}

bool fuzzyEq(const Quaternion &q, float x, float y, float z, float w) {
  return fuzzyEq(q.x, x) and fuzzyEq(q.y, y) and fuzzyEq(q.z, z) and fuzzyEq(q.w, w);
}

bool fuzzyEq(const Quaternion &q, const Quaternion &k) {
  return fuzzyEq(q.x, k.x) and fuzzyEq(q.y, k.y) and fuzzyEq(q.z, k.z) and fuzzyEq(q.w, k.w);
}

void printq(const Quaternion &q) {
  printf("%f %f %f %f\n", q.x, q.y, q.z, q.w);
}

SUITE(QuaternionTest) {
  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
  TEST_FIXTURE(QuaternionTestFixtures, AxAngle_X90) {
    quat.fromAxAngle(1, 0, 0, rad(90));
    CHECK(fuzzyEq(quat, 0.7071, 0, 0, 0.7071));
  }
  TEST_FIXTURE(QuaternionTestFixtures, AxAngle_X45) {
    quat.fromAxAngle(1, 0, 0, rad(45));
    CHECK(fuzzyEq(quat, 0.3827, 0, 0, 0.9239));
  }
  TEST_FIXTURE(QuaternionTestFixtures, AxAngle_Y90) {
    quat.fromAxAngle(0, 1, 0, rad(90));
    CHECK(fuzzyEq(quat, 0, 0.7071, 0, 0.7071));
  }
  TEST_FIXTURE(QuaternionTestFixtures, AxAngle_Y45) {
    quat.fromAxAngle(0, 1, 0, rad(45));
    CHECK(fuzzyEq(quat, 0, 0.3827, 0, 0.9239));
  }
  TEST_FIXTURE(QuaternionTestFixtures, AxAngle_XY90) {
    quat.fromAxAngle(1, 1, 0, rad(90));
    CHECK(fuzzyEq(quat, 0.5, 0.5, 0, 0.7071));
  }
  TEST_FIXTURE(QuaternionTestFixtures, AxAngle_XY45) {
    quat.fromAxAngle(1, 1, 0, rad(45));
    CHECK(fuzzyEq(quat, 0.2706, 0.2706, 0, 0.9239));
  }

  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/steps/index.htm
  TEST_FIXTURE(QuaternionTestFixtures, Euler_X90) {
    quat.fromEuler(rad(90), 0, 0);
    CHECK(fuzzyEq(length(quat), 1));
    CHECK(fuzzyEq(quat, 0, 0.7071, 0, 0.7071));
  }
  TEST_FIXTURE(QuaternionTestFixtures, Euler_Y90) {
    quat.fromEuler(0, rad(90), 0);
    CHECK(fuzzyEq(length(quat), 1));
    CHECK(fuzzyEq(quat, 0, 0, 0.7071, 0.7071));
  }
  TEST_FIXTURE(QuaternionTestFixtures, Euler_XY90) {
    quat.fromEuler(rad(90), rad(90), 0);
    CHECK(fuzzyEq(length(quat), 1));
    CHECK(fuzzyEq(quat, 0.5, 0.5, 0.5, 0.5));
  }
}

int main() {
  return UnitTest::RunAllTests();
}
