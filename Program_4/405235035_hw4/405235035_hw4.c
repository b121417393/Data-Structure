#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_VERTEX_SIZE 64
#define MAX_EDGE_SIZE 256

// The child node of adjacency list
struct tail_node
{
    int vertex;
    int weight;
    struct tail_node *next;
};
typedef struct tail_node Tail_Node;

// The first node of the adjacency list.
struct head_node
{
    int count;
    struct tail_node *next;
};
typedef struct head_node Head_Node;

int edge_count = 0;                      // the number of edges
int max_vertex = 0;                      // the number of vertices
Head_Node forward_list[MAX_VERTEX_SIZE]; // Forward adjacency list
Head_Node reverse_list[MAX_VERTEX_SIZE]; // Reverse adjacency list

int ee[MAX_VERTEX_SIZE]; // Early Event Time
int le[MAX_VERTEX_SIZE]; // Late Event Time

int e[MAX_EDGE_SIZE];    // When can edge start
int l[MAX_EDGE_SIZE];    // When does edge start at the latest
int diff[MAX_EDGE_SIZE]; // l[i] - e[i]

int stack[MAX_VERTEX_SIZE];
int stack_count = 0; // Number of data in the stack

// Take three numbers out of the string , Input is a string ,and three variables
// to store three numbers.
void get_number(char text[32], int *start, int *dest, int *weight)
{
    int index = 0, i = 0;
    char *ptr, *token = " ";

    ptr = strtok(text, token);
    *start = atoi(ptr);
    if (*start > max_vertex)
        max_vertex = *start;

    ptr = strtok(NULL, token);
    *dest = atoi(ptr);
    if (*dest > max_vertex)
        max_vertex = *dest;

    ptr = strtok(NULL, token);
    *weight = atoi(ptr);

    return;
}

// Initialize the contents of the adjacency list
void init_list(void)
{
    int i = 0;
    for (i = 0; i < MAX_VERTEX_SIZE; i++)
    {
        forward_list[i].count = -1;
        forward_list[i].next = NULL;
        reverse_list[i].count = -1;
        reverse_list[i].next = NULL;
    }
    return;
}

// Insert an edge into two adjacency lists
void insert(int start, int dest, int during)
{
    // Insert into the forward adjacency list
    Tail_Node *forward_new = (Tail_Node *)malloc(sizeof(Tail_Node));
    forward_new->vertex = dest;
    forward_new->weight = during;
    forward_new->next = NULL;

    Tail_Node *temp;

    // If this vertex is not yet enabled
    if (forward_list[start].count == -1)
        forward_list[start].count = 0;

    // If this vertex is not yet enabled
    if (forward_list[dest].count == -1)
        forward_list[dest].count = 1;
    // This vertex is already enabled
    else
        forward_list[dest].count++;

    // Insert into the appropriate location in the adjacency list.
    if (forward_list[start].next == NULL)
        forward_list[start].next = forward_new;
    else
    {
        temp = forward_list[start].next;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = forward_new;
    }

    // Insert into the reverse adjacency list
    Tail_Node *reverse_new = (Tail_Node *)malloc(sizeof(Tail_Node));
    reverse_new->vertex = start;
    reverse_new->weight = during;
    reverse_new->next = NULL;

    // If this vertex is not yet enabled
    if (reverse_list[dest].count == -1)
        reverse_list[dest].count = 0;

    // If this vertex is not yet enabled
    if (reverse_list[start].count == -1)
        reverse_list[start].count = 1;
    // This vertex is already enabled
    else
        reverse_list[start].count++;

    // Insert into the appropriate location in the adjacency list.
    if (reverse_list[dest].next == NULL)
        reverse_list[dest].next = reverse_new;
    else
    {
        temp = reverse_list[dest].next;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = reverse_new;
    }

    return;
}

// Release all memory in the adjacency lists
void free_list(void)
{
    int i = 0;
    Tail_Node *current, *temp;

    for (i = 0; i <= max_vertex; i++)
    {
        current = forward_list[i].next;
        while (current != NULL)
        {
            temp = current;
            current = current->next;
            free(temp);
        }

        current = reverse_list[i].next;
        while (current != NULL)
        {
            temp = current;
            current = current->next;
            free(temp);
        }
    }

    return;
}

// Push a vertex into the stack
void push(int vertex)
{
    stack_count++;
    stack[stack_count] = vertex;

    return;
}

// Pop a vertex from the stack
int pop(void)
{
    int temp = stack[stack_count];
    stack_count--;

    return temp;
}

