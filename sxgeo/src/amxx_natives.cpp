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

/*
* Look up the two character country code for a given IP address.
* e.g: "RU", "KZ", "US", etc.
*
* @param ip         The IP address to look up.
* @param output     The result buffer. If the lookup does not succeed, the buffer is not modified.
*
* @return           true on a successful lookup, false on a failed lookup.
*
* native bool:sxgeo_code(const ip[], output[], const len, const lang[] = "en");
*/
cell AMX_NATIVE_CALL sxgeo_code(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_ip, arg_output, arg_len, arg_lang };

	return sxgeo_lookup(amx, params[arg_ip], [&amx, &params](std::unique_ptr<Location> &loc)
	{
		char langbuf[4];
		const char *lang = getAmxString(amx, params[arg_lang], langbuf);

		cell *dest = getAmxAddr(amx, params[arg_output]);
		setAmxString(dest, loc->getCountryIso().c_str(), params[arg_len]);
		return TRUE;
	});
}

/*
* Look up the full country name for the given IP address.
*
* @param ip         The IP address to look up.
* @param output     The output of the sxgeo look up.
* @param len        The maximum length of the output buffer.
* @param lang       Which language to the output of result the sxgeo lookup.
*                   Supported languages:
*                   "ru", "en"
*
* @return           true on a successful lookup, false on a failed lookup.
*
* native bool:sxgeo_country(const ip[], output[], const len, const lang[] = "en");
*/
cell AMX_NATIVE_CALL sxgeo_country(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_ip, arg_output, arg_len, arg_lang };

	return sxgeo_lookup(amx, params[arg_ip], [&amx, &params](std::unique_ptr<Location> &loc)
	{
		char langbuf[4];
		const char *lang = getAmxString(amx, params[arg_lang], langbuf);

		cell *dest = getAmxAddr(amx, params[arg_output]);
		setAmxString(dest, loc->getCountryName(lang).c_str(), params[arg_len]);
		return TRUE;
	});
}

/*
* Look up the full city name for the given IP address.
*
* @param ip         The IP address to look up.
* @param output     The output of the sxgeo look up.
* @param len        The maximum length of the output buffer.
* @param lang       Which language to the output of result the sxgeo lookup.
*                   Supported languages:
*                   "ru", "en"
*
* @return           true on a successful lookup, false on a failed lookup.
*
* native bool:sxgeo_city(const ip[], output[], const len, const lang[] = "en");
*/
cell AMX_NATIVE_CALL sxgeo_city(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_ip, arg_output, arg_len, arg_lang };

	return sxgeo_lookup(amx, params[arg_ip], [&amx, &params](std::unique_ptr<Location> &loc)
	{
		char langbuf[4];
		const char *lang = getAmxString(amx, params[arg_lang], langbuf);

		cell *dest = getAmxAddr(amx, params[arg_output]);
		setAmxString(dest, loc->getCityName(lang).c_str(), params[arg_len]);
		return TRUE;
	});
}

/*
* Look up the city's latitude for the given IP address.
*
* @param ip         The IP address to look up.

* @return           The result of the sxgeo look up, 0 if latitude is not found.
*
* native Float:sxgeo_latitude(const ip[]);
*/
cell AMX_NATIVE_CALL sxgeo_latitude(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_ip };

	return sxgeo_lookup(amx, params[arg_ip], [](std::unique_ptr<Location> &loc) {
		return amx_ftoc(loc->getLat());
	});
}

/*
* Look up the city's longitude for the given IP address.
*
* @param ip         The IP address to look up.

* @return           The result of the sxgeo look up, 0 if longitude is not found.
*
* native Float:sxgeo_longitude(const ip[]);
*/
cell AMX_NATIVE_CALL sxgeo_longitude(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_ip };

	return sxgeo_lookup(amx, params[arg_ip], [](std::unique_ptr<Location> &loc) {
		return amx_ftoc(loc->getLon());
	});
}

