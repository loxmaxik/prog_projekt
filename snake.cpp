#include "snake.h"

objectpart::objectpart(int col,int row, int type) {
	x=col;
	y=row;
	type=type;
}

objectpart::objectpart() {
	x=0;
	y=0;
	type=1;
}


WINDOW* snakeclass::create_newwin(int height, int width, int starty, int startx) {

	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0, 0);
	wrefresh(local_win);
	return local_win;

}

void snakeclass::destroy_win(WINDOW *local_win) {

	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);

}

void snakeclass::initialize() {
	//start the screen,  prepare colors for later use,
	initscr();
	raw();
	start_color();
	init_pair(1,COLOR_GREEN,COLOR_BLACK);
	init_pair(2,COLOR_YELLOW,COLOR_BLACK);
	init_pair(3,COLOR_RED,COLOR_BLACK);
	init_pair(4,COLOR_CYAN,COLOR_BLACK);
	init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(6,COLOR_BLUE,COLOR_BLACK);
	init_pair(7,COLOR_WHITE,COLOR_BLACK);


	init_pair(8, COLOR_BLACK, COLOR_GREEN);
	init_pair(9,COLOR_BLACK,COLOR_YELLOW);
	init_pair(10,COLOR_BLACK,COLOR_RED);
	init_pair(11,COLOR_BLACK,COLOR_CYAN);
	init_pair(12,COLOR_BLACK,COLOR_MAGENTA);
	init_pair(13,COLOR_BLACK,COLOR_BLUE);
	init_pair(14,COLOR_BLACK,COLOR_WHITE);

	color=7;

	nodelay(stdscr,true); //getch will not wait until the user presses a key
	keypad(stdscr,true); //allow keypad and f-keys
	noecho(); // won't show the characters typed
	curs_set(0); //cursor invisible
	getmaxyx(stdscr,maxheight,maxwidth);

#ifdef _WIN32
	{
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode;
		GetConsoleMode(hStdin, &mode);
		SetConsoleMode(hStdin, mode & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_INSERT_MODE));
	}
#endif

}


snakeclass::snakeclass() {
	//default declarations
	partchar='x';
	borderchar=' ';
	foodchar='*';
	again=0;
	fruit=1;
	mode=normal;
	snakespeed = average;
}

snakeclass::~snakeclass() {

	nodelay(stdscr,false);
	getch();
	endwin();

}

void snakeclass::putmine() {
	while(1) {

		int tmpx=rand()%maxwidth+1;
		int tmpy=rand()%maxheight+1;

		if(tmpx>=maxwidth-2 || tmpy>=maxheight-3)
			continue;
		for(int i=0; i<snake.size(); i++)
			if(snake[i].x==tmpx && snake[i].y==tmpy)
				continue;
		for(int i=0; i<fruit; i++) {
			if(food[i].x==tmpx && food[i].y==tmpy)
				continue;
		}

		for(int i=0; i<mine.size(); i++) {
			if(mine[i].x==tmpx && mine[i].y==tmpx)
				continue;

		}
		mine.insert(mine.begin(),objectpart(tmpx,tmpy,1));
		break;

	}
	attron(COLOR_PAIR(8));
	mvaddch(mine[0].y,mine[0].x,borderchar);
	attroff(COLOR_PAIR(8));
	refresh();

}

