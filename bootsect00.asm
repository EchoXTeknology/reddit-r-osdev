;===============================================================================
;
;
;
;===============================================================================

;*********** BEGINNING OF MASTER BOOT RECORD CODE BLOCK ( <= 512B) *************

[bits	16]									; Declare CPU Instructions as REAL MODE (16-bit)
[org	0x0000]									; Declare Memory Stack at Location [0x0000]...
										; Declare Memory Stack Allocation Instructions later...


jmp	_glEntry								; Relocate to Entry Function [FUNC]



;===============================================================================
;
;
;
;===============================================================================
%include	"BPB_FAT12.inc"							; Refer to ['BPB_FAT12.inc'] For More Information...
										; External Function(s) [FUNC(s)] & Literal(s):
%include	"bootsect_literal_constants.inc"				; Refer to ['bootsect_literal_constants.inc'] For More Information...
%include	"output16_char_array.inc"					; Refer to ['output16_char_array.inc'] For More Information...
%include	"volume_management.inc"						; Refer to ['volume_management.inc'] For More Information...






;===============================================================================
;
;===============================================================================
reboot:										; Simpilified [WARM] [REBOOT] Function...
	;mov	si,	[..]							; Assign [] Memory Address Value to [SI] Register
	;call	output16_char_array						; Call [INTERNAL] [OUTPUT_BUFFER] Function [FUNC]
	xor	ax,	ax							; Zero [AX] Register
	int	0x16								; BIOS Call for Keystrokes...
	xor	ax,	ax							; Zero [AX] Register
	int	0x19								; BIOS Call for Hardware Reboot...
										; Continue Code Execution if [ERROR] Occurs:
	cli									; Disable CPU Interrupts...
	hlt									; Halt CPU Execution...



;===============================================================================
;
;===============================================================================
_glEntry:									; Globalized Entry Function...
										; Declare Stack Below [BOOTSECTOR] & Overlay [FAT_RESERVE] : (0x6900->0x7900) 4KiBs
	cli									; Disable CPU Interrupts
	xor	ax,	ax							; Zero [AX] Register
	mov	ax,	0x0690							; Assign [0x0690] to [AX] : (26880B)
	;add	ax,	0x0690							; Add [0x0690] to [AX] : (1680P | 26880B) | [AX] : (0x6900))
	mov	ss,	ax							; Assign [SS] to [AX] : (0x0690)
	;xor	ax,	ax							; Zero [AX] Register
	;mov	ax,	0x0100							; Assign [0x0700] to [AX] : (28672B)
	;add	ax,	0x0100							; Add [0x0100] to [AX] : (256P | 4096B | [AX] : (0x1000))
	;mov	sp,	ax							; Assign [SP] to [AX] : (0x0100)
	mov	sp,	0x1000							; Assign [SP] to [0x1000] : (4096B)
	sti									; Enable CPU Interrupts
										; Declare FAT Below [STACK_SPACE] : (0x0500->0x6900) 25KiBs
	xor	ax,	ax							; Zero [AX] Register
	xor	bx,	bx							; Zero [BX] Register
	mov	ax,	0x0050							; Assign [0x0050] to [AX] : (1280B)
	;add	ax,	0x0050							; Add [0x0050] to [AX] : (80P | 1280B | [AX] : (0x0500))
	mov	bx,	ax							; Assign [BX] to [AX] : (0x0050)
	mov	es,	bx							; Assign [ES] to [BX] : (0x0050)
	xor	bx,	bx							; Zero [BX] Register
	xor	ax,	ax							; Zero [AX] Register
	push	es								; Store [ES] Register to ( [STACK_SPACE]:[OFFSET] ) : [ES] [STORED]
	push	bx								; Store [BX] Register to ( [STACK_SPACE]:[OFFSET] ) : [BX] [STORED]
										; Declare [BOOTSECTOR] Above [STACK_SPACE] & [FAT_RESERVE] : (0x7C00->0x7DFF) 512B
	xor	ax,	ax							; Zero [AX] Register
	mov	ax,	0x07C0							; Assign [AX] to [0x07C0] : (0x7C00)
	;add	ax,	0x07C0							; Add [0x07C0] to [AX] : (1984P | 31744B | [AX] : (0x7C00))
	mov	ds,	ax							; Assign [DS] to [AX] : (0x07C0)
	xor	ax,	ax	        					; Zero [AX] Register
	;pop	bx								; Restore [BX] Register from ( [STACK_SPACE]:[OFFSET] ) : [BX] [RESTORED]
	;pop	es								; Restore [ES] Register from ( [STACK_SPACE]:[OFFSET] ) : [ES] [RESTORED]
										; Continue Code Exection to Proceeding Function [FUNC]:
	jmp	global_volume_management					; [RESERVED] : [VOLUME_MANAGEMENT] [ENTRY_HANDLER] [FUNC]
										; Continue Code Execution if [ERROR] Occurs:
	jmp	reboot								; Relocate to [ERROR_HANDLING] Function [FUNC]
										; Continue Code Execution if [ERROR] Occurs:
	xor	ax,	ax							; Zero [AX] Register
	int	0x16								; BIOS Call for Keystrokes...
	xor	ax,	ax							; Zero [AX] Register
	int	0x19								; BIOS Call for Hardware Reboot...
										; Continue Code Execution if [ERROR] Occurs:
	cli									; Disable CPU Interrupts...
	hlt									; Halt CPU Execution...
	


;===============================================================================
;
;===============================================================================
times	510 - ( $ - $$ )	db	0x00					; Pad [BOOTSECTOR] with Zero(s)...
dw	0xAA55									; Assign 511B & 512B with Magic Value(s)...






;************** END OF MASTER BOOT RECORD CODE BLOCK ( <= 512B) ****************
;===============================================================================
;************ BEGINNING OF VOLUME BOOT RECORD CODE BLOCK ( > 512B) *************






;===============================================================================
;
;===============================================================================
_glExtendedEntry:								; Globalized 'Extended' Entry Function...

