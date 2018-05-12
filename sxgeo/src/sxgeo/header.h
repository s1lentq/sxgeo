/*
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#pragma once

enum class DatabaseType: std::uint8_t
{
	UNIVERSAL = 0,

	SXGEO_COUNTRY = 1,
	SXGEO_CITY    = 2,

	GEOIP_COUNTRY = 11,
	GEOIP_CITY    = 12,

	IPGEOBASE = 21
};

enum class Charset
{
	NONE   = -1,
	UTF8   = 0,
	LATIN1 = 1,
	CP1251 = 2,
	MAX
};

#pragma pack(push, 1)

struct Header
{
	static const std::uint8_t VERSION_SUPPORTED = 22;

	std::uint8_t  tag[3];
	std::uint8_t  version;
	std::uint32_t time;
	std::uint8_t  type;
	std::uint8_t  charset;
	std::uint8_t  b_idx_len;
	std::uint16_t m_idx_len;
	std::uint16_t range;
	std::uint32_t db_items;
	std::uint8_t  id_len;
	std::uint16_t max_region;
	std::uint16_t max_city;
	std::uint32_t region_size;
	std::uint32_t city_size;
	std::uint16_t max_country;
	std::uint32_t country_size;
	std::uint16_t pack_size;

	// Checks the header has a valid format.
	bool validate()
	{
		if ('S' != tag[0] ||
			'x' != tag[1] ||
			'G' != tag[2])
		{
			AMXX_Log("The tag is invalid.");
			return false;
		}

		if (version != VERSION_SUPPORTED)
		{
			AMXX_Log("SxGeo API version mismatch; expected %d, real %d.", VERSION_SUPPORTED, version);
			return false;
		}

		if (getCharset() != Charset::UTF8)
		{
			AMXX_Log("The charset should be utf-8.");
			return false;
		}

		if (0 == b_idx_len
		|| 0 == m_idx_len
		|| 0 == range
		|| 0 == db_items
		|| 0 == time
		|| 0 == id_len)
		{
			AMXX_Log("The format is invalid.");
			return false;
		}

		if (pack_size == 0)
		{
			AMXX_Log("The file hasn't information about packing.");
			return false;
		}

		return true;
	}

	Charset getCharset()
	{
		if ((Charset)charset >= Charset::MAX)
		{
			AMXX_Log("The file has unknown charset: %d.", charset);
			return Charset::NONE;
		}

		return static_cast<Charset>(charset);
	}
};

#pragma pack(pop)

// See the documentation: http://sypexgeo.net/ru/docs/sxgeo22/
typedef char assert_that_header_has_valid_size[(40 == sizeof(Header)) * 2 - 1];
