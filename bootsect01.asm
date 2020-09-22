[bits	16]
[org	0x0000]



jmp	_glExtendedEntry

%include	"output16_char_array.inc"				; Refer to ['output16_char_array.inc'] For More Information...

hello	db	"Hello, World!",	0x0D, 0x0A, 0x00

_glExtendedEntry:
	;xor	ax,	ax
	;xor	bx,	bx
	;xor	cx,	cx
	;mov	ax,	0x07E0
	;mov	ds,	ax
	;mov	es,	ax
	;xor	ax,	ax
	mov	si,	hello
	call	output16_char_array
	cli
	hlt
