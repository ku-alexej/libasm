# libasm

About project:
- assembly
- registers
- commands
- comparations
- variables for function
- return value


# Mandatory part

functions, allowed functions

- `ft_strlen` (man 3 strlen)
- `ft_strcpy` (man 3 strcpy)
- `ft_strcmp` (man 3 strcmp)
- `ft_write`  (man 2 write) + errno
- `ft_read`   (man 2 read) + errno
- `ft_strdup` (man 3 strdup) + malloc

1. Function `malloc` allowed for `ft_strdup`.  
2. Errors during syscalls must be checked and handled properly when need. Code must set the variable `errno` properly. So using `extern` `___error` or `errno_location` is allowed.

## ft_strlen

**Original**:	`size_t strlen(const char *s)`  
**Custom**:		`size_t ft_strlen(const char *s)`

```python
# basic logic

strlen (s):
    length = 0

    while s[len] != '\0':
        length = length + 1

    return length
```

## ft_strcpy

**Original**:	`char *strcpy(char *restrict dst, const char *restrict src)`  
**Custom**:		`char ft_strcpy(char *dst, const char *src);`

`restrict` - overloop of two memory fields is restricted. The programmer is responsible for allocating a destination buffer large enough, that is, `strlen(src) + 1`.

```python

```
