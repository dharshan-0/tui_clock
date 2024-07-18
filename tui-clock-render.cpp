#ifndef RENDER_H
#define RENDER_H

#ifndef __linux__
#error ONLY WORKS ON LINUX
#endif


#define FIRST_HOUR_OFF         6+2+1+2+6+5
#define SECOND_HOUR_OFF        6+2+1+2+5
#define FIRST_COLOR_OFF        6+2+2
#define FIRST_MIN_OFF          6
#define SECOND_MIN_OFF         0
#define SECOND_COLOR_OFF       -(5+1)
#define FIRST_SEC_OFF          -(5+2+1+2)
#define SECOND_SEC_OFF         -(5+2+1+2+6)

#define FRAME_RATE   60

#include "tui-clock-screen.cpp"

#include <ctime>

typedef struct
{
    int MID_ROW;
    int MID_COL;

}TERM_CENTER;

typedef struct std::tm TIME;  // Don't create object as named "time"
typedef const char** char_num;

class Render : public Screen
{
private:

    bool changer = true;

    const char* zero[5] = {
        "╭───╮",
        "│   │",
        "│   │",
        "│   │",
        "╰───╯"
    };

    const char* one[5] = {
        " ╭╮  ",
        "  │  ",
        "  │  ",
        "  │  ",
        " ╶┴╴ "
    };

    const char* two[5] = {
        "╭───╮",
        "    │",
        "╭───╯",
        "│    ",
        "╰───╯"
    };

    const char* three[5] = {
        "╭───╮",
        "    │",
        " ───┤",
        "    │",
        "╰───╯"
    };

    const char* four[5] = {
        "╭    ",
        "│ ╷  ",
        "╰─┼─ ",
        "  │  ",
        "  ╵  "
    };

    const char* five[5] = {
        "╭───╮",
        "│    ",
        "╰───╮",
        "    │",
        "╰───╯"
    };

    const char* six[5] = {
        "╭───╮",
        "│    ",
        "├───╮",
        "│   │",
        "╰───╯"
    };

    const char* seven[5] = {
        "╭───╮",
        "  ╭─╯",
        "  │  ",
        "  │  ",
        "  ╵  "
    };
    

    const char* eight[5] = {
        "╭───╮",
        "│   │",
        "├───┤",
        "│   │",
        "╰───╯"
    };
    
    const char* nine[5] = {
        "╭───╮",
        "│   │",
        "╰───┤",
        "    │",
        "╰───╯"
    };

    const char* green[5] = {
        "   ",
        " \033[38;2;39;245;67m◉\033[0m ",
        "   ",
        " \033[38;2;39;245;67m◉\033[0m ",
        "   "
    };

    const char* red[5] = {
        "   ",
        " \033[38;2;219;35;54m○\033[0m ",
        "   ",
        " \033[38;2;219;35;54m○\033[0m ",
        "   "
    };

    typedef struct{
        short int FIRST_H;
        short int SECOND_H;
        short int FIRST_M;
        short int SECOND_M;
        short int FIRST_S;
        short int SECOND_S;
    }PARSED_TIME;

    void move_cursor(int row, int col);
    void parse_time(const TIME* time_to_parse, PARSED_TIME* parsed_time);


public:

    char_num c_num[12] = {zero, one, two, three, four, five, six, seven, eight, nine, green, red};

    Render(/* args */);

    void get_term_center(TERM_CENTER* center);
    void display_num(const TERM_CENTER* center, const char* num[5], int col_off = 0, int row_off = 0); //1
    void get_time(TIME* time_struct);
    void display_clock(const TERM_CENTER* center, const TIME* time_to_show);
    void update_sec(const TERM_CENTER* center, const TIME* time_to_show);
    bool is_enough_space();

    ~Render();
};

void Render::update_sec(const TERM_CENTER* center, const TIME* time_to_show)
{
    PARSED_TIME parsed_time;
    parse_time(time_to_show, &parsed_time);

    if (changer){
        display_num(center, c_num[10], FIRST_COLOR_OFF);
        display_num(center, c_num[10], SECOND_COLOR_OFF);
        changer = false;
    }
    else{
        display_num(center, c_num[11], FIRST_COLOR_OFF);
        display_num(center, c_num[11], SECOND_COLOR_OFF);
        changer = true;
    }
    
    display_num(center, c_num[parsed_time.FIRST_S], FIRST_SEC_OFF);
    display_num(center, c_num[parsed_time.SECOND_S], SECOND_SEC_OFF);

    if ( time_to_show->tm_sec == 59 ) { display_clock(center, time_to_show); }
}

bool Render::is_enough_space()
{
    TERMSIZE check;
    get_terminal_size(&check);

    if (check.ws_row >= 5 && check.ws_col >= 43){ return true; }  
    return false;
}

void Render::parse_time(const TIME* time_to_parse, PARSED_TIME* parsed_time)
{
    parsed_time->FIRST_H = time_to_parse->tm_hour / 10;
    parsed_time->SECOND_H = time_to_parse->tm_hour % 10;

    parsed_time->FIRST_M = time_to_parse->tm_min / 10;
    parsed_time->SECOND_M = time_to_parse->tm_min % 10;

    parsed_time->FIRST_S = time_to_parse->tm_sec / 10;
    parsed_time->SECOND_S = time_to_parse->tm_sec % 10;
}


void Render::display_clock(const TERM_CENTER* center, const TIME* time_to_show)
{
    PARSED_TIME parsed_time;
    parse_time(time_to_show, &parsed_time);

    display_num(center, c_num[parsed_time.FIRST_H], FIRST_HOUR_OFF);
    display_num(center, c_num[parsed_time.SECOND_H], SECOND_HOUR_OFF);

    display_num(center, c_num[10], FIRST_COLOR_OFF);

    display_num(center, c_num[parsed_time.FIRST_M], FIRST_MIN_OFF);
    display_num(center, c_num[parsed_time.SECOND_M], SECOND_MIN_OFF);
    
    display_num(center, c_num[10], SECOND_COLOR_OFF);
    
    display_num(center, c_num[parsed_time.FIRST_S], FIRST_SEC_OFF);
    display_num(center, c_num[parsed_time.SECOND_S], SECOND_SEC_OFF);
}

void Render::get_time(TIME* time_struct)
{
    std::time_t now = std::time(0);
    TIME* tmp = std::localtime(&now);
    time_struct->tm_hour = tmp->tm_hour;
    time_struct->tm_min = tmp->tm_min;
    time_struct->tm_sec = tmp->tm_sec;
    if (time_struct->tm_hour >= 13) { time_struct->tm_hour -= 12; }
}

void Render::move_cursor(int row, int col)
{
    std::cout<< "\x1B[" << row << ";" << col << "f" << std::flush;
}


void Render::display_num(const TERM_CENTER* center, const char* num[5], int col_off, int row_off)
{
    for (int i = 0; i <= 4; i++)
    {
        move_cursor((center->MID_ROW - row_off -1) + i, (center->MID_COL - col_off +2));
        std::cout << num[i] << std::flush;
    }
}


void Render::get_term_center(TERM_CENTER* center)
{
    TERMSIZE w;
    get_terminal_size(&w);
    center->MID_ROW = w.ws_row / 2;
    center->MID_COL = w.ws_col / 2;
}

Render::Render(/* args */)
{
}

Render::~Render()
{
}

#endif // RENDER_H