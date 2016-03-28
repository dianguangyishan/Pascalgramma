#include <stdio.h>
#include <stdlib.h>
#define CHAR_COUNT 62
#define MAX_CODE_LEN 256
#define MAX_NODE_COUNT 10000
typedef enum {FALSE,TRUE} BOOLEAN;
typedef struct trie_node_t
{
    struct trie_node_t* next[CHAR_COUNT];
    BOOLEAN is_tail;
}trie_node_t;
typedef struct trie_tree_t
{
    trie_node_t *root;
    int size;
    int capacity;
    trie_node_t nodes[MAX_NODE_COUNT];
}trie_tree_t;
trie_tree_t* trie_tree_create(void)
{
    trie_tree_t *tree = (trie_tree_t*)malloc(sizeof(trie_tree_t));
    tree->root=&(tree->nodes[0]);
    tree->capacity = 0;
    memset(tree->nodes,0,sizeof(tree->nodes));
    tree->size = 1;
    return tree;
}
void trie_tree_destroy(trie_tree_t *tree)
{
    free(tree);
    tree = NULL;
}
void trie_tree_clear(trie_tree_t *tree)
{
    memset(tree->nodes, 0, sizeof(tree->nodes));
    tree->size = 1;
    tree->capacity = 0;
}
BOOLEAN trie_tree_insert(trie_tree_t *tree, char *word)
{
    BOOLEAN ok=TRUE;
    int curword = 0;
    trie_node_t *p = tree->root;
    while (*word)
    {
        curword = *word;
        if(curword <= '9')
            curword = curword - '0';
        else if(curword <= 'Z')
            curword = curword - 'A' + 10;
        else
            curword = curword - 'a' + 36;
        if (p->next[curword] == NULL)
            p->next[curword] = &(tree->nodes[tree->size++]);
        p = p->next[curword];
        if (p->is_tail) ok=FALSE;
        word++;
    }
    if (!(p->is_tail))
        {p->is_tail = TRUE;
        tree->capacity++;}
    return ok;
}
BOOLEAN trie_tree_query(trie_tree_t *tree, char *word)
{
    int curword = 0;
    trie_node_t *p = tree->root;
    while (*word)
    {
        curword = *word;
        if(curword <= '9')
            curword = curword - '0';
        else if(curword <= 'Z')
            curword = curword - 'A' + 10;
        else
            curword = curword - 'a' + 36;
        if(p->next[curword] == NULL)
            return FALSE;
        p = p->next[curword];
        word++;
    }
    if(p->is_tail)
        return TRUE;
    return FALSE;
}
