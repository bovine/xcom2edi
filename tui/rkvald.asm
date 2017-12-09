; OBJASM version 2.0 released on Jan 3, 1991
; (C) Copyright 1988,1989,1990,1991 by Robert F. Day.  All rights reserved

	TITLE	rkval.c
; [000C]	; Compiler:	TC86 Borland Turbo C++ 1.00
; [002E]; Dependent file: 	rkval.c
; [0040]; Dependent file: 	regkey.h
; [0053]; Dependent file: 	rkintern.h
; [0068]; Dependent file: 	regkey.h
; [007B]; End of dependency list	
; [0081]; Compiler: 'C' using medium model with underscores on	
	.386p					; Enable USE32/USE16 usage
	.387					; Enable floating point also
RKVAL_TEXT	SEGMENT	BYTE PUBLIC USE16 'CODE'
RKVAL_TEXT	ENDS
_DATA	SEGMENT	WORD PUBLIC USE16 'DATA'
_DATA	ENDS
_BSS	SEGMENT	WORD PUBLIC USE16 'BSS'
_BSS	ENDS
DGROUP	GROUP	_DATA, _BSS

	EXTRN		_regKeyModInfoGet:NEAR
	EXTRN		_bRegKeyBaseGet:NEAR
	EXTRN		_bHash:NEAR
	EXTRN		_bMod:NEAR
	EXTRN		_bEqual:NEAR
	EXTRN		_bMult:NEAR
	EXTRN		_stringToB:NEAR
	EXTRN		_bExpModN:NEAR
	EXTRN		_rkr:NEAR
	EXTRN		_initMod:NEAR
	EXTRN		_bModInitedGet:NEAR
	PUBLIC		_rkv			; Located at 1:0000h Type = 1

RKVAL_TEXT	SEGMENT
	assume	cs: RKVAL_TEXT


_rkv:	push	bp
	mov	bp,sp
	sub	sp,+05Ch
	call	far ptr _regKeyModInfoGet
	mov	[bp-008h],ax
	mov	[bp-006h],dx
	mov	ax,[bp+006h]
	or	ax,[bp+008h]
	jz	$L1
	mov	ax,[bp+00Ah]
	or	ax,[bp+00Ch]
	jz	$L1
	mov	ax,[bp+00Eh]
	or	ax,[bp+010h]
	jz	$L1
	mov	ax,[bp+01Ah]
	or	ax,[bp+01Ch]
	jnz	$L2
$L1:	mov	al,00h
	jmp	$L3
$L2:	call	far ptr _bModInitedGet
	mov	[bp-004h],ax
	mov	[bp-002h],dx
	les	bx,dword ptr [bp-004h]
	mov	ax,es:[bx]
	neg	ax
	sbb	ax,ax
	inc	ax
	mov	es:[bx],ax
	push	[bp+018h]
	push	[bp+016h]
	push	[bp+014h]
	push	[bp+012h]
	call	far ptr _rkr
	add	sp,+008h
	call	far ptr _initMod
	push	[bp+010h]
	push	[bp+00Eh]
	push	ss
	lea	ax,[bp-05Ch]
	push	ax
	call	far ptr _stringToB
	add	sp,+008h
	push	[bp+00Ch]
	push	[bp+00Ah]
	push	ss
	lea	ax,[bp-050h]
	push	ax
	call	far ptr _stringToB
	add	sp,+008h
	mov	word ptr [bp-020h],0000h
	mov	word ptr [bp-01Eh],0000h
	mov	word ptr [bp-01Ch],0000h
	mov	word ptr [bp-01Ah],0000h
	mov	word ptr [bp-018h],0000h
	mov	word ptr [bp-016h],0000h
	mov	ax,[bp-04Ah]
	mov	[bp-020h],ax
	mov	ax,[bp-048h]
	mov	[bp-01Eh],ax
	mov	ax,[bp-046h]
	mov	[bp-01Ch],ax
	mov	word ptr [bp-02Ch],0000h
	mov	word ptr [bp-02Ah],0000h
	mov	word ptr [bp-028h],0000h
	mov	word ptr [bp-026h],0000h
	mov	word ptr [bp-024h],0000h
	mov	word ptr [bp-022h],0000h
	mov	ax,[bp-050h]
	mov	[bp-02Ch],ax
	mov	ax,[bp-04Eh]
	mov	[bp-02Ah],ax
	mov	ax,[bp-04Ch]
	mov	[bp-028h],ax
	push	[bp-006h]
	push	[bp-008h]
	push	[bp+008h]
	push	[bp+006h]
	push	ss
	lea	ax,[bp-014h]
	push	ax
	call	far ptr _bHash
	add	sp,+00Ch
	push	[bp-006h]
	push	[bp-008h]
	push	ss
	lea	ax,[bp-014h]
	push	ax
	call	far ptr _bRegKeyBaseGet
	push	dx
	push	ax
	push	ss
	lea	ax,[bp-038h]
	push	ax
	call	far ptr _bExpModN
	add	sp,+010h
	push	[bp-006h]
	push	[bp-008h]
	push	ss
	lea	ax,[bp-020h]
	push	ax
	push	ss
	lea	ax,[bp-05Ch]
	push	ax
	push	ss
	lea	ax,[bp-044h]
	push	ax
	call	far ptr _bExpModN
	add	sp,+010h
	push	[bp-006h]
	push	[bp-008h]
	push	ss
	lea	ax,[bp-02Ch]
	push	ax
	push	ss
	lea	ax,[bp-020h]
	push	ax
	push	ss
	lea	ax,[bp-050h]
	push	ax
	call	far ptr _bExpModN
	add	sp,+010h
	push	ss
	lea	ax,[bp-050h]
	push	ax
	push	ss
	lea	ax,[bp-044h]
	push	ax
	call	far ptr _bMult
	add	sp,+008h
	push	[bp-006h]
	push	[bp-008h]
	push	ss
	lea	ax,[bp-044h]
	push	ax
	call	far ptr _bMod
	add	sp,+008h
	push	ss
	lea	ax,[bp-044h]
	push	ax
	push	ss
	lea	ax,[bp-038h]
	push	ax
	call	far ptr _bEqual
	add	sp,+008h
	or	ax,ax
	jz	$L4
	mov	al,01h
	jmp	short $L5
$L4:	mov	al,00h
$L5:	les	bx,dword ptr [bp+01Ah]
	mov	es:[bx],al
	mov	al,01h
$L3:	mov	sp,bp
	pop	bp
	retf

RKVAL_TEXT	ENDS
	END
