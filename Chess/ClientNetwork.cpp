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

#include "ClientNetwork.h"
#include "NetworkMsgDef.h"

CClientNetwork::CClientNetwork(void)
{
    m_nSockFd = INVALID_SOCKET;
}

CClientNetwork::~CClientNetwork(void)
{
}

void CClientNetwork::InitClient()
{
    m_nSockFd = CSockWrap::Socket(AF_INET, SOCK_STREAM, 0);
}

void CClientNetwork::ConnToServer( const char *pIpAddr, int nPort )
{
    sockaddr_in stServerAddr;
    CSockWrap::GetRemotAddrInfo(pIpAddr, nPort, stServerAddr);

    CSockWrap::Connect(m_nSockFd, (sockaddr *)&stServerAddr, sizeof(sockaddr));
}

int CClientNetwork::SendMsg( char szMsg[], int nSize )
{
    return CSockWrap::Send(m_nSockFd, szMsg, nSize, 0);
}

int CClientNetwork::RecvMsg( char szMsg[] )
{
    return CSockWrap::Recv(m_nSockFd, szMsg, MAX_MSG_SIZE, 0);
}

void CClientNetwork::StopClient()
{
    CSockWrap::Close(m_nSockFd);
}
