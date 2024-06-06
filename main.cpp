#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <random>
using namespace std;
enum KeyDef { NONE, UP, DOWN, LEFT, RIGHT, KEYDEF_COUNT = 8 };
enum ScreenSize { HEIGHT = 16, WIDTH = 16 };
const int tps = 8;
int DelayTime = (int)(1000 / tps);

struct vecd {
	int x = NULL, y = NULL;
	friend ostream& operator <<(ostream& out, const vecd& s) {
		out << s.x << "," << s.y;
		return out;
	};
	vecd operator+(vecd Input) {
		vecd out;
		out.x = x + Input.x;
		out.y = y + Input.y;
		return out;
	};
	vecd operator-(vecd Input) {
		vecd out;
		out.x = x - Input.x;
		out.y = y - Input.y;
		return out;
	};
	bool operator==(vecd Input) {
		return x == Input.x && y == Input.y;
	};
	bool operator!=(vecd Input) {
		return x != Input.x || y != Input.y;
	};
	vecd() {};
	vecd(int Inx, int Iny) {
		x = Inx;
		y = Iny;
	};
};

int HeadDirection = 0;
int SettingsMode = 0;
const int KeyConfig[KEYDEF_COUNT * 2] = { 'w',UP,'s',DOWN,'a',LEFT,'d',RIGHT };
int LastKey = NULL;
vector<vecd> Snake(1,vecd(0,0));
vecd Food(0, 0);
bool FoodEaten = true;
bool GameOver = false;
int Score = 0;

int MakeFood() {
	uniform_int_distribution<> distribx(0, WIDTH - 1), distriby(0, HEIGHT - 1);
	random_device rdx, rdy;
	mt19937 genx(rdx()), geny(rdy());
	Food = vecd(distribx(genx), distriby(geny));
	FoodEaten = false;
	return 0;
};

int Input() {
	int InKey;
	for (;;) {
		if (!_kbhit())
			return 0;
		InKey = _getch();
		if (InKey != LastKey)
			break;
	};
	LastKey = InKey;
	for (int i = 0; i < KEYDEF_COUNT; i++) {
		if(InKey==KeyConfig[i])
			HeadDirection = KeyConfig[i + 1];
		i++;
	};
	return InKey;
};

int Move() {
	vecd NextHead = Snake.front();
	switch (HeadDirection) {
	case UP:
		NextHead.y--;
		break;
	case DOWN:
		NextHead.y++;
		break;
	case LEFT:
		NextHead.x--;
		break;
	case RIGHT:
		NextHead.x++;
		break;
	};
	if (NextHead.x >= WIDTH)
		NextHead.x = NextHead.x - WIDTH;
	if (NextHead.y >= HEIGHT)
		NextHead.y = NextHead.y - HEIGHT;
	if (NextHead.x < 0)
		NextHead.x = NextHead.x + WIDTH;
	if (NextHead.y < 0)
		NextHead.y = NextHead.y + HEIGHT;
	Snake.insert(Snake.begin(), NextHead);
	if (NextHead != Food) {
		Snake.pop_back();
	}
	else {
		FoodEaten = true;
	};
	auto it = Snake.begin();
	for (it++; it != Snake.end(); it++) {
		if (NextHead == *it)
			GameOver = true;
	};
	return 0;
};

int Draw() {
	//Draw screen edge
	cout << "\033[0;0H";
	cout << "+";
	for (int i = 0; i < WIDTH; i++) {
		cout << "--";
	};
	cout << "+\n";
	for (int i = 0; i < HEIGHT; i++) {
		cout << "|";
		for (int j = 0; j < WIDTH; j++) {
			cout << "  ";
		};
		cout << "|\n";
	};
	cout << "+";
	for (int i = 0; i < WIDTH; i++) {
		cout << "--";
	};
	cout << "+\n";

	auto it = Snake.begin();
	vecd NowBody = *it;
	cout << "\033[" << (NowBody.y + 2) << ";" << (NowBody.x * 2 + 2) << "H" << "\033[42m  \033[0m";
	for (it++; it != Snake.end(); it++) {
		NowBody = *it;
		cout << "\033[" << (NowBody.y + 2) << ";" << (NowBody.x * 2 + 2) << "H" << "\033[47m  \033[0m";
	};
	cout << "\033[" << (Food.y + 2) << ";" << (Food.x * 2 + 2) << "H" << "()";
	

	return 0;
};

int main(int argc, char* argv[]) {
	/*
	//Get the handle of the default buffer
	HANDLE hOutput;
	COORD coord = { 0, 0 };
	DWORD bytes = 0;
	char data[3200];
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	//Create new buffer
	HANDLE hOutBuf = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	//Set the new buffer as the active buffer
	SetConsoleActiveScreenBuffer(hOutBuf);
	//隐藏两个缓冲区的光标
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(hOutput, &cci);
	SetConsoleCursorInfo(hOutBuf, &cci);
	*/
	system("cls");
	cout << "\033[?25l";

	for (;;) {
		if (FoodEaten == true)
			MakeFood();
		Input();
		Move();
		Draw();
		if (GameOver == true)
			break;

		//ReadConsoleOutputCharacterA(hOutput, data, 3200, coord, &bytes);
		//WriteConsoleOutputCharacterA(hOutBuf, data, 3200, coord, &bytes);

		Sleep(DelayTime);
	};
	cout << "\033[" << HEIGHT + 3 << ";0HGameOver";
	return 0;
};