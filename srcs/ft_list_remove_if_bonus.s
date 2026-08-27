section .text

global ft_list_remove_if
extern free

; ft_list_remove_if - removes nodes according cmp logic
; Input:  rdi - t_list **begin_list
;         rsi - void *data_ref
;         rdx - int (*cmp)() - function to compare data
;         rcx - void (*free_fct)(void *) - function to free data
; Output: none (void function)

ft_list_remove_if:
    push    rbx                 ; entry 8 -> 0
    push    r12                 ; 0 -> 8
    push    r13                 ; 8 -> 0
    push    r14                 ; 0 -> 8
    push    r15                 ; 8 -> 0  (5 pushes, odd -> aligned, no padding needed)

    mov     r14, rdi            ; cur_ptr_addr = begin_list
    mov     rbx, rsi            ; rbx = data_ref
    mov     r12, rdx            ; r12 = cmp
    mov     r13, rcx            ; r13 = free_fct

.loop:
    mov     r15, [r14]          ; r15 = *cur_ptr_addr = current node (or NULL)
    test    r15, r15
    jz      .end                ; reached end of list

    mov     rdi, [r15]          ; arg1 = node->data
    mov     rsi, rbx            ; arg2 = data_ref
    call    r12                 ; eax = cmp(node->data, data_ref)   [rsp%16==0]

    cmp     eax, 0
    jne     .advance            ; not a match -> keep node, move forward

    ; --- match: remove this node ---
    mov     rdx, [r15 + 8]      ; rdx = node->next
    mov     [r14], rdx          ; *cur_ptr_addr = node->next (unlink)

    mov     rdi, [r15]          ; free_fct(node->data)
    call    r13                 ; [rsp%16==0]

    mov     rdi, r15            ; free(node)
    call    free wrt ..plt      ; [rsp%16==0]

    jmp     .loop               ; cur_ptr_addr unchanged -- re-check what's now here

.advance:
    lea     r14, [r15 + 8]      ; cur_ptr_addr = &(node->next)
    jmp     .loop

.end:
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    ret
