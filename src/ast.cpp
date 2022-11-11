#include "ast.h"

std::string k_str = "decl @getint() : i32\n"
                    "decl @getch() : i32\n"
                    "decl @getarray(*i32) : i32\n"
                    "decl @putint(i32)\n"
                    "decl @putch(i32)\n"
                    "decl @putarray(i32, *i32)\n"
                    "decl @starttime()\n"
                    "decl @stoptime()\n\n";
