#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_QUEUE_SIZE 1024
#define MAX_CODING_SIZE 16
#define MAX_ENCRYPTE_SIZE 4096

// Linked list node that records characters and frequency of occurrence
struct list_node
{
    char character;
    int frequency;
    char code[10];

    struct list_node *next;
};
typedef struct list_node List_Node;

// Binary tree node
struct tree_node
{
    char character;
    int frequency;
    // min heap tree
    struct tree_node *leftChild;
    struct tree_node *rightChild;
    // Huffman Coding tree
    struct tree_node *leftleaf;
    struct tree_node *rightleaf;
};
typedef struct tree_node Tree_Node;

// Linked list's head 、 Binary tree's root 、 priorityQueue's Array
List_Node *list_head = NULL;
Tree_Node *tree_root = NULL;
Tree_Node *priorityQueue[MAX_QUEUE_SIZE];
int Index, list_count;
char coding[MAX_CODING_SIZE];
char encrypted_text[MAX_ENCRYPTE_SIZE];

// Push a character into the linked list
List_Node *push_list(char key)
{
    // The linked list is empty
    if (list_head == NULL)
    {
        List_Node *new = (List_Node *)malloc(sizeof(List_Node));
        new->character = key;
        new->frequency = 1;
        new->next = NULL;
        list_count++;
        return new;
    }

    List_Node *current = list_head;
    List_Node *previous;

    while (current != NULL)
    {
        // There is a matching character in the linked list
        if (key == current->character)
        {
            current->frequency++;
            return list_head;
        }
        previous = current;
        current = current->next;
    }

    // No matching character in the linked list
    List_Node *new = (List_Node *)malloc(sizeof(List_Node));
    new->character = key;
    new->frequency = 1;
    new->next = NULL;
    previous->next = new;
    list_count++;
    return list_head;
}

// Output the contents of the linked list in order
void print_list(FILE *fd)
{
    List_Node *current = list_head;
    while (current != NULL)
    {
        if (current->character != ' ')
        {
            printf("%c\t%d\t%s\n", current->character, current->frequency,
                   current->code);
            fprintf(fd, "%c\t%d\t%s\n", current->character, current->frequency,
                    current->code);
        }
        else
        {
            printf("space\t%d\t%s\n", current->frequency, current->code);
            fprintf(fd, "space\t%d\t%s\n", current->frequency, current->code);
        }
        current = current->next;
    }
    return;
}

// Encode the string according to Huffman Coding
void encrypt_list(char text[])
{
    Index = 0;
    // Take each character of the string separately
    while (text[Index] != '\n' && text[Index] != '\0')
    {
        List_Node *current = list_head;
        // Find the encoding of the character in the linked list
        while (current != NULL)
        {
            if (current->character == text[Index])
            {
                strcat(encrypted_text, current->code);
                break;
            }
            else
                current = current->next;
        }
        Index++;
    }
    strcat(encrypted_text, "\n");
}

// Release all memory in the linked list
void free_list(void)
{
    while (list_head != NULL)
    {
        List_Node *temp = list_head;
        list_head = list_head->next;
        free(temp);
    }
    return;
}

// Exchange data between two nodes in the tree
void swap_tree_node(Tree_Node *new, Tree_Node *old)
{
    Tree_Node *temp = (Tree_Node *)malloc(sizeof(Tree_Node));
    temp->character = new->character;
    temp->frequency = new->frequency;
    temp->leftleaf = new->leftleaf;
    temp->rightleaf = new->rightleaf;

    new->character = old->character;
    new->frequency = old->frequency;
    new->leftleaf = old->leftleaf;
    new->rightleaf = old->rightleaf;

    old->character = temp->character;
    old->frequency = temp->frequency;
    old->leftleaf = temp->leftleaf;
    old->rightleaf = temp->rightleaf;

    free(temp);
}

// Add a node to the heap
Tree_Node *push_tree(char key, int freq)
{
    if (Index > MAX_QUEUE_SIZE - 1)
        fprintf(stderr, "The heap is full\n");

    // Malloc a new node
    priorityQueue[Index] = (Tree_Node *)malloc(sizeof(Tree_Node));
    priorityQueue[Index]->character = key;
    priorityQueue[Index]->frequency = freq;
    priorityQueue[Index]->leftChild = NULL;
    priorityQueue[Index]->rightChild = NULL;
    priorityQueue[Index]->leftleaf = NULL;
    priorityQueue[Index]->leftleaf = NULL;

    // The heap is empty
    if (Index == 1)
        return priorityQueue[Index];
    // The heap already exists, adding a node
    else
    {
        if (Index % 2 == 0)
            priorityQueue[Index / 2]->leftChild = priorityQueue[Index];
        else
            priorityQueue[Index / 2]->rightChild = priorityQueue[Index];
    }

    // Adjust the location of newly added nodes
    int temp = Index;
    while (temp > 1 &&
           priorityQueue[temp]->frequency < priorityQueue[temp / 2]->frequency)
    {
        swap_tree_node(priorityQueue[temp], priorityQueue[temp / 2]);
        temp = temp / 2;
    }

    return tree_root;
}

// Take a node from the min heap
Tree_Node *pop_tree(void)
{
    // The last node is swapped to the pop out position
    swap_tree_node(priorityQueue[1], priorityQueue[Index]);
    if (Index % 2 == 0)
        priorityQueue[Index / 2]->leftChild = NULL;
    else if (Index != 1)
        priorityQueue[Index / 2]->rightChild = NULL;

    return priorityQueue[Index];
}

