#include <stdio.h>
#include <stdlib.h>
#include "libasm_bonus.h"

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

static void test_atoi_base(void)
{
    struct
    {
        char *str;
        char *base;
    } casesBaseTen[] = {
        /* Invalid arguments */
        {"", "0123456789"},
        {"42+1", "0123456789"},
        {"42-1", "0123456789"},
        {"42 1", "0123456789"},
        {"42\t1", "0123456789"},
        {"42\n1", "0123456789"},
        {"42\r1", "0123456789"},
        {"42\v1", "0123456789"},
        {"42\f1", "0123456789"},
        {"42000000000000000", "0123456789"},

        /* Basic */
        {"42", "0123456789"},
        {"0", "0123456789"},
        {"1", "0123456789"},
        {"123456789", "0123456789"},

        /* Signs */
        {"-42", "0123456789"},
        {"+42", "0123456789"},
        {"--42", "0123456789"},
        {"++42", "0123456789"},
        {"-+42", "0123456789"},
        {"+-42", "0123456789"},
        {"-0", "0123456789"},
        {"+0", "0123456789"},

        /* Whitespace */
        {"  42", "0123456789"},
        {"\t42", "0123456789"},
        {"\n42", "0123456789"},
        {" \t\n\r42", "0123456789"},
        {"\v\f42", "0123456789"},
        {"  -42", "0123456789"},
        {"\t+42", "0123456789"},
        {"\n-42", "0123456789"},
        {" \t\n\r+42", "0123456789"},
        {"\v\f-42", "0123456789"},
        {"\t\t\t\t\t42", "0123456789"},
        {"\n\n\n\n\n42", "0123456789"},
        {" \t\n\r \t\n\r42", "0123456789"},
        {"\v\f\v\f\v\f42", "0123456789"},

        /* Invalid characters in string */
        {"42abc;", "0123456789"},
        {"123xyz", "0123456789"},
        {"2Axyz", "0123456789"},
        {"xyz42", "0123456789"},
        {"abc", "0123456789"},
        {";", "0123456789"},
        {"42;", "0123456789"},
        {"42 67", "0123456789"},
    };

    struct
    {
        char *str;
        char *base;
    } casesBases[] = {

        /* Invalid bases */
        {"42", ""},
        {"42", "0"},
        {"42", "1"},
        {"42", "0011"},
        {"42", "0123456788"},
        {"42", "0123456789+"},
        {"42", "0123456789-"},
        {"42", "0123456789 "},
        {"42", "0123456789\t"},
        {"42", "0123456789\n"},
        {"42", "0123456789\r"},
        {"42", "0123456789\v"},
        {"42", "0123456789\f"},

        /* Different bases */
        {"101", "01"},
        {"11111111", "01"},
        {"2A", "0123456789ABCDEF"},
        {"FF", "0123456789ABCDEF"},
        {"10", "0123456789"},
        {"10", "01"},
        {"10", "01234567"},
        {"10", "0123456789ABCDEF"},

    };

    log_title("ft_atoi_base vs atoi_base / (int)strtol(str, (char **)NULL, 10)");

    for (size_t i = 0; i < sizeof(casesBaseTen) / sizeof(*casesBaseTen); i++)
    {
        int got = ft_atoi_base(casesBaseTen[i].str, casesBaseTen[i].base);
        int expected = (int)strtol(casesBaseTen[i].str, (char **)NULL, 10);

        printf(
            "ft_atoi_base(%s, %s) "
            "got=[%d] "
            "expected=[%d] "
            "[%s]\n",
            casesBaseTen[i].str,
            casesBaseTen[i].base,
            got,
            expected,
            (got == expected) ? OK : FAIL);
    }

    log_title("ft_atoi_base not base 10");

    for (size_t i = 0; i < sizeof(casesBases) / sizeof(*casesBases); i++)
    {
        int got = ft_atoi_base(casesBases[i].str, casesBases[i].base);

        printf(
            "ft_atoi_base(%s, %s) "
            "got=[%d]\n",
            casesBases[i].str,
            casesBases[i].base,
            got);
    }
}

static void print_list(t_list *list)
{
    for (t_list *n = list; n; n = n->next)
        printf("%d ", *(int *)n->data);
    printf("\n");
}

static void free_noop(void *p)
{
	(void)p;
}

void ft_list_clear(t_list **begin_list, void (*free_fct)(void *))
{
    t_list *current;
    t_list *next;

    if (!begin_list)
        return;

    current = *begin_list;
    while (current)
    {
        next = current->next;
        if (free_fct)
            free_fct(current->data);
        free(current);
        current = next;
    }
    *begin_list = NULL;
}

static void test_list_push_front_and_size(void)
{
    log_title("ft_list_push_front / ft_list_size");

    t_list *list = NULL;
    static int values[] = {1, 2, 3};

    printf("empty list size: %u\n", ft_list_size(list));

    ft_list_push_front(&list, &values[0]);
    ft_list_push_front(&list, &values[1]);
    ft_list_push_front(&list, &values[2]);

    printf("after 3 pushes, size=%u, order: ", ft_list_size(list));
    print_list(list); /* expect: 3 2 1 */

	ft_list_clear(&list, free_noop);
}

static int cmp_int(void *a, void *b)
{
    return *(int *)a - *(int *)b;
}

static void test_list_sort(void)
{
    log_title("ft_list_sort");

    t_list *list = NULL;
    static int values[] = {3, 1, 4, 1, 5, 9, 2, 6};

    for (int i = 7; i >= 0; i--)
        ft_list_push_front(&list, &values[i]);

    printf("before sort: ");
    print_list(list);

    ft_list_sort(&list, cmp_int);

    printf("after sort:  ");
    print_list(list); /* expect ascending: 1 1 2 3 4 5 6 9 */

	ft_list_clear(&list, free_noop);
}

static void test_list_remove_if(void)
{
    log_title("ft_list_remove_if");

    t_list *list = NULL;
    static int values[] = {1, 2, 1, 3, 1};

    for (int i = 4; i >= 0; i--)
        ft_list_push_front(&list, &values[i]);

    printf("before remove_if: ");
    print_list(list); /* 1 2 1 3 1 */

    int target = 1;
    /* stack-allocated data here, so use a no-op free_fct */
    ft_list_remove_if(&list, &target, cmp_int, free_noop);

    printf("after remove_if:  ");
    print_list(list); /* expect: 2 3 */

	ft_list_clear(&list, free_noop);
}

int main(void)
{
    test_atoi_base();
    test_list_push_front_and_size();
    test_list_sort();
    test_list_remove_if();
    return 0;
}