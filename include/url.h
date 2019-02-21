#ifndef _BOO_HTTP_CLIENT_URL_H_
#define _BOO_HTTP_CLIENT_URL_H_

#include <string>
#include <map>
#include <list>
#include <sstream>
#include <cctype>
#include <iomanip>

using namespace std;

namespace boohttp 
{
    class Url 
    {
    public:   
        Url() {};
        Url(string url);
        static map<string, string> parseQuery(string query);
        static string queryString(map<string, string> query);
        static string escape(const string & url);
        static string unescape(const string & url);
        string str();
    public:
        string schema = "https";
        string username = "";
        string password = "";
        string host;
        string port;
        string path;
        map<string, string> query;
        string hash;
    private:
        string parseUrl(string & url, list<string> seps);
        void handleUser(string & user);
        void handleDomain(const string & domain);
        static void handleSlice( map<string, string> & ret, string * slice);
    };
}

#endif