void snakeclass::putfood(int x1, int y1, int x2, int y2, int num, int type) {
	while(1) {

		int tmpx=rand()%x2+x1+1;
		int tmpy=rand()%y2+y1+1;

		if(tmpx>=x2 || tmpy>=y2)
			continue;

		for(int i=0; i<snake.size(); i++)
			if(snake[i].x==tmpx && snake[i].y==tmpy)
				continue;

		for(int i=0; i<3; i++) {
			if(i!=num)
				if(food[i].x==food[num].x && food[i].y==food[num].x)
					continue;

		}
		if(mode==mines)
			for(int i=0; i<mine.size(); i++) {
				if(mine[i].x==tmpx && mine[i].y==tmpy)
					continue;

				else if(mode==ying)
					for(int i=0; i<snake2.size(); i++)
						if(snake2[i].x==tmpx && snake2[i].y==tmpy)
							continue;

			}

		food[num].x=tmpx;
		food[num].y=tmpy;
		food[num].type=type;
		break;
	}


	if(food[num].type==0)
		attron(COLOR_PAIR(color));
	else
		attron(COLOR_PAIR(color2));
	mvaddch(food[num].y,food[num].x,foodchar);

	refresh();
}

bool snakeclass::collision(int x2,int y2) {
	get=false;

	if(mode!=walls) {
		if(snake[0].x==0 || snake[0].x==maxwidth-1 || snake[0].y==0 || snake[0].y==maxheight-2)
			return true;
	}
	if(mode==mines) {
		for (int i=0; i<mine.size(); i++)
			if(snake[0].x==mine[i].x && snake[0].y==mine[i].y)
				return true;
	}

	if(mode==ying) {
		for (int i=0; i<snake.size(); i++)
			if(snake[0].x==snake2[i].x && snake[0].y==snake2[i].y)
				return true;
	}

	for (int i=4; i<snake.size(); i++)
		if(snake[0].x==snake[i].x && snake[0].y==snake[i].y)
			return true;

	for(int i=0; i<fruit; ++i) {
		if(snake[0].x==food[i].x && snake[0].y==food[i].y) {
			if(food[i].type!=0)
				return true;


			get=true;
			if(mode==ying)
				putfood(0,0,x2,y2,i,rand()%2);
			else
				putfood(0,0,x2,y2,i,0);
			score+=10;
			mvprintw(maxheight-1,0,"score: %d", score);
			if((score%30)==0)
				if(delay>40000) {
					if(delay<=100000)
						delay-=1500;
					else
						delay -=3000;
				}
			if(mode==mines)
				if((score%30)==0)
					putmine();
		} else if(mode==ying)
			if(snake2[0].x==food[i].x && snake2[0].y==food[i].y) {
				if(food[i].type!=1)
					return true;

				get=true;
				putfood(0,0,x2,y2,i,rand()%2);
				score+=10;
				mvprintw(maxheight-1,0,"score: %d", score);
				if((score%30)==0)
					if(delay>40000) {
						if(delay<=100000)
							delay-=1500;
						else
							delay -=3000;
					}
				if(mode==mines)
					if((score%30)==0)
						putmine();
			}
	}
	return false;
}

