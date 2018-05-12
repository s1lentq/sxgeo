/*
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#pragma once

#include "sxgeo/uint24_t.h"

#pragma pack(push, 1)

struct RawCityHeader
{
	std::uint24_t region_seek;	// M:region_seek
	std::uint8_t country_id;	// T:country_id
	std::uint24_t id;			// M:id
	std::int32_t lat;			// N5:lat
	std::int32_t lon;			// N5:lon
};

struct RawCityAccess
{
	static constexpr const char *FORMAT = "M:region_seek/T:country_id/M:id/N5:lat/N5:lon/b:name_ru/b:name_en";

	const RawCityHeader *header;
	const char          *name_ru;  // b:name_ru
	const char          *name_en;  // b:name_en

	RawCityAccess(const RawCityHeader *header, const char *name_ru, const char *name_en) :
		header(header),
		name_ru(name_ru),
		name_en(name_en)
	{
	}

	double getLat() const
	{
		return header->lat / std::pow(10, 5);  // N5:lat
	}

	double getLon() const
	{
		return header->lon / std::pow(10, 5);  // N5:lon
	}

	explicit operator std::string() const
	{
		std::stringstream result;

		result << "region_seek: " << header->region_seek << std::endl;
		result << "country_id: " << header->country_id << std::endl;
		result << "id: " << header->id << std::endl;
		result << "lat: " << header->lat << " (" << getLat() << ")" << std::endl;
		result << "lon: " << header->lon << " (" << getLon() << ")" << std::endl;
		result << "name_ru: " << name_ru << std::endl;
		result << "name_en: " << name_en;

		return result.str();
	}
};

#pragma pack(pop)
