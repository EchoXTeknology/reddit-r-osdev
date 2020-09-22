;===============================================================================
;
;
;
;===============================================================================

;******** BEGINNING OF MASTER BOOT RECORD VOLUME MANAGEMENT CODE BLOCK *********

;===============================================================================
;
;===============================================================================
global_volume_management:							; Globalized [VOLUME] Entry Function...
	xor	eax,	eax							; BIOS Bug Fix: Some BIOSes Cause Register Addressing Fault...
										; Continue Code Execution:


;===============================================================================
;
;===============================================================================
volume16_compare_descriptors:							; Compare BIOS [VOLUME] Descriptors to [BPB] [VOLUME] Descriptors Function...
	;xor	cx,	cx							; Zero [CX] Register
	;xor	dx,	dx							; Zero [DX] Register
	cmp	dl,	0x00							; Compare [DL] Register to [0x00] : ([FLOPPY_DESCRIPTOR])
	je	.volume16_descriptors_done					; [DL] Register is [0x00] Call Function [FUNC]
	mov	[LITERAL_VARIABLE_00],	dl					; Assign [LITERAL_VARIABLE_00] to [DL] Register Value
	mov	ah,	0x08							; Assign [AH] Register to [0x08] : (8)
	int	0x13								; BIOS Call for [VOLUME] Parameters...
	jc	volume16_fatal_error						; [CARRY_FLAG] is != [0x(..)] Proceed to Call Function [FUNC]
	and	cx,	0x3F							; Maximumize [CX] Register to [0x3F] : (63)
	;and	cx,	0x40							; Maximumize [CX] Register to [0x40] : (64)
	mov	[bpbSectorsPerTrack],	cx					; Assign [bpbSectorsPerTrack] to [CX] Register Value
	movzx	dx,	dh							; Assign [DX] to [DH] & Zero [DX] Register
	add	dx,	0x0001							; Assign [DX] to [0x0001] : (1)
	mov	[bpbNumberOfCylinders],	dx					; Assign [bpbNumberOfCylinders] to [DX] Register Value
										; Continue Code Execution:
	.volume16_descriptors_done:						; BIOS [VOLUME] Descriptors Matched [(..)] Descriptors Function...
		xor	eax,	eax						; BIOS Bug Fix: Some BIOSes Cause Register Addressing Fault...
										; Continue Code Execution:


