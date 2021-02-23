SECTION .text

global exchange
exchange:

        push ebp
        mov ebp, esp
        push ebx
    
        push 0

        dec edx
        mov ebx, eax
        add edx, edx
        add edx, edx
        add ebx, edx

.LOOP:   
        mov ecx, dword[eax]
        cmp ecx, dword[ebx]
        jle .SKIP

        mov edx, dword[ebx]
        mov dword[eax], edx
        mov dword[ebx], ecx
        inc dword[esp]
.SKIP:  
        inc eax
        inc eax
        inc eax
        inc eax
        
        dec ebx
        dec ebx
        dec ebx
        dec ebx
        
        cmp eax, ebx
        jl .LOOP

        pop ecx

        pop ebx
        mov esp, ebp
        pop ebp
        ret




