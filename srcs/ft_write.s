section .text

global ft_write
extern __errno_location

; ft_write - writes data to a file descriptor
; Input:  rdi - file descriptor (fd)
;         rsi - pointer to buffer (buf)
;         rdx - number of bytes to write (count)
; Output: rax - number of bytes written on success
;         -1 on error, with errno set appropriately

ft_write:
    mov     rax, 1                      ; SYS_write = 1 on Linux x86-64
    syscall                             ; rdi=fd, rsi=buf, rdx=count
    cmp     rax, 0						; check syscall return value with 0
    jl      .error						; if less than 0, jump to error handling
    ret                                 ; return number of bytes written on success (rax)
.error:
    neg     rax                         ; rax = positive error code
    push    rax                         ; save error code, align stack for call
    call    __errno_location wrt ..plt  ; rax = pointer to errno
    pop     rdx                         ; rdx = saved error code
    mov     dword [rax], edx            ; *errno_ptr = error code
    mov     rax, -1                     ; return -1
    ret
