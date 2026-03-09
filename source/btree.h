#ifndef BTREE_H
#define BTREE_H

// maximum height  of the tree the lower the lower the lower amount
// of disk reads which translates into faster?
#define B_TREE_ORDER 4

typedef struct b_tree_node b_tree_node;
struct  b_tree_node
{

    // store the values
    string8 keys[B_TREE_ORDER - 1];
    csv_row *rows[B_TREE_ORDER - 1];

    b_tree_node *parent;
    // handle to store children faster than linked list
    // because now we can iteratate over the nodes instead of having cache misses
    // when jumping through linked nodes
    b_tree_node *children[B_TREE_ORDER];

    // NOTE(nasr): reference count ::: check how many leaves are using this node
    // also not needed for now because we don't free individual node because of arena allocator
    // s32 *refc;

    s32 key_count;

    b32 leaf;

};

typedef struct b_tree b_tree;
struct b_tree
{
    // NOTE(nasr): make sure this always stays in memory
    // so that initial fetch never requires a disk read
    b_tree_node *root;

};




// TODO(nasr): 1. splitting the tree when getting too big? (horizontally) 2. joining trees?

internal b_tree_node *
node_alloc(mem_arena *arena)
{
    b_tree_node *node = PushStructZero(arena, type);
    node->leaf = 1;
    return node;
}

// NOTE(nasr): @return the index of of the found element
internal s32
node_find_pos(mem_arena *arena, string8 value)
{
    s32 i = 0;
    while (i < n->key_count && str8_cmp(n->keys[i], k) < 0)
    {
        ++i;
    }

    return i;
}

interal void
b_tree_create(mem_arena *arena, b_tree *tree)
{
    tree->root = node_alloc(arena);
    tree->root->leaf = 1;
    tree->root->key_count = 0;
}

// NOTE(nasr): nodes that get  passed as parameters should've already been loaded into memory
internal void
b_tree_search(b_tree_node *node, string8 key)
{
    s32 found_index = node_find_pos(node, key);

     if (found_index < n->key_count && string_compare(n->keys[i], key) == 0)
     {
         return n->rows[i];
     }
     if (n->leaf)
     {
         return NULL;

     }

     return b_tree_search(n->children[i], key);


}

internal void
b_tree_insert()
{

}

internal void
b_tree_write()
{
    // TODO(nasr): write the b_tree to disk
}

#endif /* BTREE_H */
