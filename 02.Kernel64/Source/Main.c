#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "PIC.h"
#include "Console.h"
#include "ConsoleShell.h"
#include "Task.h"
#include "PIT.h"
#include "DynamicMemory.h"

void Main(void)
{
	int iCursorX,iCursorY;

	// 콘솔을 먼저 초기화한 후, 다음 작업을 수행
	kInitializeConsole(0, 10);
	kPrintf("Switch To IA-32e Mode Success~!!\n");
	kPrintf("IA-32e C Language Kernel Start..............[Pass]\n");
	kPrintf("Initialize Console..........................[Pass]\n");

	// 부팅 상황을 화면에 출력
	kGetCursor(&iCursorX, &iCursorY);
	kPrintf("GDT Initialize And Switch For IA-32e Mode...[    ]");
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_STARTADDRESS);
	kSetCursor(45, iCursorY++);
	kPrintf("Pass\n");

	kPrintf("TSS Segment Load............................[    ]");
	kLoadTR(GDT_TSSSEGMENT);
	kSetCursor(45, iCursorY++);
	kPrintf("Pass\n");

	kPrintf("IDT Initialize..............................[    ]");
	kInitializeIDTTables();
	kLoadIDTR(IDTR_STARTADDRESS);
	kSetCursor(45, iCursorY++);
	kPrintf("Pass\n");

	kPrintf("Total RAM Size Check........................[    ]");
	kCheckTotalRAMSize();
	kSetCursor(45, iCursorY++);
	kPrintf("Pass], Size = %d MB\n", kGetTotalRAMSize());

	kPrintf("TCB Pool And Scheduler Initialize...........[Pass]\n");
	iCursorY++;
	kInitializeScheduler();

	// 동적 메모리 초기화
	kPrintf("Dynamic Memory Initialize...................[Pass]\n]");
	iCursorY++;
	kInitializeDynamicMemory();

	// 1ms당 한 번씩 인터럽트가 발생하도록 설정
	kInitializePIT(MSTOCOUNT(1), 1);

	// 키보드 활성화
	kPrintf("Keyboard Activate And Queue Initialize......[    ]");

	if (kInitializeKeyboard() == TRUE) {
		kSetCursor(45, iCursorY++);
		kPrintf("Pass\n");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	} else {
		kSetCursor(45, iCursorY++);
		kPrintf("Fail\n");
		while (1);
	}

	// PIC 컨트롤러 초기화 및 모든 인터럽트 활성화
	kPrintf("PIC Controller And Interrupt Initialize.....[    ]");

	kInitializePIC();
	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kSetCursor(45, iCursorY++);
	kPrintf("Pass\n");

	// 유휴 태스크를 생성
	kCreateTask(TASK_FLAGS_LOWEST | TASK_FLAGS_THREAD | TASK_FLAGS_SYSTEM | TASK_FLAGS_IDLE, 0, 0, (QWORD)kIdleTask);

	// 셸을 시작
	kStartConsoleShell();
}
