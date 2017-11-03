<<<<<<< HEAD
; определение обработчика без ошибки
;%macro ISR_NOERRCODE 1
;[global isr%1]
;isr%1:
;    cli                         ; запрещаем все прерывания
;    push byte 0                 ; помещаем в стек фиктивный код ошибки
;                                ; так как когда происходит прерывание, туда процессор помещает
;                                ; номер ошибки, что вызвало ошибку
;   push byte %1                ; помещаем в стек номер прерывания
;    jmp isr_common_stub         ; переходим к обработчику прерывания
;%endmacro

; определение обработчика с ошибкой
;%macro ISR_ERRCODE 1
;[global isr%1]
;isr%1:
;    cli                         ; запрещаем прерывания
;    push byte %1                ; отправляем на стек номер ошибки
;    jmp isr_common_stub
;%endmacro

;%macro IRQ 2
;[global irq%1]
;irq%1:
;    cli
;    push byte 0
;    push byte %2
;    jmp irq_common_stub
;%endmacro

; макро определения вместо функций
;ISR_NOERRCODE 0
;ISR_NOERRCODE 1
;ISR_NOERRCODE 2
;ISR_NOERRCODE 3
;ISR_NOERRCODE 4
;ISR_NOERRCODE 5
;ISR_NOERRCODE 6
;ISR_NOERRCODE 7
;ISR_ERRCODE   8
;ISR_NOERRCODE 9
;ISR_ERRCODE   10
;ISR_ERRCODE   11
;ISR_ERRCODE   12
;ISR_ERRCODE   13
;ISR_ERRCODE   14
;ISR_NOERRCODE 15
;ISR_NOERRCODE 16
;ISR_NOERRCODE 17
;ISR_NOERRCODE 18
;ISR_NOERRCODE 19
;ISR_NOERRCODE 20
;ISR_NOERRCODE 21
;ISR_NOERRCODE 22
;ISR_NOERRCODE 23
;ISR_NOERRCODE 24
;ISR_NOERRCODE 25
;ISR_NOERRCODE 26
;ISR_NOERRCODE 27
;ISR_NOERRCODE 28
;ISR_NOERRCODE 29
;ISR_NOERRCODE 30
;ISR_NOERRCODE 31

;IRQ 0, 32
;IRQ 1, 33
;IRQ 2, 34
;IRQ 3, 35
;IRQ 4, 36
;IRQ 5, 37
;IRQ 6, 38
;IRQ 7, 39
;IRQ 8, 40
;IRQ 9, 41
;IRQ 10, 42
;IRQ 11, 43
;IRQ 12, 44
;IRQ 13, 45
;IRQ 14, 46
;IRQ 15, 47

; определена в isr.c
;[extern isr_handler]

; обработчик прерывания общий
; сохраняет состояние процессора и вызывает наш обработчик,
; написанный на С
;isr_common_stub:
;    pusha                   ; сохраняем edi,esi,ebp,esp,ebx,edx,ecx,eax
;
;    mov ax, ds              ; eax = ds
;    push eax                ; сохраняем дескриптор сегмента

;    mov ax, 0x10            ; заружаем сегмент данных ядра
;    mov ds, ax
;    mov es, ax
;    mov fs, ax
;    mov gs, ax


; Defined in isr.c
[extern isr_handler]
[extern irq_handler]

; Common ISR code
isr_common_stub:
    ; 1. Save CPU state
    pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax, ds ; Lower 16-bits of eax = ds.
    push eax ; save the data segment descriptor
    mov ax, 0x10  ; kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; 2. Call C handler
    call isr_handler
    
    ; 3. Restore state
    pop eax 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8 ; Cleans up the pushed error code and pushed ISR number
    sti
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; Common IRQ code. Identical to ISR code except for the 'call' 
; and the 'pop ebx'
irq_common_stub:
    pusha 
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call irq_handler ; Different than the ISR code
    pop ebx  ; Different than the ISR code
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret

;[extern irq_handler]
;irq_common_stub:
;    pusha

;    mov ax, ds
;    push eax

;    mov ax, 0x10
;    mov ds, ax
;    mov es, ax
;    mov fs, ax
;    mov gs, ax

;    call irq_handler
    
;    pop ebx
;    mov ds, bx
;    mov es, bx
;    mov fs, bx
;    mov gs, bx

;    popa
;    add esp, 8
;    sti
;   iret 
    
