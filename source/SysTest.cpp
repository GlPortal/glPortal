#include <stdexcept>
#include <string>
#include <cstdio>
#include <iostream>

#include <radix/env/Environment.hpp>
#include <radix/World.hpp>
#include <radix/system/PlayerSystem.hpp>
#include <radix/system/PhysicsSystem.hpp>

int main(const int argc, char *argv[]) {
  radix::Environment::init();
  radix::World wld;
  wld.addSystem<radix::PlayerSystem>();
  wld.addSystem<radix::PhysicsSystem>();
  wld.computeSystemOrder();
  return 0;
}
