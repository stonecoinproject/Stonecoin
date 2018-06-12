#include "updater.h"

int progress_callback(void* p, double dltotal, double dlnow, double ultotal, double ulnow)
{
    double dl, dt;
    dl = dlnow / 1024 / 1024;
    dt = dltotal / 1024 / 1024;
    double perc = dl / dt * 100;

    // printf("%f : %f : %d%s\n", dl, dt, (int)perc, "%");

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

#ifndef WIN32
std::string getexepath()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}
#else
std::string getexepath()
{
    char result[MAX_PATH];
    return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}
#endif

std::string getFileName(const std::string& s)
{
    char sep = '/';

#ifdef _WIN32
    sep = '\\';
#endif

    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos) {
        return (s.substr(i + 1, s.length() - i));
    }

    return ("");
}


bool updateFile(const char* oldFile, const char* newFile)
{
    int res = 0;
    if (getFileName(getexepath()).compare(oldFile) == 0)
        res = rename(oldFile, "temp~");
    else
        res = remove(oldFile); //this also calls unlink

    if (res == 0) {
        rename(newFile, oldFile);
        fprintf(stdout, "%s updated.\n", oldFile);
    } else {
        fprintf(stderr, "unlink failed on '%s' error: %d update aborted.\n path: %s\n", oldFile, errno, getexepath().c_str());
        return false;
    }
    return true;
}

bool downloadUpdatefile(Consensus::Params& params, std::string os, std::string bits, std::string file)
{
#ifndef WIN32
    if (downloadFile(params.nUpdateLocation + os + "/" + bits + "/" + file, file + "_tmp")) {
        if (!updateFile(file.c_str(), (file + "_tmp").c_str()))
            return false;
    } else {
        fprintf(stderr, "upgrade of '%s' failed: file not found or server error\nUpdate aborted.\n", file);
        return false;
    }
#else
    if (downloadFile(params.nUpdateLocation + os + "/" + bits + "/" + file + ".exe", file + "_tmp")) {
        if (!updateFile((file + ".exe").c_str(), (file + "_tmp").c_str()))
            return false;
    } else {
        fprintf(stderr, "upgrade of '%s.exe' failed: file not found or server error\nUpdate aborted.\n", file);
        return false;
    }
#endif

    return true;
}


bool doupdate(Consensus::Params& params, std::string os, std::string bits)
{
    if (!downloadUpdatefile(params, os, bits, "stonecoind"))
        return false;
    if (!downloadUpdatefile(params, os, bits, "stonecoin-cli"))
        return false;
    if (!downloadUpdatefile(params, os, bits, "stonecoin-tx"))
        return false;
    return true;
}


void downloadUpdate(Consensus::Params& params)
{
    //boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); //needed ?
    remove("temp~");
    std::string runningApp = getexepath();

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
        if (!doupdate(params, "linux", "32"))
            return;

        execlp(runningApp.c_str(), getFileName(runningApp).c_str(), NULL); //replaces current running process with a new image
        fprintf(stderr, "failed to restart '%s'\n", getFileName(runningApp).c_str());
        break;
    }
    case LINUX_64: {
        if (!doupdate(params, "linux", "64"))
            return;

        execlp(runningApp.c_str(), getFileName(runningApp).c_str(), NULL); //replaces current running process with a new image
        fprintf(stderr, "failed to restart '%s'\n", getFileName(runningApp).c_str());
        break;
    }
    case MAC_OSX: {
        if (!doupdate(params, "mac", "osx"))
            return;

        execlp(runningApp.c_str(), getFileName(runningApp).c_str(), NULL); //replaces current running process with a new image
        fprintf(stderr, "failed to restart '%s'\n", getFileName(runningApp).c_str());
        break;
    }
    case PI2: {
        if (!doupdate(params, "pi", "2"))
            return;

        execlp(runningApp.c_str(), getFileName(runningApp).c_str(), NULL); //replaces current running process with a new image
        fprintf(stderr, "failed to restart '%s'\n", getFileName(runningApp).c_str());
        break;
    }
    default: {
    }
    }
}


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
