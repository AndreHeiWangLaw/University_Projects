; Andre Hei Wang Law
; June 3, 2021
; sample program to add two numbers which
; are stored somewhere in memory

section .data

; put your data in this section
; db, dw, dd directions

mick dw 2 ; define one word (2 bytes) of data
keith dw 3 ; define another word of data with value 3

section .bss

; put UNINITIALIZED data here using
; this program has nothing in the .bss section

section .text
	global _start

_start:
	mov ax,[mick]	; store contents of memory word at
			; location mick into the ax register
ron:	mov bx,[keith]	;store contents of memory word at
			; location keith into the bx register
	add ax,bx	; ax = ax + bx
			; contents of register bx is added to the
			; original contents of register ax and the
			; result is stored in register ax (overweriting
			; the original content)


	mov eax,1	; the system call for exit (sys_exit)
	mov ebx,0	; exit with return code of 0 (no error)
	int 80h
