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
*  File:       SockWrap.cpp
*  Author:     Richard Zou
*  Created on: 2015-03-01
*/

#include "SockWrap.h"

#include <stdio.h>


CSockWrap::CSockWrap(void)
{
}


CSockWrap::~CSockWrap(void)
{
}

bool CSockWrap::InitNetwork()
{
    int nRet = 0;
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    nRet = WSAStartup(wVersionRequested, &wsaData);

    if (nRet != 0)
    {
        printf("WSAStartup failed");
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        HandleErrMsg("Could not find a usable version of Winsock.dll", __FILE__, GetLastError(), __LINE__);
        WSACleanup();
        return false;
    }
    return true;
}

int CSockWrap::StopNetwork()
{
    int n;
    if ((n = WSACleanup()) != 0)
    {
        HandleErrMsg("WSACleanup failed", __FILE__, GetLastError(), __LINE__);
    }

    return n;
}

void CSockWrap::GetLocalAddrInfo(int nPort, sockaddr_in &stLocalAddr )
{
    //Get Ip Address
    char szHostname[50];
    int nResult;
    stLocalAddr.sin_family = AF_INET;
    stLocalAddr.sin_port=htons(nPort);
    nResult=gethostname(szHostname,50);
    if(nResult != 0)
    {
        HandleErrMsg("gethostname failed", __FILE__, GetLastError(), __LINE__);
    }
    //     hostent *hst=NULL;
    //     struct in_addr ia;
    //     hst=gethostbyname((LPCTSTR)hostname);
    //     memcpy(&ia.S_un.S_addr,hst->h_addr_list[0],sizeof(ia.S_un.S_addr));
    // 
    //     //Fill localaddr
    //     LocalAddr.sin_addr=ia;

    stLocalAddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
}

void CSockWrap::HandleErrMsg(char *pErrMsg,char *pFileName,int nErrCode,int nLineNumber)
{
    char szErrBuff[1024];
    if(pErrMsg==NULL)
    {
        return;
    }
    sprintf(szErrBuff, "%s File:%s,Error Code:%d,Line:%d.\n",pErrMsg,pFileName,nErrCode,nLineNumber);
    ::MessageBox(NULL,szErrBuff,"错误信息",MB_OK|MB_ICONWARNING); //弹出错误信息
    //     //写入文件
    //     CTime m_Time=CTime::GetCurrentTime();
    //     CString m_FileName=m_Time.Format("%Y-%m-%d")+_T(".log");
    //     CStdioFile file(m_FileName,CFile::modeCreate|CFile::modeNoTruncate | CFile::modeReadWrite);
    //     file.SeekToEnd();
    //     file.WriteString(m_Time.Format("%Y-%m-%d %H:%M:%S")+_T("\r\n"));
    //     file.WriteString(ErrStr);
    //     file.Close();
}

SOCKET CSockWrap::Socket( int family, int type, int protocol )
{
    SOCKET      n;

    if ( (n = socket(family, type, protocol)) == INVALID_SOCKET)
    {
        int n = GetLastError();
        HandleErrMsg("Create socket failed", __FILE__, GetLastError(), __LINE__);
    }

    return n;
}

int CSockWrap::Bind( SOCKET fd, const struct sockaddr *sa, socklen_t salen )
{
    int n;
    if ((n = bind(fd, sa, salen)) < 0)
    {
        HandleErrMsg("Bind socket failed", __FILE__, GetLastError(), __LINE__);
    }

    return n;
}

int CSockWrap::Accept( SOCKET fd, struct sockaddr *sa, socklen_t *salenptr )
{
    int     n;

    if ( (n = accept(fd, sa, salenptr)) < 0) 
    {
        HandleErrMsg("Accept failed", __FILE__, GetLastError(), __LINE__);
    }

    return(n);
}

int CSockWrap::Connect( SOCKET fd, const struct sockaddr *sa, socklen_t salen )
{
    int n;
    if ((n = connect(fd, sa, salen)) < 0)
    {
        HandleErrMsg("Connect failed", __FILE__, GetLastError(), __LINE__);
    }

    return n;
}

int CSockWrap::Getsockopt( SOCKET fd, int level, int optname, char *optval, socklen_t *optlenptr )
{
    int n;
    if ((n = getsockopt(fd, level, optname, optval, optlenptr)) < 0)
    {
        HandleErrMsg("Getsockopt failed", __FILE__, GetLastError(), __LINE__);
    }

    return n;
}

int CSockWrap::Listen( SOCKET fd, int backlog )
{
    int n;
    if ((n = listen(fd, backlog)) < 0)
    {
        HandleErrMsg("Listen failed", __FILE__, GetLastError(), __LINE__);
    }

    return n;
}

int CSockWrap::Setsockopt( SOCKET fd, int level, int optname, const char *optval, socklen_t optlen )
{
    int n;
    if ((n = setsockopt(fd, level, optname, optval, optlen)) < 0)
    {
        HandleErrMsg("Setsockopt failed", __FILE__, GetLastError(), __LINE__);
    }

    return n;
}

int CSockWrap::Recv( SOCKET fd, char *ptr, size_t nbytes, int flags )
{
    int n = 0;
    if ( (n = recv(fd, ptr, nbytes, flags)) < 0)
    {
        HandleErrMsg("Recv failed", __FILE__, GetLastError(), __LINE__);
    }

    return n;
}

int CSockWrap::Select( int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout )
{
    int     n;

    if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
    {
        HandleErrMsg("Select failed", __FILE__, GetLastError(), __LINE__);
    }
    return(n);      /* can return 0 on timeout */
}

int CSockWrap::Send( SOCKET fd, const char *ptr, size_t nbytes, int flags )
{
    int n;
    n = send(fd, ptr, nbytes, flags); 
    if (n!= nbytes)
    {
        HandleErrMsg("Send failed", __FILE__, WSAGetLastError(), __LINE__);
    }

    return n;
}

void CSockWrap::GetRemotAddrInfo( const char* pHostName, int nPort, sockaddr_in &stRemoteAddr )
{
    stRemoteAddr.sin_family=AF_INET;
    stRemoteAddr.sin_port=htons(nPort);

    hostent *hst=NULL;
    struct in_addr ia;
    hst=gethostbyname(pHostName);
    int n = WSAGetLastError();
    int m = GetLastError();
    memcpy(&ia.S_un.S_addr,hst->h_addr_list[0],sizeof(ia.S_un.S_addr));

    //Fill RemoteAddr
    stRemoteAddr.sin_addr=ia;
}

int CSockWrap::Close( SOCKET fd )
{
    int n = 0;
    if ((n = closesocket(fd)) != 0)
    {
        HandleErrMsg("Close failed", __FILE__, WSAGetLastError(), __LINE__);
    }
    return n;
}

int CSockWrap::SetNoBlock( SOCKET fd )
{
    u_long iMode = 1;
    int n = ioctlsocket(fd, FIONBIO, &iMode);
    if (n != 0)
    {
        HandleErrMsg("SetNoBlock failed", __FILE__, WSAGetLastError(), __LINE__);
    }
    return n;
}
