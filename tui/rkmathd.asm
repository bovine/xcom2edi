; OBJASM version 2.0 released on Jan 3, 1991
; (C) Copyright 1988,1989,1990,1991 by Robert F. Day.  All rights reserved

	TITLE	rkmath.c
; [000D]	; Compiler:	Turbo Assembler  Version 2.0
; [002F]; End of dependency list	
; [003A]
	.386p					; Enable USE32/USE16 usage
	.387					; Enable floating point also
RKMATH_TEXT	SEGMENT	BYTE PUBLIC USE16 'CODE'
RKMATH_TEXT	ENDS
_DATA	SEGMENT	WORD PUBLIC USE16 'DATA'
_DATA	ENDS
_BSS	SEGMENT	WORD PUBLIC USE16 'BSS'
_BSS	ENDS
DGROUP	GROUP	_BSS, _DATA
; [0306]; Dependent file: 	rkmath.c
; [0319]; Dependent file: 	regkey.h
; [032C]; Dependent file: 	rkintern.h
; [0341]; Dependent file: 	regkey.h
; [0354]; End of dependency list	


	PUBLIC		_bOneGet		; Located at 1:0000h Type = 1
	PUBLIC		_bRegKeyModGet		; Located at 1:000Ah Type = 1
	PUBLIC		_regKeyModInfoGet	; Located at 1:0014h Type = 1
	PUBLIC		_bModInitedGet		; Located at 1:001Eh Type = 1
	PUBLIC		_bModMinusOneGet	; Located at 1:0028h Type = 1
	PUBLIC		_modMinusOneInfoGet	; Located at 1:0032h Type = 1
	PUBLIC		_bRegKeyBaseGet		; Located at 1:003Ch Type = 1
	PUBLIC		_initMod		; Located at 1:0046h Type = 1
	PUBLIC		_bToString		; Located at 1:0082h Type = 1
	PUBLIC		_stringToB		; Located at 1:0103h Type = 1
	PUBLIC		_bShiftLeft		; Located at 1:017Dh Type = 1
	PUBLIC		_bEqual			; Located at 1:01D9h Type = 1
	PUBLIC		_bShiftRight		; Located at 1:022Ah Type = 1
	PUBLIC		_bShiftRightOne		; Located at 1:0286h Type = 1
	PUBLIC		_bAdd			; Located at 1:02A1h Type = 1
	PUBLIC		_bSub			; Located at 1:02C7h Type = 1
	PUBLIC		_bSignedMult		; Located at 1:02EDh Type = 1
	PUBLIC		_bMult			; Located at 1:0383h Type = 1
	PUBLIC		_bMod			; Located at 1:048Fh Type = 1
	PUBLIC		_bSignedMod		; Located at 1:05C2h Type = 1
	PUBLIC		_genModInfo		; Located at 1:0622h Type = 1
	PUBLIC		_bExpModN		; Located at 1:06BDh Type = 1
	PUBLIC		_bHash			; Located at 1:07D4h Type = 1
	PUBLIC		_bSpecialMod		; Located at 1:08F3h Type = 1
	PUBLIC		_bGCD			; Located at 1:0ACAh Type = 1
	PUBLIC		_bSignedDiv		; Located at 1:0C14h Type = 1
	PUBLIC		_bDiv			; Located at 1:0CD8h Type = 1
	PUBLIC		_bNegate		; Located at 1:0ED3h Type = 1
	PUBLIC		_bGCDWithCo		; Located at 1:0F1Bh Type = 1
	PUBLIC		_bLinearSolve		; Located at 1:111Dh Type = 1

RKMATH_TEXT	SEGMENT
	assume	cs: RKMATH_TEXT


_bOneGet:
	push	bp
	mov	bp,sp
	mov	dx,ds
	mov	ax,offset DGROUP:$S1
	pop	bp
	retf
_bRegKeyModGet:
	push	bp
	mov	bp,sp
	mov	dx,ds
	mov	ax,offset DGROUP:$S2
	pop	bp
	retf
_regKeyModInfoGet:
	push	bp
	mov	bp,sp
	mov	dx,ds
	mov	ax,offset DGROUP:$S3
	pop	bp
	retf
_bModInitedGet:
	push	bp
	mov	bp,sp
	mov	dx,ds
	mov	ax,offset DGROUP:$S4
	pop	bp
	retf
_bModMinusOneGet:
	push	bp
	mov	bp,sp
	mov	dx,ds
	mov	ax,offset DGROUP:$S5
	pop	bp
	retf
_modMinusOneInfoGet:
	push	bp
	mov	bp,sp
	mov	dx,ds
	mov	ax,offset DGROUP:$S6
	pop	bp
	retf
_bRegKeyBaseGet:
	push	bp
	mov	bp,sp
	mov	dx,ds
	mov	ax,offset DGROUP:$S7
	pop	bp
	retf
_initMod:
	push	bp
	mov	bp,sp
	assume	ds: DGROUP
	cmp	word ptr $S4,+000h
	jnz	$L8
	push	ds
	mov	ax,offset DGROUP:$S2
	push	ax
	push	ds
	mov	ax,offset DGROUP:$S3
	push	ax
	push	cs
	call	_genModInfo
	nop
	add	sp,+008h
	push	ds
	mov	ax,offset DGROUP:$S5
	push	ax
	push	ds
	mov	ax,offset DGROUP:$S6
	push	ax
	push	cs
	call	_genModInfo
	nop
	add	sp,+008h
	mov	word ptr $S4,0001h
	mov	word ptr $S9,0000h
$L8:	pop	bp
	retf
_bToString:
	push	bp
	mov	bp,sp
	sub	sp,+010h
	push	si
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	mov	[bp-010h],ax
	mov	ax,es:[bx+002h]
	mov	[bp-00Eh],ax
	mov	ax,es:[bx+004h]
	mov	[bp-00Ch],ax
	mov	ax,es:[bx+006h]
	mov	[bp-00Ah],ax
	mov	ax,es:[bx+008h]
	mov	[bp-008h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-006h],ax
	mov	ax,[bp+008h]
	mov	dx,[bp+006h]
	add	dx,+014h
	mov	[bp-004h],dx
	mov	[bp-002h],ax
	les	bx,dword ptr [bp-004h]
	mov	byte ptr es:[bx],00h
	dec	word ptr [bp-004h]
	mov	si,0014h
	jmp	short $L10