void snakeclass::movesnake() {
	int tmp=getch();
	switch(tmp) {
		case 'a' :
		case 'A':
		case KEY_LEFT  :
			if(direction!=right)
				direction= left ;
			break;
		case 'd':
		case 'D':
		case KEY_RIGHT:
			if(direction!=left)
				direction=right;
			break;
		case 'w':
		case 'W':
		case KEY_UP:
			if(direction!=down)
				direction=up;
			break;
		case 's':
		case 'S':
		case KEY_DOWN:
			if(direction!=up)
				direction=down;
			break;
		case KEY_BACKSPACE:
			quit = true;
			break;
	}

	if(!get) {
		move(snake[snake.size()-1].y,snake[snake.size()-1].x);
		addch(' ');

		snake.pop_back();
		if(mode==ying) {
			move(snake2[snake2.size()-1].y,snake2[snake2.size()-1].x);
			addch(' ');

			snake2.pop_back();
		}
	}

	switch(direction) {
		case left:
			snake.insert(snake.begin(),objectpart(snake[0].x-1,snake[0].y,0));
			break;
		case right:
			snake.insert(snake.begin(),objectpart(snake[0].x+1,snake[0].y,0));
			break;
		case up:
			snake.insert(snake.begin(),objectpart(snake[0].x,snake[0].y-1,0));
			break;
		case down:
			snake.insert(snake.begin(),objectpart(snake[0].x,snake[0].y+1,0));
			break;
	}
	if(mode==ying) {
		switch(direction) {
			case left:
				snake2.insert(snake2.begin(),objectpart(snake2[0].x+1,snake2[0].y,1));
				break;
			case right:
				snake2.insert(snake2.begin(),objectpart(snake2[0].x-1,snake2[0].y,1));
				break;
			case up:
				snake2.insert(snake2.begin(),objectpart(snake2[0].x,snake2[0].y+1,1));
				break;
			case down:
				snake2.insert(snake2.begin(),objectpart(snake2[0].x,snake2[0].y-1,1));
				break;
		}
	}

	if(mode==walls) {
		if(snake[0].x==0)
			snake[0].x=maxwidth-2;

		else if(snake[0].x==maxwidth-1)
			snake[0].x=1;

		else if(snake[0].y==0)
			snake[0].y=maxheight-3;

		else if(snake[0].y==maxheight-2)
			snake[0].y=1;
	}

	attron(COLOR_PAIR(color));
	mvaddch(snake[0].y,snake[0].x,partchar);
	if(mode==ying) {
		attron(COLOR_PAIR(color2));
		mvaddch(snake2[0].y,snake2[0].x,partchar);
	}
	attroff(COLOR_PAIR(color));
}


void snakeclass::drawbox(int x1, int y1, int x2, int y2, char borderchar) {
	attron(COLOR_PAIR(8));
	for(int i=x1; i<=x2; i++) {
		mvaddch(y1,i,borderchar);
		mvaddch(y2,i,borderchar);
	}

	for(int i=y1; i<=y2; i++) {
		mvaddch(i,x1,borderchar);
		mvaddch(i,x2,borderchar);
	}
	attroff(COLOR_PAIR(8));
	attron(COLOR_PAIR(color));
	mvprintw(maxheight-1,0,"score: %d",score);
	attroff(COLOR_PAIR(color));
}

void snakeclass::snakeimg(int y, int x) {
	attron(COLOR_PAIR(1));
	mvprintw(y+1,x,"          /^\\/^\\");
	mvprintw(y+2,x,"        _|__|  O|  ");
	mvprintw(y+3,x,"\\/     /~     \\_/ \\");
	mvprintw(y+4,x," \\____|__________/  \\");
	mvprintw(y+5,x,"        \\_______     \\");
	mvprintw(y+6,x,"                `\\     \\                 \\");
	mvprintw(y+7,x,"                  |     |                  \\");
	mvprintw(y+8,x,"                 /      /                    \\");
	mvprintw(y+9,x,"                /     /                       \\\\");
	mvprintw(y+10,x,"              /      /                         \\ \\");
	mvprintw(y+11,x,"             /     /                            \\  \\");
	mvprintw(y+12,x,"           /     /             _----_            \\   \\");
	mvprintw(y+13,x,"          /     /           _-~      ~-_         |   |");
	mvprintw(y+14,x,"         (      (        _-~    _--_    ~-_     _/   |");
	mvprintw(y+15,x,"          \\      ~-____-~    _-~    ~-_    ~-_-~    /");
	mvprintw(y+16,x,"            ~-_           _-~          ~-_       _-~");
	mvprintw(y+17,x,"               ~--______-~                ~-___-~ ");
	attroff(COLOR_PAIR(1));
}