/*
* Look up the full region/state name for the given IP address.
*
* @param ip         The IP address to look up.
* @param output     The output of the sxgeo look up.
* @param len        The maximum length of the output buffer.
* @param lang       Which language to the output of result the sxgeo lookup.
*                   Supported languages:
*                   "ru", "en"
*
* @return           true on a successful lookup, false on a failed lookup.
*
* native bool:sxgeo_region(const ip[], output[], const len, const lang[] = "en");
*/
cell AMX_NATIVE_CALL sxgeo_region(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_ip, arg_output, arg_len, arg_lang };

	return sxgeo_lookup(amx, params[arg_ip], [&amx, &params](std::unique_ptr<Location> &loc)
	{
		char langbuf[4];
		const char *lang = getAmxString(amx, params[arg_lang], langbuf);

		cell *dest = getAmxAddr(amx, params[arg_output]);
		setAmxString(dest, loc->getRegionName(lang).c_str(), params[arg_len]);
		return TRUE;
	});
}

/*
* Look up the region/state code for the given IP address.
* e.g. "RU-TOM", "US-OH", "DE-HH", IT-82, "FR-U", etc.
*
* @param ip         The IP address to look up.
* @param output     The output of the sxgeo look up.
* @param len        The maximum length of the output buffer.
*
* @return           true on a successful lookup, false on a failed lookup.
*
* native bool:sxgeo_region_code(const ip[], output[], const len);
*/
cell AMX_NATIVE_CALL sxgeo_region_code(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_ip, arg_output, arg_len, arg_lang };

	return sxgeo_lookup(amx, params[arg_ip], [&amx, &params](std::unique_ptr<Location> &loc)
	{
		char langbuf[4];
		const char *lang = getAmxString(amx, params[arg_lang], langbuf);

		cell *dest = getAmxAddr(amx, params[arg_output]);
		setAmxString(dest, loc->getRegionIso().c_str(), params[arg_len]);
		return TRUE;
	});
}

/*
* Calculate the distance between geographical coordinates, latitude and longitude.
*
* @param ip         The IP address to look up.
*
* @param lat1       The first IP latitude.
* @param lon1       The first IP longitude.
* @param lat2       The second IP latitude.
* @param lon2       The second IP longitude.
* @param system     The system of measurement, 0 = Metric(kilometers) or 1 = English(miles).
*
* @return           The distance as result in specified system of measurement.
*
* native Float:sxgeo_distance(Float:lat1, Float:lon1, Float:lat2, Float:lon2, system = SYSTEM_METRIC);
*/
cell AMX_NATIVE_CALL sxgeo_distance(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_lat1, arg_lon1, arg_lat2, arg_lon2, arg_system };

	float earthRadius = params[arg_system] ? 3958.0 : 6370.997; // miles / km

	float lat1 = amx_ctof(params[arg_lat1]) * (M_PI / 180);
	float lon1 = amx_ctof(params[arg_lon1]) * (M_PI / 180);
	float lat2 = amx_ctof(params[arg_lat2]) * (M_PI / 180);
	float lon2 = amx_ctof(params[arg_lon2]) * (M_PI / 180);

	return amx_ftoc(earthRadius * acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon2 - lon1)));
}

AMX_NATIVE_INFO sxgeo_natives[] =
{
//	{ "sxgeo_id",           sxgeo_id          },	// country	:id
	{ "sxgeo_code",         sxgeo_code        },	// country	:iso
	{ "sxgeo_country",      sxgeo_country     },	// country	:name

	{ "sxgeo_city",         sxgeo_city        },	// city		:name
//	{ "sxgeo_city_id",      sxgeo_city_id     },	// city		:id
	{ "sxgeo_latitude",     sxgeo_latitude    },	// city		:lat
	{ "sxgeo_longitude",    sxgeo_longitude   },	// city		:lon

//	{ "sxgeo_region_id",    sxgeo_region_id   },	// region	:id
	{ "sxgeo_region",       sxgeo_region      },	// region	:name
	{ "sxgeo_region_code",  sxgeo_region_code },	// region	:iso

	{ "sxgeo_distance",     sxgeo_distance    },

	{ nullptr, nullptr }
};

void RegisterNatives()
{
	if (!g_sxgeo.is_load())
	{
		// if sxgeo unsuccessfully
		fillNatives(sxgeo_natives, [](AMX *amx, cell *params) -> cell
		{
			AMXX_LogError(amx, AMX_ERR_NATIVE, "SxGeo: isn't available");
			return FALSE;
		});
	}

	g_amxxapi.AddNatives(sxgeo_natives);
}
