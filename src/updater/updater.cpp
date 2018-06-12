#include "updater.h"

int progress_callback(void* p, double dltotal, double dlnow, double ultotal, double ulnow)
{
    //struct myprogress* myp = (struct myprogress*)p;
    //CURL* curl = myp->curl;

    double dl, dt;
    dl = dlnow / 1024 / 1024;
    dt = dltotal / 1024 / 1024;
    double perc = dl / dt * 100;

    printf("%f : %f : %d%s\n", dl, dt, (int)perc, "%");

    return 0;
    //return 1 to stop
}

updater_ostype getCurrentOs()
{
    if (BOOST_OS_WINDOWS) {
        if (BOOST_ARCH_X86_64) {
            return WINDOWS_64;
        } else if (BOOST_ARCH_X86 || BOOST_ARCH_X86_32) {
            return WINDOWS_32;
        }
    } else if (BOOST_OS_LINUX) {
        if (BOOST_ARCH_ARM) // Pi2
        {
            return PI2;
        } else {
            if (BOOST_ARCH_X86_64) {
                return LINUX_64;
            } else if (BOOST_ARCH_X86 || BOOST_ARCH_X86_32) {
                return LINUX_32;
            }
        }
    } else if (BOOST_OS_MACOS)
        return MAC_OSX;
    else
        return UNKNOWN;
}


bool updateFile(const char* oldFile, const char* newFile)
{
    int res = remove(oldFile); //this also calls unlink
    if (res == 0) {
        rename(newFile, oldFile);
        fprintf(stdout, "%s updated.\n", oldFile);
    } else {
        fprintf(stderr, "unlink failed on '%s' update aborted.\n", oldFile);
        return false;
    }
    return true;
}


void downloadUpdate(Consensus::Params& params)
{
    //test download
    //downloadFile("https://github.com/tomevoll/stonecoin/releases/download/v1.0.0.3-pre1/stonecrypto-1.0.0.3-linux64.tar.gz", "temp.tar.gz");
    switch (getCurrentOs()) {
    case WINDOWS_32: {
        printf("%s", "WINDOWS_32");
        return;
        break;
    }
    case WINDOWS_64: {
        printf("%s", "WINDOWS_64");
        return;
        break;
    }
    case LINUX_32: {
        printf("%s", "LINUX_32\n");
        bool bDaemon = downloadFile(params.nUpdateLocation + "linux/32/stonecoind", "stonecoind_tmp");
        if (bDaemon) {
			if (!updateFile("stonecoind", "stonecoind_tmp"))
                return;
        } else {
            fprintf(stderr, "upgrade of stonecoind failed: file not found or server error\nUpdate aborted.\n");
            return;
        }

        bool bCli = downloadFile(params.nUpdateLocation + "linux/32/stonecoin-cli", "stonecoin-cli_tmp");
        if (bCli) {
            if (!updateFile("stonecoin-cli", "stonecoin-cli_tmp")) 
                return;
        } else {
            fprintf(stderr, "upgrade of stonecoin-cli failed: file not found or server error\nUpdate aborted.\n");
            return;
        }
		
		bool bTx = downloadFile(params.nUpdateLocation + "linux/32/stonecoin-tx", "stonecoin-tx_tmp");
        if (bTx) {
            if (!updateFile("stonecoin-tx", "stonecoin-tx_tmp"))
                return;
        } else {
            fprintf(stderr, "upgrade of stonecoin-tx failed: file not found or server error\nUpdate aborted.\n");
            return;
        }

		//detect running file stonecoind/stonecoin-qt and relaunch here

		break;
    }
    case LINUX_64: {
        printf("%s", "LINUX_64");
        bool bDaemon = downloadFile(params.nUpdateLocation + "linux/64/stonecoind", "stonecoind_tmp");
        if (bDaemon) {
            if (!updateFile("stonecoind", "stonecoind_tmp"))
                return;
        } else {
            fprintf(stderr, "upgrade of stonecoind failed: file not found or server error\nUpdate aborted.\n");
            return;
        }

        bool bCli = downloadFile(params.nUpdateLocation + "linux/64/stonecoin-cli", "stonecoin-cli_tmp");
        if (bCli) {
            if (!updateFile("stonecoin-cli", "stonecoin-cli_tmp"))
                return;
        } else {
            fprintf(stderr, "upgrade of stonecoin-cli failed: file not found or server error\nUpdate aborted.\n");
            return;
        }

        bool bTx = downloadFile(params.nUpdateLocation + "linux/64/stonecoin-tx", "stonecoin-tx_tmp");
        if (bTx) {
            if (!updateFile("stonecoin-tx", "stonecoin-tx_tmp"))
                return;
        } else {
            fprintf(stderr, "upgrade of stonecoin-tx failed: file not found or server error\nUpdate aborted.\n");
            return;
        }

		//detect running file stonecoind/stonecoin-qt and relaunch here

        break;
    }
    case MAC_OSX: {
        break;
    }
    case PI2: {
        break;
    }
    default: {
    }
    }
}


//test curl
bool downloadFile(std::string url, std::string saveas)
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); //Follow any redirection github uses
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
        //curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

        FILE* f = fopen(saveas.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
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
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(f);
    }

    curl_global_cleanup();

    return res == CURLE_OK;
}
