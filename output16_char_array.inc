;=================================================================================
;
;=================================================================================
output16_char_array:								; Use BIOS to Display Characters Function
	xor	ah,	ah							; Zero [AH] Register
	xor	al,	al							; Zero [AL] Register
	mov	ah,	0x0E							; Assign [AH] to [0x0E]

	.output16_char_loop:							; Looping Function for BIOS Display
		lodsb								; Grab single [BYTE] Character from [SI]
		or	al,	al						; Check [AL] for [NULL] Termination Value [0x0]
		jz	.output16_char_done					; [AL] Register is [0x0] Call Function [FUNC]
		int	0x10							; BIOS Call for Display...
		jmp	.output16_char_loop					; [AL] Register != [0x0] Continue Loop Function [FUNC]

	.output16_char_done:							; Exit Function for BIOS Display
		ret								; Return to Entry Function [FUNC]
