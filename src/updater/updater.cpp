#include "updater.h"

std::string getServersideVersion(Consensus::Params& params)
{
    return downloadSHA(params.nUpdateLocation + "version");
}


void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65])
{
    int i = 0;

    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }

    outputBuffer[64] = 0;
}

std::string calc_sha256(const char* path)
{
    char output[65];
    FILE* file = fopen(path, "rb");
    if (!file)
        return "";

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    char* buffer = (char*)malloc(bufSize);
    memset(buffer, 0, bufSize);
    int bytesRead = 0;
    if (!buffer)
        return "";
    while ((bytesRead = fread(buffer, 1, bufSize, file))) {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);

    sha256_hash_string(hash, output);
    fclose(file);
    free(buffer);

    return std::string(output, SHA256_DIGEST_LENGTH * 2);
}


int progress_callback(void* p, double dltotal, double dlnow, double ultotal, double ulnow)
{
    // double dl, dt;
    // dl = dlnow / 1024 / 1024;
    // dt = dltotal / 1024 / 1024;
    //double perc = dl / dt * 100;

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
        return MACOSX;
    else
        return UNKNOWN;
}

#ifndef WIN32
std::string getexepath()
{
#ifndef MAC_OSX
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
#else
    int ret;
    pid_t pid;
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];

    pid = getpid();
    ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
    return std::string(pathbuf, (ret > 0) ? ret : 0);

#endif
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
    if (getFileName(getexepath()).compare(oldFile) == 0) {
        res = rename(oldFile, "temp~");
    } else
        res = remove(oldFile); //this also calls unlink

    if (res == 0) {
        rename(newFile, oldFile);
#ifdef MAC_OSX
        chmod(oldFile, S_IRWXU | S_IRWXG | S_IRWXO);
#endif
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
    if (file_exist(file.c_str())) {
        remove((file + "_tmp").c_str());
        if (downloadFile(params.nUpdateLocation + os + "/" + bits + "/" + file, file + "_tmp")) {
            std::string onlineSha = downloadSHA(params.nUpdateLocation + os + "/" + bits + "/" + file + "_sha");
            std::string localSha = calc_sha256((file + "_tmp").c_str());
            if (onlineSha.compare(localSha) != 0) {
                fprintf(stderr, "\nupgrade of '%s' failed:\n sha256 signature mismatch\n\n%s\n%s\n", file.c_str(), onlineSha.c_str(), localSha.c_str());
                return false;
            }
            if (!updateFile(file.c_str(), (file + "_tmp").c_str()))
                return false;
        } else {
            fprintf(stderr, "upgrade of '%s' failed: file not found or server error\nUpdate aborted.\n", file.c_str());
            return false;
        }
    }
#else

    if (file_exist((file + ".exe").c_str())) {
        if (downloadFile(params.nUpdateLocation + os + "/" + bits + "/" + file + ".exe", file + "_tmp")) {
            std::string onlineSha = downloadSHA(params.nUpdateLocation + os + "/" + bits + "/" + file + "_sha");
            std::string localSha = calc_sha256((file + "_tmp").c_str());
            if (onlineSha.compare(localSha) != 0) {
                fprintf(stderr, "\nupgrade of '%s' failed:\n sha256 signature mismatch\n\n", file.c_str());
                return false;
            }
            if (!updateFile((file + ".exe").c_str(), (file + "_tmp").c_str()))
                return false;
        } else {
            fprintf(stderr, "upgrade of '%s.exe' failed: file not found or server error\nUpdate aborted.\n", file.c_str());
            return false;
        }
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
    if (!downloadUpdatefile(params, os, bits, "stonecoin-qt"))
        return false;
    return true;
}


bool downloadUpdate(Consensus::Params& params)
{
    //boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); //needed ?
    remove("temp~");

    std::string version = getServersideVersion(params);
    if (version.compare(FormatFullVersion()) == 0) {
        fprintf(stdout, "skipping update, already updated '%s'\n", FormatFullVersion().c_str());
        return false;
    }

    fprintf(stdout, "Updating from '%s' -> '%s'\n", FormatFullVersion().c_str(), version.c_str());

    std::string runningApp = getexepath();
    fprintf(stderr, "path:%s", runningApp.c_str());
    switch (getCurrentOs()) {
    case WINDOWS_32: {
        if (!doupdate(params, "windows", "32"))
            return false;
        
        break;
    }
    case WINDOWS_64: {
        if (!doupdate(params, "windows", "64"))
            return false;
        break;
    }
    case LINUX_32: {
        if (!doupdate(params, "linux", "32"))
            return false;
        break;
    }
    case LINUX_64: {
        if (!doupdate(params, "linux", "64"))
            return false;
        break;
    }
    case MACOSX: {
        if (!doupdate(params, "mac", "osx"))
            return false;
        break;
    }
    case PI2: {
        if (!doupdate(params, "pi", "2"))
            return false;
        break;
    }
    default: {
    }
    }

    //PrepareShutdown();
    //-delay-start
    
	//execlp(runningApp.c_str(), getFileName(runningApp).c_str(), "-delay-start", NULL); //replaces current running process with a new image
    //fprintf(stderr, "failed to restart '%s'\nCan not recover, Please relaunch manually\n", getFileName(runningApp).c_str());
    return true;
}


static std::string readBuffer;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    readBuffer.append((char*)contents, realsize);
    return realsize;
}

std::string downloadSHA(std::string url)
{
    CURL* curl;
    CURLcode res;

    readBuffer = "";
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return readBuffer;
}

bool downloadFile(std::string url, std::string saveas)
{
    CURL* curl;
    CURLcode res = CURLE_OK;

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
