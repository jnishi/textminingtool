#include <cassert>
#include <smmintrin.h>
#include <algorithm>

#include "BitArray.hpp"


inline uint64_t popcount(uint64_t x){
  return _mm_popcnt_u64(x);
}

BitArray::BitArray(const uint64_t size) : length(size),
					  isBuild(false)
{
  initBitBlocks();
  initRankTable();
  
}

void BitArray::initBitBlocks()
{
  bit_blocks.resize((length -1 / block_size)+1, 0LLU);
}

void BitArray::initRankTable()
{
  rank_table.resize((length -1) / (rank_table_blocks * block_size)+1, 0LLU);
}

uint64_t BitArray::access(const uint64_t pos) const
{
  assert(pos < length);
  return (bit_blocks[pos / block_size] & 1LLU << (pos % block_size))
    ? 1 :0 ;
  
}

void BitArray::setbit(const uint64_t bit, uint64_t pos)
{
  assert(pos < length);
  bit_blocks[pos / block_size] = (bit_blocks[pos / block_size]
				  & (~(1LLU << (pos % block_size)))
				  | bit << (pos % block_size));
  isBuild = false;
  return;
 }

void BitArray::build()
{
  buildRankTable();
  isBuild = true;
}

void BitArray::buildRankTable()
{
  rank_table[0] = 0;
  for(uint64_t i=1;i<length / (rank_table_blocks * block_size);++i){
    rank_table[i] = rank_table[i-1] + calcTableRank(i-1);
  }
}

uint8_t BitArray::calcTableRank(uint64_t tidx) const{
  uint8_t rank = 0;
  for(uint64_t i=0;i < rank_table_blocks;++i){
    rank += popcount(bit_blocks[tidx * rank_table_blocks + i]);
  }
  return rank;
}

uint8_t BitArray::calcTableRank(uint64_t tidx, uint64_t idx) const{
  uint8_t rank = 0;
  for(uint64_t i=0;i < idx / block_size;++i){
    rank += popcount(bit_blocks[tidx * rank_table_blocks + i]);
  }
  rank += popcount(bit_blocks[tidx * rank_table_blocks + idx / block_size]
		   << (block_size - (idx % block_size) -1));
  return rank;
}

uint64_t BitArray::rank(uint64_t bit, uint64_t pos) const
{
  assert(isBuild);
  return bit ? rank1(pos) : (pos - rank1(pos)+1);
}

uint64_t BitArray::rank1(uint64_t pos) const
{
  uint64_t table_page = pos /(rank_table_blocks * block_size);
  return rank_table[table_page] + calcTableRank(table_page, pos % (rank_table_blocks * block_size));
}

uint64_t BitArray::select(uint64_t bit, uint64_t idx) const
{
  assert(bit < 2);
  uint64_t tidx = binarySearch(bit, idx, 0);
  uint64_t residue = std::min(rank_table_blocks * block_size * tidx, length);
  uint64_t c_rank = bit ? rank_table[tidx] : residue - rank_table[tidx];
  
  for(uint64_t t=0;t<rank_table_blocks;++t){
    uint64_t count = popcount(bit_blocks[tidx * rank_table_blocks + t]);
    count = bit ? count : (std::min(block_size+0, length - (tidx * rank_table_blocks + t) * block_size) - count);
    if(idx <= c_rank + count){
      return selectInBlock(bit, idx, c_rank, tidx * rank_table_blocks + t);
    } else {
      c_rank += count;
    }
  }
}
    
uint64_t BitArray::selectInBlock(uint64_t bit, uint64_t idx, uint64_t c_rank, uint64_t bidx) const
{
  uint64_t pos = bidx * block_size;
  for(uint64_t i=0;i<block_size;++i){
    if(bit == access(pos+i)) c_rank++;
    if(c_rank == idx) return pos+i;
  }
}

uint64_t BitArray::binarySearch(uint64_t bit, uint64_t idx, uint64_t tidx) const
{
  if(rank_table.size() == 1) return 0;
  uint64_t c_rank = bit ?  rank_table[tidx] : std::min(rank_table_blocks * block_size * tidx, length)  - rank_table[tidx];
  uint64_t next_rank = bit ? rank_table[tidx+1] : std::min(rank_table_blocks * block_size * (tidx+1), length) - rank_table[tidx+1];
  if(idx <= c_rank) {
    return binarySearch(bit, idx, tidx/2);
  } else if(next_rank < idx) {
    return binarySearch(bit, idx, (rank_table.size()-tidx) / 2);
  } else {
    return tidx;
  }
}
