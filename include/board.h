#pragma once

#include <vector>
#include <map>
#include "tea.h"

using namespace std;

struct BoardConfig {
    int w, h;
    int bombs;
};

struct Pos {
    int x, y;
};

class Board {
    public:
    Board(BoardConfig &config);
    Board(int width, int height, int bombs = 8);
    ~Board();

    void generate();

    char cell(int x, int y);

    void setBombs(int bombs = 8);
    vector<Pos>& bombs();

    void setWidth(int width);
    int width();

    void setHeight(int height);
    int height();

    void resize(int width, int height);
    void print();
    void draw();

    vector<char> operator[](int y);

    private:
    int _width, _height;
    vector<Pos> _bombs;
    vector<vector<char> > _grid;
    map<char, te_rect_t> _rects;

    void addValue(int x, int y);
};
