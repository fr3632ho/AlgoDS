#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#define N 20001
#define K 512
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define MAX(a, b) (((a) >= (b)) ? (a) : (b))
#define ABS(a) ((a) > 0 ? (a) : -(a))
#define INF (1 << 15)

// START DEFINES

// END DEFINES

// START STRUCT
typedef struct xedge_t xedge_t;
typedef struct edge_t edge_t;
typedef struct edg_lst_t edg_lst_t;
typedef struct list_t list_t;

struct xedge_t
{
    int u;
    int v;
    int c;
};

struct edge_t
{
    int to, idx;
    int f;
};

struct edg_lst_t
{
    size_t size;
    edge_t *edges;
};

// END STRUCT

// START DECLARES
edg_lst_t adj[N];

struct list_t
{
    size_t size;
    int *val;
} gap[N], hlst[N];

void fill(int *, size_t, int);
void pb(list_t *, int);
int back(list_t *);
void bpop(list_t *);
void clear(list_t *);
bool empty(list_t *);

int preflow(int n, int m, int _s, int _t, xedge_t *edges);
void build_graph(int n, int m, int _s, int _t, xedge_t *edges);
// void print(list_t* l);

int s, t, n, m;
int edge_count[N] = {0};
int excess[N] = {0};
int max_h, work;
int height[N], count[N];

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

// Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

// Regular underline text
#define UBLK "\e[4;30m"
#define URED "\e[4;31m"
#define UGRN "\e[4;32m"
#define UYEL "\e[4;33m"
#define UBLU "\e[4;34m"
#define UMAG "\e[4;35m"
#define UCYN "\e[4;36m"
#define UWHT "\e[4;37m"

// Regular background
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"

// High intensty background
#define BLKHB "\e[0;100m"
#define REDHB "\e[0;101m"
#define GRNHB "\e[0;102m"
#define YELHB "\e[0;103m"
#define BLUHB "\e[0;104m"
#define MAGHB "\e[0;105m"
#define CYNHB "\e[0;106m"
#define WHTHB "\e[0;107m"

// High intensty text
#define HBLK "\e[0;90m"
#define HRED "\e[0;91m"
#define HGRN "\e[0;92m"
#define HYEL "\e[0;93m"
#define HBLU "\e[0;94m"
#define HMAG "\e[0;95m"
#define HCYN "\e[0;96m"
#define HWHT "\e[0;97m"

// Bold high intensity text
#define BHBLK "\e[1;90m"
#define BHRED "\e[1;91m"
#define BHGRN "\e[1;92m"
#define BHYEL "\e[1;93m"
#define BHBLU "\e[1;94m"
#define BHMAG "\e[1;95m"
#define BHCYN "\e[1;96m"
#define BHWHT "\e[1;97m"
#define RESET "\033[0m"

// #define DEBUG
#ifdef DEBUG

#define LOG(...)                      \
    do                                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
    } while (0)

