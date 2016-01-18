#include "Graph.h"
graph createGraph() {
    graph myGraph = (graph)malloc(sizeof(struct Graph));
    myGraph->allVertexes = malloc(sizeof(vertex) * max_vertexes);
    myGraph->num_vertexes = 0; 
    return myGraph;
}
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
edge createEdge(vertex myDestination, int myCost) {
    edge myEdge = (edge)malloc(sizeof(struct Edge));
    myEdge->destination = myDestination;
    myEdge->cost =  myCost;
    return myEdge;
}
queue createQueue() {
    queue myQueue = (queue)malloc(sizeof(struct PriorityQueue));
    myQueue->front = NULL;
    return myQueue;
}
void addToQueue(queue myQueue, node myNode) {
    if(myNode == NULL)
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
void deepDestroy(graph myGraph) {
    int graph_index;
    int edge_index;
    for(graph_index = 0; graph_index < myGraph->num_vertexes; graph_index++) {
        for(edge_index = 0; edge_index < myGraph->allVertexes[graph_index]->num_edges; edge_index++);  
            free(myGraph->allVertexes[graph_index]->myEdges[edge_index]);
        free(myGraph->allVertexes[graph_index]->name);
        free(myGraph->allVertexes[graph_index]);
    }
    free(myGraph);
}
node createNode(vertex myVertex) {
    node myNode = (node)malloc(sizeof(struct Node));
    myNode->myVertex = myVertex;
    myNode->next = NULL;
    return myNode;
}
void shortestPath(char *mySource) {
    printf("\nmethod 1: \n");
    int index;
    int num_visited = 0;
    int min_value;
    vertex origin = NULL;
    for(index = 0; index < myGraph->num_vertexes; index++) {
        if(strcmp(mySource, myGraph->allVertexes[index]->name) == 0) { 
            origin = myGraph->allVertexes[index];
            break;
        }    
    }
    if(origin == NULL) {
        printf("\nThat city doesn't exist!");
        return;
    }
    vertex myOrigin = origin;
    origin->visited = TRUE;
    origin->cost = 0;
    strcpy(origin->cheap_source, origin->name);
    queue myQueue = createQueue();
    addToQueue(myQueue, createNode(origin));
    while(num_visited < myGraph->num_vertexes) {
        if(isEmpty(myQueue))
            break;
        node temp = pop(myQueue); 
        if(temp->myVertex->cost == INT_MAX)
            break; 
        num_visited++;    
        for(index = 0; index < temp->myVertex->num_edges; index++) {
            if(temp->myVertex->myEdges[index]->cost + temp->myVertex->cost < temp->myVertex->myEdges[index]->destination->cost) {
                temp->myVertex->myEdges[index]->destination->cost = temp->myVertex->myEdges[index]->cost + temp->myVertex->cost;
                strcpy(temp->myVertex->myEdges[index]->destination->cheap_source, temp->myVertex->name);
                if(!(temp->myVertex->myEdges[index]->destination->visited))
                    addToQueue(myQueue, createNode(temp->myVertex->myEdges[index]->destination)); 
            }
        }
        free(temp);
    }
    for(index = 0; index < myGraph->num_vertexes; index++) {
        printf("\nthe cheapest path to %s is %d from %s", myGraph->allVertexes[index]->name, myGraph->allVertexes[index]->cost,
        myGraph->allVertexes[index]->cheap_source); 
    }
    for(index = 0; index < myGraph->num_vertexes; index++) {
        myGraph->allVertexes[index]->cost = INT_MAX; 
        myGraph->allVertexes[index]->visited = FALSE;
    }
    /*printf("\n\nmethod 2: \n");
    myOrigin->visited = TRUE;
    myOrigin->cost = 0;
    num_visited = 1;
    while(num_visited < myGraph->num_vertexes) {
        for(index = 0; index < myOrigin->num_edges; index++) {
            if(myOrigin->myEdges[index]->cost + myOrigin->cost < myOrigin->myEdges[index]->destination->cost) {
                myOrigin->myEdges[index]->destination->cost = myOrigin->myEdges[index]->cost + myOrigin->cost;
                strcpy(myOrigin->myEdges[index]->destination->cheap_source, myOrigin->name);
            }
        }
        min_value = INT_MAX;
        for(index = 0; index < myGraph->num_vertexes; index++) {
            if(myGraph->allVertexes[index]->cost < min_value && !(myGraph->allVertexes[index]->visited)) {
                myOrigin = myGraph->allVertexes[index]; 
                min_value = myGraph->allVertexes[index]->cost;
            }
        }
        if(min_value == INT_MAX)
            break;
        myOrigin->visited = TRUE;
        num_visited++;
    }
    for(index = 0; index < myGraph->num_vertexes; index++) {
        printf("\nthe cheapest path to %s is %d from %s", myGraph->allVertexes[index]->name, myGraph->allVertexes[index]->cost,
        myGraph->allVertexes[index]->cheap_source); 
    }*/
}
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
        //printQueue(myQueue);                
    }
}
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
        if(temp->myVertex->cost == INT_MAX)
            break;
        if(strcmp(temp->myVertex->name, destination_name) == 0) {
            sourceVertex = temp->myVertex;
            free(temp);
            break;
        }
        for(index = 0; index < temp->myVertex->num_edges; index++) {
            if(temp->myVertex->myEdges[index]->cost + temp->myVertex->cost < temp->myVertex->myEdges[index]->destination->cost) {
                temp->myVertex->myEdges[index]->destination->cost = temp->myVertex->myEdges[index]->cost + temp->myVertex->cost;
                strcpy(temp->myVertex->myEdges[index]->destination->cheap_source, temp->myVertex->name);
                reorderQueue(myQueue, temp->myVertex->myEdges[index]->destination);
            }
            //printQueue(myQueue);
        }
        free(temp); 
    }
    if(strcmp(sourceVertex->name, destination_name) == 0) {
        printf("\nthe cheapest cost to get to %s is %d miles", sourceVertex->name, sourceVertex->cost);
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
    free(myQueue);
}
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
    //queue myQueue = createQueue();
    //addToQueue(myQueue, createNode(sourceVertex));
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
    //free(myQueue);
}
void push(queue myStack, node myNode) {
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

