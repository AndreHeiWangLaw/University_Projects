; Andre Hei Wang Law 
; 4017 5600
; Program: Lower-case into upper-case converter

section .data
_mes	db	'juMping JAck flaSh #1',10,0

section .text
	global _start
_start:
_print_before:	mov eax, 4	; system call for write
		mov ebx, 1	; file descriptor 1 - standard output
		mov ecx, _mes	; put the address into ECX
		mov edx, 22	; 22 total characters
		int 80h		; calls the interrupt

	mov esi, 0		; assign 0 into ESI
	mov ebp, _mes		; array _mes into ebp
_top:	mov al, [ebp + esi]	; direct first element
	cmp al, 0		; compare if al = 0
	je _exit_loop		; if yes, exit loop

		cmp al, 'a'	; compare if al < 'a'
		jl _next	; if yes, jump to _next
		cmp al, 'z'	; compare if al > 'z'
		jg _next	; if yes, jump to _next
		sub al, 32	; substract al by 32 (upper-case)
		mov [ebp + esi], al	; replace lower by new upper

	_next:	inc esi		; increase counter/pointer
		jmp _top	; jump to continue loop
_exit_loop:
_print_after:	mov eax, 4	; system call for write
		mov ebx, 1	; file descriptor 1- standard output
		mov ecx, _mes	; put the address into ECX
		mov edx, 22	; 22 total characters
		int 80h		; calls the interrupt

_exit:	mov eax, 1		; system call for exit
	mov ebx, 0		; exit w/ return code of 0
	int 80h
