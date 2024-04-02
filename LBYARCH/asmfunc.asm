; assembly part using x86-64

section .data

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
	mov r15, [rbp + 32] ; get Z
	; This is how to get array elements within the first four params.
	; X[] should be in rbp+16
	; LEA r10, [r8+8]
	; turns out this is really broken im dereferencing twice
	
	; This is how to get array elements past four params.
	; mov r10, qword [rbp+32]
	; LEA r11, [r10+8]
	

	; ebx is our loop iterator
	xor rbx, rbx
	L1:
		cmp rbx, rcx
		jae DONE
		; Calculate for A * X[i]
		; xmm15 for the product
		xorpd xmm7, xmm7
		
		; get X[ebx] inside [r8 + ebx * 8]
		; start multiplying
		movsd xmm7, [r8 + rbx * 8]
		mulsd xmm7, xmm1
		; we dont write the result back to X

		; calculate for Y[i] + product
		; get Y[ebx] inside [r9 + ebx * 8]
		; start adding
		xorpd xmm8, xmm8
		movsd xmm8, [r9 + rbx * 8]
		addsd xmm7, xmm8

		; store xmm14 into Z[i]
		
		movsd [r15 + rbx * 8], xmm7 ; this is Z[i]

		inc rbx
		jmp L1

	DONE:
	pop rbp
	ret