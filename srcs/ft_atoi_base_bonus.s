section .text

global ft_atoi_base
extern ft_strlen

; ft_atoi_base - converts a string to an integer using the specified base.
; Input:  rdi - const char *str to convert
;         rsi - const char *base (string of valid digits)
; Output: rax - the converted integer value, or 0 if the base is invalid or no valid conversion could be performed.

ft_atoi_base:
    push    rbx                     ; entry 8 -> 0
    push    r12                     ; 0 -> 8
    push    r13                     ; 8 -> 0  (3 pushes, odd -> aligned)
    mov     rbx, rdi                ; rbx = str
    mov     r12, rsi                ; r12 = base

    mov     rdi, r12
    call    ft_strlen wrt ..plt     ; rax = strlen(base)     [rsp%16==0]
    mov     r13, rax                ; r13 = base_len

    cmp     r13, 2
    jl      .invalid                ; base too short (empty or 1 char)

; --- validate base: no '+', '-', whitespace, and no duplicate chars ---
    xor     rcx, rcx                ; i = 0

.validate_outer:
    cmp     rcx, r13
    jge     .validate_done
    mov     al, [r12 + rcx]         ; al = base[i]

    cmp     al, '+'
    je      .invalid
    cmp     al, '-'
    je      .invalid
    cmp     al, ' '
    je      .invalid
    cmp     al, 0x09                ; tab
    je      .invalid
    cmp     al, 0x0A                ; newline
    je      .invalid
    cmp     al, 0x0B                ; vertical tab
    je      .invalid
    cmp     al, 0x0C                ; form feed
    je      .invalid
    cmp     al, 0x0D                ; carriage return
    je      .invalid

    lea     r8, [rcx + 1]           ; j = i + 1

.validate_inner:
    cmp     r8, r13
    jge     .validate_inner_done
    mov     r9b, [r12 + r8]         ; r9b = base[j]
    cmp     al, r9b
    je      .invalid                ; duplicate character found
    inc     r8
    jmp     .validate_inner

.validate_inner_done:
    inc     rcx
    jmp     .validate_outer

.validate_done:

; --- skip leading whitespace in str ---
    xor     rcx, rcx                ; str index = 0

.skip_ws:
    mov     al, [rbx + rcx]
    cmp     al, ' '
    je      .ws_next
    cmp     al, 0x09
    je      .ws_next
    cmp     al, 0x0A
    je      .ws_next
    cmp     al, 0x0B
    je      .ws_next
    cmp     al, 0x0C
    je      .ws_next
    cmp     al, 0x0D
    je      .ws_next
    jmp     .check_sign

.ws_next:
    inc     rcx
    jmp     .skip_ws

; --- optional sign ---

.check_sign:
    mov     r8, 1                   ; sign = 1
    mov     al, [rbx + rcx]
    cmp     al, '+'
    je      .sign_plus
    cmp     al, '-'
    je      .sign_minus
    jmp     .parse_init

.sign_minus:
    mov     r8, -1

.sign_plus:
    inc     rcx

; --- main digit loop ---

.parse_init:
    xor     r9, r9                  ; result = 0

.parse_loop:
    mov     al, [rbx + rcx]
    cmp     al, 0
    je      .parse_done             ; end of string

    xor     r10, r10                ; j = 0 (search index into base)

.find_digit:
    cmp     r10, r13
    jge     .parse_done             ; char not found in base -> stop parsing
    mov     dl, [r12 + r10]
    cmp     al, dl
    je      .digit_found
    inc     r10
    jmp     .find_digit

.digit_found:
    mov     rax, r9
    imul    rax, r13                ; result * base_len
    add     rax, r10                ; + digit value
    mov     r9, rax
    inc     rcx
    jmp     .parse_loop

.parse_done:
    mov     rax, r9
    imul    rax, r8                 ; apply sign
    jmp     .return

.invalid:
    xor     rax, rax                ; return 0

.return:
    pop     r13
    pop     r12
    pop     rbx
    ret
