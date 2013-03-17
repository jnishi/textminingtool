#include <gtest/gtest.h>
#include "BitArray.hpp"

TEST(BitArray, initialize){
  uint64_t size = 32;
  BitArray ba(size);
  
  for(uint64_t i=0;i<size;i++){
    EXPECT_EQ(0LLU, ba.access(i));
  }    
  
}

TEST(BitArray, setbit){
  uint64_t size = 32;
  BitArray ba(size);
  ba.setbit(1,14);
  EXPECT_EQ(1LLU, ba.access(14));

  ba.setbit(0,14);
  EXPECT_EQ(0LLU, ba.access(14));

  ba.setbit(1, 31);
  ba.setbit(1, 29);
  EXPECT_EQ(1LLU, ba.access(31));
  EXPECT_EQ(1LLU, ba.access(29));

  ba.setbit(0, 31);
  EXPECT_EQ(0LLU, ba.access(31));
  EXPECT_EQ(1LLU, ba.access(29));
  
}

TEST(BitArray, length){
  BitArray ba(35);
  EXPECT_EQ(35LLU, ba.size());
}

TEST(BitArray, rank1){
  BitArray ba(1024);
  
  for(uint64_t i=1;i<1024;i+=2){
    ba.setbit(1LLU, i);
  }
  
  ba.build();
  EXPECT_EQ(1LLU, ba.rank(0,0));
  EXPECT_EQ(1LLU, ba.rank(1,1));
  EXPECT_EQ(1LLU, ba.rank(0,1));
  EXPECT_EQ(2LLU, ba.rank(1,3));
  EXPECT_EQ(512LLU, ba.rank(1,1023));
  
}

TEST(BitArray, rank2){
  BitArray ba(712);
  
  ba.setbit(1, 0);
  ba.setbit(1, 1);
  ba.setbit(1, 2);
  ba.setbit(1, 3);
  
  ba.build();
  EXPECT_EQ(0LLU, ba.rank(0,0));
  EXPECT_EQ(2LLU, ba.rank(1,1));
  EXPECT_EQ(1LLU, ba.rank(0,4));
  EXPECT_EQ(4LLU, ba.rank(1,6));
  EXPECT_EQ(4LLU, ba.rank(1,124));
  
}

TEST(BitArray, select1){
  BitArray ba(256);

  ba.setbit(1, 0);
  ba.setbit(1, 1);
  ba.setbit(1, 2);
  ba.setbit(1, 3);
 
  ba.build();
  EXPECT_EQ(4LLU, ba.select(0,1));
  EXPECT_EQ(0LLU, ba.select(1,1));
  EXPECT_EQ(1LLU, ba.select(1,2));
  EXPECT_EQ(2LLU, ba.select(1,3));
  EXPECT_EQ(5LLU, ba.select(0,2));
}

TEST(BitArray, select2){
  BitArray ba(1024);

  ba.setbit(1, 0);
  ba.setbit(1, 1);
  ba.setbit(1, 2);
  ba.setbit(1, 3);

  ba.setbit(1, 278);
 
  ba.build();
  EXPECT_EQ(277LLU, ba.select(0,274));
  EXPECT_EQ(279LLU, ba.select(0,275));
  EXPECT_EQ(278LLU, ba.select(1,5));
  EXPECT_EQ(2LLU, ba.select(1,3));
  EXPECT_EQ(5LLU, ba.select(0,2));
}
