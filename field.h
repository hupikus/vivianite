#ifndef FIELD
#define FIELD

#include <string>
#include <vector>
#include <deque>

class Field
{
private:
    std::string file;

    size_t current_line_size = 0;
    size_t filesize = 0;


    bool update_field = false;
    int key;
    size_t max_wanted = 0;

    std::deque<int> tasks;
public:
    Field(std::string filepath);

    bool InputLoop();

    std::vector<std::string> Text;

    size_t cursor_x = 0;
    size_t cursor_y = 0;
    float scrollpos = 0.0f;
};

#endif
