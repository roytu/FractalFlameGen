#pragma once

#include <vector>
#include <time.h>
#include "Transform.h"
#include "Grid.h"
#include "include\SFML\Graphics.hpp"

std::vector<Transform> transformList;
std::vector<D4> gradient;

Transform getRandTransform()
{
	int r = rand() % transformList.size();
	return transformList.at(r);
}

void init()
{
	srand(time(NULL));

	transformList = std::vector<Transform>();

	Transform t1;
	t1.position = D2(-0.086824, 0.492404);
	t1.rotation = D1(2.6179938779914943653855361527329);
	t1.scale = D2(0.828535, 0.828535);
	t1.color = D1(0);
	transformList.push_back(t1);

	Transform t2;
	t2.position = D2(-0.09721, -0.359677);
	t2.rotation = D1(-0.29851238861485015918914345058845);
	t2.scale = D2(0.722768, 0.722768);
	t2.color = D1(0.5);
	transformList.push_back(t2);

	Transform t3;
	t3.position = D2(0.745943, -0.396373);
	t3.rotation = D1(0);
	t3.scale = D2(0.2, 0.2);
	t3.color = D1(1);
	transformList.push_back(t3);

	gradient = std::vector<D4>();
	gradient.push_back(D4(0, 72, 148, 90));
	gradient.push_back(D4(30, 56, 40, 9));
    gradient.push_back(D4(61, 42, 5, 73));
    gradient.push_back(D4(91, 105, 61, 29));
    gradient.push_back(D4(120, 33, 45, 104));
    gradient.push_back(D4(150, 96, 14, 18));
    gradient.push_back(D4(180, 30, 143, 128));
    gradient.push_back(D4(210, 19, 28, 193));
    gradient.push_back(D4(241, 69, 3, 18));
    gradient.push_back(D4(271, 129, 34, 89));
    gradient.push_back(D4(300, 21, 70, 226));
    gradient.push_back(D4(330, 39, 180, 194));
    gradient.push_back(D4(360, 36, 6, 64));
    gradient.push_back(D4(390, 63, 95, 87));
    gradient.push_back(D4(401, 73, 128, 64));
}

sf::Color getColor(float x)
{
	int minC = 0;
	int maxC = 0;
	for(int i = 0; i < gradient.size(); ++i)
	{
		D4 color = gradient.at(i);
		if(std::get<0>(color) > x * 400)
		{
			maxC = i;
			break;
		}
	}
	minC = maxC - 1;
	D4 minC4 = gradient.at(minC);
	D4 maxC4 = gradient.at(maxC);
	int minCI = std::get<0>(minC4);
	int maxCI = std::get<0>(maxC4);
	float interp = ((x * 400) - minCI) / (maxCI - minCI);
	sf::Uint8 r = (std::get<1>(maxC4) - std::get<1>(minC4)) * interp + std::get<1>(minC4);
	sf::Uint8 g = (std::get<2>(maxC4) - std::get<2>(minC4)) * interp + std::get<2>(minC4);
	sf::Uint8 b = (std::get<3>(maxC4) - std::get<3>(minC4)) * interp + std::get<3>(minC4);
	return sf::Color(r, g, b);
}

void drawPoints(std::vector<D3>* pointList)
{
	const int W = 800;
	const int H = 600;
	const int S = 200;

	sf::Image image;
	image.create(W, H);

	// Fill colorGrid for efficient drawing
	Grid<float> colorGrid = Grid<float>(W, H);
	for(int i = 20; i < pointList->size(); ++i) // Don't plot first 20 points
	{
		D3 point = pointList->at(i);
		int x = std::get<0>(point) * S + W / 2;
		int y = std::get<1>(point) * S + H / 2;
		float c = std::get<2>(point);

		if(0 <= x && x < W && 0 <= y && y < H)
		{
			colorGrid.add(x, y, c);
		}
	}

	// Max frequency count
	int maxFreq = 0;
	for(int x = 0; x < W; ++x)
	{
		for(int y = 0; y < H; ++y)
		{
			int s = colorGrid.get(x, y).size();
			if(s > maxFreq)
			{
				maxFreq = s;
			}
		}
	}

	// Draw pixels from colorGrid
	for(int x = 0; x < W; ++x)
	{
		for(int y = 0; y < H; ++y)
		{
			std::vector<float> colorList = colorGrid.get(x, y);
			if(colorList.size() > 0)
			{
				float c = 0;
				// TODO gamma correction
				for(int i = 0; i < colorList.size(); ++i)
				{
					c = (c + colorList.at(i)) / 2;
				}
				sf::Color color = getColor(c);
				D3 sumColor = D3(color.r, color.g, color.b);

				// Log-density postprocess adjustment
				const float GAMMA = 4; // Gamma: 1 = no gamma
				const float BRIGHTNESS = 2.2;
				//float alpha = pow(log((float)colorList.size()) / log((float)maxFreq), 1 / GAMMA);
				float alpha = pow(log((float)colorList.size() / log((float)maxFreq)), 1 / GAMMA);
				//float alpha = (float)colorList.size() / maxFreq;
				std::get<0>(sumColor) *= alpha * BRIGHTNESS;
				std::get<1>(sumColor) *= alpha * BRIGHTNESS;
				std::get<2>(sumColor) *= alpha * BRIGHTNESS;

				sf::Color finalColor = sf::Color(std::get<0>(sumColor), std::get<1>(sumColor), std::get<2>(sumColor));
				image.setPixel(x, y, finalColor);
			}
		}
	}
	image.saveToFile("thing.png");
}

void deinit()
{
	transformList.clear();
	gradient.clear();
}

int main()
{
	init();

	D3 point = D3(0, 0, 0);
	std::vector<std::vector<std::vector<sf::Color> > > colorGrid = std::vector<std::vector<std::vector<sf::Color> > >();
	std::vector<D3> pointList = std::vector<D3>();
	for(int i = 0; i < 1000000; ++i)
	{
		Transform trans = getRandTransform();
		point = trans.getNextPoint(point);
		// Ugly oversampling hack
		for(int dx = -1; dx <= 1; ++dx)
		{
			for(int dy = -1; dy <= 1; ++dy)
			{
				float x = std::get<0>(point);
				float y = std::get<1>(point);
				// TODO calculate based on pixel resolution and make this nonshitty
				const float RES = 0.0005; // Magic numbers!
				pointList.push_back(D3(x + (float)dx * RES, y + (float)dy * RES, std::get<2>(point)));
			}
		}
	}
	drawPoints(&pointList);

	deinit();
}