#include <gtest/gtest.h>

#include <filesystem>
#include <regex>
#include <sstream>
#include <string>

#include "buggy.h"

class BuggyTest : public ::testing::Test {
 public:
  BuggyTest() {}

  void SetUp() {
    shutdownFlag = false;
    wordsMap.clear();
    std::queue<std::string> empty;
    std::swap(wordQueue, empty);
  }

  void TearDown() {
    shutdownFlag = false;
    wordsMap.clear();
    std::queue<std::string> empty;
    std::swap(wordQueue, empty);
  }

  ~BuggyTest() {}
};

TEST_F(BuggyTest, LookupWordsSuccessTest) {
  const std::string testfile = "test/lookupWords_successTest.txt";
  wordsMap.insert({"abc", 1});
  wordsMap.insert({"donkey", 3});
  wordsMap.insert({"cat", 5});

  testing::internal::CaptureStdout();
  FILE *fpTest = fopen(testfile.c_str(), "r");
  if (fpTest == NULL) {
    std::cerr << "\nError opening file: " << testfile << ", with errno: " << errno << std::endl;
    std::cerr << "path set to: " << std::filesystem::current_path() << std::endl;
  }

  // call buggy function to
  lookupWords(fpTest);
  const std::string output = testing::internal::GetCapturedStdout();

  std::smatch match;
  std::regex rgx(".*SUCCESS:\\sabc was present (\\d+).*");
  if (std::regex_search(output.begin(), output.end(), match, rgx)) {
    EXPECT_EQ(stoi(match[1]), 1);
  }

  rgx = (".*SUCCESS:\\sdonkey was present (\\d+).*");
  if (std::regex_search(output.begin(), output.end(), match, rgx)) {
    EXPECT_EQ(stoi(match[1]), 3);
  }

  rgx = (".*SUCCESS:\\scat was present (\\d+).*");
  if (std::regex_search(output.begin(), output.end(), match, rgx)) {
    EXPECT_EQ(stoi(match[1]), 5);
  }
}

TEST_F(BuggyTest, LookupWordsFailTest) {
  const std::string testfile = "test/lookupWords_failTest.txt";

  testing::internal::CaptureStdout();
  FILE *fpTest = fopen(testfile.c_str(), "r");
  if (fpTest == NULL) {
    std::cerr << "\nError opening file: " << testfile << ", with errno: " << errno << std::endl;
    std::cerr << "path set to: " << std::filesystem::current_path() << std::endl;
  }

  // call buggy function to
  lookupWords(fpTest);
  const std::string output = testing::internal::GetCapturedStdout();

  std::smatch match;
  std::regex rgx(".*'dog' was (NOT found).*");
  if (std::regex_search(output.begin(), output.end(), match, rgx)) {
    EXPECT_EQ(match[1], "NOT found");
  }

  rgx = ".*'giraffe' was (NOT found).*";
  if (std::regex_search(output.begin(), output.end(), match, rgx)) {
    EXPECT_EQ(match[1], "NOT found");
  }

  rgx = ".*'def' was (NOT found).*";
  if (std::regex_search(output.begin(), output.end(), match, rgx)) {
    EXPECT_EQ(match[1], "NOT found");
  }
}

TEST_F(BuggyTest, ReadInputSuccessInterruptTest) {
  const std::string testfile = "test/readInputWords_successTest.txt";

  FILE *fpTest = fopen(testfile.c_str(), "r");
  if (fpTest == NULL) {
    std::cerr << "\nError opening file: " << testfile << ", with errno: " << errno << std::endl;
    std::cerr << "path set to: " << std::filesystem::current_path() << std::endl;
  }

  // call buggy function
  readInputWords(fpTest);
  EXPECT_TRUE(shutdownFlag);

  auto it = wordsMap.find("scarf");
  EXPECT_NE(it, wordsMap.end());
  if (it != wordsMap.end()) {
    EXPECT_EQ(it->second, 1);
  }

  it = wordsMap.find("coat");
  EXPECT_NE(it, wordsMap.end());
  if (it != wordsMap.end()) {
    EXPECT_EQ(it->second, 1);
  }

  it = wordsMap.find("hat");
  EXPECT_NE(it, wordsMap.end());
  if (it != wordsMap.end()) {
    EXPECT_EQ(it->second, 1);
  }
}

