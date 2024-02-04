#include <SFML\Graphics.hpp>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

const int n_y = 25;  // Size of Maze Vertically
const int n_x = n_y; // Size of Maze Horizontally

struct pos {
  int x;
  int y;
};

int N = 0x1;
int S = 0x2;
int W = 0x4;
int E = 0x8;

int START = 0x10;
int END = 0x20;
int CURRENT = 0x40;
int VISITED = 0x80;

int grid[n_y][n_x] = {0};

vector<pair<int, int>> frontier;

sf::Color lerpColor(const sf::Color &color1, const sf::Color &color2, float t) {
  return sf::Color(static_cast<sf::Uint8>((1 - t) * color1.r + t * color2.r),
                   static_cast<sf::Uint8>((1 - t) * color1.g + t * color2.g),
                   static_cast<sf::Uint8>((1 - t) * color1.b + t * color2.b));
}

int validDirs(int x, int y) {
  int validDirections = N | S | W | E;

  if (x >= n_x - 1 || grid[y][x + 1] != 0) {

    validDirections &= ~E;
  }
  if (x <= 0 || grid[y][x - 1] != 0) {
    validDirections &= ~W;
  }
  if (y <= 0 || grid[y - 1][x] != 0) {
    validDirections &= ~N;
  }
  if (y >= n_y - 1 || grid[y + 1][x] != 0) {
    validDirections &= ~S;
  }

  return validDirections;
}

