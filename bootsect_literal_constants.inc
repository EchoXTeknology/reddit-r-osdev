;===============================================================================
;
;
;
;===============================================================================

;****** BEGINNING OF MASTER BOOT RECORD MACROS & DEFINITIONS DATA BLOCK ********

%define		RESERVED_LOGICAL_SECTORS	1				; Refer to ['fs/BPB_FAT12.inc'] : (bpbReservedSectors)
%define		NUMBER_OF_LOGICAL_FATS		2				; Refer to ['fs/BPB_FAT12.inc'] : (bpbSectorsPerFAT)
%define		LOGICAL_SECTORS_PER_FAT		32				; Refer to ['fs/BPB_FAT12.inc'] : (bpbNumberOfFATs)
%define		LOGICAL_ROOT_DIRECTORIES	512				; Refer to ['fs/BPB_FAT12.inc'] : (bpbRootDirEntries)

%define		LOGICAL_FAT_ROOT_SECTOR		((NUMBER_OF_LOGICAL_FATS * LOGICAL_SECTORS_PER_FAT) + RESERVED_LOGICAL_SECTORS)
%define		LOGICAL_SECTOR_FAT_SIZE		((LOGICAL_ROOT_DIRECTORIES * LOGICAL_SECTORS_PER_FAT) / LOGICAL_ROOT_DIRECTORIES)

;********* END OF MASTER BOOT RECORD MACROS & DEFINITIONS DATA BLOCK ***********
;===============================================================================
;******** BEGINNING OF MASTER BOOT RECORD LITERAL CONSTANTS DATA BLOCK *********

LITERAL_CONSTANT_00	db	"BOOTSECTBIN"					; File Name For 'Second Stage Boot Sector'
LITERAL_CONSTANT_01	db	"[E]:[F_V_E]",		0x0D, 0x0A, 0x00	; [FATAL] [VOLUME] [ERROR] Message
LITERAL_CONSTANT_02	db	"[E]:[F_N_P]",		0x0D, 0x0A, 0x00	; [SECOND_STAGE_BOOTSECTOR] File Not Preset on [FILE_SYSTEM] [VOLUME]
;LITERAL_CONSTANT_03	db	"           "					; Reserve 11 [BYTE] to [IO_BUFFER_STREAM]

;*********** END OF MASTER BOOT RECORD LITERAL CONSTANTS DATA BLOCK ************
;===============================================================================
;********* BEGINNING OF MASTER BOOT RECORD LITERAL VARIABLE DATA BLOCK **********

LITERAL_VARIABLE_00	db	0x00						; Reserve [BYTE] to Current [VOLUME_DESCRIPTOR_VALUE]	: ([ebpbVolumeDescriptor])

LITERAL_VARIABLE_01	dw	0x0000						; Reserve [WORD] to Current [LOGICAL_SECTOR_VALUE]	: ([])
LITERAL_VARIABLE_02	dw	0x0000						; Reserve [WORD] to Current [LOGICAL_CLUSTER_VALUE]	: ([])

;************ END OF MASTER BOOT RECORD LITERAL VARIABLE DATA BLOCK *************
