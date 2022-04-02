#include <gtest/gtest.h>

extern "C" {
#include "../static_lib/stat_lib.h"
}

TEST(start,correct){
    std::string correct_predicat = "../iz2_cpp/iz2/predicat.txt";
    char *cor_pred = new char[correct_predicat.length() + 1];
    strcpy(cor_pred, correct_predicat.c_str());
    std::string correct_var = "../iz2_cpp/iz2/variables.txt";
    char *cor_var = new char[correct_var.length() + 1];
    strcpy(cor_var, correct_var.c_str());
    EXPECT_EQ(6, start(cor_pred,cor_var));
}