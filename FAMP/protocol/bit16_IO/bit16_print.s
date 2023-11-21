use16

global print
use16
print:
	mov ah, 0x0e
.ploop:
	mov al, [si]
	cmp al, 0x0
	je .end

    ; We want to print off ascii characters ranging from `!` - `z` on the
    ; ascii table.
    cmp al, 0x20
    jge .okay

.okay:
    ; Make sure the letter is less than `z`. If not, go to check for capital letters
    cmp al, 0x7E
    jg .end

.print:
	int 0x10
	inc si
	jmp .ploop

.end:
	ret