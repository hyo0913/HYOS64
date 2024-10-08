/*
 * ConsoleShell.h
 *
 *  Created on: 2023. 7. 27.
 *      Author: root
 */

#ifndef __KERNEL64_SOURCE_CONSOLESHELL_H__
#define __KERNEL64_SOURCE_CONSOLESHELL_H__

#include "Types.h"

////////////////////////////////////////////////////////////////////////////////
//
// 매크로
//
////////////////////////////////////////////////////////////////////////////////
#define CONSOLESHELL_MAXCOMMANDBUFFERCOUNT  300
#define CONSOLESHELL_PROMPTMESSAGE          "HYOS64>"

// 문자열 포인터를 파라미터로 받는 함수 포인터 타입 정의
typedef void (* CommandFunction)(const char* pcParameter);

////////////////////////////////////////////////////////////////////////////////
//
// 구조체
//
////////////////////////////////////////////////////////////////////////////////
// 1바이트로 정렬
#pragma pack(push, 1)

// 셸의 커맨드를 저장하는 자료구조
typedef struct kShellCommandEntryStruct {
	char* pcCommand; // 커맨드 문자열
	char* pcHelp; // 커맨드의 도움말
	CommandFunction pfFunction; // 커맨드를 수행하는 함수의 포인터
} SHELLCOMMANDENTRY;

// 파라미터를 처리하기위해 정보를 저장하는 자료구조
typedef struct kParameterListStruct {
	const char* pcBuffer; // 파라미터 버퍼의 어드레스
	int iLength; // 파라미터의 길이
	int iCurrentPosition; // 현재 처리할 파라미터가 시작하는 위치
} PARAMETERLIST;

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////
//
// 함수
//
////////////////////////////////////////////////////////////////////////////////
// 실제 셸 코드
void kStartConsoleShell(void);
void kExecuteCommand(const char* pcCommandBuffer);
void kInitializeParameter(PARAMETERLIST* pstList, const char* pcParameter);
int kGetNextParameter(PARAMETERLIST* pstList, char* pcParameter);

// 커맨드를 처리하는 함수
static void kHelp(const char* pcParameterBuffer);
static void kCls(const char* pcParameterBuffer);
static void kShowTotalRAMSize(const char* pcParameterBuffer);
static void kStringToDecimalHexTest(const char* pcParameterBuffer);
static void kShutdown(const char* pcParamegerBuffer);
static void kSetTimer(const char* pcParameterBuffer);
static void kWaitUsingPIT(const char* pcParameterBuffer);
static void kReadTimeStampCounter(const char* pcParameterBuffer);
static void kMeasureProcessorSpeed(const char* pcParameterBuffer);
static void kShowDateAndTime(const char* pcParameterBuffer);
static void kCreateTestTask(const char* pcParameterBuffer);
static void kChangeTaskPriority(const char* pcParameterBuffer);
static void kShowTaskList(const char* pcParameterBuffer);
static void kKillTask(const char* pcParameterBuffer);
static void kCPULoad(const char* pcParameterBuffer);
static void kTestMutex(const char* pcParameterBuffer);
static void kCreateThreadTask(void);
static void kTestThread(const char* pcParameterBuffer);
static void kShowMatrix(const char* pcParameterBuffer);
static void kTestPIE(const char* pcParameterBuffer);
static void kShowDyanmicMemoryInformation( const char* pcParameterBuffer );
static void kTestSequentialAllocation( const char* pcParameterBuffer );
static void kTestRandomAllocation( const char* pcParameterBuffer );
static void kRandomAllocationTask( void );

#endif /* __KERNEL64_SOURCE_CONSOLESHELL_H__ */
