//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <format>

#include <algorithm>
#include <array>
#include <iterator>
#include <list>
#include <span>
#include <string>
#include <vector>

#include "benchmark/benchmark.h"
#include "make_string.h"
#include "test_macros.h"

#define CSTR(S) MAKE_CSTRING(CharT, S)

/*** Back inserter ***/

template <class Container>
static void BM_format_to_string_back_inserter(benchmark::State& state) {
  using CharT = typename Container::value_type;
  size_t size = state.range(0);
  auto str    = std::basic_string<CharT>(size, CharT('*'));

  for (auto _ : state) {
    Container output;
    benchmark::DoNotOptimize(std::format_to(std::back_inserter(output), CSTR("{}"), str));
  }
  state.SetBytesProcessed(state.iterations() * size * sizeof(CharT));
}

/*** Begin ***/

template <class Container>
static void BM_format_to_string_begin(benchmark::State& state) {
  using CharT = typename Container::value_type;
  size_t size = state.range(0);
  auto str    = std::basic_string<CharT>(size, CharT('*'));

  Container output(size, CharT('-'));
  for (auto _ : state)
    benchmark::DoNotOptimize(std::format_to(std::begin(output), CSTR("{}"), str));

  state.SetBytesProcessed(state.iterations() * size * sizeof(CharT));
}

/*** Pointer ***/

template <class CharT>
static void BM_format_to_string_span(benchmark::State& state) {
  size_t size = state.range(0);
  auto str    = std::basic_string<CharT>(size, CharT('*'));

  auto buffer = std::basic_string<CharT>(size, CharT('-'));
  std::span<CharT> output{buffer};
  for (auto _ : state)
    benchmark::DoNotOptimize(std::format_to(std::begin(output), CSTR("{}"), str));

  state.SetBytesProcessed(state.iterations() * size * sizeof(CharT));
}

template <class CharT>
static void BM_format_to_string_pointer(benchmark::State& state) {
  size_t size = state.range(0);
  auto str    = std::basic_string<CharT>(size, CharT('*'));

  auto buffer   = std::basic_string<CharT>(size, CharT('-'));
  CharT* output = buffer.data();
  for (auto _ : state)
    benchmark::DoNotOptimize(std::format_to(output, CSTR("{}"), str));

  state.SetBytesProcessed(state.iterations() * size * sizeof(CharT));
}

/*** Main ***/

BENCHMARK(BM_format_to_string_back_inserter<std::string>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_back_inserter<std::vector<char>>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_back_inserter<std::list<char>>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_begin<std::string>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_begin<std::vector<char>>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_begin<std::list<char>>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_span<char>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_pointer<char>)->RangeMultiplier(2)->Range(1, 1 << 20);

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
BENCHMARK(BM_format_to_string_back_inserter<std::wstring>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_back_inserter<std::vector<wchar_t>>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_back_inserter<std::list<wchar_t>>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_begin<std::wstring>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_begin<std::vector<wchar_t>>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_begin<std::list<wchar_t>>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_span<wchar_t>)->RangeMultiplier(2)->Range(1, 1 << 20);
BENCHMARK(BM_format_to_string_pointer<wchar_t>)->RangeMultiplier(2)->Range(1, 1 << 20);
#endif

BENCHMARK_MAIN();
