run:
	@./tui-clock

build: tui-clock.cpp 
	@g++ -Wall -Wextra -o tui-clock tui-clock.cpp

clean:
	@rm tui-clock