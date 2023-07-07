/**
 * @details
 * Algorithmic optimisations allowed: calculating the outdegrees, buffer swap,
 * change the storage format.
 **/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define GRAPH_ORDER 2000
#define GRAPH_SIZE 100000
#define DAMPING_FACTOR 0.85
#define MAX_TIME 10

typedef struct
{
    int graph_order;
    uint8_t adjacency_matrix[GRAPH_ORDER][GRAPH_ORDER];
    uint8_t outdegree[GRAPH_ORDER];
} Graph;

void initialize_graph(Graph *graph, int graph_order)
{
    graph->graph_order = graph_order;

    int i, j;
    for (i = 0; i < graph_order; i++)
    {
        for (j = 0; j < graph_order; j++)
        {
            graph->adjacency_matrix[i][j] = 0;
        }
    }
}

void add_edge(Graph *graph, int source, int destination)
{
    if (source >= 0 && source < graph->graph_order && destination >= 0 && destination < graph->graph_order)
    {
        graph->adjacency_matrix[source][destination] = 1;
    }
}

int outdegree(Graph *graph, int node)
{
    int graph_order = graph->graph_order;
    int outdegree = 0;

    for (int i = 0; i < graph_order; i++)
    {
        if (graph->adjacency_matrix[node][i] == 1)
        {
            outdegree += 1;
        }
    }

    return outdegree;
}

void calculate_pagerank(Graph *graph, double pagerank[])
{
    int graph_order = graph->graph_order;
    double initial_rank = 1.0 / graph_order;

    int i, j;
    for (i = 0; i < graph_order; i++)
    {
        pagerank[i] = initial_rank;
    }

    double damping_value = (1.0 - DAMPING_FACTOR) / graph_order;

    double diff = 1.0;
    size_t iteration = 0;
    double start = omp_get_wtime();
    double elapsed = omp_get_wtime() - start;
    double time_per_iteration = 0;
    double new_pagerank[GRAPH_ORDER];
    for (i = 0; i < graph_order; i++)
    {
        new_pagerank[i] = 0.0;
    }
    while (elapsed < MAX_TIME && (elapsed + time_per_iteration) < MAX_TIME)
    {
        double iteration_start = omp_get_wtime();
        for (i = 0; i < graph_order; i++)
        {
            new_pagerank[i] = 0.0;
        }
        #pragma omp parallel for default(none) shared(graph_order, pagerank, graph) reduction(+: new_pagerank)
        for (int i = 0; i < graph_order; i++)
        {
            for (int j = 0; j < graph_order; j++)
            {
                if (graph->adjacency_matrix[j][i] == 1)
                {
                    new_pagerank[i] += pagerank[j] / graph->outdegree[j];
                }
            }
        }
        for (i = 0; i < graph_order; i++)
        {
            new_pagerank[i] = DAMPING_FACTOR * new_pagerank[i] + damping_value;
        }

        diff = 0.0;
        for (int i = 0; i < graph_order; i++)
        {
            diff += fabs(new_pagerank[i] - pagerank[i]);
        }

        pagerank = new_pagerank;
#pragma omp single
        {
            double iteration_end = omp_get_wtime();
            elapsed = omp_get_wtime() - start;
            iteration++;
            time_per_iteration = elapsed / iteration;
        }
    }
    printf("%zu iterations achieved in %.2f seconds\n", iteration, elapsed);
}

int main()
{
    double start = omp_get_wtime();
    Graph graph;
    initialize_graph(&graph, GRAPH_ORDER);

    srand(123);
    for (int i = 0; i < GRAPH_SIZE; i++)
    {
        add_edge(&graph, rand() % GRAPH_ORDER, rand() % GRAPH_ORDER);
    }

    for (int i = 0; i < GRAPH_ORDER; i++)
    {
        graph.outdegree[i] = outdegree(&graph, i);
    }

    double pagerank[GRAPH_ORDER];
    calculate_pagerank(&graph, pagerank);

    double sum_ranks = 0.0;
    for (int i = 0; i < graph.graph_order; i++)
    {
        if (i % 100 == 0)
        {
            printf("PageRank of Node %d: %.4lf\n", i, pagerank[i]);
        }
        sum_ranks += pagerank[i];
    }
    printf("Total rank = %f.\n", sum_ranks);
    double end = omp_get_wtime();

    printf("Time taken: %.2f seconds.\n", end - start);

    return 0;
}
