#include <SFML/Graphics.hpp>
#define menu_x 300 
#define menu_y 110
#define size 21 
#define length 50
#define width 10
#define X 150
#define Y 30

using namespace sf;

struct Cell
{
public:
	bool is;//��������� �� � ������ �����/�����.
	bool show;//���������� �� �����/����.
	bool visited;
} maze_1[size][size], maze_2[size][size];

typedef struct point
{
public:  
	int y;
	int x;
} point;

void game_instruction(bool tp);//����� ����������, ��� ��������� �����
void menu(wchar_t* s1, wchar_t* s2, wchar_t* s3, int mode);//����� ����� ���� � ����� ���������� �� ����������� ����. ��������� ������ �������� ����
void choose();//��������� ���������� �������� ����� ��������������� � ����������� ��� ������ ������ ������
void game(bool tp);//�������, ��� �������� ����. ���� tp (two player) = 0, �� ��� �������������������� �����. ���� tp = 1, �� ��������������������
void play_op();//�������������������� �����
void play_tp();//�������������������� �����
void load(bool tp);//��������. tp = 0 - �������������������� �����, tp = 1 - �������������������� �����
void no_saves();//����� ��������� � ���, ��� ��� ����������
void save(bool tp);//����������. tp = 0 - �������������������� �����, tp = 1 - �������������������� �����
void generate(Cell maze[size][size]);//��������� ���������
void get_neighboar(point* neigboar, Cell maze[size][size], int x, int y);//��������������� ������� ����������. �������� ��������� ������ �������� � �������
void crash_the_wall(int y1, int x1, int y2, int x2, Cell maze[size][size]);//�������, ��������� ����� ����� ����� ��������
void make_maze(bool player);//������������ ������� ��� �������� ��������� � �������������������� ����
void show_labyrinth(Cell maze[size][size]);//����� ��������� �� �����
void show_player();//����� �������� �������� - ������
void show_score();//����� �����
void draw_borders();//����� ������ ���������
int check_walls(int y, int x, Cell maze[size][size]);//��������, ���� �� ����� � ����� ���������
void banner(bool *mode);//�������� � �����, ��� ������ �� ������

RenderWindow window(VideoMode(800, 600), "Invisible Labyrinth");//������� ���� ����������
bool m_menu = 1, op_menu = 0, tp_menu = 0, new_op_game = 0, load_op_game = 0, new_tp_game = 0, load_tp_game = 0, save_op = 0, save_tp = 0, game_over = 0;
Font font;
Text text;
int num_of_chosen_string = 1;
float moving_time = 0;
int first_maze[10][10] = { 0 }, second_maze[10][10] = { 0 };
bool maze_made = 0;
point player;
int score = 0;

enum menu_mods {MAIN_MENU, OP_MENU, TP_MENU};
void menu(wchar_t* s1, wchar_t* s2, wchar_t* s3, int mode)//0 - main_menu, 1 - op_menu, 2 - tp_menu
{
	text.setCharacterSize(16);
	text.setString(L"��� ������ ������� ����");
	text.setPosition(menu_x, 500);
	window.draw(text);

	text.setCharacterSize(32);

	text.setString(s1);
	text.setPosition(menu_x, menu_y);
	if (num_of_chosen_string == 1) 
		choose();
	else 
		window.draw(text);
		
	text.setString(s2);
	text.setPosition(menu_x, 2*menu_y);
	if (num_of_chosen_string == 2) 
		choose();
	else 
		window.draw(text);

	text.setString(s3);
	text.setPosition(menu_x, 3*menu_y);
	if 
		(num_of_chosen_string == 3) choose();
	else 
		window.draw(text);

	if (Keyboard::isKeyPressed(Keyboard::Return) && moving_time > 0.2)
	{
		moving_time = 0;

		switch (mode)
		{
		case 0:
			m_menu = 0;

			switch (num_of_chosen_string)
			{
			case 1:
				op_menu = 1;
				break;
			case 2:
				tp_menu = 1;
				break;
			case 3:
				window.close();
				break;
			default:
				break;
			}
			break;
		case 1:
			op_menu = 0;

			switch (num_of_chosen_string)
			{
			case 1:
				new_op_game = 1;
				break;
			case 2:
				load_op_game = 1;
				break;
			case 3:
				m_menu = 1;
				break;
			}
			break;
		case 2:
			tp_menu = 0;

			switch (num_of_chosen_string)
			{
			case 1:
				new_tp_game = 1;
				break;
			case 2:
				load_tp_game = 1;
				break;
			case 3:
				m_menu = 1;
				break;
			}
			break;
		}

		num_of_chosen_string = 1;
	}
}
void choose()
{
	RectangleShape rectangle;
	rectangle.setSize(Vector2f(225, 40));
	rectangle.setPosition(Vector2f(menu_x - 5, num_of_chosen_string*menu_y));

	text.setColor(Color::Black);
	window.draw(rectangle);
	window.draw(text);
	text.setColor(Color::White);
}

