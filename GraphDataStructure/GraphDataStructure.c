#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int dest;
    struct Node* next;
}Node;

void addEdge(Node** adjList, int src, int dest) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->dest = dest;
    newNode->next = NULL;
    
    if(adjList[src] == NULL) {
        adjList[src] = newNode;
    } else {
        Node* curr = adjList[src];
        while(curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
}

void printAdjList(Node** adjList, int n) {
    int i;
    for(i = 0; i < n; i++) {
        Node* curr = adjList[i];
        printf("Lesson %d: ", i + 1);
        while(curr != NULL) {
            printf("%d ", curr->dest + 1);
            curr = curr->next;
        }
        printf("\n");
    }
}

void calcIndegree(Node** adjList, int n, int* indegree) {
    int i;
    for(i = 0; i < n; i++) {
        Node* curr = adjList[i];
        while(curr != NULL) {
            indegree[i]++;
            curr = curr->next;
        }
    }
}

int* findPrerequisiteZero(int* indegree, int n, int* count) {
    if (indegree == NULL || count == NULL || n <= 0) {
        printf("Error: Invalid input\n");
        return NULL;
    }
    
    int* prerequisiteZero = (int*) malloc(n * sizeof(int));
    *count = 0;
    int i;
    for(i = 0; i < n; i++) {
        if(indegree[i] == 0) {
            prerequisiteZero[*count] = i;
            (*count)++;
        }
    }
    return prerequisiteZero;
}

int main() {
	int n, i, j;
    printf("Enter the number of lessons: ");
    scanf("%d", &n);
    /*int adj[5][5] = {{0, 0, 0, 0, 0},
                 	 {1, 0, 0, 0, 0}, 
                 	 {0, 0, 0, 0, 0},
                 	 {0, 1, 0, 0, 0}, 
                 	 {0, 0, 1, 1, 0}};
	*/
    int **adj = (int **) malloc(n * sizeof(int *));
    for(i = 0; i < n; i++) {
        adj[i] = (int *) malloc(n * sizeof(int));
    }
    
    printf("Enter the prerequisites for each lesson (0 for no prerequisite):\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            printf("Is lesson %d a prerequisite for lesson %d? (1/0): ", j + 1, i + 1);
            scanf("%d", &adj[i][j]);
        }
    }
    
    printf("\nThe adjacency matrix is:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            printf("%d ", adj[i][j]);
        }
        printf("\n");
    }
    
    Node** adjList = (Node**) malloc(n * sizeof(Node));
    for(i = 0; i < n; i++) {
        adjList[i] = NULL;
    }
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(adj[j][i] == 1) {
                addEdge(adjList, j, i);
            }
        }
    }
    
    printf("\nThe adjacency list is:\n");
    printAdjList(adjList, n);
    
    int* indegree = (int*) malloc(n * sizeof(int));
     for(i = 0; i < n; i++) {
        indegree[i] = 0;
    }
    
	calcIndegree(adjList, n, indegree);
    for (i = 0; i < n; i++){
    	printf("\n%d. dersin indegree degeri %d'dir.\n", i+1, indegree[i]);
	}
	
    int count = 0, semesters = 0, counter = 0, course, shouldContinue = 1;
    int* prerequisiteZero = findPrerequisiteZero(indegree, n, &count);
    while (count > 0 && shouldContinue == 1) {
        printf("\nSemester %d courses:\n", semesters + 1);
        
        for (i = 0; i < count; i++) {
			course = prerequisiteZero[i];
            printf("Course %d\n", course + 1);
            
            Node* curr = adjList[course];
            while (curr != NULL) {
                indegree[curr->dest]--;
                curr = curr->next;
            }
            indegree[i]--; 
        }

        calcIndegree(adjList, n, indegree);
        for (i = 0; i < n; i++){
    		printf("\n%d. dersin indegree degeri %d'dir.\n", i+1, indegree[i]);
		}
        prerequisiteZero = findPrerequisiteZero(indegree, n, &count);
        printf("Count: %d\n", count);

        semesters++;
        
        counter++;
        if (counter > n) {
            printf("Error: Input graph contains a cycle\n");
            shouldContinue = 0;
        }
    }
    printf("\nTotal semesters required: %d\n", semesters);
    
    for(i = 0; i < n; i++) {
        free(adj[i]);
    }
    free(adj); 
    
    for(i = 0; i < n; i++) {
        struct Node* curr = adjList[i];
        while(curr != NULL) {
            struct Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(adjList);
    free(indegree);
    free(prerequisiteZero);
    return 0;
}


