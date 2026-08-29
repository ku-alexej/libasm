# libasm

Reimplementation of libc functions + a linked-list toolkit in x86-64 NASM,
Linux, System V AMD64 ABI. Args: `rdi, rsi, rdx, rcx, r8, r9`. Return: `rax`.
Callee-saved: `rbx, rbp, r12–r15`. Stack must be 16-byte aligned before `call`.

For each function: **pseudocode**, then a compact table of **registers**,
**key instructions/comparisons**, and **return value**.

---

# Mandatory part

- `ft_strlen`
- `ft_strcpy`
- `ft_strcmp`
- `ft_write`
- `ft_read`
- `ft_strdup`


> `malloc` allowed only in `ft_strdup`  
> syscall errors set `errno` via `__errno_location`

## ft_strlen
`size_t ft_strlen(const char *s)`

```python
strlen(s):
    length = 0
    while s[length] != '\0': length += 1
    return length
```
- **registers**: `rdi`=s, `rax`=index→length
- **key ops**: `cmp byte [rdi+rax], 0` / `je .done`; `inc rax`
- **return**: `rax` = string length. No error path.

## ft_strcpy
`char *ft_strcpy(char *dst, const char *src)`

```python
strcpy(dest, src):
    i = 0
    while true:
        c = src[i]; dest[i] = c
        if c == '\0': break
        i += 1
    return dest
```
- **registers**: `rdi`=dest→saved in `rax`, `rsi`=src, `rcx`=i, `dl`=char
- **key ops**: `mov rax, rdi`; copy byte then `cmp dl, 0` / `je .done`
- **return**: `rax` = `dest` (unchanged pointer).

## ft_strcmp
`int ft_strcmp(const char *s1, const char *s2)`

```python
strcmp(s1, s2):
    i = 0
    while true:
        c1, c2 = s1[i], s2[i]
        if c1 != c2: return c1 - c2
        if c1 == '\0': return 0
        i += 1
```
- **registers**: `rdi`=s1, `rsi`=s2, `rcx`=i, `eax`/`al`=c1(+result), `edx`/`dl`=c2
- **key ops**: `movzx` byte loads (unsigned compare); `cmp al, dl`→`jne .diff`;
  `cmp al, 0`→`je .done`; `sub eax, edx`
- **return**: `eax` = `0` / negative / positive (`s1[i]-s2[i]`).

## ft_write / ft_read
`ssize_t ft_write(int fd, const void *buf, size_t count)`
`ssize_t ft_read(int fd, void *buf, size_t count)`

```python
ft_write(fd, buf, count):
    ret = syscall(SYS_write, fd, buf, count)   # SYS_read=0 for ft_read
    if ret < 0: errno = -ret; return -1
    return ret
```
- **registers**: `rdi`=fd, `rsi`=buf, `rdx`=count (line up with syscall ABI);
  `rax`=syscall #(1 write / 0 read)→result
- **key ops**: `syscall`; `cmp rax, 0`/`jl .error`; `neg rax`; save via
  `push/call __errno_location wrt ..plt/pop`; `mov dword [rax], edx`; `mov rax, -1`
- **return**: bytes written/read on success; `-1` + `errno` set on failure.

## ft_strdup
`char *ft_strdup(const char *s)`

```python
ft_strdup(s):
    len = ft_strlen(s)
    buf = malloc(len + 1)
    if buf == NULL: return NULL
    ft_strcpy(buf, s)
    return buf
```
- **registers**: `rbx`=s (callee-saved), `r12`=new buffer (callee-saved),
  `rax`=each sub-call's result. `push rbx/r12; sub rsp,8` for 16B alignment.
- **key ops**: `call ft_strlen`→`inc rax`→`call malloc`; `test rax,rax`/`jz .done`;
  `call ft_strcpy`; `mov rax, r12`
- **return**: `rax` = new heap copy of `s`, or `NULL` (errno set by `malloc`).

---

# Bonus part

- `ft_atoi_base`
- `ft_list_push_front`
- `ft_list_size`
- `ft_list_sort`
- `ft_list_remove_if`

> `t_list`: `data` at offset 0, `next` at offset 8.

## ft_atoi_base
`int ft_atoi_base(char *str, char *base)`

```python
atoi_base(str, base):
    if len(base) < 2 or base has '+','-',whitespace,or dup: return 0
    skip whitespace in str; sign = +1/-1 if str has a leading sign
    result = 0
    while str[i] in base:
        result = result * len(base) + index_of(str[i], base); i += 1
    return result * sign
```
- **registers**: `rbx`=str, `r12`=base, `r13`=base_len (all callee-saved),
  `rcx`=i, `r8`=sign, `r9`=result, `r10`=digit search index
