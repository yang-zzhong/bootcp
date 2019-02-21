#include "url.h"

namespace boohttp 
{
    Url::Url(string url)
    {
        auto pos = url.find("://");
        if (pos != string::npos) {
            schema = url.substr(0, pos);
            url = url.substr(pos + 3, url.length() - pos - 3);
        }
        list<string> seps;
        seps.push_back("/");
        seps.push_back("?");
        seps.push_back("#");
        handleUser(url);
        handleDomain(parseUrl(url, seps));
        if (url.length() == 0) {
            return;
        }
        seps.pop_front();
        path = parseUrl(url, seps);
        if (url.length() == 0) {
            return;
        }
        seps.pop_front();
        string q = parseUrl(url, seps);
        if (q.length() > 1) {
            query = parseQuery(q.substr(1, q.length() - 1));
        }
        if (url.length() == 0) {
            return;
        }
        seps.pop_front();
        string h = parseUrl(url, seps);
        if (h.length() > 1) {
            hash = h.substr(1, h.length() - 1);
        }
    }

    void Url::handleDomain(const string & domain)
    {
        size_t pos = domain.find(":");
        if (pos == string::npos) {
            host = domain;
            return;
        }
        host = domain.substr(0, pos);
        port = domain.substr(pos + 1, domain.length() - pos - 1);
    }

    void Url::handleUser(string & url)
    {
        size_t pos = url.find("@");
        if (pos == string::npos) {
            return;
        }
        string user = url.substr(0, pos);
        url = url.substr(pos + 1, user.length() - pos - 1);
        pos = user.find(":");
        if (pos == string::npos) {
            username = user;
            return;
        }
        username = user.substr(0, pos);
        password = user.substr(pos + 1, user.length() - pos - 1);
    }

    string Url::str()
    {
        string url = schema + "://";
        if (username.length() > 0 && password.length() > 0) {
            url += username + ":" + password + "@";
        } else if (username.length() > 0) {
            url += username + "@";
        }
        url += host;
        if (port.length() != 0) {
            url += ":" + port;
        }
        url += path;
        string qs = queryString(query);
        if (qs.length() > 0) {
            url += "?" + qs;
        }
        if (hash.length() > 0) {
            url += "#" + hash;
        }

        return  url;
    }

    string Url::queryString(map<string, string> queries)
    {
        list<string> m;
        int len = 0;
        for (auto i = queries.begin(); i != queries.end(); ++i) {
            m.push_back(i->first + "=" + i->second);
            ++len;
        }
        int count = 0;
        string ret = "";
        for (auto i = m.begin(); i != m.end(); ++i) {
            ret += *i;
            if (count++ < len - 1) {
                ret += "&";
            }
        }

        return ret;
    }

    map<string, string> Url::parseQuery(string query)
    {
        map<string, string> ret;
        size_t pos;
        while((pos = query.find('&')) != string::npos) {
            string slice = query;
            if (pos != string::npos) {
                slice = query.substr(0, pos);               
                query = query.substr(pos + 1, query.length() - pos - 1);
            }
            handleSlice(ret, &slice);
        }
        handleSlice(ret, &query);

        return ret;
    }

    void Url::handleSlice(map<string, string> & ret, string * slice)
    {
        size_t pos = slice->find('=');
        if (pos == string::npos) {
            ret[*slice] = "";
            return;
        }
        string key = slice->substr(0, pos);
        string value = slice->substr(pos + 1, slice->length() - pos - 1);
        ret[key] = value;
    }

    string Url::parseUrl(string & url, list<string> seps)
    {
        for (auto i = seps.begin(); i != seps.end(); ++i) {
            auto found = url.find(*i);
            if (found == string::npos) {
                continue;
            }
            string ret = url.substr(0, found);
            auto pos = found;
            url = url.substr(pos, url.length() - pos);
            return ret;
        }
        string ret = url.substr(0, url.length());
        url = "";

        return ret;
    }

    string Url::escape(const string & url)
    {
        ostringstream escaped;
        escaped.fill('0');
        escaped << hex;

        for (string::const_iterator i = url.begin(), n = url.end(); i != n; ++i) {
            string::value_type c = (*i);
            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }
            // Any other characters are percent-encoded
            escaped << uppercase;
            escaped << '%' << setw(2) << int((unsigned char) c);
            escaped << nouppercase;
        }

        return escaped.str();
    }

    string Url::unescape(const string & url)
    {
        string ret;
        char ch;
        int i, ii;
        for (i=0; i<url.length(); i++) {
            if (int(url[i]) == 37) {
                ret+=url[i];
                continue;
            }
            sscanf(url.substr(i+1,2).c_str(), "%x", &ii);
            ch = static_cast<char>( ii );
            ret += ch;
            i = i+2;
        }
        return ret;
    }
}