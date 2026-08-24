#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
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

static void test_write_read(void)
{
	log_title("ft_write / ft_read");
	errno = 0;

	write(1, "Type some text and press Enter (or Ctrl+D to end):\n", 0);
	printf("ft_write: ");
	ssize_t r = ft_write(1, "|one fox, two foxes, three foxes|\n", 34);
	printf("returned %zd, errno=%d (%s)\n",
		r, errno, strerror(errno));
	printf("   write: ");
	r = write(1, "|one fox, two foxes, three foxes|\n", 34);
	printf("returned %zd, errno=%d (%s)\n",
		r, errno, strerror(errno));
		
	printf("\n");
	r = ft_write(999, "x", 1);
	printf("ft_write bad fd: returned %zd, errno=%d (%s)\n",
		r, errno, strerror(errno));
	r = write(999, "x", 1);
	printf("   write bad fd: returned %zd, errno=%d (%s)\n",
		r, errno, strerror(errno));
	
	printf("\n");
	char buf[32];
	memset(buf, 0, sizeof(buf));
	r = ft_read(0, buf, 0); /* zero-length read: should return 0, no error */
	printf("ft_read count=0: returned %zd\n", r);


	printf("\n");
	r = ft_read(999, buf, 10);
	printf("ft_read bad fd: returned %zd, errno=%d (%s)\n",
		r, errno, strerror(errno));
}

static void test_strdup(void)
{
	const char *samples[] = {"", "Hi", "Hello, world!"};

	log_title("ft_strdup");
	for (size_t i = 0; i < sizeof(samples) / sizeof(*samples); i++)
	{
		char *dup = ft_strdup(samples[i]);
		if (!dup)
		{
			printf("case=%-20s -> NULL, errno=%d (%s)\n",
				samples[i], errno, strerror(errno));
			continue;
		}
		int ok = strcmp(dup, samples[i]) == 0 && dup != samples[i];
		printf("case=%-20s -> %-20s : src=[%p] dup=[%p] [%s" RESET "]\n",
			samples[i], dup, (void *)samples[i], (void *)dup, ok ? GREEN "OK" : RED "FAIL");
		free(dup);
	}
}

int main(void)
{
	test_strlen();
	test_strcpy();
	test_strcmp();
	test_write_read();
	test_strdup();
	return 0;
}