#pragma once

#include <string>
#include <unordered_map>

class Server
{
public:
    static const char* ParseInput(const char* input);
    static const char* PushData(const char* input);
    static const char* EditData(const char* input);
    static const char* RemoveData(const char* input);
    static const char* ListEntries();
    static const char* ClearEntries();
    static const char* GetEntryData(const char* input);

    static const char* ParseSetInput(const char* input);
    static const char* ParseGetInput(const char* input);

    static const char* SetWorkingDirectory(const char* input);
    static const char* GetWorkingDirectory();

    static const char* SetCurrentExtension(const char* input);
    static const char* GetCurrentExtension();

    static const char* SetCurrentSeriesName(const char* input);
    static const char* GetCurrentSeriesName();

    static const char* SetCurrentSeason(const char* input);
    static const char* GetCurrentSeason();

    static const char* RunCurl();

private:
    static inline std::unordered_map<std::string, std::string> s_CurlList = std::unordered_map<std::string, std::string>(); 
    static inline std::string s_MSG = std::string();

    static inline std::string s_SetWorkingDiretory = "";
    static inline std::string s_SetExtension = "";

    static inline std::string s_SetSeriesName = "";
    static inline std::string s_SetSeason = "";
};
