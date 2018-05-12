/*
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#pragma once

#include <stdint.h>

namespace std
{
	typedef struct uint24_t
	{
		uint8_t bytes[3];

		uint24_t() :
			bytes{0, 0, 0}
		{
		}

		operator uint32_t() const
		{
			return 0 // For pretty formatting
				+ (bytes[0] << 0)
				+ (bytes[1] << 8)
				+ (bytes[2] << 16);
		}
	} uint24_t;
} //  namespace std
