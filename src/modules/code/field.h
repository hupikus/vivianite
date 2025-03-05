#include <string>
#include <vector>
#include <deque>

#ifndef FIELD
#define FIELD

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

    bool InputLoop(int pos_x, int pos_y, size_t width, size_t height, float deltatime);

    std::vector<std::string> Text;

    size_t cursor_x = 0;
    size_t cursor_y = 0;
    size_t cursor_visual_x = 0;
    size_t& cursor_visual_y = cursor_y;
    float scrollpos = 0.0f;

    bool is_text_cursor = false;
};

#endif
