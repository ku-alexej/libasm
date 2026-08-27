section .text

global ft_list_push_front
extern malloc

; ft_list_push_front - adds a new node at the beginning of the list.
; Input:  rdi - t_list **begin_list
;         rsi - void *data
; Output: none (void function)
;
; t_list layout: offset 0 = data (8 bytes), offset 8 = next (8 bytes)

ft_list_push_front:
    push    rbx                 ; entry 8 -> 0
    push    r12                 ; 0 -> 8
    sub     rsp, 8              ; 8 -> 0 (padding, 2 pushes = even, need alignment fix)
    mov     rbx, rdi            ; rbx = begin_list
    mov     r12, rsi            ; r12 = data

    mov     rdi, 16             ; sizeof(t_list) = 8 + 8
    call    malloc wrt ..plt    ; rax = new_node, or NULL   [rsp%16==0]

    test    rax, rax
    jz      .done               ; malloc failed -> do nothing (no way to report failure, void fn)

    mov     [rax], r12          ; new_node->data = data
    mov     rdx, [rbx]          ; rdx = *begin_list (old head)
    mov     [rax + 8], rdx      ; new_node->next = old head
    mov     [rbx], rax          ; *begin_list = new_node

.done:
    add     rsp, 8
    pop     r12
    pop     rbx
    ret
