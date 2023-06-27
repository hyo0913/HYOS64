[ORG 0x00]	; 코드의 시작 어드레스를 0x00으로 설정
[BITS 16]	; 이하의 코드는 16비트 코드로 설정

SECTION .text	; text 섹션(섹그먼트)을 정의

jmp 0x07C0:START
TOTALSECTORCOUNT: dw 0x02 		; 부트 로더를 제외한 HYOS64 OS 이미지의 크기
									; 최대 1152 섹터(0x9000byte)까지 가능
KERNEL32SECTORCOUNT: dw 0x02 	; 보호 모드 커널의 총 섹터 수

START:
	mov ax, 0x07C0
	mov ds, ax
	mov ax, 0xB800
	mov es, ax

	mov ax, 0x0000
	mov ss, ax
	mov sp, 0xFFFE
	mov bp, 0xFFFE

	; 화면을 모두 지우고, 속성값을 녹색으로 설정
	mov si, 0

.SCREENCLEARLOOP:
	mov byte [es:si], 0
	mov byte [es:si+1], 0x0A

	add si, 2

	cmp si, 80 * 25 * 2
	jl .SCREENCLEARLOOP

	; 화면 상단에 시작 메시지 출력
	push MESSAGE1
	push 0
	push 0
	call PRINTMESSAGE
	add sp, 6

	; OS 이미지를 로딩한다는 메시지 출력
	push IMAGELOADINGMESSAGE
	push 1
	push 0
	call PRINTMESSAGE
	add sp, 6

	; 디스크에서 OS 이미지를 로딩
	; 디스크를 읽기 전에 먼저 리셋

; 디스크를 리셋하는 코드의 시작
RESETDISK:
	; BIOS Reset Function 호출
	; 서비스 번호 0, 드라이브 번호 (0=Floppy)
	mov ax, 0
	mov dl, 0
	int 0x13
	; 에러가 발생하면 에러 처리로 이동
	jc HANDLEDISKERROR

	; 디스크에서 섹터를 읽음
	mov si, 0x1000
	mov es, si
	mov bx, 0x0000

	mov di, word [TOTALSECTORCOUNT]

; 디스크를 읽는 코드의 시작
READDATA:
	cmp di, 0
	je READEND
	sub di, 0x01

	; BIOS Read Function 호출
	mov ah, 0x02
	mov al, 0x01
	mov ch, byte [TRACKNUMBER]
	mov cl, byte [SECTORNUMBER]
	mov dh, byte [HEADNUMBER]
	mov dl, 0x00
	int 0x13
	jc HANDLEDISKERROR

	; 복사할 어드레스와 트랙, 헤드, 섹터 어드레스 계산
	add si, 0x0020
	mov es, si

	mov al, byte [SECTORNUMBER]
	add al, 0x01
	mov byte [SECTORNUMBER], al
	cmp al, 37 ; cmp al, 19
	jl READDATA

	xor byte [HEADNUMBER], 0x01
	mov byte [SECTORNUMBER], 0x01

	cmp byte [HEADNUMBER], 0x00
	jne READDATA

	add byte [TRACKNUMBER], 0x01
	jmp READDATA

READEND:
	; OS 이미지가 완료되었다는 메시지를 출력
	push LOADINGCOMPLETEMESSAGE
	push 1
	push 20
	call PRINTMESSAGE
	add sp, 6

	; 로딩한 가상 OS 이미지 실행
	jmp 0x1000:0x0000

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 함수 코드 영역
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; 디스크 에러를 처리하는 함수
HANDLEDISKERROR:
	push DISKERRORMESSAGE
	push 1
	push 20
	call PRINTMESSAGE

	jmp $

; 메시지를 출력하는 함수
; parameters: x 좌표, y좌표, 문자열
PRINTMESSAGE:
	push bp
	mov bp, sp

	push es
	push si
	push di
	push ax
	push cx
	push dx

	mov ax, 0xB800
	mov es, ax

	; X, Y의 좌표로 비디오 메모리의 어드레스를 계산함
	mov ax, word [bp+6]
	mov si, 160
	mul si
	mov di, ax

	mov ax, word [bp+4]
	mov si, 2
	mul si
	add di, ax

	mov si, word [bp+8]

.MESSAGELOOP:
	mov cl, byte [si]
	cmp cl, 0
	je .MESSAGEEND

	mov byte [es:di], cl

	add si, 1
	add di, 2

	jmp .MESSAGELOOP

.MESSAGEEND:
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop es
	pop bp
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 데이터 영역
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; 부트 로더 시작 메시지
MESSAGE1: db 'HYOS64 OS Boot Loader Start~!!', 0
DISKERRORMESSAGE: db 'DISK Error~!!', 0
IMAGELOADINGMESSAGE: db 'OS Image Loading...', 0
LOADINGCOMPLETEMESSAGE: db 'Complete~!!', 0

; 디스크 읽기에 관련된 변수들
SECTORNUMBER: db 0x02
HEADNUMBER:db 0x00
TRACKNUMBER: db 0x00

times 510 - ($ - $$)	db	0x00	; $: 현재 라인의 어드레스
									; $$: 현재 섹션(.text)의 시작 어드레스
									; $ - $$: 현재 섹션을 기준으로 하는 오프셋
									; 510 - ($ - $$): 현재부터 어드레스 510까지
									; db 0x00: 1바이트를 선언하고 값은 0x00
									; times: 반복 수행
									; 현재 위치에서 어드레스 510까지 0x00으로 채움

db 0x55	; 1바이트를 선언하고 값은 0x55
db 0xAA	; 1바이트를 선언하고 값은 0xAA
			; 어드레스 511, 512에 0x55, 0xAA를 써서 부트 섹터로 표기함
