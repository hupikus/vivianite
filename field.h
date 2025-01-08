#ifndef FIELD
#define FIELD

#include <string>
#include <vector>

class Field
{
private:
    int symbol, column;
public:
    Field();

    std::vector<std::string> Text;
};

#endif
