#if defined _sxgeo_included
	#endinput
#endif

#define _sxgeo_included

#if AMXX_VERSION_NUM >= 175
	#pragma reqlib sxgeo
	#if !defined AMXMODX_NOAUTOLOAD
		#pragma loadlib sxgeo
	#endif
#else
	#pragma library sxgeo
#endif

/*
* Look up the two character country code for a given IP address.
* e.g: "RU", "KZ", "US", etc.
*
* @param ip         The IP address to look up.
* @param output     The result buffer. If the lookup does not succeed, the buffer is not modified.
*
* @return           true on a successful lookup, false on a failed lookup.
*/
native bool:sxgeo_code(const ip[], output[], const len, const lang[] = "en");

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
*/
native bool:sxgeo_country(const ip[], output[], const len, const lang[] = "en");

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
*/
native bool:sxgeo_city(const ip[], output[], const len, const lang[] = "en");

/*
* Look up the city's latitude for the given IP address.
*
* @param ip         The IP address to look up.

* @return           The result of the sxgeo look up, 0 if latitude is not found.
*/
native Float:sxgeo_latitude(const ip[]);

/*
* Look up the city's longitude for the given IP address.
*
* @param ip         The IP address to look up.

* @return           The result of the sxgeo look up, 0 if longitude is not found.
*/
native Float:sxgeo_longitude(const ip[]);

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
*/
native bool:sxgeo_region(const ip[], output[], const len, const lang[] = "en");

/*
* Look up the region/state code for the given IP address.
* e.g. "RU-TOM", "US-OH", "DE-HH", IT-82, "FR-U", etc.
*
* @param ip         The IP address to look up.
* @param output     The output of the sxgeo look up.
* @param len        The maximum length of the output buffer.
*
* @return           true on a successful lookup, false on a failed lookup.
*/
native bool:sxgeo_region_code(const ip[], output[], const len);

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
*/

#define SYSTEM_METRIC   0 // kilometers
#define SYSTEM_IMPERIAL 1 // statute miles

native Float:sxgeo_distance(Float:lat1, Float:lon1, Float:lat2, Float:lon2, system = SYSTEM_METRIC);
