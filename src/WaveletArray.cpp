#include "WaveletArray.hpp"

inline bit_t getbit(uint16_t c, uint64_t pos)
{
  return (c & (1U << pos)) ? ONE : ZERO;
}

inline bool is_zero(uint16_t c, uint64_t pos)
{
  return (c & (1U << pos)) == 0;
}

inline bool is_one(uint16_t c, uint64_t pos)
{
  return (c & (1U << pos)) != 0;
}

inline uint16_t decodebit(bit_t b, uint64_t l, uint16_t res)
{
  return res | (b << l);
}

WaveletArray::WaveletArray(uint16_t *strings, uint64_t length) : 
  bit_array(std::vector<BitArray>(sizeof(uint16_t) * 8)), 
  size(length), 
  z(std::vector<uint64_t>(sizeof(uint16_t) * 8))
{
  
  for(std::vector<BitArray>::iterator iter = bit_array.begin();
      iter != bit_array.end();++iter){
    iter->resize(length);
  }
  
  build(strings);
}

void WaveletArray::build(uint16_t *strings)
{
  for(uint64_t l=0;l<bit_array.size();++l){
    build_bitarray(strings, l);
    bit_array[l].build();
    build_nextstring(strings, l);
  }
  return;
}

void arraycpy(uint16_t *src, uint16_t *dest, uint64_t length)
{
  for(uint64_t i=0;i<length;++i){
    dest[i] = src[i];
  }
  return;
}

void WaveletArray::build_bitarray(uint16_t *strings, uint64_t l)
{
  for(uint64_t i=0;i<size;++i){
    bit_t bit = getbit(strings[i], l);
    bit_array[l].setbit(bit, i);
    if(bit == ZERO) z[l]++;
  }
  return;
}

void WaveletArray::build_nextstring(uint16_t *strings, uint64_t l)
{
  uint16_t nextstring[size];
  nextstring[size] = '\0';
  uint64_t pos=0;
  
  for(uint64_t i=0;i<size;++i){
    if(is_zero(strings[i], l)){
      nextstring[pos] = strings[i];
      pos++;
    }
  }

  for(uint64_t i=0;i<size;++i){
    if(is_one(strings[i], l)) {
      nextstring[pos] = strings[i];
      pos++;
    }
  }

  arraycpy(nextstring, strings, size);
  return;
}

uint64_t WaveletArray::child(bit_t bit, uint64_t pos, uint64_t l)
{
  return z[l] * bit + bit_array[l].rank(bit, pos) -1;
}

uint16_t WaveletArray::access(uint64_t pos)
{
  uint16_t res = 0;
  uint64_t p = pos;
  for(uint64_t l=0;l<bit_array.size();++l){
    bit_t b = bit_array[l].access(p);
    res = decodebit(b, l, res);
    p = child(b, p, l);
  }
  return res;  
}

uint64_t WaveletArray::rank(uint16_t c, uint64_t pos)
{
  uint64_t base = bit_array[0].select(getbit(c, 0), 1);
  return base_rank(c, pos) - (base_rank(c, base)-1);
}

uint64_t WaveletArray::base_rank(uint16_t c, uint64_t pos)
{
  uint64_t p = pos;
  for(uint64_t l=0;l < bit_array.size();++l){
    bit_t b = getbit(c, l);
    p = child(b, p, l);
  }
  return p;
}

uint64_t WaveletArray::parent(bit_t b, uint64_t pos, uint64_t l)
{
  return bit_array[l].select(b, pos - z[l] * b +1);
}

uint64_t WaveletArray::select(uint16_t c, uint64_t idx)
{
  uint64_t base = bit_array[0].select(getbit(c, 0), 1);
  int64_t p = base_rank(c, base) + idx-1;
  for(int l=bit_array.size()-1;l>=0;l--){
    bit_t b = getbit(c, l);
    p = parent(b, p, l);
  }
  return p;
}
