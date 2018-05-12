/*
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#pragma once

#include "sxgeo/uint24_t.h"

#pragma pack(push, 1)

struct RawRegionHeader
{
	std::uint16_t country_seek;	// S:country_seek
	std::uint24_t id;			// M:id
};

struct RawRegionAccess
{
	static constexpr const char *FORMAT = "S:country_seek/M:id/b:name_ru/b:name_en/b:iso";

	const RawRegionHeader *header;
	const char            *name_ru; // b:name_ru
	const char            *name_en; // b:name_en
	const char            *iso;     // b:iso

	RawRegionAccess(const RawRegionHeader *header, const char *name_ru, const char *name_en, const char *iso) :
		header(header),
		name_ru(name_ru),
		name_en(name_en),
		iso(iso)
	{
	}
};

#pragma pack(pop)
