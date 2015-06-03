#include "MaterialLoader.hpp"
#include <assets/texture/TextureLoader.hpp>
#include <env/Environment.hpp>
#include <tinyxml.h>

namespace glPortal {

std::map<std::string, Material> MaterialLoader::materialCache = {};

const Material MaterialLoader::loadFromXML(const std::string &path) {
  std::string dir = path.substr(0, path.find_last_of("/\\"));
  TiXmlDocument doc(Environment::getDataDir() + "/textures/" + path + ".gmd");
  doc.LoadFile();
  TiXmlHandle docHandle(&doc);
  TiXmlElement *root = docHandle.FirstChildElement().ToElement();
  TiXmlHandle rootH(root);

  std::string name; root->QueryStringAttribute("name", &name);

  Material mat;

  mat.name = name;
  root->QueryStringAttribute("fancyname", &mat.fancyname);

  TiXmlElement *diffE = rootH.FirstChildElement("diffuse").ToElement();
  if (diffE) {
    std::string diffP("");
    diffE->QueryStringAttribute("path", &diffP);
    if (diffP.length() > 0) {
      diffP = dir + "/" + diffP;
      printf("%s\n", diffP.c_str());
      mat.diffuse = TextureLoader::getTexture(diffP);
    }
  }

  TiXmlElement *portalE = rootH.FirstChildElement("portal").ToElement();
  if (portalE) {
    portalE->QueryBoolAttribute("able", &mat.portalable);
    portalE->QueryBoolAttribute("bump", &mat.portalBump);
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

} /* namespace glPortal */
