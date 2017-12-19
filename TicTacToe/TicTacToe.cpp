#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <vector>
#include <time.h>       /* time */
#include <random>
#include "Connector.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <Windows.h>





using namespace sf;
using namespace std;


//measure performance

/*	auto start = chrono::steady_clock::now();
auto end = chrono::steady_clock::now();
double elapsed_time = double(chrono::duration_cast<chrono::nanoseconds>(end - start).count());*/


//board_status.board[old_pos_white_rook].state = 0;
//board_status.board[old_pos_black_rook].state = 0;
//board_status.board[new_pos_white_rook].state = 5;
//board_status.board[new_pos_black_rook].state = 5;
//
//
//board_status.board = board_status.update_board();






int main() {

	sf::RenderWindow window(sf::VideoMode(450, 450), "MattseTicTacToe!");
	sf::Vector2i window_pos;
	window_pos.x = 0;
	window_pos.y = 0;
	window.setPosition(window_pos);








	sf::Texture _t1;
	sf::Texture _t2;
	_t1.loadFromFile("images/whitefield.png");
	_t2.loadFromFile("images/blackfield.png");
	

	sf::Sprite board[5][5];


	//set sprites for fields
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			if ((i + j) % 2 == 0) {
				board[i][j] = sf::Sprite(_t1);
			}
			else {
				board[i][j] = sf::Sprite(_t2);
			}

			board[i][j].setPosition(i * 56, j * 56);

		}

	}

	


	//


	while (window.isOpen())
	{
		auto start = chrono::steady_clock::now();
		sf::Event e;
		


		while (window.pollEvent(e)) {

			if (e.type == sf::Event::Closed)
				window.close();
			break;

		}

		

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					window.draw(board[i][j]);
				}
			}
	










		auto end = chrono::steady_clock::now();
		double elapsed_time = double(chrono::duration_cast<chrono::nanoseconds>(end - start).count());
		//cout << "elapsed_time    " << elapsed_time << endl;



		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
		

		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
		

		}

		CloseConnection();
	}


}

