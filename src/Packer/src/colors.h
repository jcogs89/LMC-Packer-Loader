#pragma once

#define RESET		"\033[0m"
#define RED(x)		"\n\033[31m" x "\033[0m\n"
#define GREEN(x)	"\033[32m" x "\033[0m"
#define BLUE(x)		"\033[34m" x "\033[0m"
#define YELLOW(x)	"\033[33m" x "\033[0m"

#define DEBUG 1
#if DEBUG == 1
#define Log(x) std::cout << BLUE(x)
#else
#define Log(x)
#endif

#define TITLE "\033[33m\
 _______  _______  _______  __   __  _______  __    _ \n\
|       ||   _   ||       ||  |_|  ||   _   ||  |  | |\n\
|    _  ||  |_|  ||       ||       ||  |_|  ||   |_| |\n\
|   |_| ||       ||       ||       ||       ||       |\n\
|    ___||       ||      _||       ||       ||  _    |\n\
|   |    |   _   ||     |_ | ||_|| ||   _   || | |   |\n\
|___|    |__| |__||_______||_|   |_||__| |__||_|  |__|\033[0m\n"
