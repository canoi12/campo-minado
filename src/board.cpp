#include "board.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "managers.h"
#include "tea.h"

Board::Board(int w, int h, int bombs) {
    this->_grid = std::vector<std::vector<char> >();
    
    this->setBombs(bombs);
    this->resize(w, h);
    this->generate();
}

Board::Board(BoardConfig &config) {
    this->_width = config.w;
    this->_height = config.h;
    this->_bombs = config.bombs;
}

void Board::generate() {
    srand(time(NULL));
    std::cout << "Board generated" << std::endl;
    std::vector<std::vector<char> >::iterator it;
    for (it = this->_grid.begin(); it != this->_grid.end(); it++) {
        std::fill(it->begin(), it->end(), 0);
    }
    for (int i = 0; i < this->_bombs; i++) {
        int xx = rand() % this->_width;
        int yy = rand() % this->_height;
        // garante que todas as bombas estejam em posições diferentes
        while (this->_grid[yy][xx] == '*') {
            xx = rand() % this->_width;
            yy = rand() % this->_height;
        }
        this->_grid[yy][xx] = '*';

        this->addValue(xx - 1, yy - 1);
        this->addValue(xx, yy - 1);
        this->addValue(xx + 1, yy - 1);

        this->addValue(xx - 1, yy);
        this->addValue(xx + 1, yy);

        this->addValue(xx - 1, yy + 1);
        this->addValue(xx, yy + 1);
        this->addValue(xx + 1, yy + 1);
    }
    
}

char Board::cell(int x, int y) {
    if (x >= 0 && x < this->_width &&
        y >= 0 && y < this->_height)
        return this->_grid[y][x];
    return -1;
}

void Board::setBombs(int bombs) {
    this->_bombs = bombs;
}

int Board::bombs() {
    return this->_bombs;
}

void Board::setWidth(int width) {
    this->_width = width;
}

int Board::width() {
    return this->_width;
}

void Board::setHeight(int height) {
    this->_height = height;
}

int Board::height() {
    return this->_height;
}

void Board::resize(int width, int height) {
    this->_width = width;
    this->_height = height;
    this->_grid.resize(height);
    std::vector<std::vector<char> >::iterator iter = this->_grid.begin();
    for (; iter != this->_grid.end(); iter++) iter->resize(width);
}

void Board::draw() {
    for (int yy = 0; yy < this->_height; yy++) {
        for (int xx = 0; xx < this->_width; xx++) {
            TEA_TNUM dx = (xx * 16);
            TEA_TNUM dy = (yy * 16);
            char str[2];
            sprintf(str, "%c", this->_grid[yy][xx]);
            tea_print(str, dx + 4, dy + 4);
			te_rect_t dest = TEA_RECT(dx, dy, 16, 16);
			te_rect_t rect;
			switch (this->_grid[yy][xx]) {
				case '1': rect = TextureManager::Instance()->Rect("one"); break;
				case '2': rect = TextureManager::Instance()->Rect("two"); break;
				case '3': rect = TextureManager::Instance()->Rect("three"); break;
				case '4': rect = TextureManager::Instance()->Rect("four"); break;
				case '5': rect = TextureManager::Instance()->Rect("five"); break;
				case '6': rect = TextureManager::Instance()->Rect("six"); break;
				case '7': rect = TextureManager::Instance()->Rect("seven"); break;
				case '8': rect = TextureManager::Instance()->Rect("eight"); break;
				case '9': rect = TextureManager::Instance()->Rect("nine"); break;
				case '*': rect = TextureManager::Instance()->Rect("bomb"); break;
				default:
					rect = TextureManager::Instance()->Rect("empty");
			}
			te_texture_t *tex = TextureManager::Instance()->Texture();
			tea_texture_draw(tex, &dest, &rect);


        }
    }
}

std::vector<char> Board::operator[](int y) {
    return this->_grid[y];
}
#if 0
char Board::Cell(int x, int y) {
    if (x >= 0 && x < this->_width &&
        y >= 0 && y < this->_height)
        return this->_grid[y][x];
    return -1;
}

int Board::Bombs() {
	return this->_bombs;
}

int Board::Width() { return this->_width; }
int Board::Height() { return this->_height; }
void Board::Resize(int w, int h, int bombs) {}

void Board::Print() {
    for (int yy = 0; yy < this->_height; yy++) {
        for (int xx = 0; xx < this->_width; xx++) {
            char cell = this->_grid[yy][xx];
            if (cell < 10)
                cell += '0';
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }
}

void Board::Draw(int x, int y) {
    for (int yy = 0; yy < this->_height; yy++) {
        for (int xx = 0; xx < this->_width; xx++) {
            TEA_TNUM dx = x + (xx * 16);
            TEA_TNUM dy = y + (yy * 16);
            char str[2];
            sprintf(str, "%c", this->_grid[yy][xx]);
            tea_print(str, dx + 4, dy + 4);
			te_rect_t dest = TEA_RECT(dx, dy, 16, 16);
			te_rect_t rect;
			switch (this->_grid[yy][xx]) {
				case '1': rect = TextureManager::Instance()->Rect("one"); break;
				case '2': rect = TextureManager::Instance()->Rect("two"); break;
				case '3': rect = TextureManager::Instance()->Rect("three"); break;
				case '4': rect = TextureManager::Instance()->Rect("four"); break;
				case '5': rect = TextureManager::Instance()->Rect("five"); break;
				case '6': rect = TextureManager::Instance()->Rect("six"); break;
				case '7': rect = TextureManager::Instance()->Rect("seven"); break;
				case '8': rect = TextureManager::Instance()->Rect("eight"); break;
				case '9': rect = TextureManager::Instance()->Rect("nine"); break;
				case '*': rect = TextureManager::Instance()->Rect("bomb"); break;
				default:
					rect = TextureManager::Instance()->Rect("empty");
			}
			te_texture_t *tex = TextureManager::Instance()->Texture();
			tea_texture_draw(tex, &dest, &rect);


        }
    }
}
#endif
void Board::addValue(int x, int y) {
    if (x >= 0 && x < this->_width &&
        y >= 0 && y < this->_height &&
        this->_grid[y][x] != '*') {
        if (this->_grid[y][x] == 0) this->_grid[y][x] = '0';
        this->_grid[y][x] += 1;
    }
}

Board::~Board() {
    std::vector<std::vector<char> >::iterator iter = this->_grid.begin();
    for (; iter != this->_grid.end(); iter++) {
        iter->clear();
    }
    this->_grid.clear();
}
