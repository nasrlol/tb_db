/* single header b-tree implementation by Abdellah El Morabit */
#ifndef B_TREE_H
#define B_TREE_H

// maximum height of the tree the lower the lower the lower amount
// of disk reads which translates into faster?
#if 0
global_variable read_only s16 B_TREE_ORDER = 4;
#endif
#define B_TREE_ORDER  4

//- NOTE(nasr): defining a key to improve sorting
//  i think saying that a key is a combination of the column + row is a good way of appraoching this
typedef struct key key;
struct key
{
    string8 header;
    s32     row;
};

typedef struct b_tree_node b_tree_node;
struct b_tree_node
{
    // store the key values of the sub nodes? if they are leaves?
    key keys[B_TREE_ORDER - 1];
    // TODO(nasr): replace with something more generic?
    // NOTE(nasr): cons of void * -> no type safety
    // is there a way to still have some sort of that?
    // size not variable
    void *payload_per_key[B_TREE_ORDER - 1];
    b_tree_node *parent;
    // handle to store children faster than linked list
    // because now we can iterate over the nodes instead of having cache misses
    // when jumping through linked nodes
    b_tree_node *children[B_TREE_ORDER];
    // this shouldn't change things but could be a performance improvement on a bigger scale??
    s32 *max_children;
    // NOTE(nasr): reference count ::: check how many leaves are using this node
    // also not needed for now because we don't free individual node because of arena allocator
    // s32 *refc;
    s32 key_count;
    b32 leaf;


    // NOTE(nasr): do we hold the reference to the arena? or do we pass is it as a reference? 
    // this could solve memory location issues?
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

// NOTE(nasr): @return the index of the found element
internal s32
btree_node_find_pos(string8 value, b_tree_node *node)
{
    unused(value);
    unused(node);

#if 0
    s32 i = 0;
    for (; i < node->key_count && string8_cmp(node->keys[i], value) < 0; ++i);
    return i;
#endif

    return 0;

}

internal void
b_tree_create(mem_arena *arena, b_tree *tree)
{
    tree->root = btree_node_alloc(arena);
    tree->root->leaf = 1;
    tree->root->key_count = 0;
}


// NOTE(nasr): nodes that get passed as parameters should've already been loaded into memory
internal void *
b_tree_search(b_tree_node *node, string8 key)
{
    unused(node);
    unused(key);

#if 0
    s32 i = btree_node_find_pos(key, node);

    if (i < node->key_count && string8_cmp(node->keys[i], key) == 0)
    {
        return node->payload_per_key[i];
    }
    if (node->leaf)
    {
        return NULL;
    }
    return b_tree_search(node->children[i], key);
#endif

    return NULL;
}


// TODO(nasr): split node when key_count == B_TREE_ORDER - 1 (node is full)
internal void
b_tree_insert(b_tree *tree, string8 key, void *payload)
{
    unused(tree);
    unused(key);
    unused(payload);
#if 0
    b_tree_node *current_node = tree->root;

    if (current_node->leaf)
    {
        // find the insertion position and shift keys right to make room
        s32 i = btree_node_find_pos(key, current_node);
        for (s32 j = current_node->key_count; j > i; --j)
        {
            current_node->keys[j]            = current_node->keys[j - 1];
            current_node->payload_per_key[j] = current_node->payload_per_key[j - 1];
        }
        current_node->keys[i]            = key;
        current_node->payload_per_key[i] = payload;

        if(current_node->key_count < (B_TREE_ORDER - 1))
        {
            current_node->key_count         += 1;
        }
        else {
            // TODO(nasr): creating a new branch / tree?
            // make a seperate function for this
        }
    }
    // TODO(nasr): internal node case walk down then split on the way back up
#endif
}

internal void
b_tree_write(b_tree *bt)
{
    // TODO(nasr): write the b_tree to disk
    unused(bt);
}

#endif /* B_TREE_IMPLEMENTATION */
#endif /* B_TREE_H */
