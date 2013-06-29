using namespace std;

#include "fetcher.hpp"
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>

/**
 * Helper functions to talk to cUrl.
 */

static size_t cb_curl_write(void * buf, size_t size, size_t num, void *user_out)
{
    if (user_out)
    {
        ostream& output = * static_cast<ostream *>(user_out);
        streamsize len = size * num;
        if (output.write(static_cast <char *>(buf), len))
        {
            return len;
        }
    }
    return 0;
}

CURLcode do_curl_read(const string& url, ostream& output)
{
    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();

    if(curl)
    {
        if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &cb_curl_write))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &output))
                && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
        {
            code = curl_easy_perform(curl);
        }
        curl_easy_cleanup(curl);
    }
    return code;
}

string do_url_fetch(string url)
{
    ostringstream output;
    CURLcode code;
    if (CURLE_OK == (code = do_curl_read(url, output)))
    {
        return output.str();
    }
    cerr << curl_easy_strerror(code) << endl;
    exit (1);
}

/**
 * Done with helper functions.  To use, call (string data = du_url_fetch(string url));
 */

Fetcher::Fetcher(string new_url)
{
    this->url = string(new_url);
    this->data = string("");
}

string Fetcher::fetch()
{
    if (this->data.length())
        return this->data;
    return (this->data = do_url_fetch(this->url));
}

Fetcher::~Fetcher()
{
    return;
}
