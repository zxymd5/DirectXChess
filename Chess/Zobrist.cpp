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
 *  File:       Zobrist.cpp
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#include "Zobrist.h"

CZobrist::CZobrist(void)
{
    Reset();
}

CZobrist::~CZobrist(void)
{
}

void CZobrist::Init(CRC4 &clRC4)
{
    m_dwKey = clRC4.Next4Bytes();
    m_dwLock0 = clRC4.Next4Bytes();
    m_dwLock1 = clRC4.Next4Bytes();
}

void CZobrist::Reset()
{
    m_dwKey = 0;
    m_dwLock0 = 0;
    m_dwLock1 = 0;
}

void CZobrist::Xor( const CZobrist &clZobr )
{
    m_dwKey ^= clZobr.m_dwKey;
    m_dwLock0 ^= clZobr.m_dwLock0;
    m_dwLock1 ^= clZobr.m_dwLock1;
}

void CZobrist::Xor( const CZobrist &clZobr1, const CZobrist &clZobr2 )
{
    m_dwKey = clZobr1.m_dwKey ^ clZobr2.m_dwKey;
    m_dwLock0 = clZobr1.m_dwLock0 ^ clZobr2.m_dwLock0;
    m_dwLock1 = clZobr1.m_dwLock0 ^ clZobr2.m_dwLock1;
}