$L12:	mov	bx,[bp-010h]
	and	bx,+01Fh
	mov	al,[bx+$S11]
	les	bx,dword ptr [bp-004h]
	mov	es:[bx],al
	dec	word ptr [bp-004h]
	mov	ax,0005h
	push	ax
	push	ss
	lea	ax,[bp-010h]
	push	ax
	push	cs
	call	_bShiftRight
	nop
	add	sp,+006h
$L10:	mov	ax,si
	dec	si
	or	ax,ax
	jnz	$L12
	pop	si
	mov	sp,bp
	pop	bp
	retf
_stringToB:
	push	bp
	mov	bp,sp
	push	si
	xor	si,si
	les	bx,dword ptr [bp+006h]
	mov	word ptr es:[bx],0000h
	mov	word ptr es:[bx+002h],0000h
	mov	word ptr es:[bx+004h],0000h
	mov	word ptr es:[bx+006h],0000h
	mov	word ptr es:[bx+008h],0000h
	mov	word ptr es:[bx+00Ah],0000h
	jmp	short $L13
$L14:	inc	word ptr [bp+00Ah]
$L13:	les	bx,dword ptr [bp+00Ah]
	cmp	byte ptr es:[bx],20h
	jz	$L14
	jmp	short $L15
$L18:	mov	ax,0005h
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	near ptr _bShiftLeft
	nop
	add	sp,+006h
	les	bx,dword ptr [bp+00Ah]
	mov	al,es:[bx]
	cbw
	shl	ax,1
	mov	bx,ax
	mov	ax,[bx+$S16]
	les	bx,dword ptr [bp+006h]
	or	es:[bx],ax
	inc	word ptr [bp+00Ah]
$L15:	les	bx,dword ptr [bp+00Ah]
	cmp	byte ptr es:[bx],00h
	jz	$L17
	mov	ax,si
	inc	si
	cmp	ax,0014h
	jl	$L18
$L17:	pop	si
	pop	bp
	retf
_bShiftLeft:
	push	bp
	mov	bp,sp
	dec	sp
	dec	sp
	push	si
	push	di
	xor	di,di
	mov	ax,0010h
	sub	ax,[bp+00Ah]
	mov	[bp-002h],ax
	xor	si,si
	jmp	short $L19
$L20:	mov	ax,si
	shl	ax,1
	les	bx,dword ptr [bp+006h]
	add	bx,ax
	mov	ax,es:[bx]
	mov	cl,[bp+00Ah]
	shl	ax,cl
	or	ax,di
	mov	dx,ax
	mov	ax,si
	shl	ax,1
	mov	bx,[bp+006h]
	add	bx,ax
	mov	ax,es:[bx]
	mov	cl,[bp-002h]
	shr	ax,cl
	mov	di,ax
	mov	ax,si
	shl	ax,1
	mov	bx,[bp+006h]
	add	bx,ax
	mov	ax,dx
	mov	es:[bx],ax
	mov	ax,si
	inc	ax
	mov	si,ax
$L19:	cmp	si,+006h
	jl	$L20
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	retf
_bEqual:
	push	bp
	mov	bp,sp
	sub	sp,+008h
	mov	ax,[bp+008h]
	mov	dx,[bp+006h]
	add	dx,+00Ah
	mov	[bp-004h],dx
	mov	[bp-002h],ax
	mov	ax,[bp+00Ch]
	mov	dx,[bp+00Ah]
	add	dx,+00Ah
	mov	[bp-008h],dx
	mov	[bp-006h],ax
	mov	cl,06h
	jmp	short $L21
$L24:	les	bx,dword ptr [bp-004h]
	mov	ax,es:[bx]
	les	bx,dword ptr [bp-008h]
	cmp	ax,es:[bx]
	jz	$L22
	xor	ax,ax
	jmp	short $L23
$L22:	sub	word ptr [bp-004h],+002h
	sub	word ptr [bp-008h],+002h
$L21:	mov	al,cl
	dec	cl
	or	al,al
	jnz	$L24
	mov	ax,0001h
$L23:	mov	sp,bp
	pop	bp
	retf
_bShiftRight:
	push	bp
	mov	bp,sp
	dec	sp
	dec	sp
	push	si
	push	di
	xor	di,di
	mov	ax,0010h
	sub	ax,[bp+00Ah]
	mov	[bp-002h],ax
	mov	si,0005h
	jmp	short $L25
$L26:	mov	ax,si
	shl	ax,1
	les	bx,dword ptr [bp+006h]
	add	bx,ax
	mov	ax,es:[bx]
	mov	cl,[bp+00Ah]
	shr	ax,cl
	or	ax,di
	mov	dx,ax
	mov	ax,si
	shl	ax,1
	mov	bx,[bp+006h]
	add	bx,ax
	mov	ax,es:[bx]
	mov	cl,[bp-002h]
	shl	ax,cl
	mov	di,ax
	mov	ax,si
	shl	ax,1
	mov	bx,[bp+006h]
	add	bx,ax
	mov	ax,dx
	mov	es:[bx],ax
	mov	ax,si
	dec	ax
	mov	si,ax
$L25:	or	si,si
	jnl	$L26
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	retf
_bShiftRightOne:
	push	bp
	mov	bp,sp
	push	si
	push	ds
	push	si
	lds	si,dword ptr [bp+006h]
	add	si,+00Ah
	mov	cx,0006h
	clc
$L27:	rcr	word ptr [si],1
	dec	si
	dec	si
	loop	$L27
	pop	si
	pop	ds
	pop	si
	pop	bp
	retf
_bAdd:	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	push	es
	push	si
	push	di
	les	di,dword ptr [bp+006h]
	lds	si,dword ptr [bp+00Ah]
	mov	cx,0006h
	clc
$L28:	mov	ax,[si]
	adc	es:[di],ax
	inc	si
	inc	si
	inc	di
	inc	di
	loop	$L28
	pop	di
	pop	si
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bp
	retf
