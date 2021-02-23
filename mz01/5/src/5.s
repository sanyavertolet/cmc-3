        .text
        .globl  exchange
exchange:
        push    %ebp
        mov     %esp, %ebp
        push    %ebx

        push    $0

        dec     %edx
        mov     %eax, %ebx
        add     %edx, %edx
        add     %edx, %edx
        add     %edx, %ebx
.LOOP:
        mov     (%eax), %ecx
        cmp     (%ebx), %ecx
        jbe .SKIP

        mov     (%ebx), %edx
        mov     %edx, (%eax)
        mov     %ecx, (%ebx)
        incl    (%esp)
.SKIP:  
        inc     %eax
        inc     %eax
        inc     %eax
        inc     %eax

        dec     %ebx
        dec     %ebx
        dec     %ebx
        dec     %ebx

        cmp     %ebx, %eax
        jb .LOOP

        pop     %ecx

        pop     %ebx
        leave
        ret
