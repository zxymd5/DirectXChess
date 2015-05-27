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
 *  File:       RC4.cpp
 *  Author:     Richard Zou
 *  Created on: 2015-03-01
 */

#include "RC4.h"

CRC4::CRC4(void)
{
}

CRC4::~CRC4(void)
{
}

void CRC4::Init()
{
    m_nIndex1 = 0;
    m_nIndex2 = 0;
    for (int i = 0; i < 256; i++)
    {
        m_szKey[i] = i;
    }

    int j = 0;
    BYTE uc = 0;
    for (int i = 0; i < 256; i++)
    {
        j = (j + m_szKey[i]) & 255;
        uc = m_szKey[i];
        m_szKey[i] = m_szKey[j];
        m_szKey[j] = uc;
    }
}

BYTE CRC4::NextByte()
{
    BYTE uc;
    m_nIndex1 = (m_nIndex1 + 1) & 255;
    m_nIndex2 = (m_nIndex2 + m_szKey[m_nIndex1]) & 255;

    uc = m_szKey[m_nIndex1];
    m_szKey[m_nIndex1] = m_szKey[m_nIndex2];
    m_szKey[m_nIndex2] = uc;

    return m_szKey[(m_szKey[m_nIndex1] + m_szKey[m_nIndex2]) & 255];
}

DWORD CRC4::Next4Bytes()
{
    BYTE uc0 = NextByte();
    BYTE uc1 = NextByte();
    BYTE uc2 = NextByte();
    BYTE uc3 = NextByte();

    return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
}