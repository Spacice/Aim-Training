#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <fstream>
#include <Windows.h>

using namespace sf;
using namespace std;

void setRandom(CircleShape * obj, unsigned int maxX, unsigned int maxY) {
	maxX -= obj->getRadius() * 2;
	maxY -= obj->getRadius() * 2;
	obj->setPosition((rand() % maxX) + obj->getRadius(), (rand() % maxY) + obj->getRadius());
}

float getFPS(float gTime) {
	return 1000000 / gTime;
}

bool checkClick(CircleShape * obj, Vector2f mousePos) {
	if (sqrt(pow(obj->getPosition().x - mousePos.x, 2) + pow(obj->getPosition().y - mousePos.y, 2)) <= obj->getRadius())
		return 1;
	else return 0;
}

string getTime(float time) {
	return to_string(((int)(time / 60000))) + ":" + to_string(((int)(time / 1000) - (int)(time / 60000) * 60));
}

bool windowMode = 0;
unsigned int windowWidth = 640;
unsigned int windowHeight = 480;
unsigned int fps = 0;
unsigned int radius = 20;
unsigned short gamemode = 0;
unsigned int lifeTime = 1000;
unsigned int startTime = 0;
unsigned int prepTime = 5;
unsigned int rAim = 255;
unsigned int gAim = 255;
unsigned int bAim = 255;
unsigned int rText = 255;
unsigned int gText = 255;
unsigned int bText = 255;
unsigned int aText = 255;

void loadConfig() {
	ifstream cfg("config.txt");
	if (cfg.is_open()) {
		char str[32];
		cfg.getline(str, 32);
		windowMode = (bool)atoi(str);
		cfg.getline(str, 32);
		windowWidth = atoi(str);
		cfg.getline(str, 32);
		windowHeight = atoi(str);

		cfg.getline(str, 32);
		fps = atoi(str);
		cfg.getline(str, 32);
		radius = atoi(str);
		cfg.getline(str, 32);
		gamemode = atoi(str);
		cfg.getline(str, 32);
		lifeTime = atoi(str);
		cfg.getline(str, 32);
		startTime = atoi(str);
		cfg.getline(str, 32);
		prepTime = atoi(str);

		cfg.getline(str, 32);
		rAim = atoi(str);
		cfg.getline(str, 32);
		gAim = atoi(str);
		cfg.getline(str, 32);
		bAim = atoi(str);

		cfg.getline(str, 32);
		rText = atoi(str);
		cfg.getline(str, 32);
		gText = atoi(str);
		cfg.getline(str, 32);
		bText = atoi(str);
		cfg.getline(str, 32);
		aText = atoi(str);
		//text.setString("Config loaded.");
	}
	else {
		//text.setString("Failed to load config.");
	}
	cfg.close();

	if (windowMode > 1) {
		windowMode = 1;
	}
	if (windowWidth > 3840) {
		windowWidth = 3840;
	}
	if (windowHeight > 2160) {
		windowHeight = 2160;
	}
	if (gamemode > 2) {
		gamemode = 2;
	}
	if (rAim > 255) {
		rAim = 255;
	}
	if (gAim > 255) {
		gAim = 255;
	}
	if (bAim > 255) {
		bAim = 255;
	}
	if (rText > 255) {
		rText = 255;
	}
	if (gText > 255) {
		gText = 255;
	}
	if (bText > 255) {
		bText = 255;
	}
	if (aText > 255) {
		aText = 255;
	}
}

int main()
{
	unsigned int score = 0;
	float tempLifeTime = 0;
	float tempTime = 0;
	float tempPrepTime = 0;
	bool started = false;

	srand(time(0));
	ostringstream strText;
	Font font;
	Text text;
	Uint32 st;
	CircleShape shape;
	Clock gClock;
	SoundBuffer buffer;
	Sound sound;
	Image iCur;
	Texture tCur;
	Sprite sCur;

	buffer.loadFromFile("hit.wav");
	sound.setBuffer(buffer);
	font.loadFromFile("font.TTF");
	text.setCharacterSize(20);
	text.setFont(font);
	iCur.loadFromFile("cursor.png");
	iCur.createMaskFromColor(Color::Black);
	tCur.loadFromImage(iCur);
	sCur.setTexture(tCur);
	sCur.setOrigin(16, 16);
	
	loadConfig();
	tempPrepTime = prepTime * 1000;
	tempTime = startTime * 1000;

	if(windowMode)st = Style::Fullscreen;
	else st = Style::Default;

	RenderWindow window(VideoMode(windowWidth, windowHeight), "Aim Training", st);
	if (fps != 0)window.setFramerateLimit(fps);
	window.setMouseCursorVisible(false);

	shape.setRadius(radius);
	shape.setOrigin((float)radius, (float)radius);
	shape.setPosition(windowWidth / 2, windowHeight / 2);
	shape.setFillColor(Color(rAim, gAim, bAim));

	while (window.isOpen())
	{
		float gTime = gClock.getElapsedTime().asMicroseconds();
		gClock.restart();
		if (!started) {
			tempPrepTime -= gTime / 1000;
			if (tempPrepTime <= 0)started = true;
		}
		else {
			if (tempLifeTime >= lifeTime) {
				setRandom(&shape, windowWidth, windowHeight);
				tempLifeTime = 0;
			}
			else {
				tempLifeTime += gTime / 1000;
			}
			if (gamemode == 0 || gamemode == 1) {
				tempTime += gTime / 1000;
			}
		}
		sCur.setPosition(Vector2f(Mouse::getPosition(window)));
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed) {
				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					window.close();
				}
			}
			if (event.type == Event::MouseButtonPressed) {
				if(started)
				if(checkClick(&shape, Vector2f(Mouse::getPosition(window)))){
					tempLifeTime = 0;
					score += 1;
					setRandom(&shape, windowWidth, windowHeight);
					sound.play();
				}
			}
		}

		text.setFillColor(Color(rText, gText, bText, aText));

		window.clear();

		strText.str("");
		strText << "FPS: " << getFPS(gTime);
		text.setPosition(0, windowHeight - 22);
		text.setString(strText.str());
		window.draw(text);

		strText.str("");
		strText << "Score: " << score;
		text.setPosition(0, 0);
		text.setString(strText.str());
		window.draw(text);

		if (started) {
			strText.str("");
			strText << "Time: " << getTime(tempTime);
			text.setPosition(200, 0);
			text.setString(strText.str());
			window.draw(text);

			window.draw(shape);
		}
		else {
			strText.str("");
			strText << "Time: " << getTime(tempPrepTime + 1000);
			text.setFillColor(Color::Red);
			text.setPosition(200, 0);
			text.setString(strText.str());
			window.draw(text);
			text.setFillColor(Color::White);
		}
		window.draw(sCur);
		window.display();
	}

	return 0;
}