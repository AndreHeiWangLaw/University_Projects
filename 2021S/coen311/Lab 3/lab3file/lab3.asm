; Andre Hei Wang Law
; 4017 5600
; heiwangandrelaw128@gmail.com
; AL-X
; June 10, 2021

section .data

array db 3,2,4,1,5,6

; these 6 numbers represent a 2D array 
; of 3 rows and 2 columns
;	 array=	3 2
;		4 1
;		5 6

section .bss

element_value resb 1	; array [row index][column index]

section .text
global _start
_start:
	mov al, 0	; al holds row index
	mov bl, 0	; bl holds column index

	; displacement_value = ((row_index * #_of_columns) + column_index) * size_of_array_element
	; cl -> [array_offset_address + displacement_value]

	; ax = al * number_of_columns (2)
	; al = al + bl
	; al = al * l 
	
	mov esi, eax	; load displacement_value to eax
	mov ebp, array	; load array offset address to esi
	mov cl, [ebp + esi]
	mov [element_value], cl

	mov eax,1
	mov ebx,0
	int 80h
