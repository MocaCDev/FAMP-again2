use16

global print
use16
print:
	mov ah, 0x0e
.ploop:
	mov al, [si]
	cmp al, 0x0
	je .end

	int 0x10
	inc si
	jmp .ploop
.end:
	ret