_bSub:	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	push	es
	push	si
	push	di
	les	di,dword ptr [bp+006h]
	lds	si,dword ptr [bp+00Ah]
	mov	cx,0006h
	clc
$L29:	mov	ax,[si]
	sbb	es:[di],ax
	inc	si
	inc	si
	inc	di
	inc	di
	loop	$L29
	pop	di
	pop	si
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bp
	retf
_bSignedMult:
	push	bp
	mov	bp,sp
	sub	sp,+00Ch
	push	si
	xor	si,si
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	mov	[bp-00Ch],ax
	mov	ax,es:[bx+002h]
	mov	[bp-00Ah],ax
	mov	ax,es:[bx+004h]
	mov	[bp-008h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-006h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-004h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-002h],ax
	les	bx,dword ptr [bp+006h]
	test	word ptr es:[bx+00Ah],8000h
	jz	$L30
	mov	si,0001h
	push	[bp+008h]
	push	bx
	push	cs
	call	_bNegate
	nop
	add	sp,+004h
$L30:	test	word ptr [bp-002h],8000h
	jz	$L31
	mov	ax,si
	neg	ax
	sbb	ax,ax
	inc	ax
	mov	si,ax
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	cs
	call	_bNegate
	nop
	add	sp,+004h
$L31:	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	near ptr _bMult
	nop
	add	sp,+008h
	or	si,si
	jz	$L32
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bNegate
	nop
	add	sp,+004h
$L32:	pop	si
	mov	sp,bp
	pop	bp
	retf
_bMult:	push	bp
	mov	bp,sp
	sub	sp,+024h
	push	si
	push	di
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx]
	mov	[bp-018h],ax
	mov	ax,es:[bx+002h]
	mov	[bp-016h],ax
	mov	ax,es:[bx+004h]
	mov	[bp-014h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-012h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-010h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-00Eh],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	mov	[bp-024h],ax
	mov	ax,es:[bx+002h]
	mov	[bp-022h],ax
	mov	ax,es:[bx+004h]
	mov	[bp-020h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-01Eh],ax
	mov	ax,es:[bx+008h]
	mov	[bp-01Ch],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-01Ah],ax

	mov	word ptr [bp-00Ch],0000h
	mov	word ptr [bp-00Ah],0000h
	mov	word ptr [bp-008h],0000h
	mov	word ptr [bp-006h],0000h
	mov	word ptr [bp-004h],0000h
	mov	word ptr [bp-002h],0000h
	push	es
	push	ds
	push	di
	push	si
	mov	ax,ss
	mov	ds,ax
	mov	es,ax
	xor	cl,cl
$L37:	xor	ch,ch
	mov	ax,cx
	shl	ax,1
	lea	di,[bp-00Ch]
	add	di,ax
	lea	si,[bp-024h]
	add	si,ax
	mov	bx,[si]
	lea	si,[bp-018h]
$L36:	mov	ax,[si]
	mul	bx
	add	es:[di],ax
	inc	di
	inc	di
	adc	es:[di],dx
	jnc	$L33
	mov	ah,ch
	adc	ah,cl
	mov	dx,di
$L35:	inc	ah
	cmp	ah,06h
	jnl	$L34
	add	di,+002h
	add	word ptr es:[di],+001h
	jc	$L35
$L34:	mov	di,dx
$L33:	add	si,+002h
	inc	ch
	cmp	ch,03h
	jl	$L36
	inc	cl
	cmp	cl,03h
	jl	$L37
	pop	si
	pop	di
	pop	ds
	pop	es
	les	bx,dword ptr [bp+006h]
	mov	ax,[bp-00Ch]
	mov	es:[bx],ax
	mov	ax,[bp-00Ah]
	mov	es:[bx+002h],ax
	mov	ax,[bp-008h]
	mov	es:[bx+004h],ax
	mov	ax,[bp-006h]
	mov	es:[bx+006h],ax
	mov	ax,[bp-004h]
	mov	es:[bx+008h],ax
	mov	ax,[bp-002h]
	mov	es:[bx+00Ah],ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	retf
_bMod:	push	bp
	mov	bp,sp
	sub	sp,+004h
	push	si
	les	bx,dword ptr [bp+00Ah]
	mov	si,es:[bx]
	mov	ax,si
	dec	ax
	mov	dx,000Ch
	imul	dx
	mov	dx,[bp+00Ch]
	add	bx,ax
	inc	bx
	inc	bx
	mov	[bp-004h],bx
	mov	[bp-002h],dx
	jmp	$L38
$L58:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+00Ah]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+00Ah]
	jz	$L39
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+00Ah]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+00Ah]
	jbe	$L40
	mov	ax,0001h
	jmp	short $L41
$L40:	xor	ax,ax
$L41:	jmp	$L42
$L39:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+008h]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+008h]
	jz	$L43
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+008h]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+008h]
	jbe	$L44
	mov	ax,0001h
	jmp	short $L45
$L44:	xor	ax,ax
$L45:	jmp	$L42
$L43:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+006h]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+006h]
	jz	$L46
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+006h]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+006h]
	jbe	$L47
	mov	ax,0001h
	jmp	short $L48
$L47:	xor	ax,ax
$L48:	jmp	short $L42
$L46:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+004h]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+004h]
	jz	$L49
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+004h]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+004h]
	jbe	$L50
	mov	ax,0001h
	jmp	short $L51
$L50:	xor	ax,ax
$L51:	jmp	short $L42
$L49:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+002h]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+002h]
	jz	$L52
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+002h]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx+002h]
	jbe	$L53
	mov	ax,0001h
	jmp	short $L54
$L53:	xor	ax,ax
$L54:	jmp	short $L42
$L52:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx]
	les	bx,dword ptr [bp-004h]
	cmp	ax,es:[bx]
	jc	$L55
	mov	ax,0001h
	jmp	short $L42
$L55:	xor	ax,ax
$L42:	or	ax,ax
	jz	$L56
	push	[bp-002h]
	push	[bp-004h]
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bSub
	add	sp,+008h
$L56:	sub	word ptr [bp-004h],+00Ch
$L38:	mov	ax,si
	dec	si
	or	ax,ax
	jz	$L57
	jmp	$L58
