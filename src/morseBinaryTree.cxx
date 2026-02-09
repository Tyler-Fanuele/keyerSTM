#include <morseBinaryTree.h>

MorseBinaryTree::MorseBinaryTree()
{
    
}


char MorseBinaryTree::parseMorseCode(const char path[])
{
    int index = 0; // Start at the root of the tree
    for (int i = 0; path[i] != '\0'; ++i)
    {
        if (path[i] == '.')
        {
            index = 2 * index + 1; // Go left (dit)
        }
        else if (path[i] == '-')
        {
            index = 2 * index + 2; // Go right (dah)
        }
        else
        {
            // Invalid input, return null character
            return '\0';
        }
        // Optional: Check bounds
        if (index >= TREESIZE)
            return '\0';
    }
    return _tree[index];
}

int MorseBinaryTree::printTree()
{
    //std::cout << "\n";

    for (int i = 0; i < TREESIZE; i++)
    {
        if (_tree[i] != '\0')
        {
            //std::cout << _tree[i];
        }
        else
        {
            //std::cout << "-";
        }
    }
    //std::cout << "\n";
    return 0;
}
