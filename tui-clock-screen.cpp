#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CURSOR_TO_TOP 1
#define NO_SPACE 1

typedef struct winsize TERMSIZE;

class Screen
{
private:
    struct termios raw, normal;
    TERMSIZE before, after;
    int error_code;

    void update_terminal_size_variable(TERMSIZE* w);
    void init_terminal_size_variables(TERMSIZE* source, TERMSIZE* destination);
public:
    Screen();

    void clear_screen(int CURSOR_TOP = 0);
    void get_terminal_size(TERMSIZE* term);
    void set_exit_message(int err_code);
    int get_keys_press(char* key, int deciseconds_timeout = 1);
    bool is_term_size_changed();
    
    ~Screen();
};

void Screen::set_exit_message(int err_code)
{
    error_code = err_code;
}

void Screen::init_terminal_size_variables(TERMSIZE* source, TERMSIZE* destination)
{
    destination->ws_col = source->ws_col;
    destination->ws_row = source->ws_row;
    destination->ws_xpixel = source->ws_xpixel;
    destination->ws_ypixel = source->ws_ypixel;
}

bool Screen::is_term_size_changed()
{
    get_terminal_size(&after);
    if ( before.ws_col != after.ws_col || before.ws_row != after.ws_row){
        init_terminal_size_variables(&after, &before);
        return true;
    }

    return false;
}

int Screen::get_keys_press(char* key, int deciseconds_timeout)
{
    raw.c_cc[VTIME] = deciseconds_timeout;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    return read(STDIN_FILENO, key, 1);  
}

void Screen::clear_screen(int CURSOR_TOP)
{
    std::cout<< "\x1B[2J" << std::flush;
    if( CURSOR_TOP == CURSOR_TO_TOP ){ std::cout<< "\x1B[1;1f" << std::flush; }
}


void Screen::get_terminal_size(TERMSIZE* term)
{
    update_terminal_size_variable(term);
}


void Screen::update_terminal_size_variable(TERMSIZE* w)
{
    ioctl(STDOUT_FILENO, TIOCGWINSZ, w);
}

Screen::Screen()
{

    get_terminal_size(&before);

    tcgetattr(STDIN_FILENO, &raw);
    tcgetattr(STDIN_FILENO, &normal);


    std::cout << "\x1B[s";                                   // Save cursor location
    std::cout << "\x1B[?47h";                                // Save screen
    std::cout << "\x1B[?25l";                                // Hide cursor
    std::cout << "\x1B[1049h";                               // Enable alternate buffer
    std::cout << "\x1B[1;1f" << std::flush;                  // Move cursor to the top


    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    
    
}

Screen::~Screen()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &normal);

    std::cout << "\x1B[1049l";                               // Disable alternate buffer
    std::cout << "\x1B[?25h";                                // Unhide cursor
    std::cout << "\x1B[?47l";                                // Restore screen
    std::cout << "\x1B[u" << std::flush;                     // Rstore cursor location
    
    switch (error_code)
    {
    case 1:
        std::cout<< "NO ENOUGH SPACE TO DRAW" << std::endl;
        break;
    
    default:
        break;
    }
}