;===============================================================================
;
;===============================================================================
volume16_arrange_system_registers:						; Prepare [SYSTEM] for BIOS [VOLUME] Read [STREAM] Function...
	;xor	ax,	ax							; Zero [AX] Register
	mov	ax,	LOGICAL_FAT_ROOT_SECTOR					; Assign [AX] to [(LOGICAL_FAT_ROOT_SECTOR)] [MACRO] Value
	call	volume16_convert_lba						; Call [INTERNAL] [LBA_CTS_CONVERSION] Function [FUNC]
	mov	ah,	0x02							; Assign [AH] to [0x02] : (2)
	mov	al,	LOGICAL_SECTOR_FAT_SIZE					; Assign [AL] to [(LOGICAL_SECTOR_FAT_SIZE)] [MACRO] Value
	pusha									; Store [(..)] Register Values to ( [STACK_SPACE]:[OFFSET] ) : [(..)] [STORED]
										; Continue Code Execution:
	.volume16_read_fs_table:						; BIOS Read [VOLUME] [FILE_SYSTEM] Table into [MEMORY] Function...
		popa								; Restore [(..)] Register Values from ( [STACK_SPACE]:[OFFSET] ) : [(..)] [RESTORED]
		;pusha								; Store [(..)] Register Values to ( [STACK_SPACE]:[OFFSET] ) : [(..)] [STORED]
		stc								; BIOS Bug Fix: Some BIOSes Improperly set [CARRY_FLAG] on [ERROR]
		int	0x13							; BIOS Call for [VOLUME] Parameters...
		jnc	.volume16_scan_fs_table					; [CARRY_FLAG] is != [0x(..)] Proceed to Call Function [FUNC]
		call	volume16_reset_controller				; Call [INTERNAL] [VOLUME_CONTROLLER] Reset Function [FUNC]
		jnc	.volume16_read_fs_table					; [CARRY_FLAG] is != [0x(..)] Proceed to Call Function [FUNC]
		jmp	reboot							; Relocate to [ERROR_HANDLING] Function [FUNC]

	.volume16_scan_fs_table:						; Scan [FILE_SYSTEM] [MEMORY_STREAM] Table Function...
		;popa								; Restore [(..)] Register Values from ( [STACK_SPACE]:[OFFSET] ) : [(..)] [RESTORED]
		pop	bx							; Restore [BX] Register from ( [STACK_SPACE]:[OFFSET] ) : [BX] [RESTORED]
		pop	es							; Restore [ES] Register from ( [STACK_SPACE]:[OFFSET] ) : [ES] [RESTORED]
		xor	cx,	cx						; Zero [CX] Register
		mov	cx,	WORD	[bpbRootDirEntries]			; Assign [AX] to [WORD] [bpbRootDirEntries] Memory Address Value
		xor	ax,	ax						; Zero [AX] Register
		push	es							; Store [ES] Register to ( [STACK_SPACE]:[OFFSET] ) : [ES] [STORED]
		push	bx							; Store [BX] Register to ( [STACK_SPACE]:[OFFSET] ) : [BX] [STORED]
										; Continue Code Execution:
	.volume16_scan_fs_table_entries:					; Scan [FILE_SYSTEM] [MEMORY_STREAM] Table Entries Function...
		xchg	cx,	dx						; Swap [CX] to [DX] & Zero [CX] Register
		mov	si,	LITERAL_CONSTANT_00				; Assign [SI] to [LITERAL_CONSTANT_00] Memory Address [STREAM]
		mov	cx,	0x000B						; Assign [CX] to [0x000B] : (11)
		rep	cmpsb							; Compare [SI] to [MEMORY_STREAM]
		je	.volume16_fs_table_entry_located			; [SI] Matched [MEMORY_STREAM] Proceed to Call Function [FUNC]
		add	ax,	0x0020						; Add [0x0020] to [AX] : (2P | 32B)
		xor	di,	di						; Zero [DI] Register
		add	di,	ax						; Add [AX] to [DI]
		xchg	dx,	cx						; Swap [DX] to [CX] & Zero [DX] Register
		loop	.volume16_scan_fs_table_entries				; Continue to Loop Function [FUNC] Until [CX] is [0x0000]
		mov	si,	LITERAL_CONSTANT_02				; Assign [SI] to [LITERAL_CONSTANT_02] Memory Address [STREAM]
		call	output16_char_array					; Call [INTERAL] [OUTPUT_BUFFER] Function [FUNC]...
		jmp	reboot							; Relocate to [ERROR_HANDLING] Function [FUNC]
	
	.volume16_fs_table_entry_located:					; Located & Prepare [FILE_SYSTEM] Table Entry Read Function...
		mov	ax,	WORD	[es:di+0x0F]				; Assign [AX] to [WORD] [ES:DI+0x0F] : [MEMORY_ADDRESS_POINTER]
		mov	WORD	[LITERAL_VARIABLE_02],	ax			; Assign [AX] to [WORD] [LITERAL_VARIABLE_02] Memory Address [WORD]
		mov	ax,	0x0001						; Assign [AX] to [0x0001] : (1)
		call	volume16_convert_lba					; Call [INTERNAL] [LBA_CTS_CONVERSION] Function [FUNC]
		xor	di,	di						; Zero [DI] Register
		mov	bx,	di						; Assign [BX] to [DI]
		mov	ah,	0x02						; Assign [AH] to [0x0002] : (2)
		mov	al,	0x20						; Assign [AL] to [0x0020] : (32)
		pusha								; Store [(..)] Register Values to ( [STACK_SPACE]:[OFFSET] ) : [(..)] [STORED]
										; Continue Code Execution:
	.volume16_read_fs_table_entry:						; Read [FILE_SYSTEM] Table Entry TO [MEMORY_BUFFER] Function...
		popa								; Restore [(..)] Register Values from ( [STACK_SPACE]:[OFFSET] ) : [(..)] [RESTORED]
		pusha								; Store [(..)] Register Values to ( [STACK_SPACE]:[OFFSET] ) : [(..)] [STORED]
		stc								; BIOS Bug Fix: Some BIOSes Improperly set [CARRY_FLAG] on [ERROR]
		int	0x13							; BIOS Call for [VOLUME] Parameters...
		jnc	.volume16_fs_table_entry_read				; [CARRY_FLAG] is != [0x(..)] Proceed to Call Function [FUNC]
		call	volume16_reset_controller				; Call [INTERNAL] [VOLUME_CONTROLLER] Reset Function [FUNC]
		jnc	.volume16_read_fs_table_entry				; [CARRY_FLAG] is != [0x(..)] Proceed to Call Function [FUNC]
		call	volume16_fatal_error					; Call [INTERNAL] [VOLUME_ERROR] Function [FUNC]...
		jmp	reboot							; Relocate to [ERROR_HANDLING] Function [FUNC]
	
	.volume16_fs_table_entry_read:						; Relocate Registers for [FILE_SYSTEM] Table Entry Read Function...
		popa								; Restore [(..)] Register Values from ( [STACK_SPACE]:[OFFSET] ) : [(..)] [RESTORED]
		;xor	ax,	ax						; Zero [AX] Register
		mov	ax,	0x07E0						; Assign [AX] to [0x07E0] : (0x7E00)
		;add	ax,	0x07E0						; Add [0x07E0] to [AX] : (2016P | 32256B | [AX] : (0x7E00))
		mov	es,	ax						; Assign [ES] to [AX] : (0x07E0)
		xor	bx,	bx						; Zero [BX] Register
		mov	ah,	0x02						; Assign [AH] to [0x02] : (2)
		mov	al,	0x01						; Assign [AL] to [0x01] : (1)
		push	ax							; Store [AX] Register to ( [STACK_SPACE]:[OFFSET] ) : [AX] [STORED]
		push	es							; Store [ES] Register to ( [STACK_SPACE]:[OFFSET] ) : [ES] [STORED]
		push	bx							; Store [BX] Register to ( [STACK_SPACE]:[OFFSET] ) : [BX] [STORED]
										; Continue Code Execution:
	.volume16_load_file_sector:						; Read [FILE_SYSTEM] File Sector to [MEMORY_BUFFER] Function...
		mov	ax,	WORD	[LITERAL_VARIABLE_02]			; Assign [WORD] [LITERAL_VARIABLE_02] to [AX]
		add	ax,	0x001F						; Add [0x001F] to [AX] : (31)
		call	volume16_convert_lba					; Call [INTERNAL] [LBA_CTS_CONVERSION] Function [FUNC]
		;xor	ax,	ax						; Zero [AX] Register
		mov	ax,	0x07E0						; Assign [AX] to [0x07E0] : (0x7E00)
		;add	ax,	0x07E0						; Add [0x07E0] to [AX] : (2016P | 32256B | [AX] : (0x7E00))
		mov	es,	ax						; Assign [ES] to [AX] : (0x7E00)
		mov	bx,	WORD	[LITERAL_VARIABLE_01]			; Assign [WORD] [LITERAL_VARIABLE_01] to [BX]
		stc								; BIOS Bug Fix: Some BIOSes Improperly set [CARRY_FLAG] on [ERROR]
		int	0x13							; BIOS Call for [VOLUME] Parameters...
		jnc	.volume16_calculate_cluster				; Calculate [CLUSTER] [BIT_SET] : [ODD] | [EVEN]
		call	volume16_reset_controller				; Call [INTERNAL] [VOLUME_CONTROLLER] Reset Function [FUNC]
		jmp	.volume16_load_file_sector				; Relocate to [INTERNAL] [LOAD_VOLUME_SECTOR] Function [FUNC]

	.volume16_calculate_cluster:						; 
		mov	ax,	[LITERAL_VARIABLE_02]				; 
		xor	dx,	dx						; 
		mov	bx,	0x0003						; 
		mul	bx							; 
		mov	bx,	0x0002						; 
		div	bx							; 
		xor	si,	si						; 
		add	si,	ax						; 
		mov	ax,	0x07E0						; 
		or	dx,	dx						; 
		jz	.volume16_cluster_even					; 
										; Continue Code Execution:
		.volume16_cluster_odd:						; 
			shr	ax,	0x0004					; 
			jmp	short	.volume16_consecutive_cluster		; 

		.volume16_cluster_even:						; 
			and	ax,	0x0FFF					; 
										; 
	.volume16_consecutive_cluster:						; 
		mov	WORD	[LITERAL_VARIABLE_02],	ax			; 
		cmp	ax,	0x0FF8						; 
		jae	.volume16_cluster_end					; 
		add	WORD	[LITERAL_VARIABLE_01],	0x0200			; 
		jmp	.volume16_load_file_sector				; 

	.volume16_cluster_end:							; 
		mov	dl,	BYTE	[LITERAL_VARIABLE_00]			; 
		jmp	0x07E0:0x0000						; 



