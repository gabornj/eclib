﻿/*!
\file c_ipc.h
\author	kipway@outlook.com
\update 2018.2.5

InterProcess Communication with socket

eclib Copyright (c) 2017-2018, kipway
source repository : https://github.com/kipway/eclib

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#pragma once
#include <stdint.h>
#include <time.h>

#ifndef _WIN32
#include <unistd.h>
#include <pthread.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>
#endif

#include "c_str.h"
#include "c11_vector.h"
#include "c_stream.h"
#include "c11_thread.h"
#include "c11_critical.h"
#include "c_tcp_tl.h"

#ifndef IPCMSG_MAXSIZE
#define IPCMSG_MAXSIZE  (1024 * 1024 * 100)
#endif 

#define ECIPC_ST_SEND_ERR		(-1)
#define ECIPC_ST_SEND_NOTLOGIN  (-2)
#define ECIPC_ST_SEND_PKGERR    (-3)
namespace ec
{		
	class ipcpkg // IPC package
	{
	public:
		ipcpkg() : _rbuf(1024 * 32) {
		}
		virtual ~ipcpkg() {}
		struct t_head {
			uint8_t  sync;
			uint8_t  flag;
			uint32_t msglen;
		};
	public:		
		static int send(SOCKET sock, const void *pmsg, size_t sizemsg)
		{
			if (sizemsg > IPCMSG_MAXSIZE)
				return ECIPC_ST_SEND_PKGERR;
			unsigned char head[8]; //sync(1),flag(1),pkglen(4)
			ec::cStream ss(head, sizeof(head));
			ss < (uint8_t)0xF5;		
			ss < (uint8_t)0x10;		
			ss < (uint32_t)(sizemsg);
			int ns = tcp_send(sock, head, 6);
			if (ns < 0)
				return ECIPC_ST_SEND_ERR;
			ns = tcp_send(sock, pmsg, (int)sizemsg);
			if (ns < 0)
				return ECIPC_ST_SEND_ERR;
			return ns;
		}
	protected:
		ec::vector<uint8_t>	_rbuf;
	public:
		int parse(const uint8_t* pdata, size_t usize, ec::vector<uint8_t> *pout)
		{
			if (pdata && usize)
				_rbuf.add(pdata, usize);
			return parsepkg(pout);
		}
		inline void clear()
		{
			_rbuf.clear();
		}
	protected:
		int parsepkg(ec::vector<uint8_t> *pout)//return 0:wait; -1:err; 1:OK
		{
			size_t  ulen = _rbuf.size();
			uint8_t* pu = _rbuf.data();
			if (_rbuf.size() < 6)
				return 0;
			ec::cStream ss((void*)pu, _rbuf.size());
			t_head h;
			ss > &h.sync;
			ss > &h.flag;
			ss > &h.msglen;
			if (h.sync != 0xF5 || h.flag != 0x10 || h.msglen > IPCMSG_MAXSIZE)
				return -1;
			if (h.msglen + 6 > _rbuf.size())
			{
				if (_rbuf.capacity() < h.msglen + 6)
					_rbuf.set_grow(h.msglen + 8);
				return 0;
			}			
			pout->clear();
			pout->add(pu + 6, h.msglen);						
			_rbuf.erase(0, h.msglen + 6);
			_rbuf.shrink(1024 * 32);
			_rbuf.set_grow(1024 * 16);
			return 1;
		}
	};
	class cIpc_s : public ec::cThread //ipc server
	{
	public:
		cIpc_s() : _busenagle(false), m_wport(0), m_sListen(INVALID_SOCKET), _sclient(INVALID_SOCKET), _nlogin(0), _msgr(1024 * 32) {
			_psw[0] = '\0';
		}
		virtual ~cIpc_s() {
			StopIpcs();
		}
	protected:
		bool    _busenagle;
		uint16_t m_wport;
		char   _psw[32];
		SOCKET	 m_sListen;
		SOCKET   _sclient;
		std::atomic_int _nlogin;
		ipcpkg  _pkg;
		ec::vector<uint8_t> _msgr;
		ec::cCritical _cs;
	protected:
		virtual	void dojob() {
			int nr;
			uint8_t buf[1024 * 32];
			while (!_bKilling)//accept,only one connection
			{
				_pkg.clear();
				_nlogin = 0;
				_sclient = acp();
				if (INVALID_SOCKET == _sclient)
					continue;
				while (!_bKilling)//read
				{
					nr = tcp_read(_sclient, buf, (int)sizeof(buf), 100);
					if (SOCKET_ERROR == nr)
					{
						closeclient();
						break;
					}
					else if (nr > 0) {
						if (!onread(buf, nr)) {
							closeclient();
							break;
						}
					}
				}
			}
			closeclient();
		};
		bool onread(const uint8_t* pdata, size_t usize)
		{
			int nr = _pkg.parse(pdata, usize,&_msgr);
			while (1 == nr)
			{
				if (!_nlogin)
				{
					_msgr.add((uint8_t)0);
					if (_msgr.size() > 32 || strcmp(_psw, (const char*)_msgr.data()))
						return false;
					_nlogin = 1;
					send("success", 8);					
				}
				else {
					if (!OnMsg())
						return false;
				}
				nr = _pkg.parse(nullptr, 0, &_msgr);
			}
			return -1 != nr;
		}
		virtual bool OnMsg() = 0; //Processes messages in _msgr, returning true if successful, false will disconnect		
	protected:
		void closeclient()
		{
			if (INVALID_SOCKET == _sclient)
				return;
			_nlogin = 0;
			_pkg.clear();
#ifdef _WIN32
			shutdown(_sclient, SD_BOTH);
			closesocket(_sclient);
#else						
			shutdown(_sclient, SHUT_WR);
			close(_sclient);
#endif
			_sclient = INVALID_SOCKET;
		}
		bool init(uint16_t wport,const char* psw)
		{
			if (!wport)
				return false;
			if (m_sListen != INVALID_SOCKET)
				return true;
			_psw[0] = 0;
			if (psw)
				ec::str_ncpy(_psw, psw, sizeof(_psw));
			_busenagle = false;
			m_wport = wport;
			struct sockaddr_in	InternetAddr;

			if ((m_sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
			{
#ifndef _WIN32
				fprintf(stderr, "cIpc_s @port %u bind error!\n", m_wport);
#endif
				return false;
			}
			InternetAddr.sin_family = AF_INET;
			InternetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
			InternetAddr.sin_port = htons(m_wport);
			if (bind(m_sListen, (const sockaddr *)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
			{
#ifdef _WIN32
				shutdown(m_sListen, SD_BOTH);
				closesocket(m_sListen);
#else
				fprintf(stderr, "ERR:SVR_PORT[%d] CEpollTcpSvr::Start\t bind failed with error %d\n", m_wport, errno);
				shutdown(m_sListen, SHUT_WR);
				close(m_sListen);
#endif
				m_sListen = INVALID_SOCKET;
				return false;
			}
			if (listen(m_sListen, 2) == SOCKET_ERROR)
			{
#ifdef _WIN32
				shutdown(m_sListen, SD_BOTH);
				closesocket(m_sListen);
#else
				fprintf(stderr, "ERR:SVR_PORT[%d] CEpollTcpSvr::Start\t listen failed with error %d\n", m_wport, errno);
				shutdown(m_sListen, SHUT_WR);
				close(m_sListen);
#endif
				m_sListen = INVALID_SOCKET;
				return false;
			}
			return true;
		}

		SOCKET acp()
		{
			int nRet;
			SOCKET	sAccept;
			struct  sockaddr_in		 addrClient;
			int		nClientAddrLen = sizeof(addrClient);

			TIMEVAL tv01 = { 1,0 };
			fd_set fdr;
			FD_ZERO(&fdr);
			FD_SET(m_sListen, &fdr);
#ifdef _WIN32
			nRet = ::select(0, &fdr, NULL, NULL, &tv01);
#else
			nRet = ::select(m_sListen + 1, &fdr, NULL, NULL, &tv01);
#endif
			if (!nRet || !FD_ISSET(m_sListen, &fdr))
				return INVALID_SOCKET;
#ifdef _WIN32
			if ((sAccept = ::accept(m_sListen, (struct sockaddr*)(&addrClient), &nClientAddrLen)) == INVALID_SOCKET)
				return INVALID_SOCKET;
#else
			if ((sAccept = ::accept(m_sListen, (struct sockaddr*)(&addrClient), (socklen_t*)&nClientAddrLen)) == INVALID_SOCKET)
				return INVALID_SOCKET;
			if (SetNoBlock(sAccept) < 0)
				return INVALID_SOCKET;
#endif
			return sAccept;
		}
	public:
#ifndef _WIN32	
		static int  SetNoBlock(int sfd)
		{
			int flags, s;

			flags = fcntl(sfd, F_GETFL, 0);
			if (flags == -1)
				return -1;

			flags |= O_NONBLOCK;
			s = fcntl(sfd, F_SETFL, flags);
			if (s == -1)
				return -1;
			return 0;
		}
#endif
		bool StartIpcs(uint16_t port,const char* psw)
		{
			if (!init(port,psw))
				return false;
			StartThread(nullptr);
			return true;
		}
		void StopIpcs()
		{
			StopThread();
			if (INVALID_SOCKET != m_sListen) {
#ifdef _WIN32
				shutdown(m_sListen, SD_BOTH);
				closesocket(m_sListen);
#else
				shutdown(m_sListen, SHUT_WR);
				close(m_sListen);
#endif
				m_sListen = INVALID_SOCKET;
			}
		}
		int send(const void* pd, size_t size) //return send bytes numbers; <0:error
		{
			ec::cSafeLock lck(&_cs);
			if (!_nlogin)
				return ECIPC_ST_SEND_NOTLOGIN;
			return _pkg.send(_sclient, pd,size);
		}
	};
	class cIpc_c : public ec::cThread //ipc client
	{
	public:
		cIpc_c() :_port(0), _psw{ 0 }, _sclient(INVALID_SOCKET), _nlogin(0), _msgr(1024*32){
		}
		virtual ~cIpc_c() {
			StopThread();
		}
	public:
		bool StartIpcc(uint16_t port, const char* psw)
		{
			if (IsRun())
				return true;
			_port = port;
			if(psw)
				ec::str_ncpy(_psw,psw,sizeof(_psw));
			else
				strcpy(_psw, "ipcpsw");
			StartThread(nullptr);
			return true;
		}
		void StopIpcc()
		{
			StopThread();
		}
		int send(const void* pd, size_t size) //return send bytes numbers; <0:error
		{
			ec::cSafeLock lck(&_cs);
			if (!_nlogin)
				return ECIPC_ST_SEND_NOTLOGIN;
			return _pkg.send(_sclient, pd, size);			
		}
	protected:
		uint16_t _port;
		char _psw[32];
		SOCKET   _sclient;
		std::atomic_int _nlogin;
		ipcpkg  _pkg;
		ec::vector<uint8_t> _msgr;		
		ec::cCritical _cs;
	protected:		
		virtual bool OnMsg() = 0; //Processes messages in _msgr, returning true if successful, false will disconnect	
		void closeclient()
		{
			if (INVALID_SOCKET == _sclient)
				return;
			_nlogin = 0;
			_pkg.clear();
#ifdef _WIN32
			shutdown(_sclient, SD_BOTH);
			closesocket(_sclient);
#else						
			shutdown(_sclient, SHUT_WR);
			close(_sclient);
#endif
			_sclient = INVALID_SOCKET;
		}
		bool onread(const uint8_t* pdata, size_t usize)
		{
			int nr = _pkg.parse(pdata, usize, &_msgr);
			while (1 == nr)
			{
				if (!_nlogin)
				{
					_msgr.add((uint8_t)0);
					if (_msgr.size() > 32 || strcmp("success", (const char*)_msgr.data()))
						return false;
					_nlogin = 1;
				}
				else {
					if (!OnMsg())
						return false;
				}
				nr = _pkg.parse(nullptr, 0, &_msgr);
			}
			return -1 != nr;
		}
		
		virtual	void dojob() // read and connect
		{			
			int nr;
			uint8_t buf[1024 * 32];
			while (!_bKilling)//connect
			{
				_pkg.clear();
				_nlogin = 0;
				_sclient = tcp_connect("127.0.0.1", _port, 2);
				if (INVALID_SOCKET == _sclient)
					continue;
				nr = _pkg.send(_sclient, _psw, (int)strlen(_psw) + 1);//send login message
				if (SOCKET_ERROR == nr)
				{
					closeclient();
					continue;
				}
				while (!_bKilling)//read
				{
					nr = tcp_read(_sclient, buf, (int)sizeof(buf), 100);
					if (SOCKET_ERROR == nr)
					{
						closeclient();
						break;
					}
					else if (nr > 0) {
						if (!onread(buf, nr)) {
							closeclient();
							break;
						}
					}
				}
				closeclient();
			}				
		}		
	};
}
/*!
\exmaple

#include "ec/c_system.h"
#include "ec/c_ipc.h"
#ifdef _WIN32
#include "ec/c_usews32.h"
#endif

class mysrv :public ec::cIpc_s
{
protected:
	virtual bool OnMsg()
	{
		_msgr.add((uint8_t)0);
		printf("srv read:%s\n", _msgr.data());
		return true;
	}
};

class mycli :public ec::cIpc_c
{
protected:
	virtual bool OnMsg()
	{
		_msgr.add((uint8_t)0);
		printf("cli read:%s\n", _msgr.data());
		return true;
	}
};


#define CMDOD_LEN 1024
int main(int argc, char* argv[])
{
#ifdef _WIN32
	ec::cUseWS_32 usews32;
#endif
	char sod[CMDOD_LEN], sw[64], sdata[256];
	memset(sod, 0, sizeof(sod));

	mysrv srv;
	srv.StartIpcs(9527, "password");

	mycli cli;
	cli.StartIpcc(9527, "password");

	while (1)
	{
		if (fgets(sod, CMDOD_LEN - 1, stdin))
		{
			int nst;
			size_t n = strlen(sod), pos = 0;
			if (!ec::str_getnextstring(' ', sod, n, pos, sw, sizeof(sw)))
				continue;
			if (!strcmp(sw, "exit"))
				break;
			else if (!strcmp(sw, "ss"))
			{
				if (!ec::str_getnextstring('\n', sod, n, pos, sdata, sizeof(sdata))) {
					printf("args error!\n");
					break;
				}
				nst = srv.send(sdata, strlen(sdata) + 1);
				if (nst < 0)
					printf("send error:%d\n", nst);
			}
			else if (!strcmp(sw, "cs"))
			{
				if (!ec::str_getnextstring('\n', sod, n, pos, sdata, sizeof(sdata))) {
					printf("args error!\n");
					break;
				}
				nst = cli.send(sdata, strlen(sdata) + 1);
				if (nst < 0)
					printf("send error:%d\n", nst);
			}
			else
			{
				printf("error command!\n");
			}
		}
	}
	cli.StopIpcc();
	srv.StopIpcs();
}
*/