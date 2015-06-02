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
 *  File:       ServerNetwork.cpp
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */
#include "ServerNetwork.h"
#include "NetworkMsgDef.h"

CServerNetwork::CServerNetwork(void)
{
    m_nListenFd = INVALID_SOCKET;
    m_nClientFd = INVALID_SOCKET;
}

CServerNetwork::~CServerNetwork(void)
{
}

bool CServerNetwork::InitServer( const char *pIpAddr, int nPort )
{
    const int On = 1;
    m_nListenFd = CSockWrap::Socket(AF_INET, SOCK_STREAM, 0);
    CSockWrap::Setsockopt(m_nListenFd, SOL_SOCKET, SO_REUSEADDR, (const char *)&On, sizeof(On));
    
    sockaddr_in stServerAddr;
    //CSockWrap::GetRemotAddrInfo(pIpAddr, nPort, stServerAddr);
    CSockWrap::GetLocalAddrInfo(nPort, stServerAddr);
    int nRet = CSockWrap::Bind(m_nListenFd, (const sockaddr *)&stServerAddr, sizeof(sockaddr));
    if (nRet < 0)
    {
        CSockWrap::Close(m_nListenFd);
        m_nListenFd = INVALID_SOCKET;
        return false;
    }
    CSockWrap::Listen(m_nListenFd, 5);

    return true;
}

int CServerNetwork::RecvMsg(char szMsg[] )
{
    int nSize = CSockWrap::Recv(m_nClientFd, szMsg, MAX_MSG_SIZE, 0);

    if (nSize <= 0)
    {
        StopServer();
    }
    return nSize;
}

int CServerNetwork::WaitForClientConn()
{
    sockaddr stClientAddr;
    int nAddrLen = sizeof(sockaddr);

    m_nClientFd = CSockWrap::Accept(m_nListenFd, &stClientAddr, &nAddrLen);
    return m_nClientFd;
}

void CServerNetwork::StopServer()
{
    if (m_nClientFd != INVALID_SOCKET)
    {
        CSockWrap::Close(m_nClientFd);
        m_nClientFd = INVALID_SOCKET;
    }
    if (m_nListenFd != INVALID_SOCKET)
    {
        CSockWrap::Close(m_nListenFd);
        m_nListenFd = INVALID_SOCKET;
    }
}

int CServerNetwork::SendMsg( char szMsg[], int nSize )
{
    return CSockWrap::Send(m_nClientFd, szMsg, nSize, 0);
}
