;===============================================================================
;
;
;
;===============================================================================

;********* BEGINNING OF MASTER BOOT RECORD BIOS PARAMETER CODE BLOCK ***********

;===============================================================================
;
;===============================================================================
bpbOEMLabel			db	"        "				; Pad [bpbOEMLabel] with 11 [0x20] : (' ')

bpbBytesPerSector:		dw	0x0200					; Assign [bpbBytesPerSector] to [0x0200] : (512B)
bpbSectorsPerCluster:		db	0x04					; Assign [bpbSectorsPerCluster] to [0x04] : (4)
bpbReservedSectors:		dw	0x0001					; Assign [bpbReservedSectors] to [0x0001] : (1)
bpbNumberOfFATs:		db	0x02					; Assign [bpbNumberOfFATs] to [0x02] : (2)
bpbRootDirEntries:		dw	0x0200					; Assign [bpbRootDirEntries] to [0x0200] : (512)
bpbTotalSectors:		dw	0x0800					; Assign [bpbTotalSectors] to [0x0800] : (2048)
bpbMediaDescriptor:		db	0xF8					; Assign [bpbMediaDescriptor] to [0xF8]
bpbSectorsPerFAT:		dw	0x0020					; Assign [bpbSectorsPerFAT] to [0x0020] : (32)

bpbSectorsPerTrack:		dw	0x0020					; Assign [bpbSectorsPerTrack] to [0x0020] : (32)
bpbNumberOfCylinders:		dw	0x0040					; Assign [bpbNumberOfCylinders] to [0x0040] : (64)
bpbHiddenSectors:		dd	0x00000800				; Assign [bpbHiddenSectors] to [0x00000800] : (2048)
bpbLargeSectors:		dd	0x00000000				; Assign [bpbLargeSectors] to [0x00000000] : (0)

ebpbVolumeDescriptor:		db	0x80					; Assign [ebpbVolumeDescriptor] to [0x80]
ebpbVolumeFlags:		db	0x01					; Assign [ebpbVolumeFlags] to [0x01]
ebpbExentedSignature:		db	0x29					; Assign [ebpbExtendedSignature] to [0x29]
ebpbVolumeSerialNumber:		dw	0x00000000				; Assign [ebpbVolumeSerialNumber] to [0x00000000]
ebpbVolumeLabel:		db	"SIF VG BOOT"				; Assign [ebpbVolumeLabel] to ["SIF VG BOOT"]
ebpbFileSystem:			db	"FAT12   "				; Assign [ebpbFileSystem] to ["FAT12   "]

;************ END OF MASTER BOOT RECORD BIOS PARAMETER CODE BLOCK **************

