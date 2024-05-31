import tkinter as tk
from tkinter import messagebox, simpledialog


class Department:
    def __init__(self, name, beds, equipment):
        self.name = name
        self.beds = beds
        self.equipment = equipment


class Edge:
    def __init__(self, department1, department2, distance):
        self.department1 = department1
        self.department2 = department2
        self.distance = distance


class UnionFind:
    def __init__(self, n):
        self.parent = list(range(n))
        self.rank = [0] * n

    def find(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def union(self, x, y):
        root_x = self.find(x)
        root_y = self.find(y)
        if root_x != root_y:
            if self.rank[root_x] < self.rank[root_y]:
                self.parent[root_x] = root_y
            elif self.rank[root_x] > self.rank[root_y]:
                self.parent[root_y] = root_x
            else:
                self.parent[root_y] = root_x
                self.rank[root_x] += 1


class HospitalResourceAllocator:
    def __init__(self):
        self.departments = []
        self.edges = []

    def add_department(self, name, beds, equipment):
        department = Department(name, beds, equipment)
        self.departments.append(department)

    def add_edge(self, department1, department2, distance):
        edge = Edge(department1, department2, distance)
        self.edges.append(edge)

    def kruskal_algorithm(self):
        print("Running Kruskal's algorithm...")
        mst_edges = []  # List to store the minimum spanning tree edges

        sorted_edges = sorted(self.edges, key=lambda x: x.distance)

        parent = {}
        rank = {}

        # Helper function to find the parent of a node
        def find(node):
            if parent[node] != node:
                parent[node] = find(parent[node])
            return parent[node]

        # Helper function to perform union of two sets
        def union(node1, node2):
            root1 = find(node1)
            root2 = find(node2)

            # Union by rank
            if rank[root1] < rank[root2]:
                parent[root1] = root2
            elif rank[root1] > rank[root2]:
                parent[root2] = root1
            else:
                parent[root2] = root1
                rank[root1] += 1

        # Initialize parent and rank for each node
        for department in self.departments:
            parent[department] = department
            rank[department] = 0

        total_beds = 0
        total_equipment = 0

        for edge in sorted_edges:
            # Ensure department objects are used for comparison
            department1 = next(
                department
                for department in self.departments
                if department.name == edge.department1
            )
            department2 = next(
                department
                for department in self.departments
                if department.name == edge.department2
            )

            if find(department1) != find(department2):
                mst_edges.append(edge)
                union(department1, department2)
                total_beds += department1.beds + department2.beds
                total_equipment += department1.equipment + department2.equipment

        return mst_edges, total_beds, total_equipment

    def allocate_resources(self):
        mst, total_beds, total_equipment = self.kruskal_algorithm()
        allocated_departments = (
            set()
        )  # Track already allocated departments to avoid duplication
        result = []  # List to store resource allocation details

        # Iterate over the minimum spanning tree edges
        for edge in mst:
            # Ensure department objects are used for comparison
            department1 = next(
                department
                for department in self.departments
                if department.name == edge.department1
            )
            department2 = next(
                department
                for department in self.departments
                if department.name == edge.department2
            )

            # Allocate resources for department1
            if department1 not in allocated_departments:
                result.append(
                    f"Allocating {department1.beds} beds and {department1.equipment} equipment to {department1.name}"
                )
                allocated_departments.add(department1)

            # Allocate resources for department2
            if department2 not in allocated_departments:
                result.append(
                    f"Allocating {department2.beds} beds and {department2.equipment} equipment to {department2.name}"
                )
                allocated_departments.add(department2)

        # Append total resource allocation summary
        result.append(f"Total Beds Allocated: {total_beds}")
        result.append(f"Total Equipment Allocated: {total_equipment}")

        return (
            result,
            total_beds,
            total_equipment,
        )  # Ensure exactly three values are returned


# Tkinter Application
class HospitalResourceApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Hospital Resource Allocator")
        self.hospital = HospitalResourceAllocator()

        # Create buttons and labels
        self.add_department_button = tk.Button(
            root, text="Add Department", command=self.add_department
        )
        self.add_edge_button = tk.Button(root, text="Add Edge", command=self.add_edge)
        self.allocate_resources_button = tk.Button(
            root, text="Allocate Resources", command=self.allocate_resources
        )
        self.quit_button = tk.Button(root, text="Quit", command=root.destroy)

        self.departments_label = tk.Label(root, text="Departments:")
        self.departments_listbox = tk.Listbox(root)
        self.edges_label = tk.Label(root, text="Edges:")
        self.edges_listbox = tk.Listbox(root)

        # Place widgets on the grid
        self.add_department_button.grid(row=0, column=0)
        self.add_edge_button.grid(row=0, column=1)
        self.allocate_resources_button.grid(row=0, column=2)
        self.quit_button.grid(row=0, column=3)
        self.departments_label.grid(row=1, column=0)
        self.departments_listbox.grid(row=2, column=0, rowspan=4)
        self.edges_label.grid(row=1, column=1)
        self.edges_listbox.grid(row=2, column=1, rowspan=4)

    def add_department(self):
        name = simpledialog.askstring("Add Department", "Enter department name:")
        beds = int(simpledialog.askstring("Add Department", "Enter number of beds:"))
        equipment = int(
            simpledialog.askstring("Add Department", "Enter amount of equipment:")
        )
        if name:
            self.hospital.add_department(name, beds, equipment)
            self.departments_listbox.insert(tk.END, name)

    def add_edge(self):
        department1 = simpledialog.askstring("Add Edge", "Enter department 1 name:")
        department2 = simpledialog.askstring("Add Edge", "Enter department 2 name:")
        distance = int(simpledialog.askstring("Add Edge", "Enter distance:"))
        if department1 and department2:
            self.hospital.add_edge(department1, department2, distance)
            self.edges_listbox.insert(
                tk.END, f"{department1} <-> {department2} (Distance: {distance})"
            )

    def allocate_resources(self):
        try:
            print("Allocating resources...")
            mst, total_beds, total_equipment = self.hospital.allocate_resources()
            print("MST:", mst)
            print("Total Beds:", total_beds)
            print("Total Equipment:", total_equipment)
            result = "\n".join(
                [
                    f"Allocating {department.beds} beds and {department.equipment} equipment to {department.name}"
                    for department in mst
                ]
            )
            result += f"\nTotal Beds Allocated: {total_beds}\nTotal Equipment Allocated: {total_equipment}"
            messagebox.showinfo("Resource Allocation", result)
        except IndexError:
            messagebox.showwarning(
                "Error",
                "Add at least one department and edge before allocating resources.",
            )


if __name__ == "__main__":
    root = tk.Tk()
    app = HospitalResourceApp(root)
    root.mainloop()
