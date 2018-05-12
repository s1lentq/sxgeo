/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#pragma once

template <typename T>
cell sxgeo_lookup(AMX *amx, cell ipaddr, const T &func)
{
	char buf[128];
	const char *ip = getAmxStringIP(amx, ipaddr, buf);

	if (unlikely(ip == nullptr || ip[0] == '\0'))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: ip can't be an empty string", __FUNCTION__);
		return FALSE;
	}

	auto loc = g_sxgeo->find(ip);
	if (unlikely(loc == nullptr))
	{
		return FALSE;
	}

	return func(loc);
}

inline void fillNatives(AMX_NATIVE_INFO *table, cell (AMX_NATIVE_CALL with)(AMX *, cell *))
{
	for (size_t i = 0; table[i].name; i++)
		table[i].func = with;
}

void RegisterNatives();
