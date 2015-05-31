/*      
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  File:       SockWrap.h
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#ifndef SOCK_WRAP_H
#define SOCK_WRAP_H

#include <WinSock2.h>
#include <WS2tcpip.h>

class CSockWrap
{
public:
    CSockWrap(void);
    ~CSockWrap(void);
    static bool InitNetwork();
    static int StopNetwork();
    static SOCKET Socket(int family, int type, int protocol);
    static void GetLocalAddrInfo(int nPort, sockaddr_in &stAddr);
    static void GetRemotAddrInfo(const char* pHostName, int nPort, sockaddr_in &stRemoteAddr);
    static int Accept(SOCKET fd, struct sockaddr *sa, socklen_t *salenptr);
    static int Bind(SOCKET fd, const struct sockaddr *sa, socklen_t salen);
    static int Connect(SOCKET fd, const struct sockaddr *sa, socklen_t salen);
    static int Getsockopt(SOCKET fd, int level, int optname, char *optval, socklen_t *optlenptr);
    static int Listen(SOCKET fd, int backlog);
    static int Setsockopt(SOCKET fd, int level, int optname, const char *optval, socklen_t optlen);
    static int Recv(SOCKET fd, char *ptr, size_t nbytes, int flags);
    static int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
    static int Send(SOCKET fd, const char *ptr, size_t nbytes, int flags);
    static int Close(SOCKET fd);
    static int SetNoBlock(SOCKET fd);
    static void HandleErrMsg(char *pErrMsg,char *pFileName,int nErrCode,int nLineNumber);
};

#endif