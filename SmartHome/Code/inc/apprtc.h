#ifndef __APPRTC_H
#define __APPRTC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l1xx_hal.h"	
#include "common.h"

void AppRtcInit();
void SetRTCDate();
void SetRTCTime();

#ifdef __cplusplus
}
#endif

#endif /* __APPRTC_H */
