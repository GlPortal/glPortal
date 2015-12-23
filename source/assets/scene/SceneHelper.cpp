#include "SceneHelper.hpp"

namespace glPortal {

EntityPair& SceneHelper::getPortalPairFromScene(int n, Scene *scene) {
  int pairCount = scene->portalPairs.size();
  if (pairCount <= n) {
    scene->portalPairs.reserve(n + 1);
    // Instantiate and initalize new portal pairs
    for (int i = pairCount; i <= n; ++i) {
      Entity &pEnt1 = scene->entities.create();
      Entity &pEnt2 = scene->entities.create();
      pEnt1.addComponent<Transform>();
      pEnt1.addComponent<Portal>();
      LightSource &ls1 = pEnt1.addComponent<LightSource>();
      pEnt2.addComponent<Transform>();
      pEnt2.addComponent<Portal>();
      LightSource &ls2 = pEnt2.addComponent<LightSource>();
      ls1.enabled = ls2.enabled = false;
      ls1.energy = ls2.energy = 5;
      ls1.distance = ls2.distance = 1.3f;
      scene->portalPairs.emplace_back(&pEnt1, &pEnt2);
    }
  }
  return scene->portalPairs.at(n);
}
} /* namespace glPortal */