void snakeclass::printmenu() {
	initialize();

	while(1==1) {

		if(again) {
			nodelay(stdscr,true);
			again=0;
			clear();
			start();
		} else

			mousemask(BUTTON1_CLICKED, NULL);

		attron(COLOR_PAIR(1));
		box(stdscr,0,0);

		attron(COLOR_PAIR(8));
		mvprintw( maxheight/6,maxwidth/6," Start game "); //12 characters
		mvprintw( maxheight/6 + 3,maxwidth/6," Settings   ");
		mvprintw( maxheight/6 + 6,maxwidth/6," Exit game  ");
		attroff(COLOR_PAIR(8));
		snakeimg((maxheight-17)/2, maxwidth/2); //17 lines

		int tmpch = getch();
		if(tmpch == KEY_MOUSE) {
			MEVENT event;
			if (getmouse(&event) == OK)
				if (event.bstate & BUTTON1_CLICKED) {
					if(event.x >= maxwidth/6 && event.x<=maxwidth/6+12 && event.y== maxheight/6 + 6 )//exit button
						break;


					else if(event.x >= maxwidth/6 && event.x<=maxwidth/6+12 && event.y== maxheight/6 + 3 ) { // settings button
						//clear();
						nodelay(stdscr,false);
						settings();
					} else if(event.x >= maxwidth/6&& event.x<=maxwidth/6+12 && event.y== maxheight/6) { //15-26 12 characters Start game button
						clear();
						nodelay(stdscr,true);

//						if(mode==infinite) {
//
//							startinf();
//						} else
						start();
					}
				}
		}

	}
	delwin(stdscr);
	endwin();

}

