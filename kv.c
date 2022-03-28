#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


typedef struct Node{
    int key;
    char* val;
    struct Node *next;
} Node;


void put(Node **curr, int key, char *val)
{
    if (*curr == NULL)
        *curr = malloc(sizeof(Node));
    else
    {
        (*curr)->next = malloc(sizeof(Node));
        *curr = (*curr)->next;
    }

    (*curr)->key = key;
    (*curr)->val = val;
}

Node* get(Node *curr, int key)
{
    while (curr != NULL)
    {
        if (curr->key == key)
        {
            return curr;
        }

        curr = curr->next;
    }

    return NULL;
}

void delete(Node *curr, int key)
{
    while (curr->next != NULL)
    {
        if (curr->next->key == key)
        {
            curr->next = curr->next->next;
            return;
        }

        curr = curr->next;
    }

    printf("%d not found\n", key);
}

void print(Node *curr)
{
    while (curr != NULL)
    {
        printf("%d,%s", curr->key, curr->val);
        curr = curr->next;
    }
    printf("\n");
}

void freeList(Node *curr)
{
    while (curr != NULL)
    {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
}

void loadFile(Node **head, Node **curr)
{
    FILE *data = fopen("database.txt", "r+");
    char *line = NULL;
    size_t len = 1;
    int key;
    char *val;

    while (getline(&line, &len, data) != -1)
    {
        key = atoi(strsep(&line, ","));
        val = strsep(&line, ",");

        put(curr, key, val);

        if (*head == NULL)
            *head = *curr;
    }

    if (line != NULL)
        free(line);
    fclose(data);
}

void storeFile(Node *curr)
{
    FILE *data = fopen("database.txt", "w+");
    char line[100];
    
    while (curr != NULL)
    {
        int key = curr->key;
        char *val = curr->val;

        sprintf(line, "%d", key);
        strcat(line, ",");
        strcat(line, val);
        if (curr->next != NULL && strchr(line, '\n') == NULL)
            strcat(line, "\n");
        fputs(line, data);

        curr = curr->next;
    }

    fclose(data);
}

int main(int argc, char* argv[])
{
    Node *head = NULL;
    Node *curr = NULL;

    loadFile(&head, &curr);

    for (int i = 1; i < argc; i++)
    {
        char *str, *token;
        char *inputs[3] = {NULL, NULL, NULL};

        str = strdup(argv[i]);
        assert(str != NULL);

        int j = 0;
        while ((token = strsep(&str, ",")) != NULL)
        {
            inputs[j] = token;
            j++;
        }
        
        if (strcmp(inputs[0],"p") == 0)
        {
            int key = atoi(inputs[1]);
            char *val = inputs[2];
            Node *temp = get(curr, key);

            if (val == NULL || key == 0)
                    printf("bad command\n");
            else
            {
                if (temp != NULL)
                    temp-> val = val;
                else
                {
                    put(&curr, key, val);
                    if (head == NULL)
                        head = curr;
                }
            }
        }
        else if (strcmp(inputs[0],"g") == 0)
        {
            if (inputs[1] == NULL || inputs[2] != NULL)
                printf("bad command\n");
            else
            {
                int key = atoi(inputs[1]);
                Node *temp = get(head, key);

                if (temp != NULL)
                    printf("%d,%s\n", key, temp->val);
                else
                    printf("%d not found\n", key);
            }
        }
        else if (strcmp(inputs[0],"d") == 0)
        {
            if (inputs[1] == NULL || inputs[2] != NULL)
                printf("bad command\n");
            else
            {
                int key = atoi(inputs[1]);

                if (head->key == key)
                    head = head->next;
                else
                    delete(head, key);
            }
        }
        else if (strcmp(inputs[0],"c") == 0)
            head = NULL;
        else if (strcmp(inputs[0],"a") == 0)
            print(head);
        else
            printf("bad command\n");

        free(str);
    }

    storeFile(head);
    freeList(head);
}