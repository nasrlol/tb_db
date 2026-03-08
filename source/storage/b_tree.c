
// TODO(nasr): 1. splitting the tree when getting too big? (horizontally) 2. joining trees?

internal b_tree_node *
node_alloc(mem_arena *arena)
{
    b_tree_node *node = PushStructZero(arena, type);
    node->leaf = 1;
    return node;
}

// NOTE(nasr): @return the index of of the found element
internal i32
node_find_pos(mem_arena *arena, string8 value)
{
    i32 i = 0;
    while (i < n->key_count && str8_cmp(n->keys[i], k) < 0)
    {
        ++i;
    }

    return i;
}

// NOTE(nasr): nodes that get  passed as parameters should've already been loaded into memory
internal void
b_tree_search(node *node)
{


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