$L57:	pop	si
	mov	sp,bp
	pop	bp
	retf
_bSignedMod:
	push	bp
	mov	bp,sp
	push	si
	les	bx,dword ptr [bp+006h]
	test	word ptr es:[bx+00Ah],8000h
	jz	$L59
	mov	si,0001h
	push	[bp+008h]
	push	bx
	push	cs
	call	_bNegate
	nop
	add	sp,+004h
	jmp	short $L60
$L59:	xor	si,si
$L60:	push	[bp+00Ch]
	push	[bp+00Ah]
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bMod
	add	sp,+008h
	or	si,si
	jz	$L61
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bNegate
	nop
	add	sp,+004h
	mov	ax,[bp+00Ah]
	inc	ax
	inc	ax
	push	[bp+00Ch]
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bAdd
	add	sp,+008h
$L61:	pop	si
	pop	bp
	retf
_genModInfo:
	push	bp
	mov	bp,sp
	sub	sp,+010h
	push	si
	xor	si,si
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	mov	[bp-010h],ax
	mov	ax,es:[bx+002h]
	mov	[bp-00Eh],ax
	mov	ax,es:[bx+004h]
	mov	[bp-00Ch],ax
	mov	ax,es:[bx+006h]
	mov	[bp-00Ah],ax
	mov	ax,es:[bx+008h]
	mov	[bp-008h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-006h],ax
$L63:	mov	ax,si
	mov	dx,000Ch
	imul	dx
	mov	dx,[bp+008h]
	mov	bx,[bp+006h]
	add	bx,ax
	inc	bx
	inc	bx
	mov	[bp-004h],bx
	mov	[bp-002h],dx
	inc	si
	les	bx,dword ptr [bp-004h]
	mov	ax,[bp-010h]
	mov	es:[bx],ax
	mov	ax,[bp-00Eh]
	mov	es:[bx+002h],ax
	mov	ax,[bp-00Ch]
	mov	es:[bx+004h],ax
	mov	ax,[bp-00Ah]
	mov	es:[bx+006h],ax
	mov	ax,[bp-008h]
	mov	es:[bx+008h],ax
	mov	ax,[bp-006h]
	mov	es:[bx+00Ah],ax
	test	ax,8000h
	jnz	$L62
	mov	ax,0001h
	push	ax
	push	ss
	lea	ax,[bp-010h]
	push	ax
	push	cs
	call	_bShiftLeft
	add	sp,+006h
	jmp	short $L63
$L62:	les	bx,dword ptr [bp+006h]
	mov	es:[bx],si
	pop	si
	mov	sp,bp
	pop	bp
	retf
_bExpModN:
	push	bp
	mov	bp,sp
	sub	sp,+018h
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	mov	[bp-00Ch],ax
	mov	ax,es:[bx+002h]
	mov	[bp-00Ah],ax
	mov	ax,es:[bx+004h]
	mov	[bp-008h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-006h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-004h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-002h],ax
	les	bx,dword ptr [bp+00Eh]
	mov	ax,es:[bx]
	mov	[bp-018h],ax
	mov	ax,es:[bx+002h]
	mov	[bp-016h],ax
	mov	ax,es:[bx+004h]
	mov	[bp-014h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-012h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-010h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-00Eh],ax
	les	bx,dword ptr [bp+006h]
	mov	word ptr es:[bx],0001h
	mov	word ptr es:[bx+002h],0000h
	mov	word ptr es:[bx+004h],0000h
	mov	word ptr es:[bx+006h],0000h
	mov	word ptr es:[bx+008h],0000h
	mov	word ptr es:[bx+00Ah],0000h
	jmp	short $L64
$L65:	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	cs
	call	_bShiftRightOne
	add	sp,+004h
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	ss
	push	ax
	push	cs
	call	_bMult
	add	sp,+008h
	push	[bp+014h]
	push	[bp+012h]
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	cs
	call	_bMod
	add	sp,+008h
$L66:	test	word ptr [bp-018h],0001h
	jz	$L65
	push	ds
	mov	ax,offset DGROUP:$S1
	push	ax
	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	cs
	call	_bSub
	add	sp,+008h
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bMult
	add	sp,+008h
	push	[bp+014h]
	push	[bp+012h]
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bMod
	add	sp,+008h
$L64:	cmp	word ptr [bp-018h],+000h
	jnz	$L66
	cmp	word ptr [bp-016h],+000h
	jnz	$L66
	cmp	word ptr [bp-014h],+000h
	jnz	$L66
	cmp	word ptr [bp-012h],+000h
	jnz	$L66

	cmp	word ptr [bp-010h],+000h
	jnz	$L66
	cmp	word ptr [bp-00Eh],+000h
	jnz	$L66
	mov	sp,bp
	pop	bp
	retf
_bHash:	push	bp
	mov	bp,sp
	sub	sp,+012h
	les	bx,dword ptr [bp+006h]
	mov	word ptr es:[bx],7AF1h
	mov	word ptr es:[bx+002h],4D92h
	mov	word ptr es:[bx+004h],0E8F4h
	mov	word ptr es:[bx+006h],0000h
	mov	word ptr es:[bx+008h],0000h
	mov	word ptr es:[bx+00Ah],0000h
	jmp	$L67
$L72:	mov	word ptr [bp-012h],0000h
	mov	word ptr [bp-010h],0000h
	mov	word ptr [bp-00Eh],0000h
	mov	word ptr [bp-00Ch],0000h
	mov	word ptr [bp-00Ah],0000h
	mov	word ptr [bp-008h],0000h
	lea	ax,[bp-012h]
	mov	[bp-004h],ax
	mov	word ptr [bp-002h],ss
	mov	byte ptr [bp-005h],06h
	jmp	short $L68
$L70:	les	bx,dword ptr [bp+00Ah]
	mov	al,es:[bx]
	les	bx,dword ptr [bp-004h]
	mov	es:[bx],al
	inc	word ptr [bp+00Ah]
	inc	word ptr [bp-004h]
$L68:	mov	al,[bp-005h]
	dec	byte ptr [bp-005h]
	or	al,al
	jz	$L69
	les	bx,dword ptr [bp+00Ah]
	cmp	byte ptr es:[bx],00h
	jnz	$L70
