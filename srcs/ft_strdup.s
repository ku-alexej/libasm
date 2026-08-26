section .text

global ft_strdup
extern ft_strlen
extern ft_strcpy
extern malloc

; ft_strdup - duplicate a string
; Input:  rdi - pointer to source string (s)
; Output: rax - pointer to newly allocated string (duplicate of s)
;         or NULL on failure (errno set by malloc)

ft_strdup:
    push    rbx                     ; Save callee-saved RBX and align stack: rsp % 16 = 8 -> 0
    push    r12                     ; Save callee-saved R12: rsp % 16 = 0 -> 8
    sub     rsp, 8                  ; Add 8 bytes of padding: rsp % 16 = 8 -> 0 for function calls
    mov     rbx, rdi                ; rbx = s (callee-saved)

    call    ft_strlen wrt ..plt     ; rax = strlen(rbi)
    inc     rax                     ; rax = rax + 1 (for '\0'); size for malloc
    mov     rdi, rax                ; arg1 = size
    call    malloc wrt ..plt        ; rax = malloc(arg1), or NULL

    test    rax, rax                ; rax == 0 ?
    jz      .done                   ; if rax == 0 -> malloc returned NULL, errno set by malloc -> go to the end

    mov     r12, rax                ; r12 = rax = new_buf = malloc(arg1) (callee-saved)
    mov     rdi, rax                ; arg1 = dest
    mov     rsi, rbx                ; arg2 = src = s
    call    ft_strcpy wrt ..plt     ; rax = ft_strcpy(arg1, arg2) = ft_strcpy(dest, s)
    mov     rax, r12                ; rax = new_buf (return value)

.done:
    add     rsp, 8                  ; Remove the 8 bytes of stack alignment padding
    pop     r12                     ; Restore the caller's original value of r12
    pop     rbx                     ; Restore the caller's original value of rbx
    ret
