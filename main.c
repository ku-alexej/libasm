#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "libasm.h"

const char *OK = "\033[32mOK\033[0m";
const char *FAIL = "\033[31mFAIL\033[0m";

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

static void log_title(const char *title)
{
    printf(BOLD YELLOW "\n------- %s -------" RESET "\n", title);
}

static void test_strlen(void)
{
    const char *samples[] = {"", "Hi", "Hello, world!"};

    log_title("ft_strlen");
    for (size_t i = 0; i < sizeof(samples) / sizeof(*samples); i++)
    {
        size_t expected = strlen(samples[i]);
        size_t got = ft_strlen(samples[i]);
        printf("case=%-20s : strlen=[%zu] ft_strlen=[%zu] [%s" RESET "]\n",
            samples[i], expected, got, expected == got ? GREEN "OK" : RED "FAIL");
    }
}

static void test_strcpy(void)
{
    const char *samples[] = {"", "Hi", "Hello, world!"};

    log_title("ft_strcpy");
    for (size_t i = 0; i < sizeof(samples) / sizeof(*samples); i++)
    {
        char buf_lib[256];
        char buf_ft[256];
        char *r1 = strcpy(buf_lib, samples[i]);
        char *r2 = ft_strcpy(buf_ft, samples[i]);
        int content_ok = strcmp(buf_lib, buf_ft) == 0;
        int retval_ok = (r1 == buf_lib) && (r2 == buf_ft);

        printf("case=%-20s : content=[%s" RESET "] retval=[%s" RESET "]\n",
            samples[i],
            content_ok ? GREEN "OK" : RED "FAIL",
            retval_ok ? GREEN "OK" : RED "FAIL");
    }
}

static void test_strcmp(void)
{
    log_title("ft_strcmp");
    const char *pairs[][2] = {
        {"Hi", "Hi"}, {"Hi", "Ho"}, {"Ho", "Hi"},
        {"", ""}, {"Hi", "Hillo"}, {"abc", "abd"}
    };

    for (size_t i = 0; i < sizeof(pairs) / sizeof(*pairs); i++)
    {
        int r1 = strcmp(pairs[i][0], pairs[i][1]);
        int r2 = ft_strcmp(pairs[i][0], pairs[i][1]);
        int sign_match = (r1 > 0) - (r1 < 0);
        int sign_got = (r2 > 0) - (r2 < 0);

        printf("case=(%s,%s)\t : libc=[%d] ft=[%d] [%s" RESET "]\n",
            pairs[i][0], pairs[i][1], r1, r2,
            sign_match == sign_got ? GREEN "OK" : RED "FAIL");
    }
}

static void print_result(const char *name, ssize_t ret, int err)
{
    printf("%-25s ret=%zd", name, ret);
    if (ret == -1)
        printf(" errno=%d (%s)", err, strerror(err));
    printf("\n");
}

