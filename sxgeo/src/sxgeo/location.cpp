/*
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#include "precompiled.h"

Location::Location(RawCountryAccess country, RawRegionAccess region, RawCityAccess city) :
	latitude(city.getLat()),
	longitude(city.getLon()),

	country_id(country.header->id),
	country_iso(std::string(country.header->iso, 2)),
	country_name_ru(country.name_ru),
	country_name_en(country.name_en),

	region_id(region.header->id),
	region_iso(region.iso),
	region_name_ru(region.name_ru),
	region_name_en(region.name_en),

	city_id(city.header->id),
	city_name_ru(city.name_ru),
	city_name_en(city.name_en)
{
}

Location::Location(RawCountryAccess country) :
	latitude(country.getLat()),
	longitude(country.getLon()),

	country_id(country.header->id),
	country_iso(std::string(country.header->iso, 2)),
	country_name_ru(country.name_ru),
	country_name_en(country.name_en),

	region_id(),
	region_iso(""),
	region_name_ru(""),
	region_name_en(""),

	city_id(),
	city_name_ru(""),
	city_name_en("")
{
}

Location::Location() :
	latitude(0),
	longitude(0),

	country_id(0),
	country_iso(""),
	country_name_ru(""),
	country_name_en(""),

	region_id(),
	region_iso(""),
	region_name_ru(""),
	region_name_en(""),

	city_id(),
	city_name_ru(""),
	city_name_en("")
{
}

Location::~Location()
{
}

double Location::getLon() const
{
	return latitude;
}

double Location::getLat() const
{
	return longitude;
}

const uint8_t Location::getCountryId() const
{
	return country_id;
}

const std::string& Location::getCountryIso() const
{
	return country_iso;
}

const std::string& Location::getCountryName(const char *lang) const
{
	if (Q_strncmp(lang, "ru", 3) == 0)
	{
		return country_name_ru;
	}

	return country_name_en;
}

const std::uint32_t Location::getRegionId() const
{
	return region_id;
}

const std::string& Location::getRegionIso() const
{
	return region_iso;
}

const std::string& Location::getRegionName(const char *lang) const
{
	if (Q_strncmp(lang, "ru", 3) == 0)
	{
		return region_name_ru;
	}

	return region_name_en;
}

const std::uint32_t Location::getCityId() const
{
	return city_id;
}

const std::string& Location::getCityName(const char *lang) const
{
	if (Q_strncmp(lang, "ru", 3) == 0)
	{
		return city_name_ru;
	}

	return city_name_en;
}
