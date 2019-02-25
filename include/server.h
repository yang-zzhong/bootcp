#ifndef _BOO_TCP_SERVER_H
#define _BOO_TCP_SERVER_H

#include "bootcp.h"
#include "openssl/ssl.h"

#include <thread>
#include <mutex>
#include <map>
#include <set>

namespace bootcp
{
    template<class T> class Server : public BooTcp<T>
    {
    public:
        Server() : BooTcp<T>()
        {
        };
        Server(int port) : BooTcp<T>()
        {
            listen(port);
        };
        bool ready()
        {
            return _running;
        };
        bool listen(int port)
        {
            if (_running) {
                return true;
            }
            _fd = socket(AF_INET, SOCK_STREAM, 0);
            if (!BooTcp<T>::isOK(_fd)) {
                return false;
            }
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            int ecode;
            ecode = ::bind(_fd, (const sockaddr*)&addr, sizeof(addr));
            if (!BooTcp<T>::isOK(ecode)) {
                return false;
            }
            ecode = ::listen(_fd, 5);
            if (!BooTcp<T>::isOK(ecode)) {
                return false;
            }
            _running = true;
            std::thread accept(&bootcp::Server<T>::accept, this);
            accept.detach();

            return true;
        };
        void stop()
        {
            _running = false;
            closeClients();
            BooTcp<T>::close(fd());
        };
        void enable(Sock fd)
        {
            if (!has(fd)) {
                return;
            }
            _clock.lock();
            clients[fd] = true;
            _clock.unlock();
        };
        void disable(Sock fd)
        {
            if (clients.find(fd) == clients.end()) {
                return;
            }
            _clock.lock();
            clients[fd] = false;
            _clock.unlock();
        };
        bool enabled(Sock fd)
        {
            if (!has(fd)) {
                return false;
            }

            return clients[fd];
        };
        bool has(Sock fd)
        {
            _clock.lock();
            bool ret = clients.find(fd) == clients.end();
            _clock.unlock();
            return ret;
        };
        void afterAccepted(std::function<bool(Sock fd)> handle)
        {
            _accepted = handle;
        };
        void close(Sock fd)
        {
            bootcp::BooTcp<T>::close(fd);
            _clock.lock();
            auto i = clients.find(fd);
            if (i != clients.end()) {
                clients.erase(fd);
            }
            _clock.unlock();
        };
        void closeClients()
        {
            _clock.lock();
            auto i = clients.begin();
            while (i != clients.end()) {
                bootcp::BooTcp<T>::close(i->first);
                clients.erase(i);
                i = clients.begin();
            }
            _clock.unlock();
        };
        void send(Msg * msg, std::set<Sock> fds)
        {
            for (auto i = clients.begin(); i != clients.end(); ++i) {
                if (!i->second || fds.find(i->first) == fds.end()) {
                    continue;
                }
                BooTcp<T>::asyncSend(i->first, msg);
            }
        };
        void broadcast(Msg * msg)
        {
            _clock.lock();
            for (auto i = clients.begin(); i != clients.end(); ++i) {
                if (!i->second) {
                    continue;
                }
                BooTcp<T>::asyncSend(i->first, msg);
            }
            _clock.unlock();
        };
        void broadcast(Msg * msg, std::set<Sock> excepts)
        {
            _clock.lock();
            for (auto i = clients.begin(); i != clients.end(); ++i) {
                if (!i->second || excepts.find(i->first) != excepts.end()) {
                    continue;
                }
                BooTcp<T>::asyncSend(i->first, msg);
            }
            _clock.unlock();
        };
        void wait()
        {
            while (_running) { }
        };
        Sock fd() override
        {
            return _fd;
        };
        SSL_METHOD * sslMethod() override
        {
            // return TSLv1_server_method();
            return nullptr;
        };
        ~Server()
        {
            stop();
        };
    private:
        void accept()
        {
#ifdef WIN32
            int len = sizeof(sockaddr);
#else
            socklen_t len = sizeof(sockaddr);
#endif
            struct sockaddr_in sockAddr;
            while (_running) {
                Sock client = ::accept(_fd, (sockaddr*)&sockAddr, &len);
                if (!BooTcp<T>::isOK(client)) {
                    continue;
                }
                BooTcp<T>::newConn(client);
                _clock.lock();
                clients[client] = true;
                _clock.unlock();
                std::thread recv(&bootcp::Server<T>::recv, this, client);
                recv.detach();
                if (_accepted != nullptr && !_accepted(client)) {
                    BooTcp<T>::close(client);
                }
            }
        };
        void recv(Sock client)
        {
            while (_running) {
                _clock.lock();
                auto i = clients.find(client);
                bool valid = i != clients.end() && clients[client];
                _clock.unlock();
                if (!valid) {
                    break;
                }
                if (!BooTcp<T>::recvSock(client)) {
                    break;
                }
            }
        };

    private:
        Sock _fd;
        bool _running = false;
        std::function<bool(Sock fd)> _accepted = nullptr;
        std::mutex _clock;
        std::map<Sock, bool> clients;
    };
}
#endif