$L69:	push	ss
	lea	ax,[bp-012h]
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bAdd
	add	sp,+008h
	push	[bp+010h]
	push	[bp+00Eh]
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bMod
	add	sp,+008h
	push	[bp+008h]
	push	[bp+006h]
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bMult
	add	sp,+008h
	push	[bp+010h]
	push	[bp+00Eh]
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bMod
	add	sp,+008h
$L67:	les	bx,dword ptr [bp+00Ah]
	cmp	byte ptr es:[bx],00h
	jz	$L71
	jmp	$L72
$L71:	mov	word ptr [bp-012h],926Ah
	mov	word ptr [bp-010h],92D3h
	mov	word ptr [bp-00Eh],6C72h
	mov	word ptr [bp-00Ch],0000h
	mov	word ptr [bp-00Ah],0000h
	mov	word ptr [bp-008h],0000h
	push	ss
	lea	ax,[bp-012h]
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bAdd
	add	sp,+008h
	push	[bp+010h]
	push	[bp+00Eh]
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bMod
	add	sp,+008h
	mov	sp,bp
	pop	bp
	retf
_bSpecialMod:
	push	bp
	mov	bp,sp
	sub	sp,+00Ch
	push	si
	xor	si,si
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	mov	[bp-00Ch],ax
	mov	ax,es:[bx+002h]
	mov	[bp-00Ah],ax
	mov	ax,es:[bx+004h]
	mov	[bp-008h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-006h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-004h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-002h],ax
	jmp	short $L73
$L92:	mov	ax,0001h
	push	ax
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	cs
	call	_bShiftLeft
	add	sp,+006h
	inc	si
$L73:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+00Ah]
	cmp	ax,[bp-002h]
	jz	$L74
	mov	ax,es:[bx+00Ah]
	cmp	ax,[bp-002h]
	jbe	$L75
	mov	ax,0001h
	jmp	short $L76
$L75:	xor	ax,ax
$L76:	jmp	$L77
$L74:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+008h]
	cmp	ax,[bp-004h]
	jz	$L78
	mov	ax,es:[bx+008h]
	cmp	ax,[bp-004h]
	jbe	$L79
	mov	ax,0001h
	jmp	short $L80
$L79:	xor	ax,ax
$L80:	jmp	short $L77
$L78:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+006h]
	cmp	ax,[bp-006h]
	jz	$L81
	mov	ax,es:[bx+006h]
	cmp	ax,[bp-006h]
	jbe	$L82
	mov	ax,0001h
	jmp	short $L83
$L82:	xor	ax,ax
$L83:	jmp	short $L77
$L81:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+004h]
	cmp	ax,[bp-008h]
	jz	$L84
	mov	ax,es:[bx+004h]
	cmp	ax,[bp-008h]
	jbe	$L85
	mov	ax,0001h
	jmp	short $L86
$L85:	xor	ax,ax
$L86:	jmp	short $L77
$L84:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+002h]
	cmp	ax,[bp-00Ah]
	jz	$L87
	mov	ax,es:[bx+002h]
	cmp	ax,[bp-00Ah]
	jbe	$L88
	mov	ax,0001h
	jmp	short $L89
$L88:	xor	ax,ax
$L89:	jmp	short $L77
$L87:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx]
	cmp	ax,[bp-00Ch]
	jc	$L90
	mov	ax,0001h
	jmp	short $L77
$L90:	xor	ax,ax
$L77:	or	ax,ax
	jz	$L91
	cmp	si,+060h
	jnl	$L91
	jmp	$L92
$L91:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+00Ah]
	cmp	ax,[bp-002h]
	jz	$L93
	mov	ax,es:[bx+00Ah]
	cmp	ax,[bp-002h]
	jbe	$L94
	mov	ax,0001h
	jmp	short $L95
$L94:	xor	ax,ax
$L95:	jmp	$L96
$L93:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+008h]
	cmp	ax,[bp-004h]
	jz	$L97
	mov	ax,es:[bx+008h]
	cmp	ax,[bp-004h]
	jbe	$L98
	mov	ax,0001h
	jmp	short $L99
$L98:	xor	ax,ax
$L99:	jmp	short $L96
$L97:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+006h]
	cmp	ax,[bp-006h]
	jz	$L100
	mov	ax,es:[bx+006h]
	cmp	ax,[bp-006h]
	jbe	$L101
	mov	ax,0001h
	jmp	short $L102
$L101:	xor	ax,ax
$L102:	jmp	short $L96
$L100:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+004h]
	cmp	ax,[bp-008h]
	jz	$L103
	mov	ax,es:[bx+004h]
	cmp	ax,[bp-008h]
	jbe	$L104
	mov	ax,0001h
	jmp	short $L105
$L104:	xor	ax,ax
$L105:	jmp	short $L96
$L103:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+002h]
	cmp	ax,[bp-00Ah]
	jz	$L106
	mov	ax,es:[bx+002h]
	cmp	ax,[bp-00Ah]
	jbe	$L107
	mov	ax,0001h
	jmp	short $L108
$L107:	xor	ax,ax
$L108:	jmp	short $L96
$L106:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx]
	cmp	ax,[bp-00Ch]
	jc	$L109
	mov	ax,0001h
	jmp	short $L96
$L109:	xor	ax,ax
$L96:	or	ax,ax
	jz	$L110
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bSub
	add	sp,+008h
$L110:	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	cs
	call	_bShiftRightOne
	add	sp,+004h
	mov	ax,si
	dec	si
	or	ax,ax
	jz	$L111
	jmp	$L91
$L111:	pop	si
	mov	sp,bp
	pop	bp
	retf
_bGCD:	push	bp
	mov	bp,sp
	sub	sp,+018h
	les	bx,dword ptr [bp+00Eh]
	mov	ax,es:[bx]
	mov	[bp-018h],ax
	mov	ax,es:[bx+002h]
	mov	[bp-016h],ax
	mov	ax,es:[bx+004h]
	mov	[bp-014h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-012h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-010h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-00Eh],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	les	bx,dword ptr [bp+006h]
	mov	es:[bx],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+002h]
	les	bx,dword ptr [bp+006h]
	mov	es:[bx+002h],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+004h]
	les	bx,dword ptr [bp+006h]
	mov	es:[bx+004h],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+006h]
	les	bx,dword ptr [bp+006h]
	mov	es:[bx+006h],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+008h]
	les	bx,dword ptr [bp+006h]
	mov	es:[bx+008h],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+00Ah]
	les	bx,dword ptr [bp+006h]
	jmp	$L112
