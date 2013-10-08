#include <stdint.h>
#include <vector>

enum bit_t {
  ZERO = 0,
  ONE= 1,
};

class BitArray {
public:
  BitArray();
  void resize(const uint64_t size);
  bit_t access(const uint64_t pos) const;
  uint64_t rank(bit_t bit, uint64_t pos) const;
  uint64_t select(bit_t bit, uint64_t idx) const;
  void setbit(const bit_t bit, const uint64_t pos);
  void build();
  uint64_t size() const{ return length; };
  
private:
  const static uint64_t default_size = 1 * 1024; // 1K
  const static uint64_t rank_table_blocks = 4;
  const static uint64_t block_size = sizeof(uint64_t) * 8;
  std::vector<uint64_t> bit_blocks;
  std::vector<uint64_t> rank_table;
  uint64_t length;
  bool isBuild;

  void initBitBlocks();
  void initRankTable();
  void buildRankTable();
  uint8_t calcTableRank(uint64_t tidx) const;
  uint8_t calcTableRank(uint64_t tidx, uint64_t idx) const;
  uint64_t selectInBlock(bit_t bit, uint64_t idx, uint64_t c_rank, uint64_t bidx) const;
  uint64_t binarySearch(bit_t bit, uint64_t idx, uint64_t tidx) const;
  uint64_t rank1(uint64_t pos) const;

};

