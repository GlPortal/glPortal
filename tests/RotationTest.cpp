#include <util/math/Math.hpp>
#include <util/math/Vector3f.hpp>
#include <util/math/Matrix4f.hpp>
#include <cstdio>

using namespace glPortal;

int main(int argc, char *argv[]) {
  Vector3f portalPos(4, 0, -2);
  Vector3f portalRot(0, -90, 0);
  Vector3f cameraPos(5, 0, -1);
  Vector3f cameraRot(-45, 90, 0);
  Vector3f oportalPos(-3, 0, 2);
  Vector3f oportalRot(0, 180, 0);

  //Set camera in other portal
  Vector3f camPos = Vector3f::sub(cameraPos, portalPos);
  Vector3f camDir = Math::toDirection(cameraRot);
  printf("The position relative to the portal:\n");
  camPos.print();

  //Invert the position and rotation to be behind the portal
  Vector3f icamPos = Vector3f::negate(camPos);
  Vector3f icamDir = Vector3f::negate(camDir);
  printf("The inverted position relative to the portal\n");
  icamPos.print();
  printf("The inverted direction relative to the portal\n");
  icamDir.print();

  //Calculate the position and rotation of the camera relative to the Z axis
  Matrix4f mAntiRot;
  mAntiRot.rotate(Vector3f::negate(portalRot));

  Vector3f nrcamPos = mAntiRot.transform(icamPos);
  Vector3f nrcamDir = mAntiRot.transform(icamDir);
  printf("The position relative to the Z axis:\n");
  nrcamPos.print();
  printf("The direction relative to the Z axis:\n");
  nrcamDir.print();

  //Calculate the position and rotation of the camera relative to the other portal
  Matrix4f mRot;
  mRot.rotate(oportalRot);

  Vector3f rcamPos = mRot.transform(nrcamPos);
  Vector3f rcamDir = mRot.transform(nrcamDir);
  printf("The position relative to the other portal:\n");
  rcamPos.print();
  printf("The direction relative to the other portal:\n");
  rcamDir.print();

  Vector3f rcamRot = Math::toEuler(rcamDir);
  printf("The rotation relative to the other portal:\n");
  rcamRot.print();

  return 0;
}