$L114:	mov	ax,[bp-018h]
	mov	[bp-00Ch],ax
	mov	ax,[bp-016h]
	mov	[bp-00Ah],ax
	mov	ax,[bp-014h]
	mov	[bp-008h],ax
	mov	ax,[bp-012h]
	mov	[bp-006h],ax
	mov	ax,[bp-010h]
	mov	[bp-004h],ax
	mov	ax,[bp-00Eh]
	mov	[bp-002h],ax
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx]
	mov	[bp-018h],ax
	mov	ax,es:[bx+002h]
	mov	[bp-016h],ax
	mov	ax,es:[bx+004h]
	mov	[bp-014h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-012h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-010h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-00Eh],ax
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	ss
	lea	ax,[bp-018h]

	push	ax
	push	cs
	call	_bSpecialMod
	add	sp,+008h
	les	bx,dword ptr [bp+006h]
	mov	ax,[bp-00Ch]
	mov	es:[bx],ax
	mov	ax,[bp-00Ah]
	mov	es:[bx+002h],ax
	mov	ax,[bp-008h]
	mov	es:[bx+004h],ax
	mov	ax,[bp-006h]
	mov	es:[bx+006h],ax
	mov	ax,[bp-004h]
	mov	es:[bx+008h],ax
	mov	ax,[bp-002h]
$L112:	mov	es:[bx+00Ah],ax
	cmp	word ptr [bp-018h],+000h
	jz	$L113
	jmp	$L114
$L113:	cmp	word ptr [bp-016h],+000h
	jz	$L115
	jmp	$L114
$L115:	cmp	word ptr [bp-014h],+000h
	jz	$L116
	jmp	$L114
$L116:	cmp	word ptr [bp-012h],+000h
	jz	$L117
	jmp	$L114
$L117:	cmp	word ptr [bp-010h],+000h
	jz	$L118
	jmp	$L114
$L118:	cmp	word ptr [bp-00Eh],+000h
	jz	$L119
	jmp	$L114
$L119:	mov	sp,bp
	pop	bp
	retf
_bSignedDiv:
	push	bp
	mov	bp,sp
	sub	sp,+018h
	push	si
	xor	si,si
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	mov	[bp-00Ch],ax
	mov	ax,es:[bx+002h]
	mov	[bp-00Ah],ax
	mov	ax,es:[bx+004h]
	mov	[bp-008h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-006h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-004h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-002h],ax
	les	bx,dword ptr [bp+00Eh]
	mov	ax,es:[bx]
	mov	[bp-018h],ax
	mov	ax,es:[bx+002h]
	mov	[bp-016h],ax
	mov	ax,es:[bx+004h]
	mov	[bp-014h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-012h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-010h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-00Eh],ax
	test	word ptr [bp-002h],8000h
	jz	$L120
	mov	si,0001h
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	cs
	call	_bNegate
	nop
	add	sp,+004h
$L120:	test	word ptr [bp-00Eh],8000h
	jz	$L121
	mov	ax,si
	neg	ax
	sbb	ax,ax
	inc	ax
	mov	si,ax
	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	cs
	call	_bNegate
	nop
	add	sp,+004h
$L121:	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	near ptr _bDiv
	nop
	add	sp,+00Ch
	or	si,si
	jz	$L122
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bNegate
	nop
	add	sp,+004h
$L122:	pop	si
	mov	sp,bp
	pop	bp
	retf
_bDiv:	push	bp
	mov	bp,sp
	sub	sp,+018h
	push	si
	xor	si,si
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	mov	[bp-00Ch],ax
	mov	ax,es:[bx+002h]
	mov	[bp-00Ah],ax
	mov	ax,es:[bx+004h]
	mov	[bp-008h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-006h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-004h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-002h],ax
	les	bx,dword ptr [bp+00Eh]
	mov	ax,es:[bx]
	mov	[bp-018h],ax
	mov	ax,es:[bx+002h]
	mov	[bp-016h],ax
	mov	ax,es:[bx+004h]
	mov	[bp-014h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-012h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-010h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-00Eh],ax
	les	bx,dword ptr [bp+006h]
	mov	word ptr es:[bx],0000h
	mov	word ptr es:[bx+002h],0000h
	mov	word ptr es:[bx+004h],0000h
	mov	word ptr es:[bx+006h],0000h
	mov	word ptr es:[bx+008h],0000h
	mov	word ptr es:[bx+00Ah],0000h
	jmp	short $L123
$L142:	mov	ax,0001h
	push	ax
	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	cs
	call	_bShiftLeft
	add	sp,+006h
	inc	si
$L123:	mov	ax,[bp-002h]
	cmp	ax,[bp-00Eh]
	jz	$L124
	cmp	ax,[bp-00Eh]
	jbe	$L125
	mov	ax,0001h
	jmp	short $L126
$L125:	xor	ax,ax
$L126:	jmp	short $L127
$L124:	mov	ax,[bp-004h]
	cmp	ax,[bp-010h]
	jz	$L128
	cmp	ax,[bp-010h]
	jbe	$L129
	mov	ax,0001h
	jmp	short $L130
$L129:	xor	ax,ax
$L130:	jmp	short $L127
$L128:	mov	ax,[bp-006h]
	cmp	ax,[bp-012h]
	jz	$L131
	cmp	ax,[bp-012h]
	jbe	$L132
	mov	ax,0001h
	jmp	short $L133
$L132:	xor	ax,ax
$L133:	jmp	short $L127
$L131:	mov	ax,[bp-008h]
	cmp	ax,[bp-014h]
	jz	$L134
	cmp	ax,[bp-014h]
	jbe	$L135
	mov	ax,0001h
	jmp	short $L136
