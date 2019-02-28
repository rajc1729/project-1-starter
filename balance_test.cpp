
///////////////////////////////////////////////////////////////////////////////
// balance_test.hpp
//
// Unit tests for the functionality declared in balance.hpp .
///////////////////////////////////////////////////////////////////////////////

#include <random>
#include <vector>

#include "gtest/gtest.h"

#include "balance.hpp"

TEST(find_dip_trivial_cases, trivial_cases) {
  { // input too small to find a dip
    std::vector<int> empty,
                     one_element{5},
                     two_elements{5, 6};
    EXPECT_EQ(empty.end(), balance::find_dip(empty));
    EXPECT_EQ(one_element.end(), balance::find_dip(one_element));
    EXPECT_EQ(two_elements.end(), balance::find_dip(two_elements));
  }

  { // input only contains a dip
    std::vector<int> dip{8, 2, 8};
    EXPECT_EQ(dip.begin(), balance::find_dip(dip));
  }

  { // input is small and definitely does not contain a dip
    std::vector<int> increasing{1, 2, 3}, zeroes{0, 0, 0};
    EXPECT_EQ(increasing.end(), balance::find_dip(increasing));
    EXPECT_EQ(zeroes.end(), balance::find_dip(zeroes));
  }
}

TEST(find_dip_nontrivial_cases, nontrivial_cases) {
  { // dip using entirely negative ints
    std::vector<int> negatives{-10, -12, -10};
    EXPECT_EQ(negatives.begin(), balance::find_dip(negatives));
  }

  { // large vector, 1 million elements, all the same
    std::vector<int> big(1000000, 1);
    EXPECT_EQ(big.end(), balance::find_dip(big));
  }

  { // large vector, rotating between four different values
    std::vector<int> values{2, 4, 6, 8}, rotating;
    for (unsigned i = 0; i < 1000000; ++i) {
      rotating.push_back(values[i % 4]);
    }
    EXPECT_EQ(rotating.end(), balance::find_dip(rotating));
  }

  { // large vector, dip near the middle
    std::vector<int> big(1000000, 1);
    size_t i = big.size() / 2;
    big[i] = 8;
    big[i+1] = 7;
    big[i+2] = 8;
    EXPECT_EQ(big.begin() + i, balance::find_dip(big));
  }

  { // large vector, dip near the very end
    std::vector<int> big(1000000, 1);
    size_t i = big.size() - 4;
    big[i] = 5;
    big[i+1] = 2;
    big[i+2] = 5;
    EXPECT_EQ(big.begin() + i, balance::find_dip(big));
  }

  { // large pseudo-random vector
    std::vector<int> big;
    std::mt19937 rng(0);
    std::uniform_int_distribution<> randint(-10, +10);
    for (unsigned i = 0; i < 1000000; ++i) {
      big.push_back(randint(rng));
    }
    ASSERT_EQ(1000000, big.size());
//    EXPECT_EQ(big.begin() + 22, balance::find_dip(big));
  }
}

TEST(longest_balanced_span_trivial_cases, trivial_cases) {
  // empty
  {
    std::vector<int> empty;
    EXPECT_FALSE(balance::longest_balanced_span(empty));
  }

  // only one element that is not zero
  {
    std::vector<int> five{5};
    EXPECT_FALSE(balance::longest_balanced_span(five));
  }

  // several elements that are not zero
  {
    std::vector<int> four{5, 2, -1, 8};
    EXPECT_FALSE(balance::longest_balanced_span(four));
  }

  // only one zero, that's the only span
  {
    std::vector<int> zero{0};
    auto got = balance::longest_balanced_span(zero);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(zero.begin(), zero.end()), *got);
  }

  { // four-element vector, zero at index 0
    std::vector<int> four{0, 2, -1, 8};
    auto got = balance::longest_balanced_span(four);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(four.begin(), four.begin() + 1), *got);
  }

  { // four-element vector, zero at index 1
    std::vector<int> four{5, 0, -1, 8};
    auto got = balance::longest_balanced_span(four);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(four.begin() + 1, four.begin() + 2), *got);
  }

  { // four-element vector, zero at index 2
    std::vector<int> four{5, 2, 0, 8};
    auto got = balance::longest_balanced_span(four);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(four.begin() + 2, four.begin() + 3), *got);
  }

  { // four-element vector, zero at index 3
    std::vector<int> four{5, 2, -1, 0};
    auto got = balance::longest_balanced_span(four);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(four.begin() + 3, four.begin() + 4), *got);
  }

  { // negatives and positives cancel
    std::vector<int> four{8, 5, -5, 7};
    auto got = balance::longest_balanced_span(four);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(four.begin() + 1, four.begin() + 3), *got);
  }

  { // two small negatives cancel a large positive
    std::vector<int> four{8, -2, -3, 5};
    auto got = balance::longest_balanced_span(four);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(four.begin() + 1, four.begin() + 4), *got);
  }
}

TEST(longest_balanced_span_nontrivial_cases, nontrivial_cases) {
  { // entire vector sums to zero
    std::vector<int> four{6, -2, -5, 1};
    auto got = balance::longest_balanced_span(four);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(four.begin(), four.end()), *got);
  }

  { // length-2 followed by length-1
    std::vector<int> six{4, 3, -3, 2, 0, 8};
    auto got = balance::longest_balanced_span(six);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(six.begin() + 1, six.begin() + 3), *got);
  }

  { // length-1 followed by length-2
    std::vector<int> six{4, 0, 2, -3, 3, 8};
    auto got = balance::longest_balanced_span(six);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(six.begin() + 3, six.begin() + 5), *got);
  }

  { // two length-2s, picks the LATER one
    std::vector<int> seven{3, 2, -2, 3, -4, 4, 3};
    auto got = balance::longest_balanced_span(seven);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(seven.begin() + 4, seven.begin() + 6), *got);
  }

  { // many length-3's, picks the LAST one
    std::vector<int> big;
    for (unsigned i = 0; i < 100; ++i) {
      big.push_back(8);
      big.push_back(-1);
      big.push_back(-1);
      big.push_back(2);
      big.push_back(7);
    }
    ASSERT_EQ(500, big.size());
    auto got = balance::longest_balanced_span(big);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(big.end() - 4, big.end() - 1), *got);
  }

  { // big vector of all zeros, picks everything
    std::vector<int> big(500, 0);
    ASSERT_EQ(500, big.size());
    auto got = balance::longest_balanced_span(big);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(big.begin(), big.end()), *got);
  }

  { // large pseudo-random vector
    std::vector<int> big;
    std::mt19937 rng(0);
    std::uniform_int_distribution<> randint(-10, +10);
    for (unsigned i = 0; i < 500; ++i) {
      big.push_back(randint(rng));
    }
    ASSERT_EQ(500, big.size());
    auto got = balance::longest_balanced_span(big);
    ASSERT_TRUE(got);
    EXPECT_EQ(balance::span(big.begin() + 47, big.begin() + 496), *got);
  }
}
