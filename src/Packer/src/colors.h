#pragma once

#define RESET		"\e[0m"
#define RED(x)		"\n\e[1;31m" x "\e[0m\n"
#define GREEN(x)	"\n\e[1;32m" x "\e[0m\n"
#define BLUE(x)		"\e[1;34m" x "\e[0m"
#define YELLOW(x)	"\n\e[1;33m" x "\e[0m"

#define DEBUG 0
#if DEBUG == 1
#define Log(x) std::cout << BLUE(x)
#define clrscr()
#else
#define clrscr()	printf("\e[2J\e[1;1H\e[0m")
#define Log(x)
#endif

#define TITLE "\e[1;33m\
 _______  _______  _______  __   __  _______  __    _ \n\
|       ||   _   ||       ||  |_|  ||   _   ||  |  | |\n\
|    _  ||  |_|  ||       ||       ||  |_|  ||   |_| |\n\
|   |_| ||       ||       ||       ||       ||       |\n\
|    ___||       ||      _||       ||       ||  _    |\n\
|   |    |   _   ||     |_ | ||_|| ||   _   || | |   |\n\
|___|    |__| |__||_______||_|   |_||__| |__||_|  |__|\e[0m\n\n"
