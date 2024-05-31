#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#define INF 99999
#define MAX_PATIENTS 1000

// Structure to represent a department
typedef struct Department
{
    int beds;
    int equipment;
    int numPatients; // Number of patients in the department
} Department;

// Structure to represent a patient
typedef struct Patient
{
    char name[50];
    int department; // Department number where patient is allocated
} Patient;

// Function prototypes
void addEdge(int **graph, int u, int v, int weight);
void dijkstra(int **graph, int src, int numVertices, int *dist);
void allocateEqualResources(int numVertices, Department *departments);
void allocateResourcesBasedOnShortestPath(int numVertices, Department *departments, int from, int to, int *dist);
int findNearestDepartment(int numVertices, Department *departments, int *dist);
void displayPatientAllocations(Patient *patients, int numPatients);
void departPatient(Patient *patients, Department *departments, int *numPatients);
void showDepartmentResources(Department *departments, int numDepartments);
int getIntInput(const char *prompt, int min, int max);

int main()
{
    int numDepartments = getIntInput("Enter the number of departments: ", 1, 100);

    Department *departments = malloc(numDepartments * sizeof(Department));
    if (departments == NULL)
    {
        perror("Failed to allocate memory for departments");
        return EXIT_FAILURE;
    }

    // Initialize the adjacency matrix
    int **graph = malloc(numDepartments * sizeof(int *));
    if (graph == NULL)
    {
        perror("Failed to allocate memory for adjacency matrix");
        free(departments);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < numDepartments; i++)
    {
        graph[i] = malloc(numDepartments * sizeof(int));
        if (graph[i] == NULL)
        {
            perror("Failed to allocate memory for adjacency matrix rows");
            for (int j = 0; j < i; j++)
            {
                free(graph[j]);
            }
            free(graph);
            free(departments);
            return EXIT_FAILURE;
        }

        for (int j = 0; j < numDepartments; j++)
        {
            graph[i][j] = INF;
        }
    }

    // Adding departments and their resources
    for (int i = 0; i < numDepartments; i++)
    {
        departments[i].beds = getIntInput("Enter the number of beds in department: ", 0, INT_MAX);
        departments[i].equipment = getIntInput("Enter the number of equipment in department: ", 0, INT_MAX);
        departments[i].numPatients = 0;
    }

    // Adding edges between departments
    int numEdges = getIntInput("Enter the number of edges: ", 0, numDepartments * (numDepartments - 1) / 2);

    for (int i = 0; i < numEdges; i++)
    {
        int u = getIntInput("Enter edge start department (1-based): ", 1, numDepartments) - 1;
        int v = getIntInput("Enter edge end department (1-based): ", 1, numDepartments) - 1;
        int weight = getIntInput("Enter distance between departments: ", 1, INT_MAX);
        addEdge(graph, u, v, weight);
    }

    // Initially, allocate resources using Dijkstra's algorithm from a central department
    int src = getIntInput("Enter the central department (1-based): ", 1, numDepartments) - 1;

    int *dist = malloc(numDepartments * sizeof(int));
    if (dist == NULL)
    {
        perror("Failed to allocate memory for distance array");
        for (int i = 0; i < numDepartments; i++)
        {
            free(graph[i]);
        }
        free(graph);
        free(departments);
        return EXIT_FAILURE;
    }

    dijkstra(graph, src, numDepartments, dist);

    int option;
    Patient *patients = malloc(MAX_PATIENTS * sizeof(Patient));
    if (patients == NULL)
    {
        perror("Failed to allocate memory for patients");
        for (int i = 0; i < numDepartments; i++)
        {
            free(graph[i]);
        }
        free(graph);
        free(departments);
        free(dist);
        return EXIT_FAILURE;
    }

    int numPatients = 0;

    do
    {
        printf("\nOptions:\n");
        printf("1. Allocate resources equally\n");
        printf("2. Allocate resources based on Dijkstra's shortest path\n");
        printf("3. Patient Entry\n");
        printf("4. Display Patient Allocations\n");
        printf("5. Depart a Patient\n");
        printf("6. Show current resources of each department and total resources and patients in each department\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            allocateEqualResources(numDepartments, departments);
            break;
        case 2:
        {
            int from = getIntInput("Enter source department (1-based): ", 1, numDepartments) - 1;
            int to = getIntInput("Enter destination department (1-based): ", 1, numDepartments) - 1;
            allocateResourcesBasedOnShortestPath(numDepartments, departments, from, to, dist);
            break;
        }
        case 3:
        {
            printf("\nPatient Entry:\n");
            printf("Enter patient name: ");
            scanf("%s", patients[numPatients].name);

            int nearestDepartment = findNearestDepartment(numDepartments, departments, dist);
            if (nearestDepartment != -1)
            {
                printf("Allocating patient %s to department %d\n", patients[numPatients].name, nearestDepartment + 1);
                departments[nearestDepartment].beds--;
                departments[nearestDepartment].equipment--;
                departments[nearestDepartment].numPatients++;
                patients[numPatients].department = nearestDepartment + 1;
                numPatients++;
            }
            else
            {
                printf("No suitable department found for patient %s\n", patients[numPatients].name);
            }
            break;
        }
        case 4:
            displayPatientAllocations(patients, numPatients);
            break;
        case 5:
            departPatient(patients, departments, &numPatients);
            break;
        case 6:
            showDepartmentResources(departments, numDepartments);
            break;
        case 7:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (option != 7);

    // Free allocated memory
    for (int i = 0; i < numDepartments; i++)
    {
        free(graph[i]);
    }
    free(graph);
    free(departments);
    free(patients);
    free(dist);

    return 0;
}

// Function to add an edge between two departments
void addEdge(int **graph, int u, int v, int weight)
{
    graph[u][v] = weight;
    graph[v][u] = weight;
}

// Dijkstra's algorithm to find the shortest path
void dijkstra(int **graph, int src, int numVertices, int *dist)
{
    int *visited = malloc(numVertices * sizeof(int));
    if (visited == NULL)
    {
        perror("Failed to allocate memory for visited array");
        return;
    }

    for (int i = 0; i < numVertices; i++)
    {
        dist[i] = INF;
        visited[i] = 0;
    }

    dist[src] = 0;

    for (int count = 0; count < numVertices - 1; count++)
    {
        int minDist = INF, minIndex;

        for (int v = 0; v < numVertices; v++)
        {
            if (!visited[v] && dist[v] <= minDist)
            {
                minDist = dist[v];
                minIndex = v;
            }
        }

        visited[minIndex] = 1;

        for (int v = 0; v < numVertices; v++)
        {
            if (!visited[v] && graph[minIndex][v] && dist[minIndex] != INF &&
                dist[minIndex] + graph[minIndex][v] < dist[v])
            {
                dist[v] = dist[minIndex] + graph[minIndex][v];
            }
        }
    }

    printf("Shortest distances from department %d to all other departments:\n", src + 1);
    for (int i = 0; i < numVertices; i++)
    {
        printf("Department %d: %d\n", i + 1, dist[i]);
    }

    free(visited);
}

// Function to find the nearest department from a central department
int findNearestDepartment(int numVertices, Department *departments, int *dist)
{
    int minDistance = INF;
    int nearestDepartment = -1;

    for (int i = 0; i < numVertices; i++)
    {
        if (departments[i].beds > 0 && departments[i].equipment > 0 && dist[i] < minDistance)
        {
            minDistance = dist[i];
            nearestDepartment = i;
        }
    }

    return nearestDepartment;
}

// Function to allocate resources equally
void allocateEqualResources(int numVertices, Department *departments)
{
    int totalBeds = 0, totalEquipment = 0;
    for (int i = 0; i < numVertices; i++)
    {
        totalBeds += departments[i].beds;
        totalEquipment += departments[i].equipment;
    }

    int averageBeds = totalBeds / numVertices;
    int averageEquipment = totalEquipment / numVertices;

    for (int i = 0; i < numVertices; i++)
    {
        departments[i].beds = averageBeds;
        departments[i].equipment = averageEquipment;
    }

    printf("\nAdjusted resources based on equal allocation:\n");
    for (int i = 0; i < numVertices; i++)
    {
        printf("Department %d - Beds: %d, Equipment: %d\n", i + 1, departments[i].beds, departments[i].equipment);
    }

    printf("\nTotal resources available across all departments:\n");
    printf("Total Beds: %d, Total Equipment: %d\n", totalBeds, totalEquipment);
}

// Function to allocate resources based on the shortest path
void allocateResourcesBasedOnShortestPath(int numVertices, Department *departments, int from, int to, int *dist)
{
    printf("Allocating resources from department %d to department %d\n", from + 1, to + 1);
    if (dist[to] < INF)
    {
        departments[to].beds += departments[from].beds / 2;
        departments[to].equipment += departments[from].equipment / 2;
        departments[from].beds /= 2;
        departments[from].equipment /= 2;

        printf("Updated resources:\n");
        printf("Department %d - Beds: %d, Equipment: %d\n", from + 1, departments[from].beds, departments[from].equipment);
        printf("Department %d - Beds: %d, Equipment: %d\n", to + 1, departments[to].beds, departments[to].equipment);
    }
    else
    {
        printf("No direct path found from department %d to department %d\n", from + 1, to + 1);
    }
}

// Function to display patient allocations
void displayPatientAllocations(Patient *patients, int numPatients)
{
    printf("\nPatient Allocations:\n");
    for (int i = 0; i < numPatients; i++)
    {
        printf("Patient %s is in department %d\n", patients[i].name, patients[i].department);
    }
}

// Function to handle patient departure
void departPatient(Patient *patients, Department *departments, int *numPatients)
{
    char patientName[50];
    printf("Enter the name of the patient to depart: ");
    scanf("%s", patientName);

    bool found = false;
    for (int i = 0; i < *numPatients; i++)
    {
        if (strcmp(patients[i].name, patientName) == 0)
        {
            int dept = patients[i].department - 1;
            departments[dept].beds++;
            departments[dept].equipment++;
            departments[dept].numPatients--;

            // Shift the remaining patients to fill the gap
            for (int j = i; j < *numPatients - 1; j++)
            {
                patients[j] = patients[j + 1];
            }
            (*numPatients)--;
            found = true;
            break;
        }
    }

    if (found)
    {
        printf("Patient %s has been departed and resources have been updated.\n", patientName);
    }
    else
    {
        printf("Patient %s not found.\n", patientName);
    }
}

// Function to display department resources
void showDepartmentResources(Department *departments, int numDepartments)
{
    printf("\nDepartment Resources:\n");
    for (int i = 0; i < numDepartments; i++)
    {
        printf("Department %d - Beds: %d, Equipment: %d, Patients: %d\n", i + 1, departments[i].beds, departments[i].equipment, departments[i].numPatients);
    }
}

// Function to get integer input with validation
int getIntInput(const char *prompt, int min, int max)
{
    int value;
    while (true)
    {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1)
        {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n')
                ; // clear the input buffer
        }
        else if (value < min || value > max)
        {
            printf("Input out of range. Please enter a number between %d and %d.\n", min, max);
        }
        else
        {
            break;
        }
    }
    return value;
}
