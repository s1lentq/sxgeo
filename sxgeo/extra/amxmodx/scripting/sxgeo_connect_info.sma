#include <amxmodx>
#include <sxgeo>

#if (AMXX_VERSION_NUM < 183) || defined NO_NATIVE_COLORCHAT
	#include <colorchat>
#else
	#define DontChange print_team_default
	#define client_disconnect client_disconnected
#endif

#pragma semicolon 1

new const PREFIX[]        = "^4[SxGeo]";
new const CONNECT_SOUND[] = "buttons/blip1.wav";

new g_pcvar_amx_language;

public plugin_init()
{
	register_plugin("[SxGeo] Connect Info", "1.0", "s1lent");
	register_dictionary("sxgeo_connect_info.txt");

	g_pcvar_amx_language = get_cvar_pointer("amx_language");
}

public client_putinserver(id)
{
	new szLanguage[3];
	get_pcvar_string(g_pcvar_amx_language, szLanguage, charsmax(szLanguage));

	new szName[32], szIP[16];
	get_user_name(id, szName, charsmax(szName));
	get_user_ip(id, szIP, charsmax(szIP), /*strip port*/ 0);

	new szCountry[64], szRegion[64], szCity[64];

	new bool:bCountryFound = sxgeo_country(szIP, szCountry, charsmax(szCountry), /*use lang server*/ szLanguage);
	new bool:bRegionFound  = sxgeo_region (szIP, szRegion,  charsmax(szRegion),  /*use lang server*/ szLanguage);
	new bool:bCityFound    = sxgeo_city   (szIP, szCity,    charsmax(szCity),    /*use lang server*/ szLanguage);

	if (bCountryFound && bCityFound && bRegionFound)
	{
		client_print_color(0, DontChange, "%s %L %L^3 %s ^4(%s, %s)", PREFIX, LANG_SERVER, "CINFO_JOINED", szName, LANG_SERVER, "CINFO_FROM", szCity, szRegion, szCountry);
	}
	else if (bCountryFound && bRegionFound)
	{
		client_print_color(0, DontChange, "%s %L %L^3 %s ^4(%s)", PREFIX, LANG_SERVER, "CINFO_JOINED", szName, LANG_SERVER, "CINFO_FROM", szRegion, szCountry);
	}
	else if (bCountryFound)
	{
		client_print_color(0, DontChange, "%s %L %L^4 %s", PREFIX, LANG_SERVER, "CINFO_JOINED", szName, LANG_SERVER, "CINFO_FROM", szCountry);
	}
	else
	{
		// we don't know where you are :(
		client_print_color(0, DontChange, "%s %L^4 ...", PREFIX, LANG_SERVER, "CINFO_JOINED", szName);
	}

	client_cmd(0, "spk %s", CONNECT_SOUND);
}