$L135:	xor	ax,ax
$L136:	jmp	short $L127
$L134:	mov	ax,[bp-00Ah]
	cmp	ax,[bp-016h]
	jz	$L137
	cmp	ax,[bp-016h]
	jbe	$L138
	mov	ax,0001h
	jmp	short $L139
$L138:	xor	ax,ax
$L139:	jmp	short $L127
$L137:	mov	ax,[bp-00Ch]
	cmp	ax,[bp-018h]
	jc	$L140
	mov	ax,0001h
	jmp	short $L127
$L140:	xor	ax,ax
$L127:	or	ax,ax
	jz	$L141
	jmp	$L142
$L141:	mov	ax,[bp-002h]
	cmp	ax,[bp-00Eh]
	jz	$L143
	cmp	ax,[bp-00Eh]
	jbe	$L144
	mov	ax,0001h
	jmp	short $L145
$L144:	xor	ax,ax
$L145:	jmp	short $L146
$L143:	mov	ax,[bp-004h]
	cmp	ax,[bp-010h]
	jz	$L147
	cmp	ax,[bp-010h]
	jbe	$L148
	mov	ax,0001h
	jmp	short $L149
$L148:	xor	ax,ax
$L149:	jmp	short $L146
$L147:	mov	ax,[bp-006h]
	cmp	ax,[bp-012h]
	jz	$L150
	cmp	ax,[bp-012h]
	jbe	$L151
	mov	ax,0001h
	jmp	short $L152
$L151:	xor	ax,ax
$L152:	jmp	short $L146
$L150:	mov	ax,[bp-008h]
	cmp	ax,[bp-014h]
	jz	$L153
	cmp	ax,[bp-014h]
	jbe	$L154
	mov	ax,0001h
	jmp	short $L155
$L154:	xor	ax,ax
$L155:	jmp	short $L146
$L153:	mov	ax,[bp-00Ah]
	cmp	ax,[bp-016h]
	jz	$L156
	cmp	ax,[bp-016h]
	jbe	$L157
	mov	ax,0001h
	jmp	short $L158
$L157:	xor	ax,ax
$L158:	jmp	short $L146
$L156:	mov	ax,[bp-00Ch]
	cmp	ax,[bp-018h]
	jc	$L159
	mov	ax,0001h
	jmp	short $L146
$L159:	xor	ax,ax
$L146:	or	ax,ax
	jz	$L160
	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	cs
	call	_bSub
	add	sp,+008h
	mov	ax,0001h
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bShiftLeft
	add	sp,+006h
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx]
	or	ax,0001h
	mov	es:[bx],ax
	jmp	short $L161
$L160:	mov	ax,0001h
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bShiftLeft
	add	sp,+006h
$L161:	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	cs
	call	_bShiftRightOne
	add	sp,+004h
	mov	ax,si
	dec	si
	or	ax,ax
	jz	$L162
	jmp	$L141
$L162:	pop	si
	mov	sp,bp
	pop	bp
	retf
_bNegate:
	push	bp
	mov	bp,sp
	sub	sp,+006h
	mov	byte ptr [bp-001h],06h
	mov	ax,[bp+008h]
	mov	dx,[bp+006h]
	mov	[bp-006h],dx
	mov	[bp-004h],ax
	jmp	short $L163
$L164:	les	bx,dword ptr [bp-006h]
	mov	ax,es:[bx]
	not	ax
	mov	es:[bx],ax
	add	word ptr [bp-006h],+002h
$L163:	mov	al,[bp-001h]
	dec	byte ptr [bp-001h]
	or	al,al
	jnz	$L164
	push	cs
	call	_bOneGet
	push	dx
	push	ax
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bAdd
	add	sp,+008h
	mov	sp,bp
	pop	bp
	retf
_bGCDWithCo:
	push	bp
	mov	bp,sp
	sub	sp,+024h
	les	bx,dword ptr [bp+00Ah]
	cmp	word ptr es:[bx],+000h
	jz	$L165
	jmp	$L166
$L165:	cmp	word ptr es:[bx+002h],+000h
	jz	$L167
	jmp	$L166
$L167:	cmp	word ptr es:[bx+004h],+000h
	jz	$L168
	jmp	$L166
$L168:	cmp	word ptr es:[bx+006h],+000h
	jz	$L169
	jmp	$L166
$L169:	cmp	word ptr es:[bx+008h],+000h
	jz	$L170
	jmp	$L166
$L170:	cmp	word ptr es:[bx+00Ah],+000h
	jz	$L171
	jmp	$L166
$L171:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx],ax
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+002h]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+002h],ax
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+004h]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+004h],ax
	les	bx,dword ptr [bp+006h]

	mov	ax,es:[bx+006h]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+006h],ax
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+008h]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+008h],ax
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx+00Ah]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+00Ah],ax
	les	bx,dword ptr [bp+012h]
	mov	word ptr es:[bx],0001h
	mov	word ptr es:[bx+002h],0000h
	mov	word ptr es:[bx+004h],0000h
	mov	word ptr es:[bx+006h],0000h
	mov	word ptr es:[bx+008h],0000h
	mov	word ptr es:[bx+00Ah],0000h
	les	bx,dword ptr [bp+016h]
	mov	word ptr es:[bx],0000h
	mov	word ptr es:[bx+002h],0000h
	mov	word ptr es:[bx+004h],0000h
	mov	word ptr es:[bx+006h],0000h
	mov	word ptr es:[bx+008h],0000h
	mov	word ptr es:[bx+00Ah],0000h
	jmp	$L172
