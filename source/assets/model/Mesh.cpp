#include "Mesh.hpp"
#include <cstdio>
namespace glPortal {

Mesh::Mesh() : handle(-1) {
}

Mesh::Mesh(int handle) : handle(handle) {
}

}