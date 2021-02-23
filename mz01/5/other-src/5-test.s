        .data
        a:      .int 1, 2, 3, 4
        format: .asciz  "%d\n"
        .text
        .globl  main
main:
        push    %ebp
        mov     %esp, %ebp
        push    %ebx
    
        mov     $a, %eax
        mov     $4, %edx 

        push    $0

        dec     %edx
        mov     %eax, %ebx
        add     %edx, %edx
        add     %edx, %edx
        add     %edx, %ebx
.LOOP:   
        mov     (%eax), %ecx
        cmp     (%ebx), %ecx
        jle .SKIP

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
        jl .LOOP

        pop     %ecx
        
        push    %ecx
        push    $format
        call    printf
        pop     %eax
        pop     %eax
        
        
        pop     %ebx
        leave
        ret




