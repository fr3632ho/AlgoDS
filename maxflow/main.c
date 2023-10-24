#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <omp.h>

// SYSCALLS
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

typedef struct xedge_t xedge_t;

struct xedge_t {
    int u;
    int v;
    int c;
};


static char* progname;

// extern int preflow(int n, int m, int source, int sink, xedge_t *edges);
extern int preflow(int n, int m, int _s, int _t, xedge_t *edges);

static int next_int()
{
        int     x;
        int     c;

    /* this is like Java's nextInt to get the next integer.
     *
     * we read the next integer one digit at a time which is
     * simpler and faster than using the normal function
     * fscanf that needs to do more work.
     *
     * we get the value of a digit character by subtracting '0'
     * so the character '4' gives '4' - '0' == 4
     *
     * it works like this: say the next input is 124
     * x is first 0, then 1, then 10 + 2, and then 120 + 4.
     *
     */

    x = 0;
        while (isdigit(c = getchar()))
                x = 10 * x + c - '0';

        return x;
}


static void* xcalloc(size_t n, size_t s)
{
    void*		p;

    p = malloc(n * s);

    /* memset sets everything (in this case) to 0. */
    memset(p, 0, n * s);

    /* for the curious: so memset is equivalent to a simple
     * loop but a call to memset needs less memory, and also
      * most computers have special instructions to zero cache 
     * blocks which usually are used by memset since it normally
     * is written in assembler code. note that good compilers 
     * decide themselves whether to use memset or a for-loop
     * so it often does not matter. for small amounts of memory
     * such as a few bytes, good compilers will just use a 
     * sequence of store instructions and no call or loop at all.
     *
     */

    return p;
}

int main(int argc, char* argv[]) {
    FILE*		in;	/* input file set to stdin	*/
    int		f;	/* output from preflow.		*/
    int		n;	/* number of nodes.		*/
    int		m;	/* number of edges.		*/

    progname = argv[0];	/* name is a string in argv[0]. */

    in = stdin;		/* same as System.in in Java.	*/

    n = next_int();
    m = next_int();

    next_int(); // skip two ints 
    next_int();

    xedge_t *edges = xcalloc(m, sizeof(xedge_t));
    int a, b, c;
    for (int i = 0; i < m; i++)
    {
        a = next_int();
        b = next_int();
        c = next_int();
        edges[i].u = a;
        edges[i].v = b;
        edges[i].c = c;
    }

    fclose(in);

    f = preflow(n, m, 0, n - 1, edges);

    printf("f = %d\n", f);

    free(edges);

    return 0;


}