$L166:	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx]
	mov	[bp-00Ch],ax
	mov	ax,es:[bx+002h]
	mov	[bp-00Ah],ax
	mov	ax,es:[bx+004h]
	mov	[bp-008h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-006h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-004h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-002h],ax
	push	[bp+00Ch]
	push	[bp+00Ah]
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	cs
	call	_bSpecialMod
	add	sp,+008h
	push	ss
	lea	ax,[bp-024h]
	push	ax
	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	[bp+010h]
	push	[bp+00Eh]
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	[bp+00Ch]
	push	[bp+00Ah]
	push	cs
	call	_bGCDWithCo
	add	sp,+014h
	les	bx,dword ptr [bp+012h]
	mov	ax,[bp-024h]
	mov	es:[bx],ax
	mov	ax,[bp-022h]
	mov	es:[bx+002h],ax
	mov	ax,[bp-020h]
	mov	es:[bx+004h],ax
	mov	ax,[bp-01Eh]
	mov	es:[bx+006h],ax
	mov	ax,[bp-01Ch]
	mov	es:[bx+008h],ax
	mov	ax,[bp-01Ah]
	mov	es:[bx+00Ah],ax
	les	bx,dword ptr [bp+006h]
	mov	ax,es:[bx]
	mov	[bp-00Ch],ax
	mov	ax,es:[bx+002h]
	mov	[bp-00Ah],ax
	mov	ax,es:[bx+004h]
	mov	[bp-008h],ax
	mov	ax,es:[bx+006h]
	mov	[bp-006h],ax
	mov	ax,es:[bx+008h]
	mov	[bp-004h],ax
	mov	ax,es:[bx+00Ah]
	mov	[bp-002h],ax
	push	[bp+00Ch]
	push	[bp+00Ah]
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	ss
	push	ax
	push	cs
	call	_bSignedDiv
	add	sp,+00Ch
	push	ss
	lea	ax,[bp-024h]
	push	ax
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	cs
	call	_bSignedMult
	add	sp,+008h
	les	bx,dword ptr [bp+016h]
	mov	ax,[bp-018h]
	mov	es:[bx],ax
	mov	ax,[bp-016h]
	mov	es:[bx+002h],ax
	mov	ax,[bp-014h]
	mov	es:[bx+004h],ax
	mov	ax,[bp-012h]
	mov	es:[bx+006h],ax
	mov	ax,[bp-010h]
	mov	es:[bx+008h],ax
	mov	ax,[bp-00Eh]
	mov	es:[bx+00Ah],ax
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	[bp+018h]
	push	bx
	push	cs
	call	_bSub
	add	sp,+008h
$L172:	mov	sp,bp
	pop	bp
	retf
_bLinearSolve:
	push	bp
	mov	bp,sp
	sub	sp,+024h
	push	ss
	lea	ax,[bp-024h]
	push	ax
	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	[bp+014h]
	push	[bp+012h]
	push	[bp+008h]
	push	[bp+006h]
	push	cs
	call	_bGCDWithCo
	add	sp,+014h
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+002h]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+002h],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+004h]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+004h],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+006h]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+006h],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+008h]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+008h],ax
	les	bx,dword ptr [bp+00Ah]
	mov	ax,es:[bx+00Ah]
	les	bx,dword ptr [bp+00Eh]
	mov	es:[bx+00Ah],ax
	push	ss
	lea	ax,[bp-00Ch]
	push	ax
	push	[bp+010h]
	push	bx
	push	[bp+010h]
	push	bx
	push	cs
	call	_bSignedDiv
	add	sp,+00Ch
	push	[bp+018h]
	push	[bp+016h]
	push	[bp+010h]
	push	[bp+00Eh]
	push	cs
	call	_bSignedMod
	add	sp,+008h
	push	ss
	lea	ax,[bp-018h]
	push	ax
	push	[bp+010h]
	push	[bp+00Eh]
	push	cs
	call	_bSignedMult
	add	sp,+008h
	push	[bp+018h]
	push	[bp+016h]
	push	[bp+010h]
	push	[bp+00Eh]
	push	cs
	call	_bSignedMod
	add	sp,+008h
	mov	sp,bp
	pop	bp
	retf

RKMATH_TEXT	ENDS
_DATA	SEGMENT

$S1	db	001h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
$S2	db	025h
	db	0D5h
	db	05Fh
	db	065h
	db	0A2h
	db	'b',000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
$S4	dw	00000h
$S5	db	024h
	db	0D5h
	db	05Fh
	db	065h
	db	0A2h
	db	'b',000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
$S7	db	083h
	db	097h
	db	008h
	db	0CFh
	db	0B5h
	db	010h
	db	000h
	db	000h
	db	000h
	db	000h
$S9	dw	00000h
$S11	db	030h
	db	'123456789ABCDEFHJKLMNPQRTUVWXYZ'
$S16	dw	00000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	001h
	db	000h
	db	002h
	db	000h
	db	003h
	db	000h
	db	004h
	db	000h
	db	005h
	db	000h
	db	006h
	db	000h
	db	007h
	db	000h
	db	008h
	db	000h
	db	009h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	00Ah,000h
	db	00Bh
	db	000h
	db	00Ch
	db	000h
	db	00Dh,000h
	db	00Eh
	db	000h
	db	00Fh
	db	000h
	db	006h
	db	000h
	db	010h
	db	000h
	db	001h
	db	000h
	db	011h
	db	000h
	db	012h
	db	000h
	db	013h
	db	000h
	db	014h
	db	000h
	db	015h
	db	000h
	db	000h
	db	000h
	db	016h
	db	000h
	db	017h
	db	000h
	db	018h
	db	000h
	db	005h
	db	000h
	db	019h
	db	000h
	db	01Ah
	db	000h
	db	01Bh,000h
	db	01Ch
	db	000h
	db	01Dh
	db	000h
	db	01Eh
	db	000h
	db	01Fh
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	00Ah,000h
	db	00Bh
	db	000h
	db	00Ch
	db	000h
	db	00Dh,000h
	db	00Eh
	db	000h
	db	00Fh
	db	000h
	db	006h
	db	000h
	db	010h
	db	000h
	db	001h
	db	000h
	db	011h
	db	000h
	db	012h
	db	000h
	db	013h
	db	000h
	db	014h
	db	000h
	db	015h
	db	000h
	db	000h
	db	000h
	db	016h
	db	000h
	db	017h
	db	000h
	db	018h
	db	000h
	db	005h
	db	000h
	db	019h
	db	000h
	db	01Ah
	db	000h
	db	01Bh,000h
	db	01Ch
	db	000h
	db	01Dh
	db	000h
	db	01Eh
	db	000h
	db	01Fh
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h
	db	000h

_DATA	ENDS
_BSS	SEGMENT

$S6	db	1154 dup(?)
$S3	db	1154 dup(?)

_BSS	ENDS
	END
