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
    // i32 *refc;

    i32 key_count;

    b32 leaf;

};

typedef struct b_tree b_tree;
struct b_tree
{
    // NOTE(nasr): make sure this always stays in memory
    // so that initial fetch never requires a disk read
    b_tree_node *root;

};




#endif /* BTREE_H */
