#include <raylib.h>

#include <array>
#include <algorithm>

#include "field.h"


#define TAB_SIZE 4

Field::Field(std::string filepath)
{
    file = filepath;
    Text = {""};
    Text =
    {
        "-- Simple Lua program placeholder",
        "print( 'Hello,Lua!' )",
        "",
        " -- Function to add two numbers",
        "",
        "function addNumbers(a, b)",
        "   return a + b",
        "end",
        "",
        "-- Example usage",
        "local num1 = 5",
        "local num2 = 10",
        "",
        "print(addNumbers(num1, num2))",
        "",
        "if num1 > num3 and num1 == 5 then",
        "   print(\"WOWIE\" + ', ' + \"a 'string'\")",
        "end",
        "",
        "more text \"word before --  \\n comment \\r \\ \"",
        "",
        "a = 5",
        "a = math.random()",
        "math.randomus()",
        "() ((()))  ([([()])])   ([)]   ())",
    };

    current_line_size = Text[0].length();
    filesize = Text.size();
}


std::array<char, 16> word_separators = {' ', '(', ')', '[', ']', '{', '}', ',', '"', '\'', '<', '>', ';', '!', '/', '\\'};
auto wbegin = word_separators.begin();
auto wend = word_separators.end();


bool Field::InputLoop(float deltatime)
{
    update_field = false;


    //modes
    bool word_mode = false;
    bool seceltion_mode = false;

    tasks.clear();
    /*
     1: KEY_LEFT
     2: KEY_RIGHT
     3: KEY_UP
     4: KEY_DOWN
     5: BACKSPACE
     6: NEWLINE
     7: TAB
     */

    //Cursor
    do
    {
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

        if ( IsKeyDown(KEY_LEFT_CONTROL) ) { word_mode = true; }

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
                }
            }
            else { --cursor_x; }
            max_wanted = cursor_x;

            if (word_mode and cursor_x != 0 and cursor_x != current_line_size)
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
                    tasks.push_front(4); //KEY_DOWN
                }
            }
            else { ++cursor_x; }
            max_wanted = cursor_x;

            if (word_mode and cursor_x != 0 and cursor_x != current_line_size)
            {
                if (std::find(wbegin, wend, Text[cursor_y][cursor_x]) == wend ) { tasks.push_front(2); } //REPEAT
            }
            break;
        case 3:
            //KEY_UP
            if (cursor_y > 0)
            {
                current_line_size = Text[--cursor_y].length();
                cursor_x = std::min(current_line_size, cursor_x);
            }
            else { cursor_x = 0; }
            break;
        case 4:
            //KEY_DOWN
            if (cursor_y + 1 < filesize)
            {
                current_line_size = Text[++cursor_y].length();
                cursor_x = std::min(current_line_size, cursor_x);
            }
            else
            {
                cursor_x = current_line_size;
            }
            break;
        case 5:
            //BACKSPACE
            if (cursor_x > 0)
            {
                if (Text[cursor_y][cursor_x - 1] == ' ')
                {
                    do
                    {
                        Text[cursor_y].erase(Text[cursor_y].begin() + --cursor_x);
                    } while ( (cursor_x != TAB_SIZE and cursor_x > 0 and Text[cursor_y][cursor_x - 1] == ' ') );
                }
                else
                {
                    Text[cursor_y].erase(Text[cursor_y].begin() + --cursor_x);
                }

                if (word_mode and cursor_x != 0)
                {
                    if (std::find(wbegin, wend, Text[cursor_y][cursor_x]) == wend ) { tasks.push_front(5); } //REPEAT
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
            if (cursor_x != current_line_size or leftovers == "") { cursor_x = 0; }
            else { cursor_x = leftovers.length(); }
            update_field = true;
            }
            break;
        case 7:
            //TAB
            for (int i = 0; i != TAB_SIZE; ++i) Text[cursor_y].insert(Text[cursor_y].begin() + cursor_x++, ' ');
            update_field = true;
            break;
        }

        tasks.pop_back();

        //typing
        key = GetCharPressed();

        if (key > 0)
        {
            Text[cursor_y].insert(Text[cursor_y].begin() + cursor_x++, key);
            update_field = true;
        }

        if (update_field)
        {
            current_line_size = Text[cursor_y].length();
            filesize = Text.size();
        }

    } while (tasks.size() > 0);

    return update_field;
}
