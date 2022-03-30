#include <gtest/gtest.h>

extern "C" {
#include "../static_lib/stat_lib.h"
}

TEST(start,correct){
    EXPECT_EQ(7, start());
}