#ifndef _BOO_TCP_H
#define _BOO_TCP_H

#include "msg.h"
#include "sock.h"
#include "sslconn.h"
#include "tcpconn.h"

#include <map>
#include <string>
#include <list>
#include <mutex>
#include <functional>
#include <thread>


namespace bootcp
{
    template<class T> class BooTcp : public SockError
    {
    public:
        BooTcp()
        {
            init();
        };

        BooTcp(std::string cert, std::string key)
        {
            init();
            withSSL(cert, key);
        };

        void on(MsgId * msgid,  std::function<void(Sock, T*)> on_msg)
        {
            _hlock.lock();
            handlers[msgid->clone()] = on_msg;
            _hlock.unlock();
        };

        void initConn(Sock fd)
        {
            if (_ssl_on && _ssl_server) {
                _conns[fd] = new SslConn(fd, _ssl_cert, _ssl_key);
            } else if (_ssl_on && !_ssl_server) {
                _conns[fd] = new SslConn(fd);
            } else {
                _conns[fd] = new TcpConn(fd);
            }
        }

        bool send(Sock fd, Msg * msg)
        {
            char * raw;
            int len;
            msg->pack(&raw, &len);
            size_t ret = _conns[fd]->write(raw, len);
            delete raw;
            if (!_ok) {
                _err_code = _conns[fd]->err();
                _err_str = _conns[fd]->errstr();
            }

            return _ok;
        };

        void asyncSend(Sock fd, Msg * msg)
        {
            Msg * m = msg->clone();
            std::thread sender(&bootcp::BooTcp<T>::asend, this, fd, m);
            sender.detach();
        };

        void withSSL(std::string cert, std::string key)
        {
            _ssl_cert = cert;
            _ssl_key = key;
            _ssl_server = true;
            _ssl_on = true;
        };

        void withSSL()
        {
            _ssl_server = false;
            _ssl_on = true;
        };

        void onNotExistHandler(std::function<void(Sock, Msg *)> handle)
        {
            _notExistHandler = handle;
        };
        void close(Sock fd)
        {
            shutdown(fd, 2);
#ifdef WIN32  
            closesocket(fd);
#else
            ::close(fd);
#endif  
        };
        virtual Sock fd() = 0;
        virtual SSL_METHOD * sslMethod() = 0;

        ~BooTcp()
        {
            for (auto i = _conns.begin(); i != _conns.end(); ++i) {
                delete i->second;
            }
#ifdef WIN32  
            if (_inited) {
                WSACleanup();
            }
#endif   
        };
    protected:
        virtual void onRecv(Sock fd, T * msg)
        {
            _hlock.lock();
            for (auto i = handlers.begin(); i != handlers.end(); ++i) {
                MsgId * msgid = msg->msgid();
                if (i->first->match(msgid)) {
                    i->second(fd, msg);
                    _hlock.unlock();
                    return;
                }
                delete msgid;
            }
            _hlock.unlock();
            if (_notExistHandler != nullptr) {
                _notExistHandler(fd, msg);
            }
        };

        bool recvSock(Sock fd)
        {
            auto msg = new T();
            if (!msg->recv(_conns[fd])) {
                return false;
            }
            onRecv(fd, msg);
            delete msg;
            return true;
        };

    private:
        void init()
        {
#ifdef WIN32  
            _inited = false;
            WORD wVersionRequested;
            wVersionRequested = MAKEWORD(2, 2);
            WSADATA wsaData;
            int nRet = WSAStartup(wVersionRequested, &wsaData);
            if (nRet != 0) {
                _err_code = 1000001;
                _err_str = "initilize error";
                return;
            }
            _inited = true;
#else
            _inited = true;
#endif  
        };
        void asend(Sock fd, Msg * msg)
        {
            send(fd, msg);
            delete msg;
        };
    private:
        std::map<MsgId*, std::function<void(Sock, T*)>> handlers;
        std::map<Sock, Conn *> _conns;
        std::mutex _hlock;

        std::string _ssl_cert;
        std::string _ssl_key;
        bool _ssl_on = false;
        bool _ssl_server = true;

        bool _inited = false;
        bool _ok = true;
        std::function<void(Sock, T*)> _notExistHandler = nullptr;
    };
}

#endif
