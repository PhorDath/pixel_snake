#include <iostream>
//#include "Game.h"

#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>

using namespace std;

class Game : public olc::PixelGameEngine
{
private:
	vector<pair<int, int>> snake;
	pair<int, int> head;
	pair<int, int> tail;
	pair<int, int> food;
	int slp = 75;
	char dir = ' ';
	std::chrono::steady_clock::time_point time;
	bool gameOver = false;
	int score = 0;
	int dotSize = 1;

public:
	Game() {
		sAppName = "pixel snake";
	}

	bool OnUserCreate() override {
		// Called once at the start, so create things here
		head.first = 16;
		head.second = 16;

		snake.push_back(head);

		tail = head;

		food.first = rand() % ScreenHeight();
		food.second = rand() % ScreenWidth();

		// draw food
		FillRect(int(food.first), int(food.second), 1, 1, olc::YELLOW);

		// draw head
		FillRect(int(snake.front().first), int(snake.front().second), 1, 1, olc::RED);
		
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::GREY);

		for (int i = 0; i < ScreenHeight(); i+=2) {			
			DrawLine(i, 0, i, ScreenHeight(), olc::DARK_GREY);			
		}

		for (int i = 0; i < ScreenWidth(); i += 2) {
			DrawLine(0, i, ScreenWidth(), i, olc::DARK_GREY);
		}

		if (gameOver == true) {
			Clear(olc::GREY);

			for (int i = 0; i < ScreenHeight(); i += 2) {
				DrawLine(i, 0, i, ScreenHeight(), olc::DARK_GREY);
			}

			for (int i = 0; i < ScreenWidth(); i += 2) {
				DrawLine(0, i, ScreenWidth(), i, olc::DARK_GREY);
			}
			DrawString(5, 5, "GAME OVER\nFINAL SCORE IS\n" + to_string(score), olc::RED, 1);
			if (GetKey(olc::Key::ESCAPE).bPressed) {
				exit(1);
			}
			return true;
		}	

		DrawString(5, 5, "Score: " + to_string(score), olc::BLUE, 1);

		// user input
		if ((GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::UP).bHeld) && dir != 'd') {
			dir = 'u';
		}
		else if ((GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::DOWN).bHeld) && dir != 'u') {
			dir = 'd';
		}
		else if ((GetKey(olc::Key::LEFT).bPressed || GetKey(olc::Key::LEFT).bHeld) && dir != 'r') {
			dir = 'l';
		}
		else if ((GetKey(olc::Key::RIGHT).bPressed || GetKey(olc::Key::RIGHT).bHeld) && dir != 'l') {
			dir = 'r';
		}
		if (GetKey(olc::Key::ESCAPE).bPressed) {
			exit(1);
		}
		if (GetKey(olc::Key::SPACE).bPressed) {
			if (slp > 0 && score > 0) {
				slp -= 1;
				score-=5;
			}
		}
		if (GetKey(olc::Key::X).bPressed) {
			if (slp >= 15) {
				slp -= 6;
			}
		}
		if (GetKey(olc::Key::Z).bPressed) {
			if (slp < 200) {
				slp += 5;
			}
		}

		// if get food
		if (head.first == food.first && head.second == food.second) {

			head.first = food.first;
			head.second = food.second;

			snake.insert(snake.begin() + 0, head);

			// new food
			food.first = rand() % ScreenHeight();
			food.second = rand() % ScreenWidth();

			score += 100;
			slp -= 1;
		}
		
		// snake movement
		if (dir == 'u') {
			head = snake.front();
			head.second -= 1;

			if (head.second < 0) {
				head.second = ScreenHeight() - 1;
			}

			snake.insert(snake.begin() + 0, head);
			snake.pop_back();
		}
		else if (dir == 'd') {
			head = snake.front();
			head.second += 1;

			if (head.second >= ScreenHeight()) {
				head.second = 0;
			}

			snake.insert(snake.begin() + 0, head);
			snake.pop_back();
		}
		else if (dir == 'l') {
			head = snake.front();
			head.first -= 1;

			if (head.first < 0) {
				head.first = ScreenWidth() - 1;
			}

			snake.insert(snake.begin() + 0, head);
			snake.pop_back();
		}
		else if (dir == 'r') {
			head = snake.front();
			head.first += 1;

			if (head.first >= ScreenWidth()) {
				head.first = 0;
			}

			snake.insert(snake.begin() + 0, head);
			snake.pop_back();
		}

		// drwing food
		FillRect(int(food.first), int(food.second), 1, 1, olc::YELLOW);

		// drawing snake
		int i = 0;
		for (auto s : snake) {
			if (i % 2 == 0) {
				FillRect(int(s.first), int(s.second), 1, 1, olc::RED);
			}
			else {
				FillRect(int(s.first), int(s.second), 1, 1, olc::BLACK);
			}
			i++;
		}
		
		// collision
		if (snake.size() > 1) {
			for (int i = 1; i < snake.size(); i++) {
				if (snake.front().first == snake.at(i).first && snake.front().second == snake.at(i).second) {
					gameOver = true;
				}
			}
		}
		
		Sleep(slp);
		return true;
	}
};

int main()
{
	Game demo;
	if (demo.Construct(20, 20, 8, 8))
		demo.Start();

	return 0;
}