void game_instruction(bool tp)
{
	text.setCharacterSize(16);
	text.setString(L"������� S ��� ����������");
	text.setPosition(50, 550);
	window.draw(text);
	text.setString(L"������� ESCAPE ��� ������ � ����");
	text.setPosition(450, 550);
	window.draw(text);

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		maze_made = new_op_game = new_tp_game = false;
		op_menu = 0;
		m_menu = 1;
	}
	else if (Keyboard::isKeyPressed(Keyboard::S))
	{
		maze_made = new_op_game = new_tp_game = false;
		if (tp) save_tp = 1;
		else save_op = 1;
		op_menu = 0;
	}
}
void game(bool tp)
{		
	if (!maze_made)
	{
		if (!tp)
		{
			generate(maze_1);
		}
		else
		{
			make_maze(0);
			make_maze(1);
		}
		maze_made = true;
		player = { 1, 1 };
	}
	else
	{
		game_instruction(tp);
		show_labyrinth(maze_1);

		if (tp) play_tp();//TODO: ������� ���� ������� PLAY �� ��� ������
		else play_op();
	}
}
void play_op()
{
	int x, y;
	Clock clock;

	show_player();
	show_score();
	moving_time += clock.getElapsedTime().asSeconds();

	if (Keyboard::isKeyPressed(Keyboard::Right) && moving_time > 0.2)
	{
		if (!check_walls(player.y, player.x + 1, maze_1))
			player.x += 2;
		moving_time = 0;
	}
	if (Keyboard::isKeyPressed(Keyboard::Left) && moving_time > 0.2)
	{
		if (!check_walls(player.y, player.x - 1, maze_1))
			player.x -= 2;
		moving_time = 0;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) && moving_time > 0.1)
	{
		if (!check_walls(player.y - 1, player.x, maze_1))
			player.y -= 2;
		moving_time = 0;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down) && moving_time > 0.1)
	{
		if (!check_walls(player.y + 1, player.x, maze_1))
			player.y += 2;
		moving_time = 0;
	}
	if (player.x == 19 && player.y == 19)
	{
		player.x = 1; player.y = 1;
		maze_made = 0;
		game_over = 1;
		new_op_game = 0;
	}
}
int check_walls(int y, int x, Cell maze[size][size])
{
	if (maze[y][x].show)
	{
		if (maze[y][x].is)//������������ � ������� ������
			return 1;
		else//������� ������
			return 0;
	}
	else
	{
		if (maze[y][x].is)//��������� �����
		{
			score++;
			maze[y][x].show = true;
			return 1;
		}
		else//��������� ������
		{
			maze[y][x].show = true;
			return 0;
		}
	}
}
void win_screen()
{
	wchar_t str[] = L"����������! �� ������ ��������\n\n\t\t\t\t\t��� ����: ";
	char num[2] = { 0 };
	int last_figure = score % 10;
	
	sprintf(num, "%d", score);

	RectangleShape rectangle;
	rectangle.setSize(Vector2f(800, 600));
	rectangle.setPosition(Vector2f(0, 0));

	text.setCharacterSize(32);
	text.setPosition(120, 250);
	text.setString(str);
	text.setColor(Color::Black);
	window.draw(rectangle);
	window.draw(text);
	text.setCharacterSize(64);
	text.setString(num);
	text.setPosition(Vector2f(480, 300));
	window.draw(text);
	text.setColor(Color::White);

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		game_over = 0;
		m_menu = 1;
		score = 0;
	}
}
void play_tp()
{

}

