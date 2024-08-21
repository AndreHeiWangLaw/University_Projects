	; Name: Andre Hei Wang Law
	; STD:
	; Email: heiwangandrelaw128@gmail.com
	; Section: AL-X
	; Date: 2021-05-27

section .data	; Data segment
			; put your data in this section using
			; db, dw, dd directions
			; this program has nothing in the .data section

section .bss	; Uninitialized data segment

			; put UNINITALIZED data here using resb, resw, resd
			; this program has nothing in the .bss section


section .text	; Code segment

	global _start	; Must be declared for linker (ld)
_start:			; tells linker entry point

	mov ax,5	; store 5 into the ax register
keith:	mov bx,2	; store 2 into the bx register
	add ax,bx	; ax = ax + bx
			; contents of register bx is added to the
			; original contents of register ax and the
			; result is stored in register ax (overwriting
			; the original content)
end_of_program:
	mov eax,1	; the system call for exit (sys_exit)
	mov ebx,0	; exit with return code of 0 (no error)
	int 80h		; call the kernel to call the relevant interrupt (80h)
			; end of program
