#include "BitArray.hpp"
#include <vector>

class WaveletArray {
public:
  WaveletArray(uint16_t *strings, uint64_t length);
  void resize(uint64_t length);
  uint16_t access(uint64_t pos);
  uint64_t rank(uint16_t c, uint64_t pos);
  uint64_t select(uint16_t c, uint64_t idx);
  
private:
  std::vector<BitArray> bit_array;
  uint64_t size;
  std::vector<uint64_t> z;

  void build(uint16_t *strings);
  uint64_t child(bit_t bit, uint64_t pos, uint64_t l);
  uint64_t parent(bit_t bit, uint64_t pos, uint64_t l);
  void build_bitarray(uint16_t *strings, uint64_t l);
  void build_nextstring(uint16_t *strings, uint64_t l);
  uint64_t base_rank(uint16_t c, uint64_t pos);
};
