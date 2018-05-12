/*
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#pragma once

#include <string>

#include "sxgeo/uint24_t.h"
#include "sxgeo/raw_country_access.h"
#include "sxgeo/raw_region_access.h"
#include "sxgeo/raw_city_access.h"

class Location
{
public:
	explicit Location(RawCountryAccess country, RawRegionAccess region, RawCityAccess city);
	explicit Location(RawCountryAccess country);

	Location();
	~Location();

	double getLat() const;
	double getLon() const;

	const std::uint8_t   getCountryId()                          const;
	const std::string&   getCountryIso()                         const;
	const std::string&   getCountryName(const char *lang = "en") const;

	const std::uint32_t  getRegionId()                           const;
	const std::string&   getRegionIso()                          const;
	const std::string&   getRegionName(const char *lang = "en")  const;

	const std::uint32_t  getCityId()                             const;
	const std::string&   getCityName(const char *lang = "en")    const;

private:
	double        latitude;
	double        longitude;

	std::uint8_t  country_id;
	std::string   country_iso;
	std::string   country_name_ru;
	std::string   country_name_en;

	std::uint24_t region_id;
	std::string   region_iso;
	std::string   region_name_ru;
	std::string   region_name_en;

	std::uint24_t city_id;
	std::string   city_name_ru;
	std::string   city_name_en;
};
