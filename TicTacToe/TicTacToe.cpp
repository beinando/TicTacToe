
#include <iostream>
#include <vector>



struct Connection {

	double weight;
	double delta_weight;

};


using namespace std;

#pragma once
class Neuron;
typedef vector<Neuron> Layer;

// ********************** class neuron **************
class Neuron {

public:
	Neuron(unsigned num_outputs);


private:

	double m_output_val;
	vector<Connection> m_output_weights;

};

// ********************** class neuron **************

class Net
{
public:
	Net(const vector<unsigned> &topology);
	//& : reference
	void feed_forward(const vector<double> &input_vals) {};
	void back_prop(const vector<double> &target_vals) {};
	void get_results(vector<double> &result_vals) const {};


private:
	vector<Layer> m_layers;
};

Net::Net(const vector<unsigned> &topology){



	unsigned num_layers = topology.size();
	for (unsigned layer_num = 0; layer_num < num_layers; layer_num++) {

		unsigned num_outputs = layer_num == topology.size() - 1 ? 0 : topology[layer_num + 1];

		m_layers.push_back(Layer());
		//We have made a new layer, now fill it with neurons, 
		//and a bias neuron to the layer:
		for (unsigned neuron_num = 0; neuron_num <= topology[layer_num]; neuron_num++) {

			m_layers.back().push_back(Neuron(num_outputs));
			cout << "made a neuron!" << endl;
		}

	}

}





int main() {

	
	vector<unsigned> topology;
	
	topology.push_back(3);
	topology.push_back(2);
	topology.push_back(1);
	Net my_net(topology);


	vector<double> input_vals;
	my_net.feed_forward(input_vals);

	const vector<double> target_vals;
	my_net.back_prop(target_vals);

	vector<double> result_vals;
	my_net.get_results(result_vals);
	

}

//
//
//sf::RenderWindow window(sf::VideoMode(450, 450), "MattseTicTacToe!");
//sf::Vector2i window_pos;
//window_pos.x = 0;
//window_pos.y = 0;
//window.setPosition(window_pos);
//
//
//
//
//
//
//
//
//sf::Texture _t1;
//sf::Texture _t2;
//_t1.loadFromFile("images/Field.png");
//_t2.loadFromFile("images/Field.png");
//sf::Texture _cross;
//sf::Texture _circle;
//_cross.loadFromFile("images/Cross.png");
//_circle.loadFromFile("images/Circle.png");
//
//
//sf::Sprite board[3][3];
//
//
////set sprites for fields
//for (int i = 0; i < 3; i++) {
//	for (int j = 0; j < 3; j++) {
//
//		if ((i + j) % 2 == 0) {
//			board[i][j] = sf::Sprite(_t1);
//		}
//		else {
//			board[i][j] = sf::Sprite(_t2);
//		}
//
//		board[i][j].setPosition(i * 150, j * 150);
//
//	}
//
//}
//
//
//sf::Sprite circle = sf::Sprite(_circle);
//sf::Sprite cross = sf::Sprite(_cross);
//circle.setPosition(150, 150);
//cross.setPosition(300, 300);
//
////
//
//
//while (window.isOpen())
//{
//	auto start = chrono::steady_clock::now();
//	sf::Event e;
//
//
//
//	while (window.pollEvent(e)) {
//
//		if (e.type == sf::Event::Closed)
//			window.close();
//		break;
//
//	}
//
//
//
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//			window.draw(board[i][j]);
//		}
//	}
//
//	window.draw(circle);
//	window.draw(cross);
//
//
//	window.display();
//	//window.close();
//
//	//win condition : 
//
//	/*	for (int i = 0; i < 3; i + 3) {
//
//	if (Field[i].status == Field[i + 1].status == Field[i + 2].status)
//	}
//	for (int i = 0; i < 3; i++) {
//	if(Field[i].status == Field[i+3].status == Field[i+6].status)
//
//
//
//	}
//
//	if (Field[0].status == Field[4].status == Field[8].status) {
//
//
//	}
//
//	if (Field[2].status == Field[4].status == Field[6].status) {
//
//
//	}*/
//
//
//
//	auto end = chrono::steady_clock::now();
//	double elapsed_time = double(chrono::duration_cast<chrono::nanoseconds>(end - start).count());
//	//cout << "elapsed_time    " << elapsed_time << endl;
//
//
//
//	if (Keyboard::isKeyPressed(Keyboard::Right))
//	{
//
//
//	}
//
//	if (Keyboard::isKeyPressed(Keyboard::Left))
//	{
//
//
//	}
//
//	CloseConnection();
//}
