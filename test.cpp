#include "test.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

bool Test::compareStrings(std::string a, std::string b)
{
    std::stringstream as(a);
    std::stringstream bs(b);

    std::string al;
    std::string bl;

    while (std::getline(as, al))
    {
        std::replace(al.begin(), al.end(), '\r', '`');
        if (!std::getline(bs, bl))
        {
            std::cout << "a: |" << al << "|\n";
            std::cout << "b: {EOF}\n";
        }
        else
        {
            std::replace(bl.begin(), bl.end(), '\r', '`');
            if (bl.compare(al) == 0)
            {
                std::cout << "   |" << al << "|\n";
            }
            else
            {
                std::cout << "a: |" << al << "|\n";
                std::cout << "b: |" << bl << "|\n";
            }
        }
    }

    while (std::getline(bs, bl))
    {
        std::replace(bl.begin(), bl.end(), '\r', '`');
        std::cout << "a: {EOF}\n";
        std::cout << "b: |" << bl << "|\n";
    }

    std::cout << "\n";
}