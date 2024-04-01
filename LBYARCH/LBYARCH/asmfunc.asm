; assembly part using x86-64

section .data
msg db "Hello World", 13, 10, 0
MAX_SIZE equ 30

section .text
bits 64
default rel ; support address relocation

global asm_DAXPY
extern printf

; this section modifies param Z
; find Z[i] = A * X[i] + Y[i]
; @param int n // ecx
; @param double A // xmm1
; @param double[] &X // xmm2
; @param double[] &Y // xmm3
; @param double[] &Z //

asm_DAXPY:
	push rbp ; stack frame
	mov rbp, rsp
	add rbp, 16 ; push rbp (+8) and return address of call (+8)
	add rbp, 8
	mov r15, [rbp] ; this is n
	; would it be 32 if the the param 3 and 4 are arrays?
	; lets test that theory
	; 16 shadow space of two 8-bytes
	; 8*30*2 because the two parameters before us are arrays with max 30 shadow space
	
	mov rsi, qword [rbp+16+8*3*2] ; rsi pointing to @param Z
	movsd xmm1, qword [rbp+8] ; move A to xmm1

	; make a loop that iterates the DAXPY calculation
	; loop iterator ebx
	xor r8, r8
	L1: 
		; get X and multiply it with A
		; put X in XMM14, then multiply --- XMM1 never changes because scalar
		movsd xmm15, qword [rbp+16+r8*8]
		movsd xmm14, xmm15
		mulsd xmm14, xmm1 ; this is now A * X[i]

		; get Y and add it to the product earlier
		; put Y in xmm13
		movsd xmm13, qword [rbp+16+8*3+r8*8]
		addsd xmm14, xmm13 ; this is now the result of A * X[i] + Y[i]
		
		; this code iterates through Z[]
		; set Z[i] value here
		;movsd [rsi], xmm14

		add rsi, 8

		inc r8
		cmp r8, 3
		jl L1
	pop rbp
	ret