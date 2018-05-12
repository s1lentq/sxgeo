/*
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#pragma once

#include <memory>

#ifdef _WIN32
	#define bswap_16(x) _byteswap_ushort(x)
	#define bswap_32(x) _byteswap_ulong(x)
#else // #ifdef _WIN32
	#include <arpa/inet.h>
	#include <byteswap.h>
#endif // #ifdef _WIN32

#include "sxgeo/header.h"
#include "sxgeo/location.h"
#include "sxgeo/raw_city_access.h"
#include "sxgeo/raw_country_access.h"
#include "sxgeo/raw_region_access.h"

namespace sxgeo
{
class Db
{
public:
	Db(const char *path);
	~Db();

	std::unique_ptr<Location> find(const char *ip) const;
	bool readFromStream(std::istream &instream);

private:
	// Loads meta information from instream.
	bool readMeta(std::istream &instream);

	char *db;
	char *regions;
	char *cities;

	Header header;

	std::uint32_t *b_idx;
	std::uint32_t *m_idx;
	std::uint32_t  block_len;
	std::string    filename;

	std::uint32_t             getLocationOffset(const char *ip) const;
	std::uint8_t              getFirstIpByte(const char *ip) const;
	std::unique_ptr<Location> parseCityFull(std::uint32_t pos) const;

	std::uint32_t             searchDb (std::uint32_t ipn, std::uint32_t min, std::uint32_t max) const;
	std::uint32_t             searchIdx(std::uint32_t ipn, std::uint32_t min, std::uint32_t max) const;

	RawCountryAccess getCountry(RawRegionAccess region_access) const;
	RawCountryAccess getCountry(std::uint32_t pos) const;
	RawRegionAccess  getRegion (RawCityAccess city_access) const;
	RawRegionAccess  getRegion (std::uint32_t pos) const;
	RawCityAccess    getCity   (std::uint32_t pos) const;

	// Creates T[elements_count], fills an array from the instream and returns pointer to array.
	template <typename T>
	T *readDb(std::istream &instream, std::uint32_t elements_count, const char *label);
};

using dbptr_t = std::unique_ptr<Db>;

} // namespace sxgeo
