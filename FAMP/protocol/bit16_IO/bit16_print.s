use16

global print
use16
print:
	mov ah, 0x0e
.ploop:
	mov al, [si]
	cmp al, 0x0
	je .end

    ; Make sure the letter is `a`, or greater
    cmp al, 0x41
    jge .okay
    jl .end ; if it's less than `a`, end. It's non-ascii

.okay:
    ; Make sure the letter is less than `z`. If not, go to check for capital letters
    cmp al, 0x5A
    jg .secondary_check
    jle .print

.secondary_check:
    ; Make sure the letter is `A`, or greater
    cmp al, 0x61
    jge .okay2
    jl .end ; if it's less than `A`, but obviously greater than `z` then go to end. It's non-ascii

.okay2:
    ; Check to see if the letter is greater than `Z`. If it is, go to end. It's non-ascii
    cmp al, 0x7A
    jg .end
    jle .print ; If it's between `A` and `Z` then print

.print:
	int 0x10
	inc si
	jmp .ploop

.end:
	ret