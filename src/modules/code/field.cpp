#include <raylib.h>

#include <cstring>

#include <array>
#include <algorithm>
#include <iostream>

#include "field.h"


#define TAB_SIZE 4

#define HORIZONTAL_MARGIN 32.0f

Field::Field(std::string filepath)
{
    file = filepath;
    Text = {""};

    current_line_size = Text[0].length();
    filesize = Text.size();
}


std::array<char, 16> word_separators = {' ', '(', ')', '[', ']', '{', '}', ',', '"', '\'', '<', '>', ';', '!', '/', '\\'};
auto wbegin = word_separators.begin();
auto wend = word_separators.end();


size_t GetByteCount(const char* text, size_t count)
{
    size_t byte = 0;
    int c;
    for (size_t i = 0; i != count; ++i)
    {
        GetCodepoint(text, &c);
        byte += c;
        text += c;
    }
    return byte;
}

bool Field::InputLoop(int pos_x, int pos_y, size_t width, size_t height, float deltatime)
{
    /*Text = {std::to_string(pos_x),std::to_string(pos_y),std::to_string(width),std::to_string(height)};
    return true; */


    //Check and set cursor
    int mousex = GetMouseX();
    int mousey = GetMouseY();

    if (mousex > pos_x + HORIZONTAL_MARGIN and mousex < pos_x + width and mousey > pos_y and mousey < pos_y + height)
    {
        if (!is_text_cursor)
        {
            SetMouseCursor(2);
            is_text_cursor = true;
        }
    }
    else if (is_text_cursor)
    {
        SetMouseCursor(0);
        is_text_cursor = false;
    }


    //returned variable
    update_field = false;


    //temp
    int codepointByteCount = 0;
    int codepoint;



    //modes
    bool ctrl_mode = IsKeyDown(KEY_LEFT_CONTROL) or IsKeyDown(KEY_RIGHT_CONTROL);
    bool shift_mode = IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT);
    bool alt_mode = IsKeyDown(KEY_LEFT_ALT) or IsKeyDown(KEY_RIGHT_ALT);
    bool seceltion_mode = false;

    float scrolldelta = 0.0f;

    tasks.clear();
    /*
     1: KEY_LEFT
     2: KEY_RIGHT
     3: KEY_UP
     4: KEY_DOWN
     5: BACKSPACE
     6: NEWLINE
     7: TAB
     8: SCROLL
     */

    if (tasks.size() == 0)
    {
        if      (IsKeyPressed(KEY_LEFT))      tasks.push_front(1);
        else if (IsKeyPressed(KEY_RIGHT))     tasks.push_front(2);
        else if (IsKeyPressed(KEY_UP))        tasks.push_front(3);
        else if (IsKeyPressed(KEY_DOWN))      tasks.push_front(4);
        else if (IsKeyPressed(KEY_BACKSPACE)) tasks.push_front(5);
        else if (IsKeyPressed(KEY_ENTER))     tasks.push_front(6);
        else if (IsKeyPressed(KEY_TAB))       tasks.push_front(7);

        else {
            if      (IsKeyPressedRepeat(KEY_LEFT))      tasks.push_front(1);
            else if (IsKeyPressedRepeat(KEY_RIGHT))     tasks.push_front(2);
            else if (IsKeyPressedRepeat(KEY_UP))        tasks.push_front(3);
            else if (IsKeyPressedRepeat(KEY_DOWN))      tasks.push_front(4);
            else if (IsKeyPressedRepeat(KEY_BACKSPACE)) tasks.push_front(5);
            else if (IsKeyPressedRepeat(KEY_ENTER))     tasks.push_front(6);
            else if (IsKeyPressedRepeat(KEY_TAB))       tasks.push_front(7);
            else { tasks.push_back(0); }

        }
    }

    //scrolldelta = GetMouseWheelMoveV().y;
    //if (scrolldelta > 0.0f) tasks.push_front(8);

    do
    {
        switch (tasks.back())
        {
        case 1:
            //KEY_LEFT
            if (cursor_x == 0)
            {
                if (cursor_y > 0)
                {
                    current_line_size = Text[--cursor_y].length();
                    cursor_x = current_line_size;
                    cursor_visual_x = GetCodepointCount(Text[cursor_y].c_str());
                }
            }
            else
            {
                codepoint = GetCodepointPrevious(&Text[cursor_y][cursor_x], &codepointByteCount);
                cursor_x -= codepointByteCount;
                if (cursor_x < 0) { cursor_x = 0; }
                --cursor_visual_x;
            }
            max_wanted = cursor_x;

            if (ctrl_mode and cursor_x > 0 and cursor_x < current_line_size)
            {
                if (std::find(wbegin, wend, Text[cursor_y][cursor_x]) == wend ) { tasks.push_front(1); } //REPEAT
                break;
            }
            break;
        case 2:
            //KEY_RIGHT
            if (cursor_x >= current_line_size)
            {
                if (cursor_y + 1 < filesize)
                {
                    cursor_x = 0;
                    cursor_visual_x = 0;
                    tasks.push_front(4); //KEY_DOWN
                }
            }
            else
            {
                //Move to the next codepoint
                codepoint = GetCodepoint(&Text[cursor_y][cursor_x], &codepointByteCount);
                cursor_x += codepointByteCount;
                ++cursor_visual_x;
            }
            max_wanted = cursor_x;

            if (ctrl_mode and cursor_x > 0 and cursor_x < current_line_size)
            {
                if (std::find(wbegin, wend, Text[cursor_y][cursor_x]) == wend ) { tasks.push_front(2); } //REPEAT
            }
            break;
        case 3:
            //KEY_UP
            if (cursor_y > 0)
            {
                current_line_size = Text[--cursor_y].length();
                //cursor_x = std::min(current_line_size, cursor_x);
                //1. get substr 2. get codepoint count in it. Not very optimized, but who cares, happens once in a minute/second
                //cursor_visual_x = GetCodepointCount(std::move(Text[cursor_y].substr(0, cursor_x)).c_str());

                cursor_visual_x = std::min((size_t)GetCodepointCount(Text[cursor_y].c_str()), cursor_visual_x);
                cursor_x = GetByteCount(Text[cursor_y].c_str(), cursor_visual_x);
            }
            else { cursor_x = 0; cursor_visual_x = 0; }
            break;
        case 4:
            //KEY_DOWN
            if (cursor_y + 1 < filesize)
            {
                current_line_size = Text[++cursor_y].length();
                cursor_visual_x = std::min((size_t)GetCodepointCount(Text[cursor_y].c_str()), cursor_visual_x);
                cursor_x = GetByteCount(Text[cursor_y].c_str(), cursor_visual_x);
            }
            else
            {
                cursor_x = current_line_size;
                cursor_visual_x = GetCodepointCount(Text[cursor_y].c_str());
            }
            break;
        case 5:
            //BACKSPACE
            if (cursor_x > 0)
            {
                GetCodepointPrevious(&Text[cursor_y][cursor_x], &codepointByteCount);
                if (Text[cursor_y][cursor_x - codepointByteCount] == ' ')
                {
                    do
                    {
                        --cursor_visual_x;
                        for (int i = 0; i != codepointByteCount; ++i)
                        {
                            Text[cursor_y].erase(Text[cursor_y].begin() + --cursor_x);
                        }
                        GetCodepointPrevious(&Text[cursor_y][cursor_x], &codepointByteCount);

                    } while ( (cursor_x % TAB_SIZE != 0 and cursor_x > 0 and Text[cursor_y][cursor_x - codepointByteCount] == ' ') );
                }
                else
                {
                    --cursor_visual_x;
                    for (int i = 0; i != codepointByteCount; ++i)
                    {
                        Text[cursor_y].erase(Text[cursor_y].begin() + --cursor_x);
                    }
                    GetCodepointPrevious(&Text[cursor_y][cursor_x], &codepointByteCount);
                }

                if (ctrl_mode and cursor_x > 0)
                {
                    if (cursor_x == 0 or std::find(wbegin, wend, Text[cursor_y][cursor_x - codepointByteCount]) == wend ) { tasks.push_front(5); } //REPEAT
                }
            }
            else
            {
                if (cursor_y > 0)
                {
                    std::string leftovers = "";
                    if (current_line_size > 0) { leftovers = Text[cursor_y]; }
                    Text.erase(Text.begin() + cursor_y--);

                    cursor_x = Text[cursor_y].length();
                    cursor_visual_x = GetCodepointCount(Text[cursor_y].c_str());

                    Text[cursor_y] = Text[cursor_y] + leftovers;
                    current_line_size = Text[cursor_y].length();
                }
            }
            update_field = true;
            break;
        case 6:
            //ENTER
            {
            std::string leftovers = "";
            if (cursor_x < current_line_size)
            {
                leftovers = Text[cursor_y].substr(cursor_x, current_line_size - cursor_x);
                Text[cursor_y] = Text[cursor_y].substr(0, cursor_x);
            }
            else if (cursor_x > 0)
            {
                //last word before cursor but reversed
                for(auto it = Text[cursor_y].end() - 1; it != Text[cursor_y].begin() - 1; --it)
                {
                    if (*it == ' ') { break; }
                    leftovers.push_back(*it);
                }

                int space = 0;
                //add tab after then, do
                if (leftovers == "od" or leftovers == "neht") { space += TAB_SIZE; }
                else if (leftovers == "dne" or leftovers == "kaerb") { space -= TAB_SIZE; }
                else if (Text[cursor_y].substr(0, 8) == "function") { space += TAB_SIZE; }
                leftovers = "";


                //stay on the same indentation level
                if (Text[cursor_y][0] == ' ')
                {
                    for(auto it = Text[cursor_y].begin(); it != Text[cursor_y].end(); ++it)
                    {
                        if (*it != ' ') { break; }
                        ++space;
                    }
                }

                if (space > 0)
                {
                    leftovers = std::string(space, ' ');
                }

            }

            Text.insert(Text.begin() + ++cursor_y, leftovers);
            if (cursor_x != current_line_size or leftovers == "") { cursor_x = 0; cursor_visual_x = 0; }
            else
            {
                cursor_x = leftovers.length();
                cursor_visual_x = GetCodepointCount(leftovers.c_str());
            }
            update_field = true;
            }
            break;
        case 7:
            //TAB
            for (int i = 0; i != TAB_SIZE; ++i) Text[cursor_y].insert(Text[cursor_y].begin() + cursor_x++, ' ');
            update_field = true;
            break;
        case 8:
            //SCROLL
            scrollpos = std::max(0.0f, std::max(scrollpos + scrolldelta, (float)filesize + 8));
            break;
        }

        tasks.pop_back();

        if (update_field)
        {
            current_line_size = Text[cursor_y].length();
            filesize = Text.size();
        }

    } while (tasks.size() > 0);
    //tasks ended

    //keycodes and input

    key = GetCharPressed();

    //alt codes
    if (IsKeyDown(KEY_RIGHT_ALT))
    {
        if(key > 47 and key < 58)
        {
            altcode *= 10;
            altcode += key - 48;

        }
        if (key > 0)
        std::cout << "ALT " << key;
    }
    else if (IsKeyReleased(KEY_RIGHT_ALT))
    {
        key = altcode;

        if (key > 0)
        {
            alt_mode = false;
            altcode = 0;
        }
    }


    //keycode input
    if (ctrl_mode or alt_mode)
    {
        if (ctrl_mode and not alt_mode)
        {
            if (IsKeyPressed(KEY_V))
            {
                std::string clip = GetClipboardText();
                Text[cursor_y].insert(cursor_x, clip);
                cursor_visual_x += GetCodepointCount(clip.c_str());
                cursor_x += clip.length();
                update_field = true;
            }
        }

    }
    //letter input
    else if (key > 0)
    {
        std::cout << key << ' ';
        int size = 0;
        const char* utf8key = CodepointToUTF8(key, &size);

        Text[cursor_y].insert(cursor_x, utf8key);

        //fast ++cursor without external checks
        codepoint = GetCodepoint(&Text[cursor_y][cursor_x], &codepointByteCount);
        cursor_x += codepointByteCount;
        ++cursor_visual_x;

        update_field = true;
    }

    return update_field;
}
