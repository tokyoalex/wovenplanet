//
//  buggy.h
//  wptest
//
//  Created by fletcher on 2022/10/06.
//

#ifndef buggy_h
#define buggy_h

#include <cstring>
#include <map>
#include <queue>

void workerThread();
bool readInput(FILE* in, std::string & strLinebuf);
void readInputWords(FILE* in);
void lookupWords(FILE* in);

extern bool shutdownFlag;
extern std::map<std::string, int> wordsMap;
extern std::queue<std::string> wordQueue;

#endif /* buggy_h */