#define LOG_ARR(arr, size)                    \
    do                                        \
    {                                         \
        fprintf(stderr, "%s : [ ", #arr);     \
        for (int aaa = 0; aaa < size; ++aaa)  \
        {                                     \
            fprintf(stderr, "%d ", arr[aaa]); \
        }                                     \
        fprintf(stderr, "]\n");               \
    } while (0)

void print_adj_list(edg_lst_t adj[N], int n)
{
    for (size_t i = 0; i < n; i++)
    {
        printf("[%zu]: (h:%d, e:%d) => {\n", i, height[i], excess[i]);
        for (size_t j = 0; j < adj[i].size; j++)
        {
            printf("u: %d, idx: %u, f: %d\n", adj[i].edges[j].to, adj[i].edges[j].idx, adj[i].edges[j].f);
        }
        printf("}\n");
    }
}

#define LOG_ADJ(adj, n) print_adj_list(adj, n)

#else
#define LOG_ARR(...)
#define LOG(...)
#define LOG_ADJ(...)
#endif

// #define PERFORMANCE
#ifdef PERFORMANCE

#define NS_PER_SECOND 1000000000

void get_delta_time(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

#define INTERVALL 1000

#define BENCHMARK_BEGIN(name)                                                             \
    struct timespec benchmark_start_##name, benchmark_end_##name, benchmark_delta_##name; \
    _Thread_local static int benchmark_count_##name = 0;                                  \
    _Thread_local static double benchmark_total_##name = 0.0;                             \
    _Thread_local static double benchmark_min_##name = 10000.0;                           \
    _Thread_local static double benchmark_max_##name = 0.0;                               \
    clock_gettime(CLOCK_REALTIME, &benchmark_start_##name)

#define PRINT_BENCHMARK(name) \
    printf("Benchmark: [" BGRN #name RESET "]: took " CYN "%.5f " RESET "s " WHT "(min: %.5f, max: %.5f, count: %d, total: %.5f, avg: %.5f)" RESET "\n", benchmark_tmp_##name, benchmark_min_##name, benchmark_max_##name, benchmark_count_##name, benchmark_total_##name, benchmark_total_##name / benchmark_count_##name);

#define BENCHMAKR_END_HELPER(name)                                                                       \
    clock_gettime(CLOCK_REALTIME, &benchmark_end_##name);                                                \
    get_delta_time(benchmark_start_##name, benchmark_end_##name, &benchmark_delta_##name);               \
    double benchmark_tmp_##name = benchmark_delta_##name.tv_sec + benchmark_delta_##name.tv_nsec * 1e-9; \
    benchmark_count_##name++;                                                                            \
    benchmark_total_##name += benchmark_tmp_##name;                                                      \
    benchmark_min_##name = MIN(benchmark_min_##name, benchmark_tmp_##name);                              \
    benchmark_max_##name = MAX(benchmark_max_##name, benchmark_tmp_##name);

#define BENCHMARK_END(name)     \
    BENCHMAKR_END_HELPER(name); \
    PRINT_BENCHMARK(name)

#define BENCHMARK_END_INTERVALL(name, intervall) \
    BENCHMAKR_END_HELPER(name);                  \
    if (benchmark_count_##name % intervall == 0) \
    {                                            \
        PRINT_BENCHMARK(name);                   \
    }

#else
#define BENCHMARK_BEGIN(...)
#define BENCHMARK_END(...)
#define BENCHMARK_END_INTERVALL(name, intervall)
#define BENCHMAKR_END_HELPER(name)
#define PRINT_BENCHMARK(name)

#endif // PERFORMANCE

void connect(int from, int to, int c)
{
    edg_lst_t *e1 = &adj[from], *e2 = &adj[to];
    LOG("[MAIN]: connect(%d, %d, %d), (e1->size=%zu, e2->size=%zu)\n", from, to, c, e1->size, e2->size);
    e1->edges[e1->size] = (edge_t){to, e2->size, c};
    e2->edges[e2->size] = (edge_t){from, e1->size, c};
    e1->size++, e2->size++;
}

void relabel(int v, int new_height)
{
    LOG("[MAIN]: relabel v=%d, new_height=%d\n", v, new_height);
    work++;
    if (height[v] != N)
    {
        count[height[v]]--;
    }

    height[v] = new_height;
    if (new_height == N)
        return;

    count[new_height]++;
    max_h = new_height;

    pb(&gap[new_height], v);
    if (excess[v] > 0)
    {
        pb(&hlst[new_height], v);
    }
}

int frontier[N] = {0};
void global_relabel()
{
    BENCHMARK_BEGIN(global_relabel);
    work = 0;

    fill(height, N, N);
    fill(count, N, 0);

    for (int i = 0; i < max_h; i++)
    {
        clear(&gap[i]);
        clear(&hlst[i]);
    }

    height[t] = 0;
    int l = 0, r = 1;
    frontier[0] = t;
    edge_t *e = NULL;
    LOG("[MAIN]: starting BFS, l=%d, r=%d\n", l, r);
    while (l < r)
    {
        int u = frontier[l++];
        LOG("[MAIN]: u=%d, height[u]=%d, neighbours=%zu\n", u, height[u], adj[u].size);
        for (int i = 0; i < adj[u].size; i++)
        {
            e = &adj[u].edges[i];
            LOG("[MAIN]: e->to=%d, e->idx=%d, e->f=%d\n", e->to, e->idx, e->f);
            if (height[e->to] == N && adj[e->to].edges[e->idx].f > 0)
            {
                frontier[r++] = e->to;
                relabel(e->to, height[u] + 1);
            }
        }
        max_h = height[u];
    }

    LOG("[MAIN]: n=%d, max_h=%d\n", n, max_h);
    LOG_ARR(height, n);
    LOG_ARR(count, n);
    BENCHMARK_END(global_relabel);
}

void push(int u, edge_t *e)
{
    if (excess[e->to] == 0)
    {
        pb(&hlst[height[e->to]], e->to);
    }

    int dt = MIN(excess[u], e->f);
    e->f -= dt;
    adj[e->to].edges[e->idx].f += dt;
    LOG("[W:0] push success for u=%d, v=%d, dt=%d\n", u, e->to, dt);

    excess[u] -= dt;
    excess[e->to] += dt;
}

void inital_push(int u, edge_t *e)
{
    if (excess[e->to] == 0)
    {
        pb(&hlst[height[e->to]], e->to);
        // assign node
    }
    int dt = MIN(excess[u], e->f);
    e->f -= dt;
    adj[e->to].edges[e->idx].f += dt;

    excess[u] -= dt;
    excess[e->to] += dt;
}
void tpush() {}

// END DECLARES

inline void fill(int *arr, size_t n, int v)
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = v;
    }
}

inline void pb(list_t *l, int v)
{
    l->val[l->size] = v;
    l->size++;
}
inline int back(list_t *l)
{
    return l->val[l->size - 1];
}

inline void bpop(list_t *l)
{
    l->size--;
    assert(l->size >= 0);
}

inline void clear(list_t *l)
{
    l->size = 0;
}

inline bool empty(list_t *l)
{
    LOG("[MAIN]: l->size=%zu\n", l->size);
    return l->size == 0;
}

void build_graph(int _n, int _m, int _s, int _t, xedge_t *e)
{
    BENCHMARK_BEGIN(build_graph);
    int i;
    n = _n;
    m = _m;

    s = _s;
    t = _t;
    for (i = 0; i < m; i++)
    {
        edge_count[e[i].u]++;
        edge_count[e[i].v]++;
    }

    for (i = 0; i < n; i++)
    {

        adj[i].edges = (edge_t *)calloc(edge_count[i], sizeof(edge_t));
        adj[i].size = 0;

        hlst[i].size = 0;
        gap[i].size = 0;

        hlst[i].val = (int *)calloc(K, sizeof(int));
        gap[i].val = (int *)calloc(K, sizeof(int));
    }

    for (i = 0; i < m; i++)
    {
        connect(e[i].u, e[i].v, e[i].c);
    }

    BENCHMARK_END(build_graph);
}

void discharge(int u)
{
    BENCHMARK_BEGIN(discharge);
    int new_height = N;
    edge_t *e = NULL;
    LOG("[W:0] starting discharge u=%d, h=%d, e=%d\n", u, height[u], excess[u]);
    for (int i = 0; i < adj[u].size; i++)
    {
        e = &adj[u].edges[i];
        if (e->f > 0)
        {
            if (height[u] == height[e->to] + 1)
            {
                push(u, e);
                if (excess[u] <= 0)
                {
                    LOG("[W:0] u=%d out of excess, breaking\n", u);
                    goto end;
                }
            }
            else
            {
                LOG("[W:0]: u=%d failed push to v=%d\n", u, e->to);
                new_height = MIN(new_height, height[e->to] + 1);
            }
        }
    }

    // relabel here
    if (count[height[u]] > 1)
    {
        relabel(u, new_height);
    }
    else
    {
        // gap here
        LOG("[MAIN]: !GAP! u=%d, height[u]=%d\n", u, height[u]);
        for (int i = height[u]; i <= max_h; i++)
        {
            for (int j = 0; j < gap[i].size; j++)
            {
                int vv = gap[i].val[j];
                relabel(vv, N);
            }
            clear(&gap[i]);
        }
    }

end:
    BENCHMARK_END_INTERVALL(discharge, 100);
}

int preflow(int _n, int _m, int _s, int _t, xedge_t *edges)
{
    build_graph(_n, _m, _s, _t, edges);

    fill(excess, n, 0);
    excess[s] = INF, excess[t] = -INF;

    global_relabel();

    for (int i = 0; i < adj[s].size; i++)
    {
        push(s, &adj[s].edges[i]);
    }

    LOG_ADJ(adj, n);
    for (; max_h >= 0; max_h--)
    {
        while (!empty(&hlst[max_h]))
        {
            int v = back(&hlst[max_h]);
            bpop(&hlst[max_h]);
            LOG("[MAIN]: assign node %d, h=%d, e=%d\n", v, height[v], excess[v]);
            discharge(v);
            if (work > 4 * N)
            {
                global_relabel();
            }
        }
    }
    return excess[t] + INF;
}
