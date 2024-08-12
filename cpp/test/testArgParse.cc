/*
 * @Author: zhuqingquan
 * @Date: 2024-08-04 
 * @FilePath: \zutils\cpp\test\testArgParse.cc
 * @Description: test for argparse
 */
#include <gtest/gtest.h>
#include "argparse.hpp"

TEST(argparserTest, add_argument)
{
    util::argparser parser("test for argparse");
    parser.add_argument<std::string>(std::string("-a"), std::string("arg1"));
    EXPECT_TRUE(1);
}