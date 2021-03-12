#pragma once

#include <algorithm>
#include <fstream>
#include <iterator>
#include <cmath>
#include <string>
#include <random>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>
#include <iostream>

#include "author.hpp"
#include "book.hpp"

namespace test::utils {

// type aliases
using Path = std::string;
using Paths = std::vector<std::string>;

// constants
inline static constexpr int kNumSamplesReserve = 30;
inline static constexpr auto kDatasetDir = std::string_view{DATASET_DIR};

/**
 * Reads and tokenizes a text file.
 * Tokens are then randomly sampled to form the result.
 * If number of samples requested exceeds the amount of data provided,
 * then the the amount of data provided will be returned.
 *
 * @param path - relative path to the text file
 * @param num_samples - max number of token samples
 * @param delim - tokens delimiter
 * @return a vector of sampled text tokens
 */
inline auto load_token_samples(const Path &path, int num_samples, char delim = '\n') -> std::vector<std::string> {
  std::vector<std::string> tokens;
  tokens.reserve(kNumSamplesReserve);

  const auto kPrefixPath = std::string{kDatasetDir};

  if (auto fs = std::ifstream(kPrefixPath + path)) {
    for (std::string token; std::getline(fs, token, delim); /* ... */) {
      tokens.emplace_back(std::move(token));
    }
  }

  num_samples = std::min(num_samples, static_cast<int>(tokens.size()));

  std::vector<std::string> token_samples;
  token_samples.reserve(num_samples);

  auto engine = std::default_random_engine{std::random_device{}()};

  std::sample(tokens.begin(), tokens.end(), std::back_inserter(token_samples), num_samples, engine);
  std::shuffle(token_samples.begin(), token_samples.end(), engine);

  return token_samples;
}

/**
 * Reads and tokenizes book contents.
 * If number of samples requested exceeds the amount of data provided,
 * then the the amount of data provided will be returned.
 *
 * @param paths - relative paths to the text files
 * @return a vector of contents read from files
 */
inline auto load_book_contents(const Paths &paths, int num_samples) -> std::vector<std::string> {
  std::vector<std::string> contents;
  contents.reserve(num_samples);

  const auto kPrefixPath = std::string{kDatasetDir} + "contents/";

  for (const auto &path: paths) {
    if (const auto fs = std::ifstream(kPrefixPath + path)) {
      std::stringstream ss;
      ss << fs.rdbuf();

      contents.emplace_back(ss.str());
    }
  }

  num_samples = std::min(num_samples, static_cast<int>(contents.size()));

  std::vector<std::string> content_samples;
  content_samples.reserve(num_samples);

  std::sample(std::make_move_iterator(contents.begin()), std::make_move_iterator(contents.end()),
              std::back_inserter(content_samples), num_samples,
              std::default_random_engine{std::random_device{}()});

  return content_samples;
}

/**
 * Reads and parses author objects from file.
 * If number of samples requested exceeds the amount of data provided,
 * then the the amount of data provided will be returned.
 *
 * @param path - relative path to the file
 * @param num_samples - max number of author samples
 * @return a vector of sampled author objects
 */
inline auto load_author_samples(const Path &path, int num_samples) -> std::vector<Author> {
  std::vector<Author> authors;
  authors.reserve(kNumSamplesReserve);

  const auto kPrefixPath = std::string{kDatasetDir};

  if (auto fs = std::ifstream(kPrefixPath + path)) {
    int age;
    int sex_id;
    std::string full_name;

    for (std::string line; std::getline(fs, line); /* ... */) {

      std::stringstream ss(line);
      ss >> full_name >> age >> sex_id;

      assert(!full_name.empty());
      assert(age >= Author::kMinAuthorAge);
      assert(sex_id >= 0 && sex_id <= static_cast<int>(Sex::UNDEFINED));

      auto author = Author();
      author.SetAge(age);
      author.SetFullName(full_name);
      author.SetSex(static_cast<Sex>(sex_id));

      authors.push_back(author);
    }
  }

  num_samples = std::min(num_samples, static_cast<int>(authors.size()));

  std::vector<Author> author_samples;
  author_samples.reserve(num_samples);

  std::sample(authors.begin(), authors.end(),
              std::back_inserter(author_samples), num_samples,
              std::default_random_engine{std::random_device{}()});

  return author_samples;
}

inline auto generate_book_samples(int num_samples) -> std::vector<Book> {
  std::vector<Book> book_samples(num_samples);

  // pre-load data
  const std::vector<std::string> contents = load_book_contents({"1.txt", "2.txt", "3.txt"}, num_samples);
  const std::vector<std::string> titles = load_token_samples("book_titles.txt", num_samples);

  num_samples = std::min(contents.size(), titles.size());

  for (int index = 0; index < num_samples; index++) {
    const std::vector<Author> authors = load_author_samples("authors.txt", 2);
    book_samples[index].SetContent(contents[index]);
    book_samples[index].SetTitle(titles[index]);

    for (const auto &author: authors) {
      book_samples[index].AddAuthor(author);
    }
  }

  return book_samples;
}

}  // namespace test::utils
