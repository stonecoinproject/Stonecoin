// Copyright (c) 2018 Stone Core dev
#include <fstream>
#include <string>
#include <boost/asio.hpp>

#ifndef UPDATER_H
#define UPDATER_H

using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;

class CUpdater
{
    private:
        std::string serverHost;
        std::string getFile;
        std::string filename;

        bool GetFile(const std::string& serverName, const std::string& getCommand);
        void http_request_done(struct evhttp_request *req, void *ctx);
    public:
        std::string getRemoteVersion();
        std::string getLocalVersion();
        bool updateAvailable();
        bool downloadFile(std::string server,std::string getfile, std::string savefile);
        bool renameBinary(std::string newName,std::string oldName);
        bool launchUpdatedBinary(char args[], int argv, std::string fileName);
};
#endif