use16

;
; TODO: Do we want to make this file the "general area" for all of the other
;       assembly programs? That way we only have to include this file in
;       other assembly programs and have access to everything.
;
%ifdef need_rmode_access
%include "protocol/enter_rmode.s"
%include "protocol/bootloader/read_disk.s"
%endif

%ifdef need_gdt_access
%include "boot/gdt.s"
%endif

%ifdef need_mbr_part_table_access
%include "protocol/bootloader/mbr_part_table.s"
%endif