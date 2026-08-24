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
    push    rbx                    ; entry: rsp%16==8 -> after push: 0
    push    r12                    ; 0 -> 8
    sub     rsp, 8                 ; 8 -> 0 (padding, keeps rsp 16-aligned for calls)
    mov     rbx, rdi                ; rbx = s (survives across calls)

    call    ft_strlen wrt ..plt     ; rax = strlen(s)     [rsp%16==0 here]
    inc     rax                     ; rax = len + 1 (room for '\0')
    mov     rdi, rax                ; malloc arg = size
    call    malloc wrt ..plt        ; rax = new buffer, or NULL  [rsp%16==0 here]

    test    rax, rax
    jz      .done                   ; malloc failed -> return NULL, errno set by malloc

    mov     r12, rax                ; r12 = new_buf (callee-saved, survives next call)
    mov     rdi, rax                ; ft_strcpy arg1 = dest
    mov     rsi, rbx                ; ft_strcpy arg2 = src = s
    call    ft_strcpy wrt ..plt     ; copy s into new buffer  [rsp%16==0 here]
    mov     rax, r12                ; rax = new_buf (return value)

.done:
    add     rsp, 8
    pop     r12
    pop     rbx
    ret

section .note.GNU-stack noalloc noexec