void show_labyrinth(Cell maze[size][size])
{
	RectangleShape horizontal;
	RectangleShape vertical;
	horizontal.setSize(Vector2f(length + width, width));
	vertical.setSize(Vector2f(width, length));

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			if (!(i % 2 && j % 2) && (i % 2 || j % 2))//�� ������ � �� ����������� ����� ���� (1, 1) � (2, 2)
			{
				if (i % 2)//�������������� ����� (0, 1), (4, 3)...
				{
					if (maze[j][i].show)//TODO: MAZE[I][J].SHOW
						if (maze[j][i].is)
							horizontal.setFillColor(Color::White);
						else
							horizontal.setFillColor(Color::Black);
					else
						horizontal.setFillColor(Color::Cyan);

					horizontal.setPosition(Vector2f(X + (i - 1) * length / 2, Y + j * length / 2));
					if (horizontal.getFillColor() != Color::Black) window.draw(horizontal);
				}
				else if (j % 2)//������������ ����� (1, 0), (3, 4)...
				{
					if (maze[j][i].show)//TODO: MAZE[I][J].SHOW
						if (maze[j][i].is)
							vertical.setFillColor(Color::White);
						else
							vertical.setFillColor(Color::Black);
					else
						vertical.setFillColor(Color::Cyan);
					
					vertical.setPosition(Vector2f(X + i * length / 2, Y + (j - 1) * length / 2));
					if (vertical.getFillColor() != Color::Black) window.draw(vertical);
				}
			}
		}
	
	draw_borders();
}
void show_player()
{
	RectangleShape rectangle;
	rectangle.setSize(Vector2f(length - 2 * width, length - 2 * width));
	rectangle.setFillColor(Color::Red);
	rectangle.setPosition(Vector2f(X + (player.x / 2) * length + 15, Y + (player.y / 2) * length + 15));
	window.draw(rectangle);
}
void show_score()
{
	char str_score[2] = { 0 };
	sprintf(str_score, "%d", score);
	text.setString(str_score);
	text.setCharacterSize(64);
	text.setPosition(Vector2f(50, 250));
	window.draw(text);
}
void draw_borders()
{
	RectangleShape horizontal;
	RectangleShape vertical;
	horizontal.setSize(Vector2f(length + width, width));
	vertical.setSize(Vector2f(width, length));
	for (int i = X; i < X + length * 10; i += length)
	{
		horizontal.setPosition(i, Y);
		window.draw(horizontal);
		horizontal.setPosition(i, Y + length * 10);
		window.draw(horizontal);
	}
	for (int j = Y; j < Y + length * 10; j += length)
	{
		vertical.setPosition(X, j);
		window.draw(vertical);
		vertical.setPosition(X + length * 10, j);
		window.draw(vertical);
	}

	//���������� �����
	vertical.setSize(Vector2f(width, length - width));
	vertical.setPosition(X + length * 10, Y + length * 9 + width);
	vertical.setFillColor(Color::Black);
	window.draw(vertical);
}
void make_maze(bool player)
{

}
void generate(Cell maze[size][size])
{
	point unvisited_neighboar;
	srand(time(NULL));
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			if (i % 2 && j % 2)//(1, 1) - ������
				maze[i][j].visited = 0;
			else//�����
			{
				maze[i][j].show = 0;//�� ���� ����� �� �����
				maze[i][j].is = 1;//� ������ ����� ���� �����
				if (i == 0 || i == size - 1 || j == 0 || j == size - 1)//������� ���������
					maze[i][j].show = 1;
			}
		}

	int y = 1, x = 1;
	maze[y][x].visited = 1;
	point way[100];
	int w = 0;
	bool end = 0;

	way[w++] = { 1, 1 };
	
	do
	{
		get_neighboar(&unvisited_neighboar, maze, y, x);
		if (unvisited_neighboar.x)//���� ���� ������������ �����, �� ����� ������ ����� �������� ����� � ������� � ������
		{
			crash_the_wall(y, x, unvisited_neighboar.y, unvisited_neighboar.x, maze);
			y = unvisited_neighboar.y; x = unvisited_neighboar.x;
			maze[y][x].visited = 1;
			way[w++] = { y, x };
		}
		else if (w)//���� ���� �� �������� � ������ � ��� �������, �� ����� ��������� ����� �� ����, ���� �� ����� ������������ �������
		{
			while (!unvisited_neighboar.x && w)//���� ��� ������� � ���� ���� ���� - ������ �������
			{
				w--;
				y = way[w].y; x = way[w].x;
				get_neighboar(&unvisited_neighboar, maze, y, x);
			}
		}
		else end = 1;//���� ���� �������� � ������ � ��� ��� �������, �� �����
		
	} while (!end);

}
void get_neighboar(point* neigboar, Cell maze[size][size], int y, int x)
{
	point variants[4] = { 0 };
	int v = 0;
	
	neigboar->x = 0;

	if (x - 2 > 0 && !(maze[y][x - 2].visited))//���� ����� ���� ������ � ��� �� ��������
		variants[v++] = { y, x - 2 };
	if (y + 2 < size && !(maze[y + 2][x].visited))
		variants[v++] = { y + 2, x };
	if (x + 2 < size && !(maze[y][x + 2].visited))
		variants[v++] = { y, x + 2 };
	if (y - 2 > 0 && !(maze[y - 2][x].visited))
		variants[v++] = { y - 2, x };

	if (v)
		*neigboar = variants[rand() % v];
}
void crash_the_wall(int y1, int x1, int y2, int x2, Cell maze[size][size])
{
	if (x1 - x2)//����� ��� ������
	{
		if (x1 - x2 > 0)//�����
			maze[y1][x1-1].is = 0;
		else//������
			maze[y1][x1 + 1].is = 0;
	}
	else//������ ��� �����
	{
		if (y1 - y2 > 0)//������
			maze[y1 - 1][x1].is = 0;
		else//�����
			maze[y1 + 1][x1].is = 0;
	}
}

