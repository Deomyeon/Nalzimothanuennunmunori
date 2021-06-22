#include "Map.h"


Map::Map(const double seed, const int breadth, const int distance, const int length) : seed(seed), breadth(breadth), distance(distance), length(length), mapLength(0) {}

Map::~Map() {}


void Map::SetParameter(const double seed, const int breadth, const int distance, const int length)
{
	this->seed = seed;
	this->breadth = breadth;
	this->distance = distance;
	this->length = length;
}

void Map::MakeMap()
{
	int length = 0;
	do
	{
		this->map.clear();
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> Mdist(1, this->distance);
		std::uniform_int_distribution<int> Sdist(1, 4);
		std::stack<Vector2, std::vector<Vector2>> s;

		s.push(Vector2::zero);
		for (double random = Mdist(gen), num = this->seed; s.size() != 0; num = num * num)
		{
			int size = s.size();
			std::vector<Vector2> news;
			for (int si = 0; si < size; si++)
			{
				this->map.push_back(s.top());
				Vector2 top = s.top();
				s.pop();
				for (int i = 0; i < this->breadth; i++)
				{
					random = Mdist(gen);
					if (random <= this->distance - num)
					{
						Vector2 v = top;
						int count = 0;
						do
						{
							switch (Sdist(gen))
							{
							case 1:
								v = top + Vector2(-1, 0);
								break;
							case 2:
								v = top + Vector2(1, 0);
								break;
							case 3:
								v = top + Vector2(0, -1);
								break;
							case 4:
								v = top + Vector2(0, 1);
								break;
							}
							count++;
						} while ((std::find(this->map.begin(), this->map.end(), v) != this->map.end() || std::find(news.begin(), news.end(), v) != news.end()) && count < 25);
						news.push_back(v);
					}
				}
			}
			for (int i = 0; i < news.size(); i++)
			{
				s.push(news[i]);
			}
		}
		auto minmax = std::minmax_element(map.begin(), map.end(), [](Vector2 a, Vector2 b) -> bool { return a.x + a.y < b.x + b.y; });
		Vector2 distance = Vector2(abs((*minmax.second - *minmax.first).x), abs((*minmax.second - *minmax.first).y));
		length = sqrt(distance.x * distance.x + distance.y * distance.y);
		this->mapLength = length;
	} while (length < this->length);
}