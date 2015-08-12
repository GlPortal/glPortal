#ifndef TIGHT_DATA_PACKER_HPP
#define TIGHT_DATA_PACKER_HPP
#include <cstdint>
#include <cstring>
#include <vector>

namespace glPortal {

class TightDataPacker {
protected:
  std::vector<uint8_t> data;

public:
  TightDataPacker() {}
  TightDataPacker(unsigned int reserveBytes);
  template<typename T> TightDataPacker& operator<<(T k) {
    data.insert(data.end(), sizeof(k), 0);
    std::memcpy(&data[data.size()-sizeof(k)], &k, sizeof(k));
    return *this;
  }
  void reserve(unsigned int bytes);
  unsigned int getSize() const;
  const uint8_t* getDataPtr() const;
};

} /* namespace glPortal */

#endif /* TIGHT_DATA_PACKER_HPP */