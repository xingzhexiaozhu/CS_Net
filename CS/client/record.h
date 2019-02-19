#ifndef RECORD_H_
#define RECORD_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <stddef.h>

std::unordered_map<std::string, int> Cmd =
{
    {"HMSET", 0},
    {"HINCRBY", 1},
    {"HDEL", 2},
    {"RENAME", 3},
    {"DEL", 4}
};

struct Record {
    uint64_t timestamp;
    int cmd;
    std::string key;
    std::vector<std::string> args;
    Record* prev;
    Record* next;
};

#endif // RECORD_H_