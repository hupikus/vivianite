#ifndef FIELD
#define FIELD

#include <string>
#include <vector>

class Field
{
private:
    std::string file;
    int symbol, column;
    float scrollpos = 0.0f;
public:
    Field(std::string filepath);

    std::vector<std::string> Text;
};

#endif