// Create a table of Early Event Time
int build_ee(void)
{
    int i = 0, head = 0, tail = 0, temp = 0, complete = 0;
    Tail_Node *current;

    // Initialize the table
    for (i = 0; i <= max_vertex; i++)
        ee[i] = 0;

    // Push the first vertex into the stack , and check for errors
    if (forward_list[0].count == 0)
        push(0);
    else
        return -1;

    // Loop until the stack becomes empty
    while (stack_count)
    {
        head = pop();
        current = forward_list[head].next;

        // Handle the edge pointed out by this vertex
        while (current != NULL)
        {
            tail = current->vertex;
            forward_list[tail].count--;
            // If there is no edge pointing to this vertex, then push it
            if (forward_list[tail].count == 0)
            {
                push(tail);
                complete++;
            }

            // Update if the new value is larger than the old value
            temp = current->weight;
            if ((ee[head] + temp) > ee[tail])
                ee[tail] = ee[head] + temp;
            current = current->next;
        }
    }

    // Check if all vertex has been processed
    if (complete != max_vertex)
        return -1;
    else
        return 0;
}

// Create a table of Late Event Time
int build_le(void)
{
    int i = 0, head = 0, tail = 0, temp = 0, complete = 0;
    Tail_Node *current;

    // Initialize the table
    for (i = 0; i <= max_vertex; i++)
        le[i] = ee[max_vertex];

    // Push the last vertex into the stack , and check for errors
    if (reverse_list[max_vertex].count == 0)
        push(max_vertex);
    else
        return -1;

    // Loop until the stack becomes empty
    while (stack_count)
    {
        head = pop();
        current = reverse_list[head].next;

        // Handle the edge pointing to this vertex
        while (current != NULL)
        {
            tail = current->vertex;
            reverse_list[tail].count--;
            // If there is no edge pointed out by this vertex, then push it
            if (reverse_list[tail].count == 0)
            {
                push(tail);
                complete++;
            }

            // Update if the new value is less than the old value
            temp = current->weight;
            if ((le[head] - temp) < le[tail])
                le[tail] = le[head] - temp;
            current = current->next;
        }
    }

    // Check if all vertex has been processed
    if (complete != max_vertex)
        return -1;
    else
        return 0;
}

int main()
{
    char text[32];
    int i = 0;
    int l_vertex[MAX_EDGE_SIZE]; // Store the end of this edge

    FILE *fp, *fd;
    fp = fopen("input.txt", "r+");
    fd = fopen("ouput.txt", "w+");

    init_list();
    fgets(text, 32, fp);
    edge_count = atoi(text);

    for (i = 1; i <= edge_count; i++)
    {
        int start_node, dest_node, weight;

        fgets(text, 32, fp);
        get_number(text, &start_node, &dest_node, &weight);
        insert(start_node, dest_node, weight);

        e[i] = start_node;       // Store the start of this edge
        l_vertex[i] = dest_node; // Store the end of this edge
        l[i] = weight;           // Store the weight of this edge
    }

    // Check if there are points that cannot be reached
    for (i = 1; i <= max_vertex; i++)
        if (forward_list[i].count == 0)
        {
            printf("No Solution!\n");
            fprintf(fd, "No Solution!\n");
            return 0;
        }

    i = build_ee();
    // If there is a vertex not processed
    if (i == -1)
    {
        printf("No Solution!\n");
        fprintf(fd, "No Solution!\n");
        return 0;
    }

    i = build_le();
    // If there is a vertex not processed
    if (i == -1)
    {
        printf("No Solution!\n");
        fprintf(fd, "No Solution!\n");
        return 0;
    }

    printf("Activity\tEarly\tLate\tSlack\tCritical\n");
    fprintf(fd, "Activity\tEarly\tLate\tSlack\tCritical\n");
    printf("\t\te\tl\tl-e\tl - e = 0\n");
    fprintf(fd, "\t\te\tl\tl-e\tl - e = 0\n");
    for (i = 1; i <= edge_count; i++)
    {
        e[i] = ee[e[i]]; // Edge can be executed when it's start vertex is ready
        l[i] = le[l_vertex[i]] - l[i]; // edge的終點最晚要執行的時間-edge的執行時間=該edge最晚必須執行的時間
        if (l[i] - e[i] == 0)
        {
            printf("a%d\t\t%d\t%d\t%d\tYes\n", i, e[i], l[i], l[i] - e[i]);
            fprintf(fd, "a%d\t\t%d\t%d\t%d\tYes\n", i, e[i], l[i], l[i] - e[i]);
        }
        else
        {
            printf("a%d\t\t%d\t%d\t%d\tNo\n", i, e[i], l[i], l[i] - e[i]);
            fprintf(fd, "a%d\t\t%d\t%d\t%d\tNo\n", i, e[i], l[i], l[i] - e[i]);
        }
    }

    fclose(fp);
    fclose(fd);
    free_list();

    return 0;
}