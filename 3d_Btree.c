#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_KEYS 5

typedef struct node
{
    int keys[MAX_KEYS];
    struct node *children[MAX_KEYS + 1];
    int num_keys;
    bool is_leaf;
} Node,*ptr;

typedef struct btree
{
    Node *root;
    int degree;
} BTree;

Node *new_node(bool is_leaf)
{
	int i;
    Node *node = (Node *)malloc(sizeof(Node));
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    for (i = 0; i < MAX_KEYS + 1; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

BTree *new_btree(int t)
{
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    tree->root = new_node(true);
    tree->degree = t;
    return tree;
}

Node *search(BTree *tree, int key)
{
    Node *node = tree->root;
    while (node != NULL)
    {
        int i = 0;
        while (i < node->num_keys && key > node->keys[i])
        {
            i++;
        }
        if (i < node->num_keys && key == node->keys[i])
        {
            return node;
        }
        if (node->is_leaf)
        {
            return NULL;
        }
        node = node->children[i];
    }
    return NULL;
}

void split_child(Node *parent, int index)
{
    int i;
    Node *child = parent->children[index];
    Node *new_child = new_node(child->is_leaf);
    new_child->num_keys = tree->degree - 1;
    for (i = 0; i < tree->degree - 1; i++)
    {
        new_child->keys[i] = child->keys[i + tree->degree];
    }
    if (!child->is_leaf)
    {
        for (i = 0; i < *tree->degree; i++)
        {
            new_child->children[i] = child->children[i + tree->degree];
        }
    }
    child->num_keys = tree->degree - 1;
    for (i = parent->num_keys; i >= index + 1; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = new_child;
    for (i = parent->num_keys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }
    parent->keys[index] = child->keys[tree->degree - 1];
    parent->num_keys++;
}

void insert(BTree *tree, int key)
{
    Node *root = tree->root;
    if (root->num_keys == 2 * tree->degree - 1)
    {
        Node *new_root = new_node(false);
        new_root->children[0] = root;
        tree->root = new_root;
        split_child(new_root, 0);
        insert_non_full(new_root, key);
    }
    else
    {
        insert_non_full(root, key);
    }
}

void insert_non_full(Node *node, int key)
{
    int i = node->num_keys - 1;
    if (node->is_leaf)
    {
        while (i >= 0 && key < node->keys[i])
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    }
    else
    {
        while (i >= 0 && key < node->keys[i])
        {
            i--;
        }
        i++;
        if (node->children[i]->num_keys == 2 * tree->degree - 1)
        {
            split_child(node, i);
            if (key > node->keys[i])
            {
                i++;
            }
        }
        insert_non_full(node->children[i], key);
    }
}

void display_in_order(Node *node)
{ int i;
    if (node != NULL)
    {
        for (i = 0; i < node->num_keys; i++)
        {
            if (!node->is_leaf)
            {
                display_in_order(node->children[i]);
            }
            printf("%d ", node->keys[i]);
        }
        if (!node->is_leaf)
        {
            display_in_order(node->children[node->num_keys]);
        }
    }
}

int main()
{
	int i;
    BTree *tree = new_btree(3);
    int keys[] = {21, 30, 56, 17, 19, 48, 29, 24};
    int num_keys = sizeof(keys) / sizeof(keys[0]);
    for (i = 0; i < num_keys; i++)
    {
        insert(tree, keys[i]);
        display_in_order(tree->root);
        printf("\n");
    }

    int search_key = 56;
    Node *node = search(tree, search_key);
    if (node != NULL)
    {
        printf("Key %d found in the B-tree :-\n", search_key);
    }
    else
    {
        printf("Key %d not found in the B-tree :-\n", search_key);
    }

    return 0;
}
