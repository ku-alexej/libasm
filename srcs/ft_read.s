section .text

global ft_read
extern __errno_location

; ft_read - reads data from a file descriptor
; Input: rdi - file descriptor (fd)
;        rsi - pointer to buffer (buf)
;        rdx - number of bytes to read (count)
; Output: rax - number of bytes read on success, or -1 on error (errno set)

ft_read:
    mov     rax, 0                      ; SYS_read = 0 on Linux x86-64
    syscall                             ; rdi=fd, rsi=buf, rdx=count
    cmp     rax, 0                      ; check syscall return value with 0
    jl      .error                      ; if less than 0, jump to error handling
    ret                                 ; return number of bytes read on success (rax)
.error:
    neg     rax                         ; rax = positive error code
    push    rax                         ; save error code, align stack for call
    call    __errno_location wrt ..plt  ; rax = pointer to errno
    pop     rdx                         ; rdx = saved error code
    mov     dword [rax], edx            ; *errno_ptr = error code
    mov     rax, -1                     ; return -1
    ret