static int test_write_read(void)
{
    char buf[100];
    ssize_t ret;
    int fd;

    log_title("ft_write");

    /* fd = 1 : stdout */
    errno = 0;
    ret = ft_write(1, "ft_write -> stdout\n", 19);
    print_result("ft_write fd=1", ret, errno);
    errno = 0;
    ret = write(1, "write    -> stdout\n", 19);
    print_result("write    fd=1", ret, errno);
    printf("\n");

    /* fd = 2 : stderr */
    errno = 0;
    ret = ft_write(2, "ft_write -> stderr\n", 19);
    print_result("ft_write fd=2", ret, errno);
    errno = 0;
    ret = write(2, "write    -> stderr\n", 19);
    print_result("write    fd=2", ret, errno);
    printf("\n");

    /* fd = 0 : stdin */
    errno = 0;
    ret = ft_write(0, "ft_write -> stdin\n", 18);
    print_result("ft_write fd=0", ret, errno);
    errno = 0;
    ret = write(0, "write    -> stdin\n", 18);
    print_result("write    fd=0", ret, errno);
    printf("\n");

    /* wrong fd */
    errno = 0;
    ret = ft_write(-1, "wrong fd\n", 9);
    print_result("ft_write fd=-1", ret, errno);
    errno = 0;
    ret = write(-1, "wrong fd\n", 9);
    print_result("write    fd=-1", ret, errno);
    printf("\n");

    errno = 0;
    ret = ft_write(9999, "wrong fd\n", 9);
    print_result("ft_write fd=9999", ret, errno);
    errno = 0;
    ret = write(9999, "wrong fd\n", 9);
    print_result("write    fd=9999", ret, errno);

    log_title("ft_read");

    /* fd = 0 : stdin */
    printf("Type something for ft_read(fd=0): ");
    fflush(stdout);

    memset(buf, 0, sizeof(buf));
    errno = 0;
    ret = ft_read(0, buf, sizeof(buf) - 1);

    print_result("ft_read fd=0", ret, errno);

    if (ret >= 0)
        printf("data ft_read: [%s]\n", buf);

    printf("Type same as before for read(fd=0): ");
    fflush(stdout);

    memset(buf, 0, sizeof(buf));
    errno = 0;
    ret = read(0, buf, sizeof(buf) - 1);

    print_result("read fd=0", ret, errno);

    if (ret >= 0)
        printf("data read: [%s]\n", buf);

    printf("\n");


    /* wrong fd */
    errno = 0;
    ret = ft_read(-1, buf, sizeof(buf));
    print_result("ft_read fd=-1", ret, errno);
    errno = 0;
    ret = read(-1, buf, sizeof(buf));
    print_result("read    fd=-1", ret, errno);
    printf("\n");

    errno = 0;
    ret = ft_read(9999, buf, sizeof(buf));
    print_result("ft_read fd=9999", ret, errno);
    errno = 0;
    ret = read(9999, buf, sizeof(buf));
    print_result("read    fd=9999", ret, errno);

    log_title("ft_write/ft_read + real file");

    /* Create a file. */
    fd = open("test_ft_read_write.txt",
              O_CREAT | O_RDWR | O_TRUNC, 0644);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    /* Write to file */
    errno = 0;
    ret = ft_write(fd, "Hello from ft_write!\n", 21);
    print_result("ft_write real file", ret, errno);
    errno = 0;
    ret = write(fd, "Hello from    write!\n", 21);
    print_result("write    real file", ret, errno);
    printf("\n");

    /* Read from file */
    memset(buf, 0, sizeof(buf));
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        return 1;
    }
    errno = 0;
    ret = ft_read(fd, buf, sizeof(buf) - 1);
    print_result("ft_read real file", ret, errno);
    if (ret >= 0)
        printf("data ft_read: [%s]\n", buf);

    memset(buf, 0, sizeof(buf));
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        return 1;
    }
    errno = 0;
    ret = read(fd, buf, sizeof(buf) - 1);
    print_result("read    real file", ret, errno);
    if (ret >= 0)
        printf("data read: [%s]\n", buf);

    log_title("ft_write/ft_read + closed fd");
    close(fd);
    
    /* fd is now closed */
    errno = 0;
    ret = ft_write(fd, "closed fd\n", 10);
    print_result("ft_write closed fd", ret, errno);
    errno = 0;
    ret = write(fd, "closed fd\n", 10);
    print_result("write    closed fd", ret, errno);
    printf("\n");

    errno = 0;
    ret = ft_read(fd, buf, sizeof(buf));
    print_result("ft_read closed fd", ret, errno);
    errno = 0;
    ret = read(fd, buf, sizeof(buf));
    print_result("read    closed fd", ret, errno);

    return 0;
}

static void test_strdup(void)
{
    const char *samples[] = {"", "Hi", "Hello, world!"};

    log_title("ft_strdup");
    for (size_t i = 0; i < sizeof(samples) / sizeof(*samples); i++)
    {
        char *dup = ft_strdup(samples[i]);
        char *libc_dup = strdup(samples[i]);
        if (!dup)
        {
            printf("case=%-20s -> NULL, errno=%d (%s)\n",
                samples[i], errno, strerror(errno));
            if (libc_dup)
                free(libc_dup);
            continue;
        }
        if (!libc_dup)
        {
            printf("case=%-20s -> NULL (libc strdup failed), errno=%d (%s)\n",
                samples[i], errno, strerror(errno));
            free(dup);
            continue;
        }
        int ok = strcmp(dup, samples[i]) == 0 && dup != samples[i];
        printf("case=%-20s -> %-20s : src=[%p] dup=[%p] libc_dup=[%p] [%s" RESET "]\n",
            samples[i], dup, (void *)samples[i], (void *)dup, (void *)libc_dup, ok ? GREEN "OK" : RED "FAIL");
        free(dup);
        free(libc_dup);
    }
}

int main(void)
{
    test_strlen();
    test_strcpy();
    test_strcmp();
    test_strdup();
    if (test_write_read() != 0)
    {
        printf("Error occurred during write/read tests.\n");
        printf("errno=%d (%s)\n", errno, strerror(errno));
        return 1;
    }
    return 0;
}