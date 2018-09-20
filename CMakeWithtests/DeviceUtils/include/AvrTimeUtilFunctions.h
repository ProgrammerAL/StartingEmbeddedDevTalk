#ifndef AvrTimeUtils_H
#define AvrTimeUtils_H

#include <stdint.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace DeviceSpecificUtils
		{
			namespace TimeFunctions
			{

#ifdef __cplusplus
				extern "C" {
#endif

					// the CTC match value in OCR1A
#define CTC_MATCH_OVERFLOW ((F_CPU/1000)/8)

					void millis_init();
					uint32_t millisecondsSinceInit();

#ifdef __cplusplus
				}
#endif
			}
		}
	}
}

#endif
