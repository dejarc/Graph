#include "Graph.h"
/*Creates a graph structure*/
graph createGraph() {
    graph myGraph = (graph)malloc(sizeof(struct Graph));
    myGraph->allVertexes = malloc(sizeof(vertex) * max_vertexes);
    myGraph->num_vertexes = 0; 
    return myGraph;
}
/*creates a vertex structure
@param myName the name of the next city to input. Duplicates 
are not allowed
@return the new vertex*/
vertex createVertex(char *myName) {
    int index;
    for(index = 0; index < myGraph->num_vertexes; index++) {
        if(strcmp(myGraph->allVertexes[index]->name, myName) == 0) {
            printf("\nThat city already exists!");
            return NULL;
        }
    }
    vertex myVertex = (vertex)malloc(sizeof(struct Vertex));
    myVertex->num_edges = FALSE;
    myVertex->name = malloc(strlen(myName) + 1);
    strcpy(myVertex->name, myName);   
    myVertex->max_edges = MAX_EDGES;
    myVertex->myEdges = malloc(sizeof(edge) * myVertex->max_edges);
    myVertex->cost = INT_MAX;
    myVertex->visited = FALSE;
    myGraph->allVertexes[myGraph->num_vertexes++] = myVertex;
    if(myGraph->num_vertexes == max_vertexes - 1) {
        max_vertexes *= 2;
        myGraph->allVertexes = realloc(myGraph->allVertexes, sizeof(vertex) * max_vertexes);
    } 
    return myVertex;    
}
/*method to print the contents of a queue
@param myQueue the queue to print*/
void printQueue(queue myQueue) {
    node temp = myQueue->front;
    int num = 0;
    printf("\ncontents of the queue");
    while(temp != NULL) {
        printf("\nvertex name: %s vertex cost: %d", temp->myVertex->name, temp->myVertex->cost);
        num++;
        fprintf(fp, "\nvertex name: %s vertex cost: %d", temp->myVertex->name, temp->myVertex->cost);
        temp = temp->next;
    }
    fprintf(fp, "\ntotal number of processes after sort %d", num);
}
/*Creates an edge with a cost and a destination.
The edges in the structure are directed
@param myDestination the destination city in the edge
@param myCost the edge cost, between 0 and 2000 miles
@return the new edge created*/
edge createEdge(vertex myDestination, int myCost) {
    edge myEdge = (edge)malloc(sizeof(struct Edge));
    myEdge->destination = myDestination;
    myEdge->cost =  myCost;
    return myEdge;
}
/* to create a queue
@return the new queue created*/
queue createQueue() {
    queue myQueue = (queue)malloc(sizeof(struct PriorityQueue));
    myQueue->front = NULL;
    return myQueue;
}
/*method to add a node based on priority.
@param myQueue the queue to add to
@param myNode the new node to insert*/
void addToQueue(queue myQueue, node myNode) {
    if(myNode == NULL || myQueue == NULL)
        return;
    if(myQueue->front != NULL) {
        if(myQueue->front->myVertex->cost >= myNode->myVertex->cost) {
            myNode->next = myQueue->front;
            myQueue->front = myNode;
        } else {
            node cur = myQueue->front;
            node prev = myQueue->front;    
            while(cur->next != NULL && cur->myVertex->cost <= myNode->myVertex->cost) {
               prev = cur;
               cur = cur->next; 
            }
            if(cur->next == NULL && cur->myVertex->cost < myNode->myVertex->cost) {
                cur->next = myNode;
            } else {
                prev->next = myNode;
                myNode->next = cur;
            }
        }
    } else {
        myQueue->front = myNode;
    }
}
/*to free up all the memory in the graph
@param myGraph the graph to destroy*/
void deepDestroy(graph myGraph) {
    int graph_index;
    int edge_index;
    for(graph_index = 0; graph_index < myGraph->num_vertexes; graph_index++) {
        for(edge_index = 0; edge_index < myGraph->allVertexes[graph_index]->num_edges; edge_index++) { 
            free(myGraph->allVertexes[graph_index]->myEdges[edge_index]);
        }
        free(myGraph->allVertexes[graph_index]->name);
        free(myGraph->allVertexes[graph_index]);
    }
    free(myGraph);
}
/*to create a new node.
@param myVertex the vertex to include in the node
@return a new node with a vertex included
*/
node createNode(vertex myVertex) {
    if(myVertex == NULL)
        return NULL;
    node myNode = (node)malloc(sizeof(struct Node));
    myNode->myVertex = myVertex;
    myNode->next = NULL;
    return myNode;
}
/*method used to reset all graph values to initial state
@param myGraph the graph to reset
*/
void resetGraphValues(graph myGraph) {
   int index;
   for(index = 0; index < myGraph->num_vertexes; index++) {
       myGraph->allVertexes[index]->cost = INT_MAX;
       myGraph->allVertexes[index]->visited = FALSE;
   }
}
/*to calculate the shortest paths to all cities from a givern vertex
@param mySource the name of the starting city, prints error message if
not present
*/
void shortestPath(char *mySource) {
    int index;
    int source_present = FALSE;
    queue myQueue = createQueue();
    for(index = 0; index < myGraph->num_vertexes; index++) {
        if(strcmp(mySource, myGraph->allVertexes[index]->name) == 0) { 
            myGraph->allVertexes[index]->cost = 0;
            strcpy(myGraph->allVertexes[index]->cheap_source, myGraph->allVertexes[index]->name);
            source_present = TRUE;
        } 
        addToQueue(myQueue, createNode(myGraph->allVertexes[index]));
    }
    if(!source_present) {
        printf("\nThat city doesn't exist!");
        return;
    }
    while(!isEmpty(myQueue)) {
        node temp = pop(myQueue); 
        if(temp->myVertex->cost == INT_MAX)
            break; 
        for(index = 0; index < temp->myVertex->num_edges; index++) {
            if(temp->myVertex->myEdges[index]->cost + temp->myVertex->cost < temp->myVertex->myEdges[index]->destination->cost) {
                temp->myVertex->myEdges[index]->destination->cost = temp->myVertex->myEdges[index]->cost + temp->myVertex->cost;
                strcpy(temp->myVertex->myEdges[index]->destination->cheap_source, temp->myVertex->name);
                reorderQueue(myQueue, temp->myVertex->myEdges[index]->destination);
            }
        }
        free(temp);
    }
    for(index = 0; index < myGraph->num_vertexes; index++) {
        printf("\nthe cheapest path to %s is %d from %s", myGraph->allVertexes[index]->name, myGraph->allVertexes[index]->cost,
        myGraph->allVertexes[index]->cheap_source); 
    }
    free(myQueue);
}
/*reorders the queue when a value has changed
@param myQueue the queue to update
@param myVertex the vertex that has become cheaper
*/
void reorderQueue(queue myQueue, vertex myVertex) {
    if(myVertex == NULL || isEmpty(myQueue))
        return;
    if(myVertex != myQueue->front->myVertex) {
        node cur = myQueue->front;
        while(cur->next != NULL && cur->next->myVertex != myVertex) {
            cur = cur->next;
        }
        node temp = cur->next;
        cur->next = cur->next->next;
        temp->next = NULL;
        addToQueue(myQueue, temp);              
    }
}
/*calculates the shortest distance between a source and destination city using
djikstra's shortest path algorithm and a priority queue.
@param source_name the name of the starting city
@param destination_name the name of the destination city
*/
void shortestPathBetweenPriorityQueue(char* source_name, char* destination_name) {
    queue myQueue = createQueue();
    vertex sourceVertex = NULL;
    int index;
    int destination_exists = FALSE;
    for(index = 0; index < myGraph->num_vertexes; index++) {
        if(strcmp(myGraph->allVertexes[index]->name, source_name) == 0) {
            myGraph->allVertexes[index]->cost = 0;
            sourceVertex = myGraph->allVertexes[index]; 
        }
       if(strcmp(myGraph->allVertexes[index]->name, destination_name) == 0) 
            destination_exists = TRUE;                
        addToQueue(myQueue, createNode(myGraph->allVertexes[index]));
    }
    if(sourceVertex == NULL || !destination_exists) {
        printf("\none of the cities doesn't exist!");
        return;
    }
    while(!isEmpty(myQueue)) { 
        node temp = pop(myQueue);
        if(temp->myVertex->cost == INT_MAX)//no connection exists break out of the loop
            break;
        if(strcmp(temp->myVertex->name, destination_name) == 0) {//destination has been found, break
            sourceVertex = temp->myVertex;
            free(temp);
            break;
        }
        for(index = 0; index < temp->myVertex->num_edges; index++) {
            //if the cost has been decreased reset the cost, copy the new cheap source, and reorder the queue storing the nodes
            if(temp->myVertex->myEdges[index]->cost + temp->myVertex->cost < temp->myVertex->myEdges[index]->destination->cost) {
                temp->myVertex->myEdges[index]->destination->cost = temp->myVertex->myEdges[index]->cost + temp->myVertex->cost;
                strcpy(temp->myVertex->myEdges[index]->destination->cheap_source, temp->myVertex->name);
                reorderQueue(myQueue, temp->myVertex->myEdges[index]->destination);
            }
        }
        free(temp); 
    }
    if(strcmp(sourceVertex->name, destination_name) == 0) {
        printf("\nthe cheapest cost to get to %s is %d miles", sourceVertex->name, sourceVertex->cost);
        queue myStack = createQueue();
        push(myStack, createNode(sourceVertex));    
        while(strcmp(sourceVertex->name,source_name) != 0) {//used to back trace the path in the map
            for(index = 0; index < myGraph->num_vertexes; index++) {
                if(strcmp(sourceVertex->cheap_source, myGraph->allVertexes[index]->name) == 0) {
                     sourceVertex = myGraph->allVertexes[index];
                     break;
                }
            }
            push(myStack, createNode(sourceVertex));
        }
        node temp = pop(myStack);
        int edge_length = temp->myVertex->cost;
        printf("\nfrom %s with a cost of %d", temp->myVertex->name, edge_length);
        free(temp);
        while(!isEmpty(myStack)) {
            node myTop = pop(myStack);
            edge_length = myTop->myVertex->cost - edge_length;
            printf("\nto %s with cost of %d", myTop->myVertex->name, edge_length);
            edge_length = myTop->myVertex->cost;
            free(myTop);
        }
        free(myStack); 
    } else {
        printf("\nno connection exists between the two points");
    }
    free(myQueue);
}
/*calculates the shortest path between two cities, without utilizing a priority queue
@param source_name the source city name
@param destination_name the name of the destination city
*/
void shortestPathBetween(char* source_name, char* destination_name) {
    int index;
    vertex sourceVertex = NULL; 
    vertex destVertex = NULL;
    int num_visited = 0;
    int min_value;
    node myTemp;
    for(index = 0; index < myGraph->num_vertexes; index++) {
        if(strcmp(myGraph->allVertexes[index]->name, source_name) == 0)
            sourceVertex = myGraph->allVertexes[index];
        if(strcmp(myGraph->allVertexes[index]->name,destination_name) == 0)
            destVertex = myGraph->allVertexes[index];
    }
    if(sourceVertex == NULL || destVertex == NULL) {
        printf("\nThat city doesn't exist!");
        return;
    }
    sourceVertex->visited = TRUE;
    sourceVertex->cost = 0;
    strcpy(sourceVertex->cheap_source, sourceVertex->name);
    while(sourceVertex != destVertex) { 
        for(index = 0; index < sourceVertex->num_edges; index++) {
            if(sourceVertex->myEdges[index]->cost + sourceVertex->cost < sourceVertex->myEdges[index]->destination->cost) {
                sourceVertex->myEdges[index]->destination->cost = sourceVertex->myEdges[index]->cost + sourceVertex->cost;
                strcpy(sourceVertex->myEdges[index]->destination->cheap_source, sourceVertex->name);
            }
        }
        min_value = INT_MAX;
        for(index = 0; index < myGraph->num_vertexes; index++) {
            if(myGraph->allVertexes[index]->cost < min_value && !(myGraph->allVertexes[index]->visited)) {
                sourceVertex = myGraph->allVertexes[index];
                min_value = myGraph->allVertexes[index]->cost;
            }
        } 
        if(min_value == INT_MAX)
            break;
        sourceVertex->visited = TRUE;
        printf("\nsource vertex is now %s with a cost of %d", sourceVertex->name, sourceVertex->cost);
    }
    if(sourceVertex == destVertex) {
        printf("\nthe cheapest cost to get to %s is %d miles", destVertex->name, destVertex->cost);
        queue myStack = createQueue();
        push(myStack, createNode(sourceVertex));    
        while(strcmp(sourceVertex->name,source_name) != 0) {
            for(index = 0; index < myGraph->num_vertexes; index++) {
                if(strcmp(sourceVertex->cheap_source, myGraph->allVertexes[index]->name) == 0) {
                     sourceVertex = myGraph->allVertexes[index];
                     break;
                }
            }
            push(myStack, createNode(sourceVertex));
        }
        node temp = pop(myStack);
        int edge_length = temp->myVertex->cost;
        printf("\nfrom %s with a cost of %d", temp->myVertex->name, edge_length);
        free(temp);
        while(!isEmpty(myStack)) {
            node myTop = pop(myStack);
            edge_length = myTop->myVertex->cost - edge_length;
            printf("\nto %s with cost of %d", myTop->myVertex->name, edge_length);
            edge_length = myTop->myVertex->cost;
            free(myTop);
        }
        free(myStack); 
    } else {
        printf("\nno connection exists between the two points");
    }
}
/*method used to push a node to the top of a stack
@param myStack the stack to push to
@param myNode to node to insert on the top of the stack
*/
void push(queue myStack, node myNode) {
    if(myStack == NULL || myNode == NULL)
        return;
    if(myStack->front == NULL) {
        myStack->front = myNode;
    } else {
        myNode->next = myStack->front;
        myStack->front = myNode;
    }
}
node pop(queue myStack) {
    if(myStack->front != NULL) {
        node temp = myStack->front;
        myStack->front = myStack->front->next;
        temp->next = NULL;
        return temp; 
    }
    return NULL;
}
int isEmpty(queue myStack) {
    if(myStack->front == NULL) 
        return TRUE;
    return FALSE;
}
void addEdge(vertex mySource, vertex myDestination, int myCost) {
    edge myEdge = createEdge(myDestination, myCost);
    mySource->myEdges[mySource->num_edges++] = myEdge;
    if(mySource->num_edges == mySource->max_edges - 1) {
        mySource->max_edges *= 2;
        mySource->myEdges = realloc(mySource->myEdges, sizeof(edge) * mySource->max_edges);
    }
}

