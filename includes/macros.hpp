#pragma once

#define NC "\e[0m"
#define GRAY "\e[90m"
#define GREEN "\e[32m"
#define PURPLE "\e[35m"
#define YELLOW "\e[93m"

#define foreach(elementType, container) \
    for (elementType::iterator it = (container).begin(); it != (container).end(); ++it)
