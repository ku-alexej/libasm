section .text

global ft_strlen

; ft_strlen - calculates the length of a null-terminated string
; Input: rdi - pointer to the null-terminated string
; Output: rax - length of the string (number of characters before the null terminator)

ft_strlen:
    xor     rax, rax                ; length = 0
.loop:
    cmp     byte [rdi + rax], 0     ; compare s[length] to '\0'
    je      .done                   ; if equal, we are done
    inc     rax                     ; length++
    jmp     .loop                   ; repeat steps
.done:
    ret                             ; return length (rax)
