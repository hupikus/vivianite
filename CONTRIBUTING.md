# Main

The project is mainly solo, but I would accept external PRs.  
General recomendations:
* No AI submissions: no AI comments, no AI unreviewed changes, no AI code you can't explain, exception being autonomous unprovoked ai agents. The code submitted or edited not by human will be likely rewritten by me or rejected.  
* I tend to not use C++ features a lot, so the codebase would be ready to be more or less easily rewritten to C.  
Using C++ features only when it will decrease readability significantly otherwise is the best.  
Usage of collections though (e.g. vector) is not discouraged at all.
* Small several commits and concise descriptions are better than "Changed stuff" changes.  
* Do not include third party code editor and lsp config files.

# Code style

* Line break before { for functions, no line break for everything else. Else is written like: `} else {` on a single line.
* For pointer asterisk and similar, put it before the name, 
Example: `const char *str` instead of `const char* str` or `const char * str`. With type casting and similar, no space: `(char*)`
* No space between function name and parentheses, example: void main(void).  
After if, while, for, switch e.t.c we add a space.  
With nested brackets you can add spaces inside from both sides if that increases readability, otherwise the statement has no space after `(`.  
* For tabulation, use tab or 4 spaces, preferrably tab.
* For if/while with a single statement inside, I tend to still use brackets.
* For a short if/while with a short single statement you can place everything on a single line, with spaces around curly brackets, example: `if (root) { delete root; }`
* An empty line between logical parts of code, it may be 2 or more proportionally if the parts don't correlate. Empty line has no trailing spaces.
* Prefer to be explicit: do `(void)` instead of `()` (only for definitions), `= NULL` instead of no initialization, `(val != NULL)` instead of `(!val)` (except for bool types), type instead of auto etc.
* Order of macros:  
First goes the parent header and the main header (for example, vivianite.h);  
Then the standard library headers by their size;  
Then the third party libraries;  
Then other headers from the codebase;  
Then conditions and define statements.
It's preferred to have a line break between them.
