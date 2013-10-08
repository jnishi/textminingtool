#include <gtest/gtest.h>
#include "BitArray.hpp"

TEST(BitArray, initialize){
  uint64_t size = 32;
  BitArray ba;
  ba.resize(size);
  
  for(uint64_t i=0;i<size;i++){
    EXPECT_EQ(0LLU, ba.access(i));
  }    
  
}

TEST(BitArray, setbit){
  uint64_t size = 32;
  BitArray ba;
  ba.resize(size);
  ba.setbit(ONE,14);
  EXPECT_EQ(1LLU, ba.access(14));

  ba.setbit(ZERO,14);
  EXPECT_EQ(0LLU, ba.access(14));

  ba.setbit(ONE, 31);
  ba.setbit(ONE, 29);
  EXPECT_EQ(1LLU, ba.access(31));
  EXPECT_EQ(1LLU, ba.access(29));

  ba.setbit(ZERO, 31);
  EXPECT_EQ(0LLU, ba.access(31));
  EXPECT_EQ(1LLU, ba.access(29));
  
}

TEST(BitArray, length){
  BitArray ba;
  uint64_t size = 35;
  ba.resize(size);
  EXPECT_EQ(size, ba.size());
}

TEST(BitArray, rank1){
  BitArray ba;
  uint64_t size = 1024;
  ba.resize(size);
  for(uint64_t i=1;i<1024;i+=2){
    ba.setbit(ONE, i);
  }
  
  ba.build();
  EXPECT_EQ(1LLU, ba.rank(ZERO,0));
  EXPECT_EQ(1LLU, ba.rank(ONE,1));
  EXPECT_EQ(1LLU, ba.rank(ZERO,1));
  EXPECT_EQ(2LLU, ba.rank(ONE,3));
  EXPECT_EQ(512LLU, ba.rank(ONE,1023));
  
}

TEST(BitArray, rank2){
  BitArray ba;
  uint64_t size = 1024;
  ba.resize(size);

  ba.setbit(ONE, 0);
  ba.setbit(ONE, 1);
  ba.setbit(ONE, 2);
  ba.setbit(ONE, 3);
  
  ba.build();
  EXPECT_EQ(0LLU, ba.rank(ZERO,0));
  EXPECT_EQ(2LLU, ba.rank(ONE,1));
  EXPECT_EQ(1LLU, ba.rank(ZERO,4));
  EXPECT_EQ(4LLU, ba.rank(ONE,6));
  EXPECT_EQ(4LLU, ba.rank(ONE,124));
  
}

TEST(BitArray, select1){
  BitArray ba;
  uint64_t size = 768;
  ba.resize(size);

  ba.setbit(ONE, 0);
  ba.setbit(ONE, 1);
  ba.setbit(ONE, 2);
  ba.setbit(ONE, 3);
 
  ba.build();
  EXPECT_EQ(4LLU, ba.select(ZERO,1));
  EXPECT_EQ(0LLU, ba.select(ONE,1));
  EXPECT_EQ(1LLU, ba.select(ONE,2));
  EXPECT_EQ(2LLU, ba.select(ONE,3));
  EXPECT_EQ(5LLU, ba.select(ZERO,2));
}

TEST(BitArray, select2){
  BitArray ba;
  uint64_t size = 768;
  ba.resize(size);

  ba.setbit(ONE, 0);
  ba.setbit(ONE, 1);
  ba.setbit(ONE, 2);
  ba.setbit(ONE, 3);

  ba.setbit(ONE, 278);
 
  ba.build();
  EXPECT_EQ(277LLU, ba.select(ZERO,274));
  EXPECT_EQ(279LLU, ba.select(ZERO,275));
  EXPECT_EQ(278LLU, ba.select(ONE,5));
  EXPECT_EQ(2LLU, ba.select(ONE,3));
  EXPECT_EQ(5LLU, ba.select(ZERO,2));
}