void snakeclass::settings() {

	settingsWin = create_newwin(settingsheight, settingswidth, (maxheight-settingsheight)/2, (maxwidth-settingswidth)/2);
	box(settingsWin, 0, 0);

	wattron(settingsWin,COLOR_PAIR(1));
	mvprintw((maxheight-settingsheight)/2-1,(maxwidth-8)/2,"SETTINGS");
	mvwprintw(settingsWin,1,settingswidth-3, "X");
	mvwprintw(settingsWin,2,(settingswidth-5)/2,"COLOR");
	mvwprintw(settingsWin,6,(settingswidth-5)/2,"SPEED");
	mvwprintw(settingsWin,10,(settingswidth-5)/2,"FRUIT");
	mvwprintw(settingsWin,14,(settingswidth-5)/2,"MODES");


	for(int i=8; i<=14; ++i) {
		wattron(settingsWin,COLOR_PAIR(i));
		mvwprintw(settingsWin,4,(settingswidth-26)/2+4*(i-8),"  ");
		wattroff(settingsWin,COLOR_PAIR(i));
	}


	while(1==1) {

		wattron(settingsWin,COLOR_PAIR(color));
		mvwprintw(settingsWin,3,(settingswidth-26)/2+4*(color-1),"\\/");




		wattron(settingsWin,COLOR_PAIR(14));
		if(snakespeed == slow)
			wattron(settingsWin,COLOR_PAIR(8));
		mvwprintw(settingsWin,8,(settingswidth-25)/2," Slow ");


		wattron(settingsWin,COLOR_PAIR(14));
		if(snakespeed == average)
			wattron(settingsWin,COLOR_PAIR(8));
		mvwprintw(settingsWin,8,(settingswidth-25)/2+9," Average ");

		wattron(settingsWin,COLOR_PAIR(14));
		if(snakespeed == fast)
			wattron(settingsWin,COLOR_PAIR(8));

		mvwprintw(settingsWin,8,(settingswidth-25)/2+21," Fast ");




		wattron(settingsWin,COLOR_PAIR(14));
		if(fruit==1)
			wattron(settingsWin,COLOR_PAIR(8));

		mvwprintw(settingsWin,12,(settingswidth-13)/2," 1 ");

		wattron(settingsWin,COLOR_PAIR(14));
		if(fruit==2)
			wattron(settingsWin,COLOR_PAIR(8));

		mvwprintw(settingsWin,12,(settingswidth-13)/2+5," 2 ");

		wattron(settingsWin,COLOR_PAIR(14));
		if(fruit==3)
			wattron(settingsWin,COLOR_PAIR(8));

		mvwprintw(settingsWin,12,(settingswidth-13)/2+10," 3 ");






		wattron(settingsWin,COLOR_PAIR(14));
		if(mode==normal)
			wattron(settingsWin,COLOR_PAIR(8));

		mvwprintw(settingsWin,16,(settingswidth-53)/2," NORMAL ");

		wattron(settingsWin,COLOR_PAIR(14));
		if(mode==walls)
			wattron(settingsWin,COLOR_PAIR(8));

		mvwprintw(settingsWin,16,(settingswidth-53)/2+10," NO WALLS ");

		wattron(settingsWin,COLOR_PAIR(14));
		if(mode==mines)
			wattron(settingsWin,COLOR_PAIR(8));

		mvwprintw(settingsWin,16,(settingswidth-53)/2+22," MINES ");

		wattron(settingsWin,COLOR_PAIR(14));
		if(mode==ying)
			wattron(settingsWin,COLOR_PAIR(8));

		mvwprintw(settingsWin,16,(settingswidth-53)/2+31," YING YANG ");

		wattron(settingsWin,COLOR_PAIR(14));
		if(mode==infinite)
			wattron(settingsWin,COLOR_PAIR(8));

		mvwprintw(settingsWin,16,(settingswidth-53)/2+44," INFINITE ");


		wattron(settingsWin,COLOR_PAIR(1));
		wrefresh(settingsWin);
		int tmpch=getch();



		if(tmpch == KEY_MOUSE) {
			MEVENT event;
			if (getmouse(&event) == OK)
				if (event.bstate & BUTTON1_CLICKED && wenclose(settingsWin, event.y, event.x)) {
					int winx = event.x-(maxwidth-settingswidth)/2;
					int winy = event.y-(maxheight-settingsheight)/2;

					if(winy == 4 ) {
						mvwprintw(settingsWin,3,(settingswidth-26)/2+4*(color-1),"  ");
						switch(winx) { //haha button cords no time to make makebutton() and isclicked()
							case 17:
							case 18:
								color=1;
								break;
							case 21:
							case 22:
								color=2;
								break;
							case 25:
							case 26:
								color=3;
								break;
							case 29:
							case 30:
								color=4;
								break;
							case 33:
							case 34:
								color=5;
								break;
							case 37:
							case 38:
								color=6;
								break;
							case 41:
							case 42:
								color=7;
								break;

						}

					}

					else if(winy == 8 ) {
						if(winx>=(settingswidth-25)/2 && winx<=(settingswidth-25)/2+5 ) //button cords T-T
							snakespeed=slow;

						else if(winx>=(settingswidth-25)/2+9 && winx<=(settingswidth-25)/2+17 )
							snakespeed=average;

						else if(winx>=(settingswidth-25)/2+21 && winx<=(settingswidth-25)/2+26 )
							snakespeed=fast;
					}

					else if(winy == 12 ) {
						if(winx>=(settingswidth-13)/2 && winx<=(settingswidth-13)/2+3 ) // button cords
							fruit=1;

						else if(winx>=(settingswidth-13)/2+5 && winx<=(settingswidth-13)/2+8)
							fruit=2;

						else if(winx>=(settingswidth-13)/2+10 && winx<=(settingswidth-13)/2+13 )
							fruit=3;






					} else if( winy == 16 ) {
						if(winx>=(settingswidth-53)/2 && winx<=(settingswidth-53)/2+8 ) //button cords
							mode=normal;

						else if(winx>=(settingswidth-53)/2+10 && winx<=(settingswidth-53)/2+20)
							mode=walls;

						else if(winx>=(settingswidth-53)/2+22 && winx<=(settingswidth-53)/2+29 )
							mode=mines;

						else if(winx>=(settingswidth-53)/2+31 && winx<=(settingswidth-53)/2+42 )
							mode=ying;


						else if(winx>=(settingswidth-53)/2+44 && winx<=(settingswidth-53)/2+53 )
							mode=infinite;




					}

//				int* winx = &event.x;
//				int* winy = &event.y;
					//wmouse_trafo(settingsWin, winx, winy, wenclose(settingsWin,event.y, event.x)); to_string isn't recognized for some reason
					if(event.x-(maxwidth-settingswidth)/2==settingswidth-3 && event.y-(maxheight-settingsheight)/2== 1 ) { //quit button
						clear();
						break;
					}
				}
		}
	}
	destroy_win(settingsWin);
}

