section .text

global ft_strcpy

; ft_strcpy.s - copies a null-terminated string from src to dest
; Input: rdi - pointer to destination buffer (dest)
;        rsi - pointer to source string (src)
; Output: rax - pointer to destination buffer (dest)

ft_strcpy:
    mov     rax, rdi            ; rax = dest (kept for return value + indexing)
    xor     rcx, rcx            ; i = 0
.loop:
    mov     dl, [rsi + rcx]     ; dl = src[i]
    mov     [rax + rcx], dl     ; dest[i] = dl
    cmp     dl, 0               ; compare dl to '\0'
    je      .done               ; if equal, we are done
    inc     rcx                 ; i++
    jmp     .loop               ; repeat steps
.done:
    ret                         ; return dest (rax)
	