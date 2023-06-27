/*
 * AssenblyUtility.h
 *
 *  Created on: 2023. 6. 29.
 *      Author: root
 */

#ifndef __ASSENBLYUTILITY_H__
#define __ASSENBLYUTILITY_H__

#include "Types.h"

BYTE kInPortByte(WORD wPort);
void kOutPortByte(WORD wPort, BYTE bData);

#endif /* __ASSENBLYUTILITY_H__ */