; We don't get information about which interrupt was caller
; when the handler is run, so we will need to have a different handler
; for every interrupt.
; Furthermore, some interrupts push an error code onto the stack but others
; don't, so we will push a dummy error code for those which don't, so that
; we have a consistent stack for all of them.

; First make the ISRs global
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31
; IRQs
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; 0: Divide By Zero Exception
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

; 1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

; 2: Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

; 3: Int 3 Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

; 4: INTO Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

; 5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

; 6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

; 7: Coprocessor Not Available Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

; 8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push byte 8
    jmp isr_common_stub

; 9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Bad TSS Exception (With Error Code!)
isr10:
    cli
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    cli
    push byte 11
    jmp isr_common_stub

; 12: Stack Fault Exception (With Error Code!)
isr12:
    cli
    push byte 12
    jmp isr_common_stub

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    cli
    push byte 13
    jmp isr_common_stub

; 14: Page Fault Exception (With Error Code!)
isr14:
    cli
    push byte 14
    jmp isr_common_stub

; 15: Reserved Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Floating Point Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check Exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine Check Exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

; 26: Reserved
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub

; IRQ handlers
irq0:
    cli
    push byte 0
    push byte 32
    jmp irq_common_stub

irq1:
    cli
    push byte 1
    push byte 33
    jmp irq_common_stub

irq2:
    cli
    push byte 2
    push byte 34
    jmp irq_common_stub

irq3:
    cli
    push byte 3
    push byte 35
    jmp irq_common_stub

irq4:
    cli
    push byte 4
    push byte 36
    jmp irq_common_stub

irq5:
    cli
    push byte 5
    push byte 37
    jmp irq_common_stub

irq6:
    cli
    push byte 6
    push byte 38
    jmp irq_common_stub

irq7:
    cli
    push byte 7
    push byte 39
    jmp irq_common_stub

irq8:
    cli
    push byte 8
    push byte 40
    jmp irq_common_stub

irq9:
    cli
    push byte 9
    push byte 41
    jmp irq_common_stub

irq10:
    cli
    push byte 10
    push byte 42
    jmp irq_common_stub

irq11:
    cli
    push byte 11
    push byte 43
    jmp irq_common_stub

irq12:
    cli
    push byte 12
    push byte 44
    jmp irq_common_stub

irq13:
    cli
    push byte 13
    push byte 45
    jmp irq_common_stub

irq14:
    cli
    push byte 14
    push byte 46
    jmp irq_common_stub

irq15:
    cli
    push byte 15
    push byte 47
    jmp irq_common_stub
    call isr_handler

    pop ebx                 ; восстанавливаем дескриптор сегмента данных
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                    ; восстанавливаем edi,esi,ebp...
    add esp, 8              ; очищаем стек от кода ошибки и номера прерывания
    sti
    iret                    ; восстанавливаем CS, EIP, EFLAGS, SS, and ESP
=======
%macro ISR_NOERRCODE 1
global isr%1		; %1 доступ к первому параметру
isr%1:
	cli					; запрещаем все прерывания
	push byte 0			; помещаем в стек фиктивный код ошибки
	push byte %1		; помещаем в стек номер прерывания (0)
	jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
	cli
	push byte %1
	jmp isr_common_stub
%endmacro

%macro IRQ 2
global irq%1
irq%1:
	cli
	push byte 0
	push byte %2
	jmp irq_common_stub
%endmacro

IRQ 	0,	32
IRQ 	1,	33
IRQ 	2,	34
IRQ 	3,	35
IRQ 	4,	36
IRQ 	5,	37
IRQ 	6,	38
IRQ 	7,	39
IRQ 	8,	40
IRQ 	9,	41
IRQ 	10,	42
IRQ 	11,	43
IRQ 	12,	44
IRQ 	13,	45
IRQ 	14,	46
IRQ 	15,	47

; вызовы макрофункций
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; определена в isr.c
[extern isr_handler]

isr_common_stub:
	pusha

	mov ax, ds
	push eax

	mov ax, 0x10		; загрузка сегмента данных ядра
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call isr_handler

	pop eax				; перезагрузка оригинального дескриптора сегмента данных
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa				; выталкиваем из стека значения edi, esi, ebp
	add esp, 8			; очищаем из стека код ошибки, помещаем в стек номер ISR
	sti
	iret				; выталкиваем из стека след. пять значений: CS, EIP, EFLAGS, SS, ESP

[extern irq_handler]

irq_common_stub:
	pusha

	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call irq_handler

	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	popa
	add esp, 8
	sti
	iret
>>>>>>> parent of dc68c92... gdt idt refresh
