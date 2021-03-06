#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>

unsigned int		ScreenX = 1200;
unsigned int		ScreenY = 800;

double				minR, maxR, minI, maxI;
int					maxN;

int findMandelbrot(double cr, double ci, int max_iterations) {
	int i = 0;
	double zr = 0.0f, zi = 0.0f;
	while (i < max_iterations && zr * zr + zi * zi < 4.0f) {
		double temp = zr * zr - zi * zi + cr;
		zi = 2.0f * zr * zi + ci;
		zr = temp;
		i++;
	}
	return i;
}

double mapToReal(int x, int imageWidth, double minR, double maxR) {
	double range = maxR - minR;
	return x * (range / imageWidth) + minR;
}

double mapToImaginary(int y, int imageHeight, double minI, double maxI) {
	double range = maxI - minI;
	return y * (range / imageHeight) + minI;
}

sf::Image			Mandelbrot() {
	sf::Image		image;
	
	unsigned int	r, g, b, a;
	double			cr, ci;
	int				n;

	image.create(ScreenX, ScreenY, sf::Color(0, 0, 0));

	for (unsigned int y = 0; y < ScreenY; y++)
	{
		for (unsigned int x = 0; x < ScreenX; x++)
		{
			cr = mapToReal(x, ScreenX, minR, maxR);
			ci = mapToImaginary(y, ScreenY, minI, maxI);
			n = findMandelbrot(cr, ci, maxN);

			r = n * static_cast<int>(std::sin(static_cast<double>(n)));
			r = (r % 256);
			g = ((n * n) % 256);
			b = (n % 256);
			a = 255;

			image.setPixel(x, y, sf::Color(r, g, b, a));
		}
	}
	return image;
}

int main()
{
	sf::RenderWindow	window(sf::VideoMode(ScreenX, ScreenY), "MandelBrot");
	sf::Clock			clock;
	sf::Texture			texture;
	sf::Sprite			sprite;

	double				zoomR = 0.1f;
	double				zoomC = 0.01f;

	bool				gen = false;

	minR = -2.0f;
	maxR = 1.0f;
	minI = -1.5f;
	maxI = 1.5f;

	maxN = 255;

	texture.loadFromImage(Mandelbrot());
	sprite.setTexture(texture);

	while (window.isOpen())
	{
		//EVENT NON DESTINES AU JEU
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::I) {
					zoomC *= 10.0f;
				}
				else if (event.key.code == sf::Keyboard::K) {
					zoomC *= 0.1f;
				}
				else if (event.key.code == sf::Keyboard::O) {
					zoomR += zoomC;
				}
				else if (event.key.code == sf::Keyboard::L) {
					zoomR -= zoomC;
				}
				else if (event.key.code == sf::Keyboard::P) {
					minR += zoomR;
					maxR -= zoomR;
					minI += zoomR;
					maxI -= zoomR;
					gen = true;
				}
				else if (event.key.code == sf::Keyboard::M) {
					minR -= zoomR;
					maxR += zoomR;
					minI -= zoomR;
					maxI += zoomR;
					gen = true;
				}
				else if (event.key.code == sf::Keyboard::Right) {
					minR += zoomR;
					maxR += zoomR;
					gen = true;
				}
				else if (event.key.code == sf::Keyboard::Left) {
					minR -= zoomR;
					maxR -= zoomR;
					gen = true;
				}
				else if (event.key.code == sf::Keyboard::Down) {
					minI += zoomR;
					maxI += zoomR;
					gen = true;
				}
				else if (event.key.code == sf::Keyboard::Up) {
					minI -= zoomR;
					maxI -= zoomR;
					gen = true;
				}

				std::cout << "minR:" << minR << " maxR:" << maxR << " minI:" << minI << " maxI:" << maxI << std::endl;
				std::cout << "zoomR:" << zoomR << " zoomC:" << zoomC << std::endl;

				if (gen == true) {
					texture.loadFromImage(Mandelbrot());
					sprite.setTexture(texture);
				}
				gen = false;
			}
		}

		window.draw(sprite);
		window.display();
	}

	return (0);
}
