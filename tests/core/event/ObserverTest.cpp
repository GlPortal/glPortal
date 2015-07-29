#include <unittest++/UnitTest++.h>
#include <engine/core/event/Observer.hpp>
#include <stdexcept>

using namespace glPortal;
using namespace std;

struct ObserverFixtures
{
  Vector3f sourceVector;
  Vector3f vector;

  ObserverFixtures() {
    doc.NewDeclaration("1.0");
    lightElement = doc.NewElement("light"); 	
    sourceVector = Vector3f(155, 266, 377);
    lightElement->SetAttribute("x", sourceVector.x);
    lightElement->SetAttribute("y", sourceVector.y);
    lightElement->SetAttribute("z", sourceVector.z);
  }
  
  ~ObserverFixtures() {}
  
};
 

SUITE(XmlReading)
{
  TEST_FIXTURE(XmlReadingFixtures, extractValidVectorDataFromXml){
    XmlHelper::pushAttributeVertexToVector(lightElement, vector);

    bool vectorIsValid(false);
    Vector3f resultVector = sourceVector - vector;
    if((resultVector.x + resultVector.y + resultVector.z) == 0 ){
      vectorIsValid = true;
    }
    CHECK(vectorIsValid);
  }

  TEST_FIXTURE(XmlReadingFixtures, extractMissingAttributeFromXml){
    lightElement->DeleteAttribute("z");
    CHECK_THROW(XmlHelper::pushAttributeVertexToVector(lightElement, vector), runtime_error); 
  }
}

int main()
{
  return UnitTest::RunAllTests();
}
