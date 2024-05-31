
# Hospital-Resource-Manager-Using-C

Hospital Resource Manager is a C-based project designed to manage the resources of a hospital, including the allocation of beds, equipment, and patient management. The project offers features such as allocating resources equally among departments, allocating resources based on Dijkstra's shortest path algorithm, managing patient entries and departures, displaying patient allocations, and showing current resources and patients in each department. This project was created as part of an Advanced Data Structures course.

This project is intended to provide a comprehensive solution for managing hospital resources efficiently. By employing algorithms like Dijkstra's shortest path algorithm and offering features for resource allocation and patient management, it aims to streamline hospital operations and improve patient care. With its user-friendly interface and robust functionality, Hospital Resource Manager is designed to be a valuable tool for hospital administrators and staff.

## Code Overview 

When you run the program, you will be prompted to enter the number of departments and their resources, followed by the edges representing the distances between the departments. You will then have the following options:

    1. Allocate resources equally among all departments.

    2. Allocate resources based on Dijkstra's shortest path algorithm from a source to a destination department.

    3. Enter a new patient and allocate them to the nearest department with available resources.

    4. Display the current patient allocations.

    5. Depart a patient and update the department resources.

    6. Show the current resources of each department and the total resources and patients in each department.

    7. Exit the program.
## Functions Explanation
### main Function
The main function serves as the entry point of the program. It initializes the program, sets up the departments, adjacency matrix, and handles the user interface. It provides options for resource allocation and patient management.

### addEdge Function
The addEdge function adds edges between departments in the adjacency matrix, facilitating the representation of connections between different departments.

### dijkstra Function
The dijkstra function implements Dijkstra's algorithm to find the shortest paths from a source department to all other departments. It calculates the shortest distances and displays them for each department.

### findNearestDepartment Function
The findNearestDepartment function identifies the nearest department from a central department with available resources. It helps in allocating patients to the most suitable department.

### allocateEqualResources Function
The allocateEqualResources function allocates resources equally among all departments. It calculates the average resources and assigns them to each department.

### allocateResourcesBasedOnShortestPath Function
The allocateResourcesBasedOnShortestPath function allocates resources based on the shortest path from one department to another using Dijkstra's algorithm. It redistributes resources between departments as needed.

### displayPatientAllocations Function
The displayPatientAllocations function displays the current patient allocations, showing which patient is assigned to which department.

### departPatient Function
The departPatient function handles the departure of a patient from a department. It updates the resources of the department accordingly.

### showDepartmentResources Function
The showDepartmentResources function displays the current resources and patients in each department, providing an overview of the hospital's status.

### getIntInput Function
The getIntInput function gets integer input from the user with validation, ensuring that only valid input is accepted throughout the program.