void load(bool tp)
{
	int i = 1;
	int order[10] = { 0 };
	char* name_of_saves[10] = { 0 };
	FILE* f;

	text.setCharacterSize(16);
	text.setString(L"�������� ����������");
	text.setPosition(menu_x, 50);
	window.draw(text);
	text.setString(L"������� ESCAPE ��� ������ � ����");
	text.setPosition(menu_x, 550);
	window.draw(text);

	text.setCharacterSize(20);

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		load_op_game = load_tp_game = 0;
		m_menu = 1;
	}

	if (tp)
	{
		if (!(f = fopen("0tp.txt", "r")))
			no_saves();
	}
	else
	{
		if (!(f = fopen("0op.txt", "r")))
			no_saves();
	}
}
void no_saves()
{
	text.setString(L"��� ����������");
	text.setPosition(menu_x, 100);
	window.draw(text);
}

void save(bool tp)
{

}

void banner(bool *mode)
{
	RectangleShape rectangle;
	rectangle.setSize(Vector2f(800, 600));
	rectangle.setPosition(Vector2f(0, 0));

	text.setCharacterSize(50);
	text.setPosition(20, 240);
	text.setString(L"������ �� ������, �������");
	text.setColor(Color::Black);
	window.draw(rectangle);
	window.draw(text);
	text.setColor(Color::White);

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		*mode = 0;
		m_menu = 1;
	}
}

int main()
{
	Clock clock;//���������� �������

	window.setVerticalSyncEnabled(true);// �������� ������������ ������������� (��� ������� ��������)

	font.loadFromFile("Arial.ttf");//��������� ������ � ���������� ���������� font
	text.setFont(font); //������������� ��� ���������� ���������� text ����� font

	setlocale(LC_CTYPE, "Russian");//������������� ������� ����

	while (window.isOpen())//���� ���� ��������� �������
	{
		Event event;//���������� �������
		while (window.pollEvent(event))//���� ���� ��������� �������, ����� �����������, ����� ������������ ����� �� ������ �������
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();//�������� ����
		
		moving_time += clock.getElapsedTime().asSeconds();//���������� ���������� ����������, ������� ������������ ��� ��������, ��� �������� ����� ��������� �������������
		clock.restart();//������ ��� �������� ������

		if (Keyboard::isKeyPressed(Keyboard::Up) && moving_time > 0.2)//���� ������ ������ "�����" � ������ >0.2 ������, ��...
		{
			num_of_chosen_string = num_of_chosen_string - 1;//������� �� ������� ������
			if (!num_of_chosen_string) num_of_chosen_string = 3;//���� ���� ������ ���, �� ������� �� ����� ������
			moving_time = 0;//�������� ����������� ����� ������
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down) && moving_time > 0.2) 
		{
			num_of_chosen_string = num_of_chosen_string % 3 + 1;//������� �� ������� ������, ���� ������ ���� ���, �� ������� ����� ������� ������
			moving_time = 0;//�������� ����������� ����� ������
		}

		//����� ������ �������� �����������. ��� ���������� ������ ���������
		if (m_menu) menu(L"���� �����", L"��� ������", L"�����", MAIN_MENU);//������� ����
		else if (op_menu)menu(L"����� ����", L"��������� ����", L"������� ����", OP_MENU);//����� ������ ������
		else if (tp_menu)menu(L"����� ����", L"��������� ����", L"������� ����", TP_MENU);//����� ���� ������� (�� �������)
		else if (new_op_game) game(0);//����� �������������������� ����
		else if (load_op_game) load(0);//�������� �������������������� ����
		else if (new_tp_game) banner(&new_tp_game);//����� �������������������� ����
		else if (load_tp_game) load(1);//�������� �������������������� ����
		else if (game_over) win_screen();//����� ��������� ����
		else if (save_op) banner(&save_op);//����� ���������� �������������������� ����
		else if (save_tp) banner(&save_tp);//����� ���������� �������������������� ����

		window.display();//���������� ����� � ����
	}

	return 0;
}
