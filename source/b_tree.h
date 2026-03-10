/* single header b-tree implementation by Abdellah El Morabit */
#ifndef B_TREE_H
#define B_TREE_H

// maximum height  of the tree the lower the lower the lower amount
// of disk reads which translates into faster?
#define B_TREE_ORDER 4

typedef struct b_tree_node b_tree_node;
struct b_tree_node
{

    // store the values
    string8 keys[B_TREE_ORDER - 1];

    // TODO(nasr): replace with someting more  generic?
    // NOTE(nasr): cons of void * -> no type safety
    // is there a way to still have some sort of that?
    void *payload_per_key[B_TREE_ORDER - 1];

    b_tree_node *parent;
    // handle to store children faster than linked list
    // because now we can iteratate over the nodes instead of having cache misses
    // when jumping through linked nodes
    b_tree_node *children[B_TREE_ORDER];

    // this shouldnt change things but could be a performance improvement on a bigger scale?? 
    s32 *max_children;

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


#ifdef B_TREE_IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////
//- b_tree.c

// TODO(nasr): 1. splitting the tree when getting too big? (horizontally) 2. joining trees?

internal b_tree_node *
btree_node_alloc(mem_arena *arena)
{
    b_tree_node *node = PushStructZero(arena, b_tree_node);
    node->leaf = 1;
    return node;
}

// NOTE(nasr): @return the index of of the found element
internal s32
btree_node_find_pos(mem_arena *arena, string8 value, b_tree_node *node)
{
    for (s32 index = 0;  index < node->key_count && string8_cmp(node->keys[i], value) < 0; ++index);
    return i;
}

internal void
b_tree_create(mem_arena *arena, b_tree *tree)
{
    tree->root = btree_node_alloc(arena);
    tree->root->leaf = 1;
    tree->root->key_count = 0;
}

// NOTE(nasr): nodes that get  passed as parameters should've already been loaded into memory
internal void
b_tree_search(b_tree_node *node, string8 key)
{
    s32 found_index = btree_node_find_pos(node, key);
    s32 i = 0;

    if (found_index < node->key_count && string_compare(node->keys[index], key) == 0)
    {
        return node->rows[i];
    }
    if (node->leaf)
    {
        return NULL;

    }

    return b_tree_search(node->children[i], key);

    return NULL;
}

internal void
b_tree_insert(b_tree_node *current_node)
{
    if(current_node->leaf)
    {

    }

}

internal void
b_tree_write()
{

    // TODO(nasr): write the b_tree to disk
}

#endif /* B_TREE_IMPLEMENTATION */
#endif /* B_TREE_H */