// Adjust the position of the first node in the heap
void top_refresh(void)
{
    int i = 1, min = 0;

    while (2 * i <= Index)
    {
        if (priorityQueue[2 * i] == NULL)
            break;
        else if (priorityQueue[2 * i + 1] == NULL)
            min = priorityQueue[2 * i]->frequency;
        else if (priorityQueue[2 * i]->frequency >
                 priorityQueue[2 * i + 1]->frequency)
            min = priorityQueue[2 * i + 1]->frequency;
        else
            min = priorityQueue[2 * i]->frequency;

        // If the value of this node is larger than the value of the child node,
        // adjust it
        if (priorityQueue[i]->frequency > min)
        {
            if (priorityQueue[2 * i]->frequency == min)
            {
                swap_tree_node(priorityQueue[i], priorityQueue[2 * i]);
                i = 2 * i;
            }
            else
            {
                swap_tree_node(priorityQueue[i], priorityQueue[2 * i + 1]);
                i = 2 * i + 1;
            }
        }
        else
            break;
    }
}

// Adjust the position of the last node in the heap
void bottom_refresh(void)
{
    if (Index == 1)
        return;

    // If the value of this node is small than the value of the parent node,
    // adjust it
    int temp = Index;
    while (temp > 1 &&
           priorityQueue[temp]->frequency < priorityQueue[temp / 2]->frequency)
    {
        swap_tree_node(priorityQueue[temp], priorityQueue[temp / 2]);
        temp = temp / 2;
    }

    return;
}

// Traverse all leaf nodes (inorder)
void traversal(Tree_Node *root)
{
    // Coding backwards
    if (root == NULL)
    {
        coding[strlen(coding) - 1] = '\0';
        return;
    }

    strcat(coding, "0");
    traversal(root->leftleaf);

    // Arriving at the leaf node
    if (root->character != '\0')
    {
        // Find the location of this leaf node in the linked list, modify its code
        List_Node *current = list_head;
        while (current != NULL)
        {
            if (current->character == root->character)
            {
                strcat(current->code, coding);
                break;
            }
            else
                current = current->next;
        }
    }

    strcat(coding, "1");
    traversal(root->rightleaf);

    // Coding backwards
    coding[strlen(coding) - 1] = '\0';
    return;
}

// Release all memory in the huffman tree
void free_tree(Tree_Node *root)
{
    if (root->leftleaf != NULL)
        free_tree(root->leftleaf);

    if (root->rightleaf != NULL)
        free_tree(root->rightleaf);

    free(root);
}

// Create a Huffman tree using the heap tree
void Huffman_tree(Tree_Node *root)
{
    while (Index > 1)
    {
        // Pop the node with the lowest priority value
        Tree_Node *new_left = pop_tree();
        priorityQueue[Index] = NULL;
        Index--;
        top_refresh();

        // Pop the node with the lowest priority value
        Tree_Node *new_right = pop_tree();
        priorityQueue[Index] = NULL;
        Index--;
        top_refresh();

        // Combine two nodes to form a new node
        Tree_Node *new = (Tree_Node *)malloc(sizeof(Tree_Node));
        new->leftleaf = new_left;
        new->rightleaf = new_right;
        new->character = '\0';
        new->frequency = new_left->frequency + new_right->frequency;
        new->leftChild = NULL;
        new->rightChild = NULL;

        // Push it in to the end of the heap, then make adjustments
        Index++;
        priorityQueue[Index] = new;
        if (Index % 2 == 0)
            priorityQueue[Index / 2]->leftChild = new;
        else if (Index != 1)
            priorityQueue[Index / 2]->rightChild = new;

        bottom_refresh();
    }

    tree_root = priorityQueue[1];
}

int main()
{
    char text[1024];

    FILE *fp, *fd;
    fp = fopen("text.txt", "r+");
    fd = fopen("code.txt", "w+");
    list_count = 0;

    // Read the string from the file and push it in to the linked list
    while (fgets(text, 1024, fp) != NULL)
    {
        Index = 0;
        while (text[Index] != '\n' && text[Index] != '\0')
        {
            list_head = push_list(text[Index]);
            Index++;
        }
    }

    // Push the data in the linked list into the heap tree.
    List_Node *current = list_head;
    Index = 0;
    while (current != NULL)
    {
        Index++;
        tree_root = push_tree(current->character, current->frequency);
        current = current->next;
    }

    // Making Huffman tree , and find the encoding for each character
    Huffman_tree(tree_root);
    coding[0] = '\0';
    traversal(tree_root);

    // Output character type and encoding result
    printf("%d\n", list_count);
    print_list(fd);

    // Encrypt the string in the file with the result of the encoding
    fseek(fp, 0, SEEK_SET);
    while (fgets(text, 1024, fp) != NULL)
    {
        encrypted_text[0] = '\0';
        encrypt_list(text);

        printf("%d\n", strlen(encrypted_text) - 1);
        fprintf(fd, "%d\n", strlen(encrypted_text) - 1);

        printf("%s", encrypted_text);
        fprintf(fd, "%s", encrypted_text);
    }

    free_list();
    free_tree(tree_root);
    fclose(fp);
    fclose(fd);
}