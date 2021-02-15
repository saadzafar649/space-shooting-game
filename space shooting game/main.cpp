#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <ctime>
using namespace sf;
bool planeIsMovingUP = false, planeIsMovingDown = false, planeIsMovingLeft = false, planeIsMovingRight =
false, is_firing = false;
static int enemy_spawn_rate = 40, fire_rate = 50, score = 0, score_rate = 1;
static float enemy_speed = 0.4, health = 10;
////////////////////////////////////Prototypes of functions//////////////////////////////////////////////////////
void stages();
void for_events(RenderWindow&, Event&, Sprite&, Sprite[], int&, Sound&);
void for_drawing(RenderWindow&, Sprite&, Sprite&, Sprite[], Sprite[], Sprite[], int&, int&, int&,
	RectangleShape[], Sprite[], int&, Text, Text&, Text&);
void plane_mov(Event&, Sprite&, Sprite[], int&);
void enemies_func(Sprite[], Sprite[], int&, int&, Sprite[], int&);
void plane_fire(Sprite[], int&);
void collision(Sprite&, Sprite[], Sprite[], Sprite[], int&, int&, int&, Sprite[], int&, Text&, Sound&, Sound&,
	Sound&);
void remove_sprite(Sprite[], int&, int&);
void similar_stage(RenderWindow&, Event&, Sprite&, Sprite[], Sprite[], Sprite[], int&, int&, int&,
	RectangleShape[], Sprite&, Sprite[], int&, Text, Text&, Text&, Sound&, Sound&, Sound&, Sound&);
