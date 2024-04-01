; assembly part using x86-64

section .data
msg db "%d", 13, 10, 0
MAX_SIZE equ 30

section .text
bits 64
default rel ; support address relocation

global asm_DAXPY
extern printf

; this section modifies param Z
; find Z[i] = A * X[i] + Y[i]
; @param int n // ecx? 0 this works
; @param double A // xmm1? 8 this works
; @param double* &X // xmm2 + ... apparently r8 first element
; @param double* &Y // xmm3 + ...? this one is r9
; @param double* &Z // this ??? rbp + 32 siguro

asm_DAXPY:
	push rbp ; stack frame
	mov rbp, rsp
	add rbp, 16 ; push rbp (+8) and return address of call (+8)

	; This is how to get array elements within the first four params.
	; X[] should be in rbp+16
	; LEA r10, [r8+8]
	
	; This is how to get array elements past four params.
	; mov r10, qword [rbp+32]
	; LEA r11, [r10+8]
	

	; ebx is our loop iterator
	xor ebx, ebx
	L1:
		; Calculate for A * X[i]
		; xmm15 for the product
		xorpd xmm15, xmm15
		
		; get X[ebx] inside [r8 + ebx * 8]
		LEA r15, [r8 + rbx * 8]

		; start multiplying
		; move r15 address to xmmR
		movsd xmm15, [r15]
		mulsd xmm15, xmm1
		xor r15, r15
		; we dont write the result back to r15

		; calculate for Y[i] + product
		; get Y[ebx] inside [r9 + ebx * 8]
		LEA r15, [r9 + rbx * 8]

		; start adding
		; move r15 to xmm14
		movsd xmm14, [r15]
		addsd xmm14, xmm15
		xor r15, r15

		; store xmm14 into Z[i]
		mov r15, qword [rbp + 32]
		LEA r14, [r15 + rbx * 8]
		movsd [r14], xmm14 ; this is Z[i]


		inc ebx
		cmp ebx, ecx
		jl L1

	DONE:

	pop rbp
	ret