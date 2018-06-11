#include "updater.h"


#include <curl/easy.h>
#include <curl/curl.h>
#include <fcntl.h> /// POSIX file control
#include <iostream>
#include <string>
#include <unistd.h>
using namespace boost::filesystem;
static const int DEFAULT_HTTP_UPDATER_TIMEOUT = 1200;
struct HTTPReply {
    int status;
    std::string body;
};
static struct evkeyvalq* input_headers;
static void http_request_done(struct evhttp_request* req, void* ctx)
{
    HTTPReply* reply = static_cast<HTTPReply*>(ctx);

    input_headers = evhttp_request_get_output_headers(req);


    struct evkeyval* header;

    TAILQ_FOREACH(header, input_headers, next)
    {
        fprintf(stdout, "%s : %s", header->key, header->value);
    }


    if (req == NULL) {
        /* If req is NULL, it means an error occurred while connecting, but
         * I'm not sure how to find out which one. We also don't really care.
         */
        reply->status = 0;
        return;
    }

    reply->status = evhttp_request_get_response_code(req);

    fprintf(stdout, "STATUS: %d\n", reply->status);

    ofstream ofs(path("test.tar.gz"), ofstream::binary);
    struct evbuffer* buf = evhttp_request_get_input_buffer(req);


    size_t size = evbuffer_get_length(buf);

    const char* data = (const char*)evbuffer_pullup(buf, size);
    if (data) {
        char* data2;
        data2 = (char*)malloc(size);
        evbuffer_copyout(buf, data2, size);
        ofs.write(data2, size);

        free(data2);

        fprintf(stdout, "FILE: test.zip written, size in bytes: %d\n", size);


        reply->body = std::string(data, size);


        //                evbuffer_drain(buf, size);
        //               fprintf(stdout, "REQUEST_DONE:\nHTTP_STATUS: %d\n", reply->status);
    }
    //		*/
    ofs.close();

    evbuffer_drain(buf, size);
}

//test curl
void downloadUpdate(Consensus::Params* params)
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/");

#ifdef SKIP_PEER_VERIFICATION
        /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
        /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}


