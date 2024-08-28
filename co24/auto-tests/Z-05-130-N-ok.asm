; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; INT 0
	push	dword 0
; TEXT
segment	.text._L2
; ALIGN
align	4
; LABEL _L2
_L2:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; LDINT
	pop	eax
	push	dword [eax]
; INT 1
	push	dword 1
; ADD
	pop	eax
	add	dword [esp], eax
; STFVAL32
	pop	eax
; JMP _L3
	jmp	dword _L3
; ALIGN
align	4
; LABEL _L3
_L3:
; LEAVE
	leave
; RET
	ret
; TEXT
segment	.text._main
; ADDR _L2
	push	dword $_L2
; BRANCH
	pop	eax
	call	eax
; TRASH 4
	add	esp, 4
; LDFVAL32
	push	eax
; INT 1
	push	dword 1
; ADD
	pop	eax
	add	dword [esp], eax
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; INT 1
	push	dword 1
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; CALL println
	call	println
; INT 0
	push	dword 0
; STFVAL32
	pop	eax
; JMP _L1
	jmp	dword _L1
; ALIGN
align	4
; LABEL _L1
_L1:
; LEAVE
	leave
; RET
	ret
; EXTERN printi
extern	printi
; EXTERN println
extern	println