void snakeclass::start() {
	score=0; //starting score score
	switch(snakespeed) {
		case slow:
			delay=180000;
			break;
		case average:
			delay = 150000;
			break;
		case fast:
			delay = 110000;
			break;
	}

	get=false;
	quit=false;
	srand (time(NULL));

	if(mode==infinite) {
		direction=up;
		drawbox(0,0, maxwidth/2, maxheight-2, borderchar);
		for(int i=0; i<5; i++)
			snake.push_back(objectpart(maxwidth/4,maxheight/2+i,0));
		for(int i=0; i<fruit; ++ i)
			putfood(0,0,maxwidth/2,maxheight-2,i,0);
	}


	else {
		drawbox(0,0, maxwidth-1, maxheight-2, borderchar);
		direction=right;
		for(int i=0; i<5; i++)
			snake.push_back(objectpart(10-i,5,0));
		for(int i=0; i<fruit; ++ i) {
			if(mode == ying) {
				int type =rand()%2;
				putfood(0,0,maxwidth-2,maxheight-3,i,type);
			} else
				putfood(0,0,maxwidth-2,maxheight-3,i,0);
		}

	}





	for(int i=0; i<snake.size(); i++) {
		attron(COLOR_PAIR(color));
		mvaddch(snake[i].y, snake[i].x, partchar);
		attroff(COLOR_PAIR(color));
	}

	if(mode==ying) {
		for(int i=0; i>-5; i--)
			snake2.push_back(objectpart(maxwidth-11-i,maxheight-7,1));

		if(color==7)
			color2=3;
		else
			color2=7;

		for(int i=0; i<snake2.size(); i++) {
			attron(COLOR_PAIR(color2));
			mvaddch(snake2[i].y, snake2[i].x, partchar);
		}
	}



	refresh();
	while(1) {
		
		bool coll;
		if(mode!=infinite)
		 coll =collision(maxwidth-2, maxheight-3);
		else
		 coll =collision(maxwidth/2, maxheight-3);
		if(coll||quit){
			nodelay(stdscr,false);

			snake.clear();
			snake2.clear();

			clear();

			mvprintw((maxheight-10)/2,(maxwidth-12)/2, "game over"); //manual cords how fun
			mvprintw((maxheight-10)/2+2,(maxwidth-12)/2, "score: %d", score);
			refresh();

			mvaddstr((maxheight-10)/2+6,(maxwidth-12)/2, "play again?");
			attron(COLOR_PAIR(8));
			mvprintw((maxheight-10)/2+9,(maxwidth-12)/2," yes ");
			mvprintw((maxheight-10)/2+9,(maxwidth-12)/2+7," no ");
			attroff(COLOR_PAIR(8));
			refresh();

			while(1) {

				int hcar = getch();
				if(hcar == KEY_MOUSE) {
					MEVENT event1;
					if (getmouse(&event1) == OK)
						if (event1.bstate & BUTTON1_CLICKED) {
							if(event1.x >= (maxwidth-11)/2 && event1.x<=(maxwidth-11)/2+4 && event1.y == (maxheight-10)/2+9 ) { //button yes isclicked would help so much here
								nodelay(stdscr,true);
								again=1;
								break;
							} else if(event1.x >= (maxwidth-11)/2+7 && event1.x<=(maxwidth-11)/2+7+3 && event1.y == (maxheight-10)/2+9) { //button no
								again=0;
								break;
							}
						}
				}
			}
			clear();
			break;

		}

		refresh();
		if(direction==up || direction==down)
			usleep(delay*2);
		else
			usleep(delay);
		if(mode==infinite)
			movefield();
		else
			movesnake();
	}
}

