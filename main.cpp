#include <iostream>
#include "color_console.hpp"
#include <stdlib.h>
#include <termios.h>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>
#define BACKSPACE_KEY (127)
namespace in
{
    static struct termios old, current;
    void initTermios(int echo)
    {
        tcgetattr(0, &old);
        current = old;
        current.c_lflag &= ~ICANON;
        if (echo)
        {
            current.c_lflag |= ECHO;
        }
        else
        {
            current.c_lflag &= ~ECHO;
        }
        tcsetattr(0, TCSANOW, &current);
    }
    void resetTermios(void)
    {
        tcsetattr(0, TCSANOW, &old);
    }
    char getch_(int echo)
    {
        char ch;
        initTermios(echo);
        ch = getchar();
        resetTermios();
        return ch;
    }
    char getch(void)
    {
        return getch_(0);
    }
    char getche(void)
    {
        return getch_(1);
    }
}
class app
{
private:
    std::vector<std::string> words;
    std::ifstream fin;
    std::string query;
    std::vector<std::pair<char, bool>> my_str; // my_str[i].second == true if my_str[i].first = query[i]
public:
    void init()
    {
        int seed = time(nullptr);
        fin.open("words.txt");
        std::string str;
        while (fin >> str)
        {
            words.push_back(str);
        }
        std::shuffle(words.begin(), words.end(), std::default_random_engine(seed));
    }
    void render_screen(const std::vector<std::pair<char, bool>> &arr)
    {
        system("clear");
        std::cout << query << "\n";
        for (const auto &it : arr)
            if (it.second == true)
            {
                std::string c;
                c.push_back(it.first);
                console.print(c, {console.green, console.bold});
            }
            else
            {
                std::string c;
                c.push_back(it.first);
                console.print(c, {console.red, console.bold});
            }
    }

    app()
    {
        this->init();

        for (int i = 0; i < 20; ++i)
        {
            query += words[i] + " ";
        }
        system("clear");
        std::cout << query << "\n\n";
        char ch;
        int idx = 0;
        while (ch = in::getch())
        {
            if (ch == query[idx])
            {
                my_str.push_back({ch, true});
            }
            else
            {
                my_str.push_back({ch, false});
            }

            if (ch == BACKSPACE_KEY && my_str.size())
            {
                my_str.pop_back();
                idx--;
            }
            render_screen(my_str);
            idx++;
            if (idx == query.size() - 1)
            {
                break;
            }
        }
    }
};
int main()
{
    app a;
}