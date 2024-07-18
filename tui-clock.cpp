// DATE: 08.06.2023

#include "tui-clock-render.cpp"


char keys;
TERM_CENTER center;
TIME current_time;
Render tui_clock;


int main()
{
    if (!tui_clock.is_enough_space()){
        tui_clock.set_exit_message(NO_SPACE);
        return EXIT_FAILURE;
    }

    tui_clock.get_keys_press(&keys);
    tui_clock.get_term_center(&center);
    tui_clock.get_time(&current_time);


    tui_clock.display_clock(&center, &current_time);

    while (true)
    {

        if (keys == 'q') { return EXIT_SUCCESS; }

        tui_clock.get_keys_press(&keys, 10);

        tui_clock.update_sec(&center, &current_time);

        tui_clock.get_time(&current_time);
        if ( current_time.tm_sec == 0 ) { tui_clock.display_clock(&center, &current_time); }

        if (tui_clock.is_term_size_changed()) {
                
            if (!tui_clock.is_enough_space()) { tui_clock.set_exit_message(NO_SPACE); return EXIT_FAILURE; }

            tui_clock.clear_screen();
            tui_clock.get_term_center(&center);

            tui_clock.display_clock(&center, &current_time);
        }

        usleep(1000*1000/FRAME_RATE);

    }

    return EXIT_SUCCESS;

}
