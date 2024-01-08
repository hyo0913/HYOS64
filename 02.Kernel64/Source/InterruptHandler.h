/*
 * InterruptHandler.h
 *
 *  Created on: 2023. 7. 19.
 *      Author: root
 */

#ifndef __KERNEL64_SOURCE_INTERRUPTHANDLER_H__
#define __KERNEL64_SOURCE_INTERRUPTHANDLER_H__

#include "Types.h"

// 함수
void kCommonExceptionHandler(int iVectorNumber, QWORD qwErrorCode);
void kCommonInterruptHandler(int iVectorNumber);
void kKeyboardHandler(int iVectorNumber);
void kTimerHandler(int iVectorNumber);

#endif /* __KERNEL64_SOURCE_INTERRUPTHANDLER_H__ */
