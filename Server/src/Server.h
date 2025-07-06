#pragma once

#include <string>
#include <unordered_map>

class Server
{
public:
    static const char* ParseInput(const char* input);
    static const char* PushData(const char* input);
    static const char* RemoveData(const char* input);
    static const char* ListEntries();
    static const char* GetEntryData(const char* input);
    static const char* RunCurl();

private:
    static inline std::unordered_map<std::string, std::string> s_CurlList = std::unordered_map<std::string, std::string>(); 
    static inline std::string s_MSG = std::string();
};
