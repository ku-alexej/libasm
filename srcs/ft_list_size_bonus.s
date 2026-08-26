section .text

global ft_list_size

; ft_list_size - returns size otf the list
; Input:  rdi - t_list *begin_list
; Output: rax - size of the list

; unsigned int ft_list_size(t_list *begin_list);
; rdi = begin_list

ft_list_size:
    xor     eax, eax            ; size = 0

.loop:
    test    rdi, rdi            ; current node == NULL?
    jz      .done               ; next node is NULL - stop here
    inc     eax                 ; size++
    mov     rdi, [rdi + 8]      ; node = node->next (offset 8)
    jmp     .loop               ; repeat steps   

.done:
    ret                         ; rex contains size of the list
