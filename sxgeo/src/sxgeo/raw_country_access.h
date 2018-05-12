/*
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#pragma once

#pragma pack(push, 1)

struct RawCountryHeader
{
	std::uint8_t id;  // T:id
	char iso[2];      // c2:iso
	std::int16_t lat; // n2:lat
	std::int16_t lon; // n2:lon
};

struct RawCountryAccess
{
	static constexpr const char *FORMAT = "T:id/c2:iso/n2:lat/n2:lon/b:name_ru/b:name_en";

	const RawCountryHeader *header;
	const char             *name_ru;  // b:name_ru
	const char             *name_en;  // b:name_en

	RawCountryAccess(const RawCountryHeader *header, const char *name_ru, const char *name_en) :
		header(header),
		name_ru(name_ru),
		name_en(name_en)
	{
	}

	double getLat() const
	{
		return header->lat / std::pow(10, 2);  // n2:lat
	}

	double getLon() const
	{
		return header->lon / std::pow(10, 2);  // n2:lon
	}
};

#pragma pack(pop)
