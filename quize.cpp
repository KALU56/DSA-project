#include <iostream>
#include <string>
#include <ctime>
using namespace std;

// I. Define a structure for task details
struct Task {
    int taskID;
    string developerName;
    string taskDescription;
    int priority; // Higher number = higher priority
    string status; // Pending, In_Progress, Completed
    string submissionDate; // Format: YYYY-MM-DD
    Task* next; // For linked list
};

// Node for the priority queue (linked list-based)
struct QueueNode {
    Task* task;
    QueueNode* next;
};

class TaskManagementSystem {
private:
    Task* head; // Head of the linked list
    QueueNode* front; // Front of the priority queue
    QueueNode* rear; // Rear of the priority queue
    int taskCount; // To track number of tasks

public:
    TaskManagementSystem() {
        head = nullptr;
        front = nullptr;
        rear = nullptr;
        taskCount = 0;
    }

    // Helper function to get current date as string
    string getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        string date = to_string(1900 + ltm->tm_year) + "-" + 
                      to_string(1 + ltm->tm_mon) + "-" + 
                      to_string(ltm->tm_mday);
        return date;
    }

    // II. Enqueue a task based on priority
    void enqueue(int taskID, string devName, string desc, int priority, string status) {
        // Create a new task
        Task* newTask = new Task;
        newTask->taskID = taskID;
        newTask->developerName = devName;
        newTask->taskDescription = desc;
        newTask->priority = priority;
        newTask->status = status;
        newTask->submissionDate = getCurrentDate();
        newTask->next = nullptr;

        // Add to linked list (for storage)
        if (!head) {
            head = newTask;
        } else {
            Task* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newTask;
        }

        // Create a queue node
        QueueNode* newNode = new QueueNode;
        newNode->task = newTask;
        newNode->next = nullptr;

        // Add to priority queue
        if (!front) {
            front = rear = newNode;
        } else if (front->task->priority < priority) {
            // Add to front if highest priority
            newNode->next = front;
            front = newNode;
        } else {
            QueueNode* current = front;
            while (current->next && current->next->task->priority >= priority) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
            if (!newNode->next) rear = newNode;
        }
        taskCount++;
    }

    // III. Dequeue a task (high-priority first)
    Task* dequeue() {
        if (!front) {
            cout << "Queue is empty!" << endl;
            return nullptr;
        }

        QueueNode* temp = front;
        Task* task = temp->task;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp; // Free queue node
        taskCount--;
        return task;
    }

    // Helper function to convert linked list to array for sorting/searching
    Task** toArray(int &size) {
        size = taskCount;
        Task** arr = new Task*[size];
        Task* current = head;
        int i = 0;
        while (current) {
            arr[i++] = current;
            current = current->next;
        }
        return arr;
    }

    // IV. Binary search by Task ID (requires sorting by taskID first)
    Task* binarySearch(int taskID) {
        int size;
        Task** arr = toArray(size);

        // Bubble sort by taskID
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j]->taskID > arr[j + 1]->taskID) {
                    Task* temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }

        // Binary search
        int left = 0, right = size - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid]->taskID == taskID) {
                Task* result = arr[mid];
                delete[] arr;
                return result;
            }
            if (arr[mid]->taskID < taskID) left = mid + 1;
            else right = mid - 1;
        }
        delete[] arr;
        return nullptr;
    }

    // V. Bubble sort by priority or submission date
    void bubbleSort(string sortBy) {
        int size;
        Task** arr = toArray(size);

        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                bool swap = false;
                if (sortBy == "priority") {
                    if (arr[j]->priority < arr[j + 1]->priority) swap = true;
                } else if (sortBy == "submissionDate") {
                    if (arr[j]->submissionDate > arr[j + 1]->submissionDate) swap = true;
                }
                if (swap) {
                    Task* temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }

        // Rebuild linked list
        head = nullptr;
        for (int i = 0; i < size; i++) {
            arr[i]->next = nullptr;
            if (!head) {
                head = arr[i];
            } else {
                Task* temp = head;
                while (temp->next) temp = temp->next;
                temp->next = arr[i];
            }
        }
        delete[] arr;
    }

    // VI. Count tasks based on submission date threshold
    int countTasksByThreshold(string thresholdDate) {
        int count = 0;
        Task* current = head;
        while (current) {
            if (current->submissionDate <= thresholdDate) {
                count++;
            }
            current = current->next;
        }
        return count;
    }

    // VII. Display all tasks in the queue
    void displayTasks() {
        QueueNode* current = front;
        if (!current) {
            cout << "Queue is empty!" << endl;
            return;
        }
        cout << "Tasks in Queue:" << endl;
        while (current) {
            Task* task = current->task;
            cout << "Task ID: " << task->taskID << ", Developer: " << task->developerName
                 << ", Description: " << task->taskDescription << ", Priority: " << task->priority
                 << ", Status: " << task->status << ", Submission Date: " << task->submissionDate << endl;
            current = current->next;
        }
    }

    // Destructor to free memory
    ~TaskManagementSystem() {
        // Free linked list
        Task* currentTask = head;
        while (currentTask) {
            Task* temp = currentTask;
            currentTask = currentTask->next;
            delete temp;
        }

        // Free queue
        QueueNode* currentNode = front;
        while (currentNode) {
            QueueNode* temp = currentNode;
            currentNode = currentNode->next;
            delete temp; // Task pointers already freed via linked list
        }
    }
};

// Main function to test the system
int main() {
    TaskManagementSystem tms;

    // Adding some tasks
    tms.enqueue(101, "Alice", "Fix login bug", 3, "Pending");
    tms.enqueue(102, "Bob", "Update UI", 1, "In_Progress");
    tms.enqueue(103, "Charlie", "Database migration", 5, "Pending");

    // Display all tasks
    cout << "Initial queue:" << endl;
    tms.displayTasks();

    // Dequeue a task
    cout << "\nDequeuing a task:" << endl;
    Task* dequeued = tms.dequeue();
    if (dequeued) {
        cout << "Dequeued Task ID: " << dequeued->taskID << ", Priority: " << dequeued->priority << endl;
    }
    tms.displayTasks();

    // Binary search for a task
    cout << "\nSearching for Task ID 102:" << endl;
    Task* found = tms.binarySearch(102);
    if (found) {
        cout << "Found Task ID: " << found->taskID << ", Developer: " << found->developerName << endl;
    } else {
        cout << "Task not found!" << endl;
    }

    // Sort by priority
    cout << "\nSorting by priority:" << endl;
    tms.bubbleSort("priority");
    tms.displayTasks();

    // Count tasks by submission date threshold
    cout << "\nCounting tasks with submission date <= 2025-05-27:" << endl;
    int count = tms.countTasksByThreshold("2025-05-27");
    cout << "Tasks count: " << count << endl;

    return 0;
}