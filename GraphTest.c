#include "Graph.h"
int main()
{
    int i;
    int j;
    time_t t;
    srand((unsigned) time(&t));
    max_vertexes = TOTAL_VERTEXES;
    myGraph = createGraph();
    char *conn_type;
    int rand_conn;
    fp = fopen("myInOrderNodes.txt", "w+");
    char cityName[CITY_LENGTH];
    char source_input[CITY_LENGTH];
    char destination_input[CITY_LENGTH];
    while(1) {
        printf("\nenter a city(20 characters) to be added to the map(d when done/q to quit): ");
        scanf(" %[^\n]s", cityName);
        if(strcmp(cityName, "d") == 0) {
            break;
        } 
        if(strcmp(cityName, "q") == 0) {
            return 0;
        }
        createVertex(cityName);
        //free(user_input);
    }
    printf("\nWould you like to enter connections manually or randomly?(m for manual r for random)");
    scanf("%ms", &conn_type);
    if(strcmp(conn_type, "m") == 0) {    
        while(1) {
            printf("\nenter a source: ");
            scanf(" %[^\n]s", source_input);
            if(strcmp(source_input, "d") == 0) {
                break;
            }   
            if(strcmp(source_input, "q") == 0) {
                return 0;
            }
            printf("\nenter a destination : ");
            scanf(" %[^\n]s", destination_input);
            //free(source_input);
            //free(destination_input);
        }   
    } else {
        printf("\nrandom was chosen");
        for(j = 0; j < myGraph->num_vertexes; j++) { 
            rand_conn = rand() % myGraph->num_vertexes;  
            for(i = 0; i < rand_conn; i++) {
                addEdge(myGraph->allVertexes[j], myGraph->allVertexes[rand() % myGraph->num_vertexes], rand() % MAX_COST);  
            }
        }
    }
    printf("\nenter a source to find the cheapest path: ");  
    scanf(" %[^\n]s", source_input);
    printf("\nenter a destination to find the cheapest path: ");  
    scanf(" %[^\n]s", destination_input);
    printf("\n%s was chosen as the source city and %s was chosen as the destination city", source_input, destination_input);
    shortestPathBetweenPriorityQueue(source_input, destination_input);
    //shortestPath(source_input);
    printf("\nwould you like to see the edges?(y or n)");
    scanf("%ms", &conn_type);
    if(strcmp(conn_type, "y") == 0) {
        for(j = 0; j < myGraph->num_vertexes; j++) { 
            printf("\ncity: %s",myGraph->allVertexes[j]->name);
            for(i = 0; i < myGraph->allVertexes[j]->num_edges; i++) {
                printf("\nedge between %s and %s with a cost of %d miles",myGraph->allVertexes[j]->name, 
                myGraph->allVertexes[j]->myEdges[i]->destination->name, myGraph->allVertexes[j]->myEdges[i]->cost);
            }
            printf("\n");
        }   
    } else {
        printf("\nthanks for playing!");
    }
    deepDestroy(myGraph);
    return 0;
}

