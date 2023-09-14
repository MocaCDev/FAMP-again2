use16

global read_disk
read_disk:

    mov ah, 0x02
    mov ch, 0x00
    mov dh, 0x00
    mov dl, 0x80
    int 0x13
    jc failed

    ret

.failed:
    mov ah, 0x0E
    mov al, 'E'
    int 0x10

    cli
    hlt
    jmp $