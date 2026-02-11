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

bool MorseBinaryTree::encodeChar(char c, char out[], int maxLen)
{
    // 1) Find the character in the tree
    int index = -1;
    for (int i = 0; i < TREESIZE; ++i)
    {
        if (_tree[i] == c)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        return false;  // character not found

    // 2) Walk backwards to root, collecting dits/dahs
    int len = 0;
    while (index != 0)
    {
        if (len >= maxLen - 1)
            return false; // output buffer too small

        if (index % 2 == 1)
            out[len++] = '.';   // left child
        else
            out[len++] = '-';   // right child

        index = (index - 1) / 2; // move to parent
    }

    // 3) Reverse the result (we built it backwards)
    for (int i = 0; i < len / 2; ++i)
    {
        char tmp = out[i];
        out[i] = out[len - 1 - i];
        out[len - 1 - i] = tmp;
    }

    out[len] = '\0';
    return true;
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
