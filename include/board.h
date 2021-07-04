#pragma once

#include <vector>

struct BoardConfig {
    int w, h;
    int bombs;
};

class Board {
    public:
    Board(BoardConfig &config);
    Board(int width, int height, int bombs = 8);
    ~Board();

    void generate();

    char cell(int x, int y);

    void setBombs(int bombs = 8);
    int bombs();

    void setWidth(int width);
    int width();

    void setHeight(int height);
    int height();

    void resize(int width, int height);
    void print();
    void draw();

    std::vector<char> operator[](int y);

    private:
    int _width, _height;
    int _bombs;
    std::vector<std::vector<char> > _grid;

    void addValue(int x, int y);
};
