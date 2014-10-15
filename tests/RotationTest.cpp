#include <util/math/Math.hpp>
#include <util/math/Vector3f.hpp>
#include <util/math/Matrix4f.hpp>

using namespace glPortal;

int main(int argc, char *argv[]) {
  Vector3f v(-45, -45, 0);

  Vector3f dir = Math::toDirection(v);
  dir.print();

  return 0;
}