;===============================================================================
;
;===============================================================================
volume16_fatal_error:								; 
	mov	si,	LITERAL_CONSTANT_01					; 
	call	output16_char_array						; Call [INTERNAL] [OUTPUT_BUFFER] Function [FUNC]
	jmp	reboot								; Relocate to [ERROR_HANDLING] Function [FUNC]



;===============================================================================
;
;===============================================================================
volume16_convert_lba:								; 
	push	ax
	push	bx
	mov	bx,	ax
	xor	dx,	dx
	div	WORD	[bpbSectorsPerTrack]
	add	dl,	0x01
	mov	cl,	dl
	mov	ax,	bx
	xor	dx,	dx
	div	WORD	[bpbSectorsPerTrack]
	xor	dx,	dx
	div	WORD	[bpbNumberOfCylinders]
	mov	dh,	dl
	mov	ch,	cl
	pop	bx
	pop	ax
	mov	dl,	BYTE	[LITERAL_VARIABLE_00]
	ret



;===============================================================================
;
;===============================================================================
volume16_reset_controller:							; 
	push	ax
	push	dx
	xor	ax,	ax
	mov	dl,	BYTE	[LITERAL_VARIABLE_00]
	stc
	int	0x13
	pop	dx
	pop	ax
	ret

;*********** END OF MASTER BOOT RECORD VOLUME MANAGEMENT CODE BLOCK ************
