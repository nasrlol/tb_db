/**
 * >> api notes
 * >> by Abdellah El Morabit
 *
 * - creation of a btree happens when doing an insertion on an
 *  empty btree where the root is null or nil
 *
 **/

/* single header b-tree implementation by Abdellah El Morabit */
#ifndef BTREE_H
#define BTREE_H

// maximum height of the tree the lower the lower the lower amount
// of disk reads which translates into faster?
#if 0
global_variable read_only s16 btree_ORDER = 4;
#endif
#define BTREE_ORDER  4

//- NOTE(nasr): defining a key to improve sorting
//  i think saying that a key is a combination of the column + row is a good way of appraoching this
typedef struct key key;
struct key
{
    s32     header_index; //- col number
    s32     row_index;
};

typedef struct btree_node btree_node;
struct btree_node
{
    // store the key values of the sub nodes? if they are leaves?
    key keys[BTREE_ORDER - 1];
    // TODO(nasr): replace with something more generic?
    // NOTE(nasr): cons of void * -> no type safety
    // is there a way to still have some sort of that? size not variable
    void *payload_per_key[BTREE_ORDER - 1];
    btree_node *parent;
    // handle to store children faster than linked list
    // because now we can iterate over the nodes instead of having cache misses
    // when jumping through linked nodes
    btree_node *children[BTREE_ORDER];
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


typedef struct btree btree;
struct btree
{
    // NOTE(nasr): make sure this always stays in memory
    // so that initial fetch never requires a disk read
    btree_node *root;
};



#ifdef BTREE_IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////
//- btree.c


internal b32
is_nil_btree_node(btree_node *node)
{
    // NOTE(nasr): looks like a useless function but could be usefull when we implement a nil_btree_node 
    return (node == NULL);
}

// TODO(nasr): 1. splitting the tree when getting too big? (horizontally) 2. joining trees?

internal b8 
key_compare(key destination_key, key source_key)
{
    s32 source_index      = source_key.header_index      + source_key.row_index;
    s32 destination_index = destination_key.header_index + destination_key.row_index;

    if(destination_index > source_index) return 1;
    else if(destination_index < source_index) return -1;
    else return 0;
}

// NOTE(nasr): @return the index of the found element
//- @param: start node could be the root node for all we care. but this enables optimizations in the future if we need them
internal s32
btree_find_ipos(key k, btree_node *start_node)
{
    s32 index = 0;
    // scan right while the node's key at [index] is strictly less than k
    while (index < start_node->key_count && key_compare(start_node->keys[index], k) < 0)
    {
        ++index;
    }
    return index;
}

// NOTE(nasr): nodes that get passed as parameters should've already been loaded into memory
internal void *
btree_search(btree_node *node, key key)
{
    s32 index = btree_find_ipos(key, node);

    if (index < node->key_count && key_compare(node->keys[index], key) == 0)
    {
        return node->payload_per_key[index];
    }
    if (node->leaf)
    {
        return NULL;
    }
    return btree_search(node->children[index], key);
}

// TODO(nasr): split node when key_count == btree_ORDER - 1 (node is full)
// -- each node has max btree_ORDER - 1 keys
//
internal void
btree_insert(mem_arena *arena, btree *tree, key key, void *payload)
{
    if(is_nil_btree_node(tree->root)) 
    {
        tree->root = PushStructZero(arena, btree_node);
        tree->root->leaf = 1;
        tree->root->key_count = 0;
    }

    btree_node *current_node = tree->root;

    if (current_node->leaf)
    {
        // find the insertion position and shift keys right to make room
        //- after: but what is this. we are expecting a new key.but we are looking for 
        //- its position in the tree is useless because it's non existent

        s32 i = btree_find_ipos(key, current_node);

        for (s32 j = current_node->key_count; j > i; --j)
        {
            current_node->keys[j]            = current_node->keys[j - 1];
            current_node->payload_per_key[j] = current_node->payload_per_key[j - 1];
        }

        current_node->keys[i]            = key;
        current_node->payload_per_key[i] = payload;

        if(current_node->key_count < (BTREE_ORDER - 1))
        {
            current_node->key_count += 1;
        }
        else {
            // TODO(nasr): creating a new branch / tree?
            // make a seperate function for this
            // NOTE(nasr): only split through the parent when one actually exists;
            // root splits need to be handled separately (promote mid key, create two children)
            if(current_node->parent != NULL)
            {
                s32 split_pos = current_node->parent->key_count / 2 - 1;
                s32 updated_keycount_p = 0;
                for(s32 index = 0; index <= split_pos; ++index)
                {
                    ++updated_keycount_p;
                }
                current_node->parent->key_count = updated_keycount_p;
            }
        }
    }
    // TODO(nasr): internal node case walk down then split on the way back up
}


internal void
btree_write(btree *bt)
{
    unused(bt);
#if 0
    temp_arena *temp_arena = temp_arena_begin(arena);
    btree_node *root = bt->root;

    for(btree_node *bt = PushStruct(arena); root->next; next = root;)
    {
        
    }

    temp_arena_end(temp);
#endif
}

// - load the entire db into memory.
#if 0
internal void
btree_load(mem_arena *arena)
{


}
#endif

#endif /* BTREE_IMPLEMENTATION */
#endif /* BTREE_H */
