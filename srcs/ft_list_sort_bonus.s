section .text

global ft_list_sort

; ft_list_sort - sirt list's elements according cmp results
; Input:  rdi - t_list *begin_list
;         rci - int (*cmp)()
; Output: nothing

ft_list_sort:
    push    rbx                  ; entry 8 -> 0
    push    r12                  ; 0 -> 8
    push    r13                  ; 8 -> 0
    push    r14                  ; 0 -> 8
    push    r15                  ; 8 -> 0  (5 pushes, odd -> aligned, no padding needed)
    mov     rbx, rdi              ; rbx = begin_list
    mov     r12, rsi              ; r12 = cmp

.outer:
    mov     r13, 0                ; swapped = false
    mov     r14, [rbx]            ; node = *begin_list

.inner:
    test    r14, r14
    jz      .outer_check          ; node == NULL -> end of this pass
    mov     r15, [r14 + 8]        ; r15 = node->next (kept across the call below)
    test    r15, r15
    jz      .outer_check          ; node->next == NULL -> end of this pass

    mov     rdi, [r14]            ; arg1 = node->data
    mov     rsi, [r15]            ; arg2 = node->next->data
    call    r12                   ; eax = cmp(node->data, node->next->data)  [rsp%16==0]

    cmp     eax, 0
    jle     .no_swap              ; already in order (or equal) -> skip

    mov     rdx, [r14]            ; rdx = node->data
    mov     rcx, [r15]            ; rcx = node->next->data
    mov     [r14], rcx            ; node->data = node->next->data
    mov     [r15], rdx            ; node->next->data = old node->data
    mov     r13, 1                ; swapped = true

.no_swap:
    mov     r14, r15              ; node = node->next (r15 still valid: callee-saved)
    jmp     .inner

.outer_check:
    cmp     r13, 0
    jne     .outer                ; another swap happened -> do another full pass

    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    ret