void snakeclass::movefield() {

	int tmp=getch();
	switch(tmp) {
		case 'a' :
		case 'A':
		case KEY_LEFT  :
			if(direction!=right)
				direction= left ;
			break;
		case 'd':
		case 'D':
		case KEY_RIGHT:
			if(direction!=left)
				direction=right;
			break;
		case 'w':
		case 'W':
		case KEY_UP:
			if(direction!=down)
				direction=up;
			break;
		case 's':
		case 'S':
		case KEY_DOWN:
			if(direction!=up)
				direction=down;
			break;
		case KEY_BACKSPACE:
			quit = true;
			break;
	}


	if(!get) {
		move(snake[snake.size()-1].y,snake[snake.size()-1].x);
		addch(' ');

		snake.pop_back();

	}



	switch(direction) {
		case left:
			for(int i=0; i<fruit; ++i) {
				mvaddch(food[i].y,food[i].x,' ');
				food[i].x+=1;


			}
			snake.insert(snake.begin(),objectpart(snake[0].x-1,snake[0].y,0));
			for (int i=0; i<snake.size(); i++) {
				mvaddch(snake[i].y,snake[i].x,' ');
				snake[i].x+=1;

			}
			break;
		case right:
			for(int i=0; i<fruit; ++i) {
				mvaddch(food[i].y,food[i].x,' ');
				food[i].x-=1;


			}
			snake.insert(snake.begin(),objectpart(snake[0].x+1,snake[0].y,0));
			for (int i=0; i<snake.size(); i++) {
				mvaddch(snake[i].y,snake[i].x,' ');
				snake[i].x-=1;

			}

			break;
		case up:
			for(int i=0; i<fruit; ++i) {
				mvaddch(food[i].y,food[i].x,' ');
				food[i].y+=1;


			}
			snake.insert(snake.begin(),objectpart(snake[0].x,snake[0].y-1,0));
			for (int i=0; i<snake.size(); i++) {
				mvaddch(snake[i].y,snake[i].x,' ');
				snake[i].y+=1;

			}
			break;
		case down:
			for(int i=0; i<fruit; ++i) {
				mvaddch(food[i].y,food[i].x,' ');
				food[i].y-=1;


			}
			snake.insert(snake.begin(),objectpart(snake[0].x,snake[0].y+1,0));
			for (int i=0; i<snake.size(); i++) {
				mvaddch(snake[i].y,snake[i].x,' ');
				snake[i].y-=1;
			}
			break;

	}

	for(int i=0; i<snake.size(); i++) {

		if(snake[i].x<=0)
			snake[i].x=maxwidth/2-snake[i].x-1;

		else if(snake[i].x>=maxwidth/2)
			snake[i].x=1+snake[i].x-maxwidth/2+1;


		else if(snake[i].y<=0)
			snake[i].y=maxheight-3-snake[i].y;

		else if(snake[i].y>=maxheight-2)
			snake[i].y=1+snake[i].y-maxheight+2;
	}

	for(int i=0; i<fruit; i++) {

		if(food[i].x==0)
			food[i].x=maxwidth/2-1;

		else if(food[i].x==maxwidth/2)
			food[i].x=1;


		else if(food[i].y==0)
			food[i].y=maxheight-3;

		else if(food[i].y==maxheight-2)
			food[i].y=1;
	}

	attron(COLOR_PAIR(color));
	for (int i=0; i<snake.size(); i++) {
		mvaddch(snake[i].y,snake[i].x,partchar);
	}
	for(int i=0; i<fruit; ++i) {
		mvaddch(food[i].y,food[i].x,foodchar);
	}
	attroff(COLOR_PAIR(color));
}


