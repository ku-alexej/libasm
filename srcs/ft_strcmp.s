section .text

global ft_strcmp

; ft_strcmp - compares two null-terminated strings
; Input:  rdi - pointer to first string (s1)
;         rsi - pointer to second string (s2)
; Output: eax - result of comparison
;         0 if s1 == s2
;         negative if s1 < s2
;         positive if s1 > s2

ft_strcmp:
    xor     rcx, rcx                ; i = 0
.loop:
    movzx   eax, byte [rdi + rcx]   ; eax = (unsigned char)s1[i]
    movzx   edx, byte [rsi + rcx]   ; edx = (unsigned char)s2[i]
    cmp     al, dl                  ; compare s1[i] to s2[i]
    jne     .diff                   ; if not equal, go to difference handling
    cmp     al, 0                   ; check if both are null terminators
    je      .done                   ; if equal, we are done (strings are equal)
    inc     rcx                     ; i++
    jmp     .loop                   ; repeat steps
.diff:
    sub     eax, edx                ; eax = s1[i] - s2[i] (strings are different)
.done:
    ret                             ; return result in eax
                                    ;     0 - if equal
                                    ;     negative if s1 < s2
                                    ;     positive if s1 > s2
