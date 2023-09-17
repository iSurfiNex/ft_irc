#pragma once

#define NC "\e[0m"
#define GRAY "\e[90m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define PURPLE "\e[35m"
#define CYAN "\e[36m"
#define YELLOW "\e[93m"

#define FRED "\e[41m"
#define FGREEN "\e[42m"

#define foreach(elementType, container) \
    for (elementType::iterator it = (container).begin(); it != (container).end(); ++it)
