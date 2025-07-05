#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <sstream>
#include <algorithm>

#include <unordered_map>
#include <vector>

class ServerStorage
{
    public:
        static bool PushData(const char* input)
        {
            // This will only come from the 'add' command, so we can ignore the first 4 bytes
            std::string in(input); 
            in.erase(0, 4);
            size_t space = in.find(' ');
            size_t eol = in.find("\r\n");

            if (space == std::string::npos)
                return false;

            std::string name = in.substr(0, space);
            std::string url = in.substr(space + 1, eol);

            name.erase(remove_if(name.begin(), name.end(), isspace), name.end());
            url.erase(remove_if(url.begin(), url.end(), isspace), url.end());

            if (name.empty() || url.empty())
                return false;

            s_CurlList.emplace(std::make_pair(name, url));

            return true;
        }

        static bool RemoveData(const char* input)
        {
            // This will only come from the 'remove' command, so we can ignore the first 7 bytes
            std::string in(input);
            in.erase(0, 6); // This should technically be all, but we will assume there are multiple names in here
            
            std::string check = in;
            check.erase(remove_if(check.begin(), check.end(), isspace), check.end());
            if (check.empty())
                return false;

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

            for (auto& key : listToRemove)
            {
                auto it = s_CurlList.find(key);
                if (it != s_CurlList.end())
                    s_CurlList.erase(it);
            }

            return true;
        }

        static const char* ListEntries()
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

        static const char* GetEntryData(const char* key)
        {
            std::string sKey(key);
            sKey.erase(0, 5);

            if (s_CurlList.empty())
                return "List is currently empty.";

            if (sKey.empty())
                return "Please provide a key! read <key>";

            auto it = s_CurlList.find(sKey);
            if (it == s_CurlList.end())
                return "Key not found!";
            return s_CurlList.at(sKey).data();
        }

        static const char* Run()
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
        
    private:
        //                        File Name    url
        static inline std::unordered_map<std::string, std::string> s_CurlList = std::unordered_map<std::string, std::string>();
        static inline std::string s_MSG = std::string();
};

const int MAX = 4096;

int fd;
struct sockaddr_in addr;


const char* ParseInput(const char* input)
{
    if (strncmp(input, "exit", 4) == 0)
        return "Goodbye!";

    if (strncmp(input, "shutdown", 8) == 0)
        return "Shutting down server.";

    if (strncmp(input, "list", 4) == 0)
        return ServerStorage::ListEntries();

    if (strncmp(input, "read", 4) == 0)
        return ServerStorage::GetEntryData(input);

    if (strncmp(input, "add", 3) == 0)
    {
        if (!ServerStorage::PushData(input))
        {
            return "Failed to upload data!";
        }

        return "Upload successful!";
    }

    if (strncmp(input, "curl", 4) == 0)
        return ServerStorage::Run();

    if (strncmp(input, "remove", 6) == 0)
    {
        if (!ServerStorage::RemoveData(input))
        {
            return "Failed to locate data!\nremove <key> - where key is the name you provided!";
        }

        return "Removal successful!";
    }

    return "Command Not Found!";
}

int main(int argc, char* argv[])
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Failed to create socket!\n");
        return -1;
    }

    int enable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        printf("Failed to set socket option!");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(8080);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        printf("Failed to bind socket!\n");
        return -1;
    }

    listen(fd, 0);

    printf("Server is Running!\n");
    
    while (true)
    {
        int addrlen = sizeof(addr);
        int new_socket = accept(fd, (struct sockaddr*)&addr, (socklen_t*)&addrlen);

        if (new_socket < 0)
        {
            printf("Failed to accept connection.\n");
            return(-1);
        }

        printf("Client connected.\n");

        char buffin[MAX];
        char* buffout;
        while (true)
        {
            bzero(buffin, sizeof(buffin));
            read(new_socket, buffin, sizeof(buffin));
            buffout = (char*)ParseInput(buffin);
            printf("client: %s", buffin);
            printf("expected out: %s\n", buffout);
            write(new_socket, buffout, strlen(buffout));
            if (strncmp(buffin, "exit", 4) == 0 | strncmp(buffin, "shutdown", 8) == 0)
                break;
        }


        printf("Client not connected.\n");

        close(new_socket);

        if (strncmp(buffin, "shutdown", 8) == 0)
            break;

    }

    close(fd);

    return 0;
}
