#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#define SET_SIZE 4
#define TRUE 1
#define FALSE 0
#define MAX_EDGES 4
#define MAX_COST 2000
#define TOTAL_VERTEXES 20597 
#define CITY_LENGTH 25
#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
typedef struct Vertex *vertex;
typedef struct Edge *edge;
typedef struct Graph *graph;
typedef struct Node *node;
//typedef struct Stack *stack;
typedef struct PriorityQueue *queue;
int max_vertexes;
FILE *fp;
graph myGraph;
struct Vertex {
    edge *myEdges;
    int num_edges;
    char *name;
    char cheap_source[CITY_LENGTH];
    int cost;
    int visited;
    int max_edges;
};
struct Node {
    vertex myVertex;
    node next;
};
struct Edge {
    vertex destination;
    int cost;
};
struct Graph {
    vertex * allVertexes;
    int num_vertexes;
};
/*struct Stack {
    node top;
};*/
struct PriorityQueue {
    node front;
};
queue createQueue();
vertex createVertex(char *myName);
edge createEdge(vertex myDestination, int myCost);
node createNode(vertex myVertex);
//stack createStack();
graph createGraph();
node pop(queue myStack);
void reorderQueue(queue myQueue, vertex myVertex); 
void shortestPathBetweenPriorityQueue(char* source_name, char* destination_name); 
void push(queue myStack, node myNode);
int isEmpty(queue myStack);
void shortestPath(char *mySource); 
void shortestPathBetween(char *mySource, char *myDestination);
void addEdge(vertex mySource, vertex myDestination, int myCost); 
void deepDestroy(graph myGraph);
void addToQueue(queue myQueue, node myNode);
void printQueue(queue myQueue);
#endif // SET_H_INCLUDED