// // /// ///////////// MAin Function ////////////////////////////////////////////////////
int main() {
	srand(time(0));
	while (true) {
		int select;
		system("cls");
		std::cout << "--------------------------------------------\n";
		std::cout << " SPACE SHOOTER \n";
		std::cout << "--------------------------------------------\n";
		std::cout << " Enter 1 to play stage mode\n";
		std::cout << " Enter 2 to exit.\n";
		std::cout << "--------------------------------------------\n";
		std::cout << " Enter your choice: ";
		std::cin >> select;
		std::cout << "--------------------------------------------\n";
		system("cls");
		switch (select) {
		case 1:
			stages();
			break;
		case 2:
			return 0;
		default:
			std::cout << "\nPlease select the correct option.";
			std::cin.clear();
			std::cin.ignore();
			break;
		}
	}
}
// // /// ///////////// Stages Function ////////////////////////////////////////////////////
void stages() {
	//declarations
	RenderWindow window(VideoMode(800, 600), "SPACE SHOOTER");
	Event event;
	int current_enemy = 0, current_enemy_fire = 0, current_my_fire = 0, current_gift = 0;
	Texture bg, pl_texture, enemies, enem_fire, mine_fire, gift_texture;
	Sprite background, plane, enemy[25], enemy_fire[40], my_fire[50], gift[2];
	RectangleShape health_bar[12];
	Font assigning_font;
	Text score_word, current_score0, stage_number, winlose;
	Music back;
	SoundBuffer fir, col, gif, heal, game_ov;
	Sound fire_sound, collision_sound, gift_sound, heal_sound, game_over_sound;
	/////////////////////////////////declarations end
	//sounds setting buffers
	fir.loadFromFile("fire.wav");
	col.loadFromFile("collision.wav");
	gif.loadFromFile("score.wav");
	heal.loadFromFile("heal.wav");
	game_ov.loadFromFile("gameover.wav");
	////////////////////////////////////
	fire_sound.setBuffer(fir);
	collision_sound.setBuffer(col);
	gift_sound.setBuffer(gif);
	heal_sound.setBuffer(heal);
	game_over_sound.setBuffer(game_ov);
	/////////////////////////////////////////////setting sound ends
	//loading textures from outside
	bg.loadFromFile("bg.jpg");
	pl_texture.loadFromFile("my_plane.png");
	enemies.loadFromFile("enemies.png");
	enem_fire.loadFromFile("fire_enemy.png");
	mine_fire.loadFromFile("fire_mine.png");
	gift_texture.loadFromFile("gift.png");
	assigning_font.loadFromFile("arial.ttf");
	////////////////////////////////////////////////loading files end
	//setting textures to the sprites
	background.setTexture(bg);
	plane.setTexture(pl_texture);
	// setting the score
	score_word.setFont(assigning_font);
	score_word.setString("Score: ");
	score_word.setPosition(5, 5);
	score_word.setCharacterSize(25);
	score_word.setFillColor(Color::Black);
	score_word.setStyle(Text::Bold);
	////////////////score num//////////////////
	current_score0.setFont(assigning_font);
	current_score0.setPosition(85, 5);
	current_score0.setCharacterSize(25);
	current_score0.setFillColor(Color::Black);
	current_score0.setStyle(Text::Bold);
	////////////////stage no//////////////////
	stage_number.setFont(assigning_font);
	stage_number.setPosition(340, 5);
	stage_number.setCharacterSize(30);
	stage_number.setFillColor(Color::Red);
	stage_number.setStyle(Text::Bold);
	//setting score ends
	//setting end screen
	winlose.setFont(assigning_font);
	winlose.setPosition(300, 200);
	winlose.setCharacterSize(2);
	winlose.setFillColor(Color::Green);
	winlose.setStyle(Text::Bold);
	///////////////////////////////////////////////setting end screen ends
	//setting texture to enemies
	for (int i = 0; i < 25; i++) {
		enemy[i].setTexture(enemies);
		enemy[i].setScale(0.3, 0.3);
	}
	///////////////////////////////////////////enemy texture ends
	//setting texture to enemies fires
	for (int i = 0; i < 40; i++) {
		enemy_fire[i].setTexture(enem_fire);
		enemy_fire[i].setScale(0.3, 0.3);
	}
	///////////////////////////////////////////enemy fire texture ends
	//setting texture to my plane fires
	for (int i = 0; i < 50; i++) {
		my_fire[i].setTexture(mine_fire);
		my_fire[i].setScale(0.5, 0.5);
	}
	////////////////////////////////////////////my plane fires texture ends
	//setting texture to my gifts
	for (int i = 0; i < 2; i++) {
		gift[i].setTexture(gift_texture);
		gift[i].setScale(0.13, 0.13);
	}
	///////////////////////////////////////////////my gift texture ends
	//filling health_bar with red color
	for (int i = 0; i < 12; i++) {
		health_bar[i].setSize({ 40,20 });
		health_bar[i].setFillColor(Color::Red);
		health_bar[i].setPosition(220 + i * 40, 570);
	}
	//health bar
	///////////////////////////////////////////////////////texture settings end
	//setting positions and scaling for initial use
	background.setPosition(0, 0);
	background.setScale(0.3, 0.3);
	plane.setPosition(400, 300);
	plane.setScale(0.4, 0.4);
	//setting positions end
	//game loop starts
	back.openFromFile("bg.wav");
	back.play();
	score = 60;
	health = 10;
	while (window.isOpen()) {
		similar_stage(window, event, plane, enemy, enemy_fire, my_fire, current_enemy_fire,
			current_my_fire, current_enemy, health_bar, background, gift, current_gift, score_word, current_score0,
			stage_number, fire_sound, collision_sound, gift_sound, heal_sound);
		if (score > 60 && current_enemy == 0 || health <= 0) {
			break;
		}
	}
	if (health <= 0) {
		winlose.setString(" OOOOOOOPPPPPPS!\n YOU LOSE");
	}
	else {
		winlose.setString("CONGRATULATIONS\n YOU WIN");
	}
	back.stop();
	game_over_sound.play();
	while (window.isOpen()) {
		window.pollEvent(event);
		if (Event::Closed == event.type)
			window.close();
		if (winlose.getCharacterSize() <= 50) {
			winlose.setCharacterSize(winlose.getCharacterSize() + 1);
			winlose.move(-2, 0);
			window.clear();
			window.draw(background);
			window.draw(winlose);
			window.display();
		}
	}
	//////////////////////////////game end
}
// // /// ///////////// for _ EVENTS ////////////////////////////////////////////////////
void for_events(RenderWindow& window, Event& event, Sprite& plane, Sprite my_fire[], int& current_my_fire,
	Sound& fire_sound) {
	while (window.pollEvent(event)) {
		switch (event.type) {
		case Event::KeyPressed:
			if (event.key.code == Keyboard::Up) {
				planeIsMovingUP = true;
			}
			else if (event.key.code == Keyboard::Down) {
				planeIsMovingDown = true;
			}
			else if (event.key.code == Keyboard::Left) {
				planeIsMovingLeft = true;
			}
			else if (event.key.code == Keyboard::Right) {
				planeIsMovingRight = true;
			}
			if (event.key.code == Keyboard::Space) {
				is_firing = true;
				fire_sound.play();
			}
			break;
		case Event::KeyReleased:
			if (event.key.code == Keyboard::Up) {
				planeIsMovingUP = false;
			}
			else if (event.key.code == Keyboard::Down) {
				planeIsMovingDown = false;
			}
			else if (event.key.code == Keyboard::Left) {
				planeIsMovingLeft = false;
			}
			else if (event.key.code == Keyboard::Right) {
				planeIsMovingRight = false;
			}
			break;
		case Event::Closed:
			window.close();
			break;
		}
	}
}
// // /// ///////////// for _ Drawing ////////////////////////////////////////////////////
void for_drawing(RenderWindow& window, Sprite& background, Sprite& plane, Sprite enemy[], Sprite
	enemy_fire[], Sprite my_fire[], int& current_enemy_fire, int& current_my_fire, int& current_enemy,
	RectangleShape health_bar[], Sprite gift[], int& current_gift, Text score_word, Text& current_score0, Text&
	stage_number) {
	window.clear();
	window.draw(background);
	for (int i = 0; i < current_enemy_fire; i++) {
		window.draw(enemy_fire[i]);
	}
	for (int i = 0; i < current_my_fire; i++) {
		window.draw(my_fire[i]);
	}
	for (int i = 0; i < current_enemy; i++) {
		window.draw(enemy[i]);
	}
	for (int i = 0; i < health; i++) {
		window.draw(health_bar[i]);
	}
	for (int i = 0; i < current_gift; i++) {
		window.draw(gift[i]);
	}
	window.draw(plane);
	window.draw(score_word);
	window.draw(current_score0);
	window.draw(stage_number);
	window.display();
}
// // /// ///////////// Plane Movement ////////////////////////////////////////////////////
void plane_mov(Event& event, Sprite& plane, Sprite my_fire[], int& current_my_fire) {
	static float x = 400, y = 300, speed = 0.7;
	if (planeIsMovingUP && y > 5) {
		y -= speed;
		plane.move(0, -speed);
	}
	if (planeIsMovingDown && y < 500) {
		y += speed;
		plane.move(0, speed);
	}
	if (planeIsMovingLeft && x > 0) {
		x -= speed;
		plane.move(-speed, 0);
	}
	if (planeIsMovingRight && x < 720) {
		x += speed;
		plane.move(speed, 0);
	}
	if (is_firing) {
		if (current_my_fire < 49) {
			my_fire[current_my_fire].setPosition(plane.getPosition().x + 33, plane.getPosition().y -
				13);
			current_my_fire++;
			if (score_rate != 1) {
				my_fire[current_my_fire].setPosition(plane.getPosition().x + 40,
					plane.getPosition().y - 13);
				current_my_fire++;
			}
		}
		is_firing = false;
	}
}
// // /// ///////////// Enemy fun ////////////////////////////////////////////////////
void enemies_func(Sprite enemy[], Sprite enemy_fire[], int& current_enemy, int& current_enemy_fire, Sprite gift[],
	int& current_gift) {

	static int tick_counter1, tick_counter2, count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0, count6 =
		0, fire_count = 0;
	static bool stage3 = false, left_motion = true, right_motion = false, enemy_remove = false;
	tick_counter1 = clock() / 50;
	if (tick_counter1 != tick_counter2) {
		tick_counter2 = tick_counter1;
		count2++;
		if (score <= 50) {
			count1++;
		}
		if (score > 20) {
			count3++;
		}
		if (score_rate == 3) {
			count4++;
		}
		if (score > 49) {
			count5++;
			count6++;
		}
	}
	//for enemy spawn stage 1 and 2.
	if (score < 50) {
		if (count1 == enemy_spawn_rate) {
			count1 = 0;
			if (current_enemy < 6) {
				enemy[current_enemy].setPosition((rand() % 600) + 50, -20);
				current_enemy++;
			}
		}
	}
	//for enemies move
	if (score < 50 || !stage3) {
		for (int i = 0; i < current_enemy; i++) {
			enemy[i].move(0, enemy_speed);
			if (enemy[i].getPosition().y >= 600) {
				remove_sprite(enemy, current_enemy, i);
			}
		}
	}
	//for loop to stop enemies in stage 3
	if (score >= 50 && !stage3) {
		for (int i = 0; i < current_enemy; i++) {
			if (enemy[i].getPosition().y >= 200) {
				stage3 = true;
			}
		}
	}
	//for stage 2 to 3 transformation
	if (!enemy_remove && score >= 50) {
		enemy_remove = true;
		for (int i = 0; i < current_enemy; i++) {
			remove_sprite(enemy, current_enemy, i);
		}
	}
	//////////////////////////////////////////////stage 3///////////////////////////////////////////
	if (score >= 50) {
		if (current_enemy == 0) {
			int pos = rand() % 300 + 50;
			for (int i = 0; i < 8; i++) {
				enemy[current_enemy].setPosition(pos + i * 38, -20);
				enemy[current_enemy + 8].setPosition(pos + i * 38, -58);
				enemy[current_enemy + 16].setPosition(pos + i * 38, -95);
				current_enemy++;
			}
			current_enemy = 24;
		}
		//enemy move left or right on stage 3
		if (stage3) {
			for (int i = 0; i < current_enemy; i++) {
				if (enemy[i].getPosition().x <= 20) {
					left_motion = true;
				}
				if (enemy[i].getPosition().x >= 680) {
					left_motion = false;
				}
				if (left_motion) {
					enemy[i].move(enemy_speed, 0);
				}
				else {
					enemy[i].move(-enemy_speed, 0);
				}
			}
		}
		if (count6 == 4) {
			count6 = 0;
			if (fire_count >= current_enemy - 1) {
				fire_count = 0;
			}
			enemy_fire[current_enemy_fire].setPosition(enemy[fire_count].getPosition().x + 17.5,
				enemy[fire_count].getPosition().y + 41);
			current_enemy_fire++;
			fire_count++;
		}
	}
	////////////////////////////////////////////stage 3 end/////////////////////////////////////////
	//for enemies fires
	if (count2 == fire_rate && score < 50) {
		count2 = 0;
		for (int i = 0; i < current_enemy; i++) {
			enemy_fire[current_enemy_fire].setPosition(enemy[i].getPosition().x + 17.5,
				enemy[i].getPosition().y + 41);
			current_enemy_fire++;
		}
	}
	//for enemy fire moves
	for (int i = 0; i < current_enemy_fire; i++) {
		enemy_fire[i].move(0, 2);
		if (enemy_fire[i].getPosition().y >= 600) {
			remove_sprite(enemy_fire, current_enemy_fire, i);
		}
	}
	////gifts
	//gift spawn
	if (score > 20) {
		if (count3 == 60) {
			count3 = 0;
			if (current_gift < 1) {
				gift[current_gift].setPosition((rand() % 600) + 50, -20);
				gift[current_gift + 1].setPosition((rand() % 600) + 50, -20);
				current_gift += 2;
			}
		}
		//for gift move
		for (int i = 0; i < current_gift; i++) {
			gift[i].move(0, enemy_speed);
			if (gift[i].getPosition().y >= 600) {
				remove_sprite(gift, current_gift, i);
			}
		}
	}
	//score remove
	if (score_rate == 3) {
		if (count4 == 80) {
			count4 = 0;
			score_rate = 1;
		}
	}
}
// // /// ///////////// plane Fire ////////////////////////////////////////////////////
void plane_fire(Sprite my_fire[], int& current_my_fire) {
	for (int i = 0; i < current_my_fire; i++) {
		my_fire[i].move(0, -2);
		if (my_fire[i].getPosition().y < -20) {
			my_fire[i].setPosition(my_fire[current_my_fire - 1].getPosition());
			my_fire[current_my_fire - 1].setPosition(900, 900);
			current_my_fire--;
		}
	}
}
// // /// ///////////// Check Collisions ////////////////////////////////////////////////////
void collision(Sprite& plane, Sprite enemy[], Sprite enemy_fire[], Sprite my_fire[], int& current_enemy, int&
	current_enemy_fire, int& current_my_fire, Sprite gift[], int& current_gift, Text& current_score0, Sound&
	collision_sound, Sound& gift_sound, Sound& heal_sound) {
	//for enemy collision with plane
	for (int i = 0; i < current_enemy; i++) {
		if (enemy[i].getGlobalBounds().intersects(plane.getGlobalBounds())) {
			remove_sprite(enemy, current_enemy, i);
			health--;
			collision_sound.play();
		}
	}
	//for my fire collision with enemy
	for (int i = 0; i < current_enemy; i++) {
		for (int j = 0; j < current_my_fire; j++) {
			if (enemy[i].getGlobalBounds().intersects(my_fire[j].getGlobalBounds())) {
				remove_sprite(enemy, current_enemy, i);
				remove_sprite(my_fire, current_my_fire, j);
				score += score_rate;
				collision_sound.play();
			}
		}
	}
	// for sfml displaying update
	current_score0.setString(std::to_string(score));
	//for enemy fire collission with my plane
	for (int i = 0; i < current_enemy; i++) {
		if (enemy_fire[i].getGlobalBounds().intersects(plane.getGlobalBounds())) {
			remove_sprite(enemy_fire, current_enemy_fire, i);
			health--;
			collision_sound.play();
		}
	}
	//for gift collision with plane.
	for (int i = 0; i < current_gift; i++) {
		if (gift[i].getGlobalBounds().intersects(plane.getGlobalBounds())) {
			remove_sprite(gift, current_gift, i);
			if (i == 0) {
				score_rate = 3;
				gift_sound.play();
			}
			if (i == 1 && health < 10) {
				health++;
				heal_sound.play();
			}
		}
	}
}
// // /// ///////////// Remeove Sprities ////////////////////////////////////////////////////
void remove_sprite(Sprite thing[], int& total_amount, int& current) {
	thing[current].setPosition(thing[total_amount - 1].getPosition());
	thing[total_amount - 1].setPosition(100, 100);
	total_amount--;
}
// // /// ///////////// Similar Stages like 1 and 2 ////////////////////////////////////////////////////
void similar_stage(RenderWindow& window, Event& event, Sprite& plane, Sprite enemy[], Sprite enemy_fire[],
	Sprite my_fire[], int& current_enemy_fire, int& current_my_fire, int& current_enemy, RectangleShape health_bar[], Sprite& background, Sprite gift[], int& current_gift, Text score_word, Text& current_score0, Text& stage_number, Sound& fire_sound, Sound& collision_sound, Sound& gift_sound, Sound& heal_sound) {
	for_events(window, event, plane, my_fire, current_my_fire, fire_sound);
	plane_mov(event, plane, my_fire, current_my_fire);
	enemies_func(enemy, enemy_fire, current_enemy, current_enemy_fire, gift, current_gift);
	plane_fire(my_fire, current_my_fire);
	collision(plane, enemy, enemy_fire, my_fire, current_enemy, current_enemy_fire, current_my_fire, gift,
		current_gift, current_score0, collision_sound, gift_sound, heal_sound);
	if (score < 21) {
		stage_number.setString("Stage no 1");
	}
	else if (score > 20 && score < 50) {
		stage_number.setString("Stage no 2");
		stage_number.setFillColor(Color::Black);
	}
	else if (score >= 50) {
		stage_number.setString("Stage no 3");
		stage_number.setFillColor(Color::White);
	}
	for_drawing(window, background, plane, enemy, enemy_fire, my_fire, current_enemy_fire,
		current_my_fire, current_enemy, health_bar, gift, current_gift, score_word, current_score0, stage_number);
}