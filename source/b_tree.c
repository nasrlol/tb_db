
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


