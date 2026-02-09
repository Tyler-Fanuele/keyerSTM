#pragma once

#define TREESIZE 63

class MorseBinaryTree
{
    public:

    MorseBinaryTree();
    ~MorseBinaryTree(){}

    static char parseMorseCode(const char path[]);

    int printTree();

    private:

    inline constexpr static const char _tree[63] = {
    '\0', 'E', 'T', 'I', 'A', 'N', 'M', 'S', 
    'U', 'R', 'W', 'D', 'K', 'G', 'O', 'H', 
    'V', 'F', '\0', 'L', '\0', 'P', 'J', 'B', 
    'X', 'C', 'Y', 'Z', 'Q', '\0', '\0', '5', 
    '4', '\0', '3', '\0', '\0', '\0', '2', '\0', 
    '\0', '\0', '\0', '\0', '\0', '\0', '1', '6', 
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', 
    '\0', '\0', '\0', '8', '\0', '9', '0'};

};