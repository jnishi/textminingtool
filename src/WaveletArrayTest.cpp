#include <gtest/gtest.h>
#include <iconv.h>
#include <iostream>
#include <cstring>
#include <errno.h>
#include "WaveletArray.hpp"

void convertToUTF16(char *input, uint16_t *output, size_t length)
{
  iconv_t cd = iconv_open("UTF-16LE", "UTF-8");
  size_t outbytesleft = BUFSIZ;

  char *output_iconv = (char *)output;
  while(length > 0){
    size_t result = iconv(cd, &input, &length, &output_iconv, &outbytesleft);
    ASSERT_GE((int) result, 0);
  }
  iconv_close(cd);
  output = (uint16_t *)output_iconv;
  return;
}

void convertToUTF8(uint16_t *input, char *output, size_t length)
{
  iconv_t cd = iconv_open("UTF-8", "UTF-16LE");
  size_t outbytesleft = BUFSIZ;
  char * input_iconv = (char *)input;
  while(length > 0){
    size_t result = iconv(cd, &input_iconv, &length, &output, &outbytesleft);
    ASSERT_GE((int) result, 0);
  }
  iconv_close(cd);
  return;
}

TEST(WaveletArray, initialize)
{
  char input[] = "ababaあいう";
  uint16_t testinput[BUFSIZ]={0};
  size_t length = strlen(input);
  convertToUTF16(input,  testinput, length);
  WaveletArray(testinput, 5LLU);
}

TEST(WaveletArray, access1)
{
  char input[] = "ababa";
  uint16_t testinput[BUFSIZ] ={0};
  size_t length = strlen(input);
  convertToUTF16(input, testinput, length);
  WaveletArray wa(testinput, 5LLU);
  
  char r1[BUFSIZ] = {0};
  uint16_t r2[BUFSIZ] = {0};
  r2[0] = wa.access(4);
  uint16_t res = wa.access(4);

  convertToUTF8(r2, r1, 2);
  ASSERT_STREQ("a", r1);
}

TEST(WaveletArray, access2)
{
  char input[] = "ababaあいう";
  //char input[] = "abracadabra";

  uint16_t testinput[BUFSIZ] ={0};
  size_t length = strlen(input);
  convertToUTF16(input, testinput, length);
  WaveletArray wa(testinput, length);
  
  char r1[BUFSIZ] = {0};
  uint16_t r2[BUFSIZ] = {0};
  for(int i=0;i<length;++i){
    r2[i] = wa.access(i);
  }
  convertToUTF8(r2, r1, 2*length);
  std::cout << r1 << std::endl;
  ASSERT_STREQ(input, r1);
}

TEST(WaveletArray, rank)
{
  char input[] = "ababa";
  uint16_t testinput[BUFSIZ] ={0};
  size_t length = strlen(input);
  convertToUTF16(input, testinput, length);
  WaveletArray wa(testinput, 5LLU);
  
  uint16_t a = 0x61;
  uint16_t b = 0x62;

  ASSERT_EQ(1, wa.rank(a,1));
  ASSERT_EQ(1, wa.rank(b,2));
  ASSERT_EQ(3, wa.rank(a,5));
  ASSERT_EQ(2, wa.rank(b,3));
}

TEST(WaveletArray, select)
{
  char input[] = "ababa";
  uint16_t testinput[BUFSIZ] ={0};
  size_t length = strlen(input);
  convertToUTF16(input, testinput, length);
  WaveletArray wa(testinput, 5LLU);
  
  uint16_t a = 0x61;
  uint16_t b = 0x62;

  ASSERT_EQ(0, wa.select(a,1));
  ASSERT_EQ(3, wa.select(b,2));
  ASSERT_EQ(2, wa.select(a,2));
  
}
