#include "utils.hpp"

bool str_alnum(std::string &str)
{
    for (std::string::iterator it = str.begin(); it != str.end(); it++)
    {
        char c = *it;
        if (!std::isalnum(c))
            return false;
    }
    return true;
}
