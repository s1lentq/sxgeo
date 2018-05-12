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

#include "precompiled.h"

CSxGeo g_sxgeo;

CSxGeo::CSxGeo() : m_HandleDB(nullptr)
{
}

CSxGeo::~CSxGeo()
{
	m_HandleDB.reset();
}

bool CSxGeo::is_load() const
{
	return m_HandleDB != nullptr;
}

bool CSxGeo::load()
{
	char path[MAX_PATH];
	g_amxxapi.BuildPathnameR(path, sizeof(path), "%s/%s", g_amxxapi.GetLocalInfo("amxx_datadir", "addons/amxmodx/data"), DB_NAME);

	if (open(path))
	{
		return false;
	}

	return true;
}

bool CSxGeo::open(const char *path)
{
	std::ifstream instream(path, std::ios::in | std::ios::binary);
	if (!instream.good())
	{
		AMXX_Log("Can't open the file: \"%s\"\n", path);
		return false;
	}

	m_HandleDB = std::make_unique<sxgeo::Db>(path);
	auto ret = m_HandleDB->readFromStream(instream);
	instream.close();
	return ret;
}