TEST_F(BuggyTest, ReadInputSuccessEndTest) {
  const std::string testfile = "test/readInputWords_successTestEnd.txt";

  FILE *fpTest = fopen(testfile.c_str(), "r");
  if (fpTest == NULL) {
    std::cerr << "\nError opening file: " << testfile << ", with errno: " << errno << std::endl;
    std::cerr << "path set to: " << std::filesystem::current_path() << std::endl;
  }

  // call buggy function
  readInputWords(fpTest);
  EXPECT_TRUE(shutdownFlag);

  auto it = wordsMap.find("scarf");
  EXPECT_NE(it, wordsMap.end());
  if (it != wordsMap.end()) {
    EXPECT_EQ(it->second, 1);
  }

  it = wordsMap.find("coat");
  EXPECT_NE(it, wordsMap.end());
  if (it != wordsMap.end()) {
    EXPECT_EQ(it->second, 1);
  }

  it = wordsMap.find("hat");
  EXPECT_NE(it, wordsMap.end());
  if (it != wordsMap.end()) {
    EXPECT_EQ(it->second, 1);
  }
}

TEST_F(BuggyTest, ReadInputFailInterruptTest) {
  const std::string testfile = "test/readInputWords_successTest.txt";

  FILE *fpTest = fopen(testfile.c_str(), "r");
  if (fpTest == NULL) {
    FAIL() << "\nError opening file: " << testfile << ", with errno: " << errno << std::endl;
    std::cerr << "path set to: " << std::filesystem::current_path() << std::endl;
  }

  // call buggy function to
  readInputWords(fpTest);
  EXPECT_TRUE(shutdownFlag);

  auto it = wordsMap.find("kimono");
  EXPECT_EQ(it, wordsMap.end());

  it = wordsMap.find("yukata");
  EXPECT_EQ(it, wordsMap.end());
}

TEST_F(BuggyTest, ReadInputEmptyInterruptTest) {
  const std::string testfile = "test/readInputWords_successTestEmpty.txt";

  FILE *fpTest = fopen(testfile.c_str(), "r");
  if (fpTest == NULL) {
    FAIL() << "\nError opening file: " << testfile << ", with errno: " << errno << std::endl;
    std::cerr << "path set to: " << std::filesystem::current_path() << std::endl;
  }

  // call buggy function to
  readInputWords(fpTest);
  EXPECT_TRUE(shutdownFlag);

  auto it = wordsMap.find("test");
  EXPECT_EQ(it, wordsMap.end());

  EXPECT_EQ(wordsMap.size(), 0);
}

TEST_F(BuggyTest, ReadInputSuccessInterruptLargeTest) {
  const std::string testfile = "test/readInputWords_successTestLarge.txt";

  FILE *fpTest = fopen(testfile.c_str(), "r");
  if (fpTest == NULL) {
    std::cerr << "\nError opening file: " << testfile << ", with errno: " << errno << std::endl;
    std::cerr << "path set to: " << std::filesystem::current_path() << std::endl;
  }

  // call buggy function
  readInputWords(fpTest);
  EXPECT_TRUE(shutdownFlag);

  auto it = wordsMap.find("scarf");
  EXPECT_NE(it, wordsMap.end());
  if (it != wordsMap.end()) {
    EXPECT_EQ(it->second, 1);
  }

  it = wordsMap.find("coat");
  EXPECT_NE(it, wordsMap.end());
  if (it != wordsMap.end()) {
    EXPECT_EQ(it->second, 1);
  }

  it = wordsMap.find("hat");
  EXPECT_NE(it, wordsMap.end());
  if (it != wordsMap.end()) {
    EXPECT_EQ(it->second, 10081);
  }
}
