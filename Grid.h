#pragma once

#include <vector>

template<class T> class Grid
{
public:
	int W;
	int H;

	std::vector<std::vector<std::vector<T> > > grid;

	Grid(int w, int h)
	{
		W = w;
		H = h;
		grid = std::vector<std::vector<std::vector<T> > >();
		for(int x = 0; x < W; ++x)
		{
			std::vector<std::vector<T> > r = std::vector<std::vector<T> >();
			for(int y = 0; y < H; ++y)
			{
				r.push_back(std::vector<T>());
			}
			grid.push_back(r);
		}
	}
	std::vector<T> get(int x, int y)
	{
		return grid.at(x).at(y);
	}
	void add(int x, int y, T value)
	{
		grid.at(x).at(y).push_back(value);
	}
};