std::pair<float, float> lerpPair(const std::pair<float, float> &start,
                                 const std::pair<float, float> &end, float t) {
  float lerpedFirst = start.first + t * (end.first - start.first);
  float lerpedSecond = start.second + t * (end.second - start.second);

  std::cout << lerpedFirst << " " << lerpedSecond << "\n\n";
  return std::make_pair(lerpedFirst, lerpedSecond);
}

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 800), "Maze Generator");

  pos start;
  start.x = rand() % n_x;
  start.y = rand() % n_y;

  grid[start.y][start.x] |= START;

  pos end;
  /*end.x = rand()%n_x;
  end.y = rand()%n_y;

  while (end.x == start.x && end.y == start.y)
  {
      pos end;
      end.x = rand()%n_x;
      end.y = rand()%n_y;
  }*/

  bool backtracking = false;

  vector<pair<int, int>> backtracks;

  int frameCount = 0;
  int updateFrame = 2;

  float lerpfactor = 0.1f;

  pair current(start.x, start.y);
  pair currentbox((float)start.x, (float)start.y);

  frontier.push_back(pair<int, int>(start.x, start.y));

  // window.setFramerateLimit(60);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();

    frameCount++;

    if (frontier.size() > 0 && frameCount >= updateFrame) {
      frameCount = 0;

      int directions = validDirs(current.first, current.second);

      if (directions != 0) {
        backtracking = false;

        int count = 0;

        count += (directions & N) % 1;
        count += (directions & S) % 1;
        count += (directions & W) % 1;
        count += (directions & E) % 1;

        int direction = pow(2, (rand() % (4)));

        while ((directions & direction) == 0) {
          direction = pow(2, (rand() % (4)));
        }

        if ((direction & N) != 0) {
          grid[current.second][current.first] &= ~N;
          grid[current.second - 1][current.first] &= ~S;

          grid[current.second][current.first] &= ~CURRENT;

          current = pair(current.first, current.second - 1);

          grid[current.second][current.first] |= CURRENT;
          grid[current.second][current.first] |= VISITED;

          frontier.push_back(pair<int, int>(current.first, current.second));

          grid[current.second][current.first] |= N;
          grid[current.second][current.first] |= W;
          grid[current.second][current.first] |= E;
        } else if ((direction & S) != 0) {
          grid[current.second][current.first] &= ~S;
          grid[current.second + 1][current.first] &= ~N;

          grid[current.second][current.first] &= ~CURRENT;

          current = pair(current.first, current.second + 1);

          grid[current.second][current.first] |= CURRENT;
          grid[current.second][current.first] |= VISITED;

          frontier.push_back(pair<int, int>(current.first, current.second));

          grid[current.second][current.first] |= S;
          grid[current.second][current.first] |= W;
          grid[current.second][current.first] |= E;
        } else if ((direction & W) != 0) {
          grid[current.second][current.first] &= ~W;
          grid[current.second][current.first - 1] &= ~E;

          grid[current.second][current.first] &= ~CURRENT;

          current = pair(current.first - 1, current.second);

          grid[current.second][current.first] |= CURRENT;
          grid[current.second][current.first] |= VISITED;

          frontier.push_back(pair<int, int>(current.first, current.second));

          grid[current.second][current.first] |= N;
          grid[current.second][current.first] |= W;
          grid[current.second][current.first] |= S;
        } else if ((direction & E) != 0) {
          grid[current.second][current.first] &= ~E;
          grid[current.second][current.first + 1] &= ~W;

          grid[current.second][current.first] &= ~CURRENT;

          current = pair(current.first + 1, current.second);

          grid[current.second][current.first] |= CURRENT;
          grid[current.second][current.first] |= VISITED;

          frontier.push_back(pair<int, int>(current.first, current.second));

          grid[current.second][current.first] |= N;
          grid[current.second][current.first] |= E;
          grid[current.second][current.first] |= S;
        }

        lerpfactor = 0.0f;
      } else {

        /*if (endChosen == false)
        {
            bool zero = false;

            for (int i = 0; i < n_y; i++)
            {
                for (int j = 0; j < n_x; j++)
                {
                    if (grid[i][j] == 0)
                    {
                        zero = true;
                    }
                }
            }
            if (zero == false)
            {
                end.x = frontier.back().first;
                end.y = frontier.back().second;

                grid[end.y][end.x] |= END;

                endChosen = true;
            }

        }*/

        if (backtracking == false) {
          backtracks.push_back(
              pair(frontier.back().first, frontier.back().second));
          backtracking = true;
        }

        grid[current.second][current.first] &= ~CURRENT;

        frontier.pop_back();

        if (frontier.size() > 0) {
          pair<int, int> pos = frontier.back();
          current = pair(pos.first, pos.second);

          grid[current.second][current.first] |= CURRENT;
        }

        lerpfactor = 0.0f;
      }
    }

    if (frontier.size() == 0 && backtracks.size() > 0) {
      int longindex = 0;
      int longest = 0;

      for (int i = 0; i < backtracks.size(); i++) {
        std::cout << i;
        int distance = abs(start.x - backtracks[i].first) +
                       abs(start.y - backtracks[i].second);

        if (distance > longest) {
          longest = distance;
          longindex = i;
        }
      }

      grid[backtracks[longindex].second][backtracks[longindex].first] |= END;

      end.x = backtracks[longindex].first;
      end.y = backtracks[longindex].second;

      backtracks.clear();
    }

    for (int i = 0; i < n_y; i++) {
      for (int j = 0; j < n_x; j++) {
        sf::Color color(35, 34, 38);
        sf::Color color2(20, 20, 20);

        if (grid[i][j] & START) {
          color = sf::Color(103, 184, 120);
        }
        if (grid[i][j] & END) {
          color = sf::Color(109, 121, 201);
        }
        /*if (grid[i][j] & CURRENT)
        {
            color = sf::Color(109, 250, 201);
        }*/

        sf::RectangleShape rect(sf::Vector2f(800.0f / n_x, 800.0f / n_x));
        rect.setFillColor(color);
        rect.setPosition(sf::Vector2f((800.0f / n_x) * j, (800.0f / n_x) * i));
        window.draw(rect);

        if ((grid[i][j] & N) != 0) {

          sf::RectangleShape rectl(
              sf::Vector2f((800.0f / n_x), (800.0f / n_x) / 5));
          rectl.setFillColor(color2);

          rectl.setPosition(
              sf::Vector2f(((800.0f / n_x) * j),
                           ((800.0f / n_x) * i) - (800.0f / n_x) / 10));
          window.draw(rectl);
        }

        if ((grid[i][j] & S) != 0) {

          sf::RectangleShape rectl(
              sf::Vector2f((800.0f / n_x), (800.0f / n_x) / 5));
          rectl.setFillColor(color2);

          rectl.setPosition(
              sf::Vector2f(((800.0f / n_x) * j),
                           ((800.0f / n_x) * (i + 1)) - (800.0f / n_x) / 10));
          window.draw(rectl);
        }

        if ((grid[i][j] & W) != 0) {

          sf::RectangleShape rectl(
              sf::Vector2f((800.0f / n_x) / 5, (800.0f / n_x)));
          rectl.setFillColor(color2);

          rectl.setPosition(
              sf::Vector2f(((800.0f / n_x) * j) - (800.0f / n_x) / 10,
                           ((800.0f / n_x) * i)));
          window.draw(rectl);
        }

        if ((grid[i][j] & E) != 0) {

          sf::RectangleShape rectl(
              sf::Vector2f((800.0f / n_x) / 5, (800.0f / n_x)));
          rectl.setFillColor(color2);

          rectl.setPosition(
              sf::Vector2f(((800.0f / n_x) * (j + 1)) - (800.0f / n_x) / 10,
                           ((800.0f / n_x) * i)));
          window.draw(rectl);
        }
      }
    }

    if (lerpfactor < 1.1f) {
      std::pair<float, float> lerpedPair =
          lerpPair(currentbox, current, lerpfactor);

      currentbox.first = lerpedPair.first;
      currentbox.second = lerpedPair.second;

      lerpfactor += 1.f / 1.f;

      if (lerpfactor > 1.0f) {
        lerpfactor = 1.0f;
        lerpfactor = 0.0f;
      }
    } else {
      lerpfactor = 0.0f;
    }

    sf::Color color = sf::Color(109, 250, 201);

    sf::RectangleShape rectl(sf::Vector2f((800.0f / n_x), (800.0f / n_x)));
    rectl.setFillColor(color);

    rectl.setPosition(sf::Vector2f(((800.0f / n_x) * (currentbox.first)),
                                   ((800.0f / n_x) * currentbox.second)));
    window.draw(rectl);

    window.display();
  }

  return 0;
}
