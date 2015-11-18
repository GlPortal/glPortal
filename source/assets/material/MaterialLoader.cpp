#include "MaterialLoader.hpp"
#include <assets/texture/TextureLoader.hpp>
#include <engine/env/Environment.hpp>
#include <engine/env/System.hpp>
#include <tinyxml2.h>
#include <cstdio>

using namespace tinyxml2;

namespace glPortal {

std::map<std::string, Material> MaterialLoader::materialCache = {};

const Material MaterialLoader::loadFromXML(const std::string &path) {
  std::string dir = path.substr(0, path.find_last_of("/\\"));
  XMLDocument doc;
  XMLError error = doc.LoadFile((Environment::getDataDir() + "/textures/" + path + ".gmd").c_str());

  if (error != XML_NO_ERROR) {
    System::Log(Error, "MaterialLoader") << "XML Error " << doc.ErrorID() << ": " <<
      doc.ErrorName() << " in " << path;
  }

  XMLHandle docHandle(&doc);
  XMLElement *root = docHandle.FirstChildElement().ToElement();
  XMLHandle rootH(root);

  std::string name = root->Attribute("name");
  std::string fancyname = root->Attribute("fancyname");

  Material mat;

  mat.name = name;
  mat.fancyname = fancyname;

  XMLElement *diffE = rootH.FirstChildElement("diffuse").ToElement();
  if (diffE) {
    std::string diffP = diffE->Attribute("path");
    if (diffP.length() > 0) {
      diffP = dir + "/" + diffP;
      System::Log(Debug, "MaterialLoader") << mat.name << ": load " << diffP;
      mat.diffuse = TextureLoader::getTexture(diffP);
    }
  } else {
    mat.diffuse = TextureLoader::getEmptyDiffuse();
  }

  XMLElement *normE = rootH.FirstChildElement("normal").ToElement();
  if (normE) {
    std::string normP = normE->Attribute("path");
    if (normP.length() > 0) {
      normP = dir + "/" + normP;
      System::Log(Debug, "MaterialLoader") << mat.name << ": load " << normP;
      mat.normal = TextureLoader::getTexture(normP);
    }
  } else {
    mat.normal = TextureLoader::getEmptyNormal();
  }

  XMLElement *specE = rootH.FirstChildElement("specular").ToElement();
  if (specE) {
    std::string specP = specE->Attribute("path");
    if (specP.length() > 0) {
      specP = dir + "/" + specP;
      System::Log(Debug, "MaterialLoader") << mat.name << ": load " << specP;
      mat.specular = TextureLoader::getTexture(specP);
    }
    specE->QueryFloatAttribute("shininess", &mat.shininess);
  } else {
    mat.specular = TextureLoader::getEmptySpecular();
  }

  XMLElement *surfaceE = rootH.FirstChildElement("surface").ToElement();
  if (surfaceE) {
    surfaceE->QueryBoolAttribute("portalable", &mat.portalable);
  }

  // TODO

  return mat;
}

const Material& MaterialLoader::getMaterial(const std::string &name) {
  auto it = materialCache.find(name);
  if (it != materialCache.end()) {
    return it->second;
  }
  std::string path = /*Environment::getDataDir() + "/textures/" +*/ name;
  Material m = loadFromXML(path);
  auto inserted = materialCache.insert(std::pair<std::string, Material>(m.name, m));
  // Return reference to newly inserted Material
  return inserted.first->second;
}

const Material& MaterialLoader::fromTexture(const std::string &name) {
  auto it = materialCache.find("rawtex/" + name);
  if (it != materialCache.end()) {
    return it->second;
  }
  Material m;
  m.name = "rawtex/" + name;
  m.diffuse = TextureLoader::getTexture(name);
  auto inserted = materialCache.insert(std::pair<std::string, Material>(m.name, m));
  // Return reference to newly inserted Material
  return inserted.first->second;
}

} /* namespace glPortal */
