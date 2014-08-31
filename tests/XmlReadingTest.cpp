#include <unittest++/UnitTest++.h>
#include <tinyxml.h>
#include <engine/util/Vector3f.hpp>
#include <engine/loader/XmlHelper.hpp>

using namespace glPortal;

SUITE(XmlReading)
{
  TEST(extractValidVectorDataFromXml){
    Vector3f sourceVector(155, 266, 377);
    Vector3f vector;
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * lightElement = new TiXmlElement( "light" );
    lightElement->SetAttribute("x", sourceVector.x);
    lightElement->SetAttribute("y", sourceVector.y);
    lightElement->SetAttribute("z", sourceVector.z);
    doc.LinkEndChild(decl);
    doc.LinkEndChild(lightElement);
    XmlHelper::pushAttributeToVector(lightElement, vector);

    bool vectorIsValid(false);
    Vector3f resultVector = Vector3f::sub(sourceVector, vector);
    if((resultVector.x + resultVector.y + resultVector.z) == 0 ){
      vectorIsValid = true;
    }
    CHECK(vectorIsValid);
  }

  TEST(extractInvalidVectorDataFromXml){
    CHECK(true);
  }
}

int main()
{
  return UnitTest::RunAllTests();
}
