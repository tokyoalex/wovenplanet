#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "buggy.h"

const int MAX_WORD_SIZE = 32;
const std::string INPUT_END = "end";
bool shutdownFlag = false;
std::map<std::string, int> wordsMap;
std::queue<std::string> wordQueue;
std::mutex mutx;
std::condition_variable condVar;

// Worker thread: consume words passed from the main thread and inserts them
// into the 'word list' (wordsMap). Terminate when the word 'end' is
// encountered.
//
void workerThread() {
  bool exitThread = false;

  while (!exitThread) {
    std::unique_lock<std::mutex> uniqueLock(mutx);
    condVar.wait(uniqueLock,
                 [] { return shutdownFlag || wordQueue.size() > 0; });

    while (wordQueue.size() > 0) {
      std::string str = wordQueue.front();  // Copy the word
      wordQueue.pop();

      // Doesnt insert duplicate words
      auto it = wordsMap.find(str);
      if (it != wordsMap.end()) {
        it->second = it->second + 1;
      } 
      else {
        wordsMap.insert({str, 1});
      }
    }
    condVar.notify_one();
    uniqueLock.unlock();

    if (shutdownFlag && wordQueue.size() == 0) {
      exitThread = true;
    }
  }
};

// Reads input from terminal, removes newlines etc
//
bool readInput(FILE* in, std::string& strLinebuf) {
  if (in == NULL) {
    std::cerr << "\nError: " << __FUNCTION__ << " -file pointer is null"
              << std::endl;
    return false;
  }

  char linebuf[MAX_WORD_SIZE];
  if (!std::fgets(linebuf, MAX_WORD_SIZE, in)) {  // EOF?
    return false;
  }
  strLinebuf = linebuf;
  strLinebuf.erase(strLinebuf.find_last_not_of("\r\n") + 1);  // remove newline
  return true;
}

// Read input words from STDIN and pass them to the worker thread for
// inclusion in the word list.
// Terminate when the word 'end' has been entered.
//
void readInputWords(FILE* in) {
  if (in == NULL) {
    std::cerr << "\nError: " << __FUNCTION__ << " -file pointer is null"
              << std::endl;
    return;
  }

  std::thread worker = std::thread(workerThread);

  while (!shutdownFlag) {
    if (in == stdin) {
      std::cout << "\nEnter a word (\"end\" or ctrl+d to exit):" << std::endl;
    }

    std::string strLinebuf;
    bool exitMain = !readInput(in, strLinebuf);
    // scoped lock to add items to queue
    {
      std::lock_guard mainlg(mutx);

      if (exitMain || strLinebuf == INPUT_END) {
        shutdownFlag = true;
      } 
      else {
        wordQueue.push(strLinebuf);  // Pass the word to the worker thread
      }
      condVar.notify_one();
    }

    // wait for the worker thread
    {
      std::unique_lock lk(mutx);
      condVar.wait(lk, [] { return wordQueue.size() == 0; });
    }

    condVar.notify_one();
  }

  worker.join();  // Wait for the worker to terminate
}

// Repeatedly ask the user for a word and check whether it was present in the
// word list Terminate on EOF
//
void lookupWords(FILE* in) {
  if (in == NULL) {
    std::cerr << "\nError: " << __FUNCTION__ << " -file pointer is null"
              << std::endl;
    return;
  }
  shutdownFlag = false;  // reset shutdown flag
  while (!shutdownFlag) {
    if (in == stdin) {
      std::cout << "\nEnter a word for lookup (ctrl+d to exit):";
    }

    std::string strLinebuf;
    if (!readInput(in, strLinebuf)) {
      break;
    }

    auto it = wordsMap.find(strLinebuf);
    if (it != wordsMap.end()) {
      std::cout << "SUCCESS: " << it->first << " was present " << it->second
                << " times in the initial word list" << std::endl;
    } 
    else {
      std::cout << "'" << strLinebuf
                << "' was NOT found in the initial word list" << std::endl;
    }
  }
}
