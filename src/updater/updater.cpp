#include "updater.h"
#include <boost/filesystem/operations.hpp>
#include <stdio.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>

#include <univalue.h>

/** Reply structure for request_done to fill in */
struct HTTPReply
{
    int status;
    std::string body;
};

void CUpdater::http_request_done(struct evhttp_request *req, void *ctx)
{
    HTTPReply *reply = static_cast<HTTPReply*>(ctx);

    if (req == NULL) {
        /* If req is NULL, it means an error occurred while connecting, but
         * I'm not sure how to find out which one. We also don't really care.
         */
        reply->status = 0;
        return;
    }

    reply->status = evhttp_request_get_response_code(req);

    struct evbuffer *buf = evhttp_request_get_input_buffer(req);
    if (buf)
    {
        size_t size = evbuffer_get_length(buf);
        const char *data = (const char*)evbuffer_pullup(buf, size);
        if (data)
            reply->body = std::string(data, size);
        evbuffer_drain(buf, size);
    }
}


bool CUpdater::GetFile(const std::string& serverName, const std::string& getCommand)
{
    struct event_base *base = event_base_new();
    if (!base)
    {
        lastError = "cannot create event_base";
        return false;
    }

    struct evhttp_connection *evcon = evhttp_connection_base_new(base, NULL, serverName.c_str(), 80); 
    if (evcon == NULL)
    {
        lastError = "create connection failed";
        return false;
    }
    evhttp_connection_set_timeout(evcon, 2);

    HTTPReply response;
    struct evhttp_request *req = evhttp_request_new(http_request_done, (void*)&response);
    if (req == NULL)
    {
        lastError = "create http request failed";
        return false;
    }

    struct evkeyvalq *output_headers = evhttp_request_get_output_headers(req);
    assert(output_headers);
    evhttp_add_header(output_headers, "Host", host.c_str());
    //TODO: add header for binary transfer

    int r = evhttp_make_request(evcon, req, EVHTTP_REQ_GET, getCommand);
    if (r != 0) {
        evhttp_connection_free(evcon);
        event_base_free(base);
        lastError = "send http request failed";
        return false;
    }

    event_base_dispatch(base);
    evhttp_connection_free(evcon);
    event_base_free(base);

    if (response.status == 0)
        lastError = "couldn't connect to server";
    else if (response.status == HTTP_UNAUTHORIZED)
        lastError = "authorization failed";
    else if (response.status >= 400 && response.status != HTTP_BAD_REQUEST && response.status != HTTP_NOT_FOUND && response.status != HTTP_INTERNAL_SERVER_ERROR)
        lastError = strprintf("server returned HTTP error %d", response.status);
    else if (response.body.empty())
        lastError = "no response from server";
    else
    {
        //TODO: store file

        return true;
    }
    return false;
}

std::string CUpdater::getRemoteVersion()
{
}

std::string CUpdater::getLocalVersion()
{
}

bool CUpdater::updateAvailable()
{
}

bool CUpdater::downloadFile(std::string server, std::string getfile, std::string savefile)
{
    string serverName = server;
    string getCommand = "/" + getfile;

//    std::ofstream outFile(savefile, std::ofstream::out | std::ofstream::binary);
    bool res = GetFile(serverName, getCommand);
//    outFile.close();
}

bool CUpdater::renameBinary(std::string newName, std::string oldName)
{
}

bool CUpdater::launchUpdatedBinary(char args[], int argv, std::string fileName)
{
}