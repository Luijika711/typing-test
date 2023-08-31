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
    int idx;
    int wpm;
    double acc;
    int goodchar = 0, badchar = 0;
    int wordcnt;
    time_t start, end;
    int CONSOLE_HEIGHT;

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
        system("stty size > getsize.txt");
        std::ifstream fin("getsize.txt");
        fin >> CONSOLE_HEIGHT;
    }
    void render_screen(std::vector<std::pair<char, bool>> &arr)
    {
        system("clear");
        // std::cout << query << "\n";
        wordcnt = idx / 5;
        for (int i = 0; i <= idx; ++i)
        {
            auto it = my_str[i];
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
        for (int i = idx + 1; i < query.size(); ++i)
        {
            std::cout << query[i];
        }
        for (int i = 0; i < CONSOLE_HEIGHT - 1; ++i)
            std::cout << "\n";
        double time_taken = (double)(end - start) / 60.0;

        double gross_wpm = (idx * 1.0 / 5.0) / time_taken;
        acc = (double(idx - badchar)) / idx * 100;
        wpm = gross_wpm * acc / 100;
        if (idx == 0 || idx == 1)
        {
            std::cout << "wpm : 0 | accuracy : 0%";
            return;
        }
        std::cout
            << "wpm : " << wpm << " | accuracy : " << (int)acc << "%";
    }

    app()
    {
        this->init();

        for (int i = 0; i < 20; ++i)
        {
            query += words[i] + " ";
        }
        system("clear");
        std::cout << query;
        char ch;
        idx = 0;
        while (ch = in::getch())
        {
            if (idx == 0)
                time(&start);
            if (ch == BACKSPACE_KEY)
                continue;
            if (ch == query[idx])
            {
                my_str.push_back({ch, true});
                goodchar++;
            }
            else
            {
                my_str.push_back({ch, false});
                badchar++;
            }
            time(&end);
            render_screen(my_str);

            idx++;
            if (idx == query.size() - 1)
            {
                break;
            }
        }
        char cha;
        std::cout << "\nwant to do another test?y/n";
        cha = in::getch();
        if (cha == 'y')
        {
            app f;
        }
        else
        {
            exit(0);
        }
    }
};
int main()
{
    app a;
}