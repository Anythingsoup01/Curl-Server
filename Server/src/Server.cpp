#include "Server.h"

#include <algorithm>
#include <vector>
#include <sstream>
#include <string.h>

const char* Server::ParseInput(const char* input)
{
    std::string check(input);
    check.erase(remove_if(check.begin(), check.end(), isspace), check.end());
    if (check.empty())
    {
        return "";
    }

    if (strncmp(input, "exit", 4) == 0)
        return "Goodbye!";
    if (strncmp(input, "shutdown", 8) ==0)
        return "Shutting down server.";
    if (strncmp(input, "list", 4) == 0)
        return ListEntries();
    if (strncmp(input, "read", 4) == 0)
        return GetEntryData(input);
    if (strncmp(input, "add", 3) == 0)
        return PushData(input);
    if (strncmp(input, "remove", 6) == 0)
        return RemoveData(input);
    if (strncmp(input, "curl", 4) == 0)
        return RunCurl();

    return "Command Not Found";
}

const char* Server::ListEntries()
{
    if (!s_MSG.empty())
        s_MSG.erase();

    if (s_CurlList.empty())
        return "List is currently empty.";

    for(auto& [key, url] : s_CurlList)
    {
        s_MSG.append(key);
        s_MSG.append("\n");
    }

    return s_MSG.data();
}

const char* Server::GetEntryData(const char* input)
{
    std::string key(input);
    key.erase(0, 5);
    key.erase(remove_if(key.begin(), key.end(), isspace), key.end());
    printf("KEY: %s\n", key.c_str());

    if (s_CurlList.empty())
        return "List is currently empty.";

    if (key.empty())
        return "Please provide a key! read <key>";

    if (!s_CurlList.contains(key))
        return "Failed to locate item!";

    return s_CurlList.at(key.data()).c_str();
}

const char* Server::PushData(const char *input)
{
    // This will only come from the 'add' command, so we can ignore the first 4 bytes
    std::string in(input); 
    in.erase(0, 4);
    size_t space = in.find(' ');
    size_t eol = in.find("\r\n");

    if (space == std::string::npos)
        return "No space seperating filename & url.";

    std::string name = in.substr(0, space);
    std::string url = in.substr(space + 1, eol);

    printf("NAME: %s\n", name.c_str());
    printf("URL: %s\n", url.c_str());

    name.erase(remove_if(name.begin(), name.end(), isspace), name.end());
    url.erase(remove_if(url.begin(), url.end(), isspace), url.end());

    if (name.empty() || url.empty())
        return "Filename and/or URL missing!";

    s_CurlList.emplace(std::make_pair(name, url));
    return "Successfully added.";
}

const char* Server::RemoveData(const char *input)
{
    std::string in(input);
    in.erase(0, 7);

    std::string check = in;
    check.erase(remove_if(check.begin(), check.end(), isspace), check.end());
    if (check.empty())
        return "No key provided.";

    std::vector<std::string> listToRemove;
    size_t pos = 0;
    size_t eol = in.find("\r\n");
    while (pos != std::string::npos)
    {
        size_t eow = in.find_first_of(" ", pos + 1);

        std::string word = in.substr(pos, eow - pos);
        word.erase(remove_if(word.begin(), word.end(), isspace), word.end());
        listToRemove.push_back(word);

        pos = eow;
    }
    bool failed = false;
    s_MSG = "Failed: \n";
    for (auto& key : listToRemove)
    {
        auto it = s_CurlList.find(key);
        if (it == s_CurlList.end())
        {
            failed = true;
            s_MSG.append(key);
            s_MSG.append("\n");
        }
        else
            s_CurlList.erase(it);
    }

    if (failed)
        return s_MSG.c_str();

    return "Successfully removed.";

}

const char* Server::RunCurl()
{
    if (s_CurlList.empty())
        return "List is currently empty.";

    std::unordered_map<std::string, std::string> newList;

    s_MSG.clear();
    s_MSG.append("Failed to curl: ");
    for (auto& [key, url] : s_CurlList)
    {
        std::stringstream ss;
        ss << "curl '" << url << "' -A 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 OPR/119.0.0.0' --output '" << key << "'";
        if (system(ss.str().c_str()) != 0)
        {
            newList.emplace(std::make_pair(key, url));
            std::stringstream err;
            err << key << "\n";
            s_MSG.append(err.str());
        }
    }

    if (!newList.empty())
    {
        return s_MSG.c_str();
    }

    return "Successfully Curled";
}
