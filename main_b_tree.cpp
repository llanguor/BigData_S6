#include <iostream>
#include "b_tree.hpp"

int compare_func(int const & s1, int const & s2)
{
    if (s1<s2) return -1;
    if (s2<s1) return 1;
    return 0;
}

int main()
{
    auto tree = new b_tree<int, int>(2, std::function(compare_func));
    tree->insert(10, 10);
    tree->insert(20, 20);
    tree->insert(30, 30);
    tree->insert(25, 25);
    tree->insert(5, 5);
    tree->insert(15, 15);
    tree->insert(27, 27);
    tree->insert(12, 12);
    tree->insert(24, 24);
    tree->insert(13, 13);
    tree->insert(14, 14);
    tree->insert(6, 6);
    tree->insert(7, 7);
    tree->insert(8, 8);

    std::cout << "COUT" << tree->elements_count() << std::endl;
    for (auto it = tree->begin(); it != tree->end(); ++it)
    {
        auto [key, value, v1, v2] = *it;
        std::cout<< "[" << key << "] = " << value << "\n";
    }



    std::cout<< tree->dispose(7);
    std::cout<< tree->dispose(14);
    std::cout<< tree->dispose(15);
    std::cout<< tree->dispose(20);
    std::cout<< tree->dispose(24);
    std::cout<< tree->dispose(25);

    //  std::cout<< tree->dispose(12);

    //  std::cout<< tree->dispose(8);

    //std::cout<< tree->dispose(10);
    /*
    auto pairs = tree->obtain_between(
        10,
        24,
        true,
        true);

    for (auto p: pairs)
    {
        std::cout<< "[" << p.key << "] = " << p.value << "\n";
    }
    */

    delete tree;

}