- **key ops**: `cmp r13,2`/`jl .invalid`; per-char `cmp al,<char>` checks reject
  `+ - <ws>` and duplicates in base; digit lookup loop `cmp al, dl`/`je .digit_found`;
  `imul rax, r13` (scale by base) then `+ digit`
- **return**: `rax` = parsed int, or `0` if base invalid.

## ft_list_push_front
`void ft_list_push_front(t_list **begin_list, void *data)`

```python
list_push_front(begin_list, data):
    node = malloc(sizeof(t_list))
    if node == NULL: return
    node->data = data; node->next = *begin_list; *begin_list = node
```
- **registers**: `rbx`=begin_list, `r12`=data (callee-saved), `rax`=new node
- **key ops**: `call malloc`; `test rax,rax`/`jz .done`; relink via `[rax]`,
  `[rax+8]`, `[rbx]`
- **return**: none (void); no-op if `malloc` fails.

## ft_list_size
`unsigned int ft_list_size(t_list *begin_list)`

```python
list_size(node):
    size = 0
    while node: size += 1; node = node->next
    return size
```
- **registers**: `rdi`=node cursor, `eax`=size
- **key ops**: `test rdi,rdi`/`jz .done`; `inc eax`; `mov rdi,[rdi+8]`
- **return**: `eax` = node count.

## ft_list_sort
`void ft_list_sort(t_list **begin_list, int (*cmp)(void*,void*))`

```python
list_sort(begin_list, cmp):
    do:
        swapped = false; node = *begin_list
        while node and node->next:
            if cmp(node->data, node->next->data) > 0:
                swap data fields; swapped = true
            node = node->next
    while swapped
```
- **registers**: `rbx`=begin_list, `r12`=cmp, `r13`=swapped, `r14`=node,
  `r15`=node->next (all callee-saved, since `cmp` is a user callback)
- **key ops**: `call r12` (indirect); `cmp eax,0`/`jle .no_swap`; swap via
  `mov`s; outer loop `cmp r13,0`/`jne .outer`
- **return**: none (void); bubble sort, swaps `data` fields in place, stable on ties.

## ft_list_remove_if
`void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(void*,void*), void (*free_fct)(void*))`

```python
list_remove_if(begin_list, data_ref, cmp, free_fct):
    cur = begin_list                 # address of pointer-to-current-node
    while *cur:
        node = *cur
        if cmp(node->data, data_ref) == 0:
            *cur = node->next; free_fct(node->data); free(node)
        else:
            cur = &(node->next)
```
- **registers**: `r14`=cur_ptr_addr, `rbx`=data_ref, `r12`=cmp, `r13`=free_fct,
  `r15`=current node (all callee-saved)
- **key ops**: `test r15,r15`/`jz .end`; `call r12`; `cmp eax,0`/`jne .advance`;
  unlink `mov [r14], rdx` then `call r13` / `call free`; else `lea r14,[r15+8]`
- **return**: none (void). Uses "address of the pointer that points at the
  current node" so head and mid-list removal share one code path.

---

# Building

```bash
make             # libasm.a (mandatory)
make bonus       # libasm.a (mandatory + bonus)
make test        # build + run test_libasm
make test_bonus  # build + run test_libasm_bonus
make re          # fclean + all
```

`nasm -f elf64` → `ar rcs` → linked with `main.c`/`main_bonus.c` via `gcc`.

# Registers

## rax vs eax vs ax vs al vs ah 

They're the same physical register, just different widths — x86-64 lets you address a register at 8/16/32/64-bit width:

| Name | Width | Bits |
|---|---|---|
| `rax` | 64-bit | full register |
| `eax` | 32-bit | low 32 bits of `rax` |
| `ax` | 16-bit | low 16 bits of `rax` |
| `al` | 8-bit | low 8 bits of `rax` |
| `ah` | 8-bit | bits 8–15 of `rax` (the byte above `al`) |

Key gotcha: writing to a 32-bit form (`eax`) zeroes the upper 32 bits of `rax` automatically. Writing to al or ax does not clear the upper bits — the old garbage stays. That's why `ft_list_size` uses `xor eax, eax` (clears all 64 bits) even though it only needs to return an `unsigned int`.

Use `rax` when working with 64-bit values (pointers, sizes), and `eax`/`al` when the C type is narrower (`int`, `char`) — matching the operation to the data width, and to avoid touching bits you don't own.

## Register order by argument number (System V AMD64)

For a function call, arguments are assigned to registers in order, left to right:

| Argument | Register |
|---|---|
| 1st | `rdi` |
| 2nd | `rsi` |
| 3rd | `rdx` |
| 4th | `rcx` |
| 5th | `r8` |
| 6th | `r9` |
| 7th+ | stack |