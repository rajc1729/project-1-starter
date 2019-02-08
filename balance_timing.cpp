///////////////////////////////////////////////////////////////////////////////
// balance_timing.cpp
//
// Example code showing how to run each algorithm while measuring
// elapsed times precisely. You should modify this program to gather
// all of your experimental data.
//
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

#include "timer.hpp"

#include "balance.hpp"

void print_bar() {
  std::cout << std::string(79, '-') << std::endl;
}

int main() {

  const size_t n = 2000;

  assert(n > 0);

  std::vector<int> input;
  {
    std::mt19937 rng(0); // Use a hardcoded seed for reproducibility between runs.
    std::uniform_int_distribution<> randint(-100, +100);
    for (size_t i = 0; i < n; ++i) {
      input.push_back(randint(rng));
    }
  }
  assert(n == input.size());

  Timer timer;
  double elapsed;

  print_bar();
  std::cout << "n = " << n << std::endl;

  print_bar();
  std::cout << "find dip" << std::endl;
  {
    timer.reset();
    balance::find_dip(input);
    elapsed = timer.elapsed();
  }
  std::cout << "elapsed time=" << elapsed << " seconds" << std::endl;

  print_bar();
  std::cout << "longest balanced span" << std::endl;
  {
    timer.reset();
    balance::longest_balanced_span(input);
    elapsed = timer.elapsed();
  }
  std::cout << "elapsed time=" << elapsed << " seconds" << std::endl;

  print_bar();

  return 0;
}
