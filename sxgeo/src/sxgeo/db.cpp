/*
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#include "precompiled.h"

#pragma comment(lib, "ws2_32.lib")

namespace sxgeo
{
Db::Db(const char *path) :
	filename(path),
	db(nullptr),
	regions(nullptr),
	cities(nullptr),
	b_idx(nullptr),
	m_idx(nullptr)
{
}

bool Db::readFromStream(std::istream &instream)
{
	if (!readMeta(instream))
	{
		return false;
	}

	b_idx = readDb<std::uint32_t>(instream, header.b_idx_len, "b_idx");
	m_idx = readDb<std::uint32_t>(instream, header.m_idx_len, "m_idx_len");

	db = readDb<char>(instream, header.db_items * block_len, "db");

	if (header.region_size <= 0)
	{
		AMXX_Log("\"region_size\" is invalid.");
		return false;
	}

	regions = readDb<char>(instream, header.region_size, "regions");

	if (header.city_size <= 0)
	{
		AMXX_Log("\"city_size\" is invalid.");
		return false;
	}

	cities = readDb<char>(instream, header.city_size, "cities");
	return true;
}

Db::~Db()
{
	if (db != nullptr)
	{
		delete[] db;
	}

	if (regions != nullptr)
	{
		delete[] regions;
	}

	if (cities != nullptr)
	{
		delete[] cities;
	}

	if (b_idx != nullptr)
	{
		delete[] b_idx;
	}

	if (m_idx != nullptr)
	{
		delete[] m_idx;
	}
}

std::unique_ptr<Location> Db::find(const char *ip) const
{
	std::uint32_t seek = getLocationOffset(ip);
	if (seek > 0)
	{
		return this->parseCityFull(seek);
	}

	return nullptr;
}

std::unique_ptr<Location> Db::parseCityFull(std::uint32_t pos) const
{
	Location *result = nullptr;

	if (pos < header.country_size)
	{
		// has only a country
		result = new Location(getCountry(pos));
	}
	else if (pos < header.city_size)
	{
		auto raw_city   = getCity(pos);
		auto raw_region = getRegion(raw_city);
		result = new Location(getCountry(raw_region), raw_region, raw_city);
	}

	return std::unique_ptr<Location>(result);
}

RawCountryAccess Db::getCountry(RawRegionAccess region_access) const
{
	return getCountry(region_access.header->country_seek);
}

RawCountryAccess Db::getCountry(std::uint32_t pos) const
{
	// TODO: check that length <= header.max_country with strnlen
	const char *offset = cities + pos;

	const RawCountryHeader *raw_header = reinterpret_cast<const RawCountryHeader *>(offset);
	const char             *name_ru    = reinterpret_cast<char *>((std::uint64_t)offset + sizeof(RawCountryHeader));
	const char             *name_en    = name_ru + strlen(name_ru) + sizeof(char);

	return RawCountryAccess(raw_header, name_ru, name_en);
}

RawRegionAccess Db::getRegion(RawCityAccess city_access) const
{
	return getRegion(city_access.header->region_seek);
}

RawRegionAccess Db::getRegion(std::uint32_t pos) const
{
	// TODO: check that length <= header.max_region with strnlen
	const char *offset = regions + pos;

	const RawRegionHeader *raw_header = reinterpret_cast<const RawRegionHeader *>(offset);
	const char            *name_ru    = reinterpret_cast<char *>((std::uint64_t)offset + sizeof(RawRegionHeader));
	const char            *name_en    = name_ru + strlen(name_ru) + sizeof(char);
	const char            *iso        = name_en + strlen(name_en) + sizeof(char);

	return RawRegionAccess(raw_header, name_ru, name_en, iso);
}

RawCityAccess Db::getCity(std::uint32_t pos) const
{
	// TODO: check that length <= header.max_city with strnlen
	const char *offset = cities + pos;

	const RawCityHeader *raw_header = reinterpret_cast<const RawCityHeader *>(offset);
	const char          *name_ru    = reinterpret_cast<char *>((std::uint64_t)offset + sizeof(RawCityHeader));
	const char          *name_en    = name_ru + strlen(name_ru) + sizeof(char);

	return RawCityAccess(raw_header, name_ru, name_en);
}

std::uint32_t Db::getLocationOffset(const char *ip) const
{
	std::uint8_t firstByte = getFirstIpByte(ip);
	std::uint32_t ipn = inet_addr(ip);

	if (0 == firstByte || 10 == firstByte || 127 == firstByte || firstByte > header.b_idx_len || INADDR_NONE == ipn)
		return 0;

	ipn = bswap_32(ipn);

	std::uint32_t minIndex = bswap_32(b_idx[firstByte - 1]);
	std::uint32_t maxIndex = bswap_32(b_idx[firstByte]);

	std::uint32_t min;
	std::uint32_t max;

	if (maxIndex - minIndex > header.range)
	{
		std::uint32_t part = searchIdx(ipn,
			(std::uint32_t)(minIndex / header.range),
			(std::uint32_t)(maxIndex / header.range) - 1);

		min = part > 0 ? part * header.range : 0;
		max = part > header.m_idx_len
				? header.db_items
				: (part + 1) * header.range;

		if (min < minIndex)
			min = minIndex;

		if (max > maxIndex)
			max = maxIndex;
	}
	else
	{
		min = minIndex;
		max = maxIndex;
	}

	return this->searchDb(ipn, min, max);
}

std::uint32_t Db::searchDb(std::uint32_t ipn, std::uint32_t min, std::uint32_t max) const
{
	const char *str = db;

	std::uint32_t buffer = 0;

	if (max - min > 0)
	{
		ipn &= 0x00FFFFFF; // We've found area by the first byte, clear its 00FF..
		std::uint32_t offset = 0;

		while (max - min > 8)
		{
			offset = (min + max) >> 1;
			std::memcpy(&buffer, str + offset * block_len, 3);
			buffer = bswap_32(buffer) >> 8; // We read only 3 bytes, so let's shift 8 bits

			if (ipn > buffer)
				min = offset;
			else
				max = offset;
		}

		str += min * block_len; // Move the cursor
		std::memcpy(&buffer, str, 3);
		buffer = bswap_32(buffer) >> 8;

		while (ipn >= buffer && min < max)
		{
			min++;
			str += block_len;
			std::memcpy(&buffer, str, 3);
			buffer = bswap_32(buffer) >> 8;
		}

		std::memcpy(&buffer, str - header.id_len, header.id_len);
	}
	else
	{
		std::memcpy(&buffer, str + min * block_len + 3, 3);
	}

	buffer = bswap_32(buffer) >> 8;
	return buffer;
}

std::uint32_t Db::searchIdx(std::uint32_t ipn, std::uint32_t min, std::uint32_t max) const
{
	std::uint32_t offset = 0;

	if (max < min)
	{
		std::uint32_t t = min;
		min = max;
		max = t;
	}

	while (max - min > 8)
	{
		offset = (min + max) >> 1;

		if (ipn > bswap_32(m_idx[offset]))
		{
			min = offset;
		}
		else
		{
			max = offset;
		}
	}

	while (ipn > bswap_32(m_idx[min]) && min++ <= max) {}

	return min;
}

constexpr int SXGEO_ASCII_ZERO_CODE = 48;

std::uint8_t Db::getFirstIpByte(const char *ip) const
{
	const char *pos = ip;
	std::int32_t result = 0;

	while (*pos != '.' && result < 256)
	{
		result = result * 10 + *pos - SXGEO_ASCII_ZERO_CODE;
		pos++;
	}

	return (result > 0 && result < 256) ?
		static_cast<std::uint8_t>(result) : 0;
}

bool Db::readMeta(std::istream &instream)
{
	instream.read(reinterpret_cast<char *>(&header), (std::uint32_t)(sizeof(Header) / sizeof(char)));
	if (!instream.good())
	{
		AMXX_Log("Can't read header from db \"%s\"", this->filename.c_str());
		return false;
	}

	header.time         = bswap_32(header.time);
	header.m_idx_len    = bswap_16(header.m_idx_len);
	header.range        = bswap_16(header.range);
	header.db_items     = bswap_32(header.db_items);
	header.max_region   = bswap_16(header.max_region);
	header.max_city     = bswap_16(header.max_city);
	header.region_size  = bswap_32(header.region_size);
	header.city_size    = bswap_32(header.city_size);
	header.max_country  = bswap_16(header.max_country);
	header.country_size = bswap_32(header.country_size);
	header.pack_size    = bswap_16(header.pack_size);

	block_len = 3 + header.id_len;

	if (!header.validate())
	{
		return false;
	}

	auto pack = std::make_unique<char[]>(this->header.pack_size);
	instream.read(pack.get(), header.pack_size);
	if (!instream.good())
	{
		AMXX_Log("Can't read pack from db \"%s\"", this->filename.c_str());
		return false;
	}

	char *format_checking = pack.get();
	std::size_t country_format_len = strlen(RawCountryAccess::FORMAT);
	if (strncmp(format_checking, RawCountryAccess::FORMAT, country_format_len) != 0)
	{
		AMXX_Log("The country has unsupported format.");
		return false;
	}

	format_checking += country_format_len + 1;
	std::size_t region_format_len = strlen(RawRegionAccess::FORMAT);
	if (Q_strncmp(format_checking, RawRegionAccess::FORMAT, region_format_len) != 0)
	{
		AMXX_Log("The region has unsupported format.");
		return false;
	}

	format_checking += region_format_len + 1;
	std::size_t city_format_len = strlen(RawCityAccess::FORMAT);
	if (Q_strncmp(format_checking, RawCityAccess::FORMAT, city_format_len) != 0)
	{
		AMXX_Log("The city has unsupported format.");
		return false;
	}

	return true;
}

template<typename T>
T *Db::readDb(std::istream &instream, std::uint32_t elements_count, const char *label)
{
	T *result = new T[elements_count];

	instream.read(reinterpret_cast<char *>(result), sizeof(T) * elements_count);
	if (!instream.good())
	{
		AMXX_Log("Can't read \"%s\" from db \"%s\"", label, this->filename.c_str());
		return nullptr;
	}

	return result;
}
} // namespace sxgeo
