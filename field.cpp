#include "field.h"

Field::Field(std::string filepath)
{
    //Text = {"#include <string>", "was", "popka"};
    symbol = 0;
    column = 0;
    file = filepath;
    Text = {
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
        "print(addNumbers(num1, num2))",
        "if num1 > num3 then",
        "   print(\"WOWIE\" + ', ' + \"a 'string'\")",
        "end",
        "more text local\"word before --  \\n comment \\r \\ \" local local local"
            };
}
