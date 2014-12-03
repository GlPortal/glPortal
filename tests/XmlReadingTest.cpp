#include <unittest++/UnitTest++.h>
#include <tinyxml.h>
#include <engine/core/math/Vector3f.hpp>
#include <assets/map/XmlHelper.hpp>
#include <stdexcept>

using namespace glPortal;
using namespace std;

struct XmlReadingFixtures
{
    Vector3f sourceVector;
    Vector3f vector;
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * lightElement = new TiXmlElement( "light" );

  XmlReadingFixtures() {
    sourceVector = Vector3f(155, 266, 377);
    lightElement->SetAttribute("x", sourceVector.x);
    lightElement->SetAttribute("y", sourceVector.y);
    lightElement->SetAttribute("z", sourceVector.z);
    doc.LinkEndChild(decl);
    doc.LinkEndChild(lightElement);
  }
  
  ~XmlReadingFixtures() {}
  
};
 

SUITE(XmlReading)
{
  TEST_FIXTURE(XmlReadingFixtures, extractValidVectorDataFromXml){
    XmlHelper::pushAttributeVertexToVector(lightElement, vector);

    bool vectorIsValid(false);
    Vector3f resultVector = *sourceVector.sub(vector);
    if((resultVector.x + resultVector.y + resultVector.z) == 0 ){
      vectorIsValid = true;
    }
    CHECK(vectorIsValid);
  }

  TEST_FIXTURE(XmlReadingFixtures, extractMissingAttributeFromXml){
    lightElement->RemoveAttribute("z");
    CHECK_THROW(XmlHelper::pushAttributeVertexToVector(lightElement, vector), runtime_error); 
  }
}

int main()
{
  return UnitTest::RunAllTests();
}
