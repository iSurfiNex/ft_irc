#include "utils.hpp"

// TODO add namespace
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

std::string itoa(int num)
{
    std::stringstream ss;
    ss << num;
    const std::string result = ss.str();
    return result;
}

char toLower(char c) {
    return std::tolower(static_cast<unsigned char>(c));
}

std::string tolowerStr(const std::string& str)
{
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), toLower);
    return lower;
}
