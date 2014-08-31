#include <unittest++/UnitTest++.h>
#include <tinyxml.h>
#include <engine/util/Vector3f.hpp>
#include <engine/loader/XmlHelper.hpp>

using namespace glPortal;

SUITE(XmlReading)
{
  TEST(extractValidVectorDataFromXml){
    Vector3f vector;
    //    TiXmlDocument doc;
    /*    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * lightElement = new TiXmlElement( "light" );
    lightElement->SetAttribute("x", 155);
    lightElement->SetAttribute("y", 266);
    lightElement->SetAttribute("z", 377);
    doc.LinkEndChild( decl );
    doc.LinkEndChild( lightElement );
    //doc.SaveFile( "madeByHand.xml" );
    XmlHelper::pushAttributeToVector(lightElement, vector);
    */
    CHECK(true);
  }

  TEST(extractInvalidVectorDataFromXml){
    CHECK(false);
  }
}

int main()
{
  return UnitTest::RunAllTests();
}
