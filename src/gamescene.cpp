#include "scenes.h"

#include "managers.h"
#include <cmath>

GameScene::GameScene() : _board(10, 10) {
    this->_x = 0;
    this->_y = 0;
    this->_scale = 2.f;
    this->_screen = nullptr;
    this->_panelHeight = 32;

    this->_count = this->_board.width()*this->_board.height();
    this->_rects[0] = TextureManager::Instance()->Rect("card");
    this->_rects[1] = TextureManager::Instance()->Rect("flag");

    this->_mask = vector<vector<char> >();
    this->resize(this->_board.width(), this->_board.height());
}

void GameScene::update(float dt) {
    te_point_t size;
    tea_window_size(&size, 0, 0);
    int w, h;
    w = this->_board.width() * 16;
    h = this->_board.height() * 16;

    this->_x = (size.x / 2.f) - ((w * this->_scale) / 2.f);
    this->_y = this->_panelHeight + (size.y / 2.f) - ((h * this->_scale) / 2.f);

    if (tea_key_pressed(TEA_KEY_R)) this->restart();

    if (tea_key_pressed(TEA_KEY_A)) this->resize(this->_board.width()-1, this->_board.height());
    else if (tea_key_pressed(TEA_KEY_D)) this->resize(this->_board.width()+1, this->_board.height());

    if (tea_key_pressed(TEA_KEY_S)) this->resize(this->_board.width(), this->_board.height()-1);
    else if (tea_key_pressed(TEA_KEY_W)) this->resize(this->_board.width(), this->_board.height()+1);

    if (tea_key_pressed(TEA_KEY_X)) {
        this->_board.setBombs(this->_board.bombs()-1);
        this->restart();
    } else if (tea_key_pressed(TEA_KEY_C)) {
        this->_board.setBombs(this->_board.bombs()+1);
        this->restart();
    }


    int mx, my;
    tea_mouse_pos(&mx, &my);
    int cx, cy;
    cx = floor((mx-this->_x) / (16*this->_scale));
    cy = floor((my-this->_y) / (16*this->_scale));
    if (tea_mouse_pressed(TEA_MOUSE_LEFT)) {
        if (this->_mask[cy][cx] == 1) this->showCell(cx, cy);
        if (this->_board.cell(cx, cy) == '*') cout << "You lose!" << endl;
        if (this->_count == this->_board.bombs()) this->restart();
    } else if (tea_mouse_pressed(TEA_MOUSE_RIGHT)) {
        if (this->_mask[cy][cx] == 1) this->_mask[cy][cx] = 2;
        else if (this->_mask[cy][cx] == 2) this->_mask[cy][cx] = 1;
    }
}

void GameScene::draw() {
    te_point_t size;
    tea_window_size(&size, 0, 0);
    tea_set_target(this->_screen);
    tea_clear(0);
    this->_board.draw();

    vector<vector<char> >::iterator row;
    tea_color(TEA_WHITE);
    float yy = 0;
    for (row = this->_mask.begin(); row != this->_mask.end(); row++) {
        vector<char>::iterator col;
        float xx = 0;
        for (col = row->begin(); col != row->end(); col++) {
            te_rect_t dest = TEA_RECT(xx*16.f, yy*16.f, 16.f, 16.f);
            if (*col > 0) tea_texture_draw(TextureManager::Instance()->Texture(), &dest, &this->_rects[*col-1]);
            xx++;
        }
        yy++;
    }
    tea_set_target(NULL);
    float ww, hh;
    ww = this->_board.width()*16*this->_scale;
    hh = this->_board.height()*16*this->_scale;

    te_rect_t dest = TEA_RECT((TEA_TNUM)this->_x, (TEA_TNUM)this->_y, ww, hh);
    tea_texture_draw(this->_screen, &dest, NULL);
    tea_color(TEA_BLACK);
    tea_mode(1);
    tea_rect(0, 0, size.x, this->_panelHeight);
    tea_color(TEA_WHITE);
    string bombs = to_string(this->_board.bombs()) + " *";
    tea_print(bombs.c_str(), 16, 8);

    string field = "Campo: " + to_string(this->_board.width()) + "x" + to_string(this->_board.height());
    tea_print(field.c_str(), 128, 8);

    string remain = "Restantes: " + to_string(this->_count - this->_board.bombs());
    tea_print(remain.c_str(), 256, 8);
}

void GameScene::restart() {
    this->_count = this->_board.width()*this->_board.height();
    vector<vector<char> >::iterator it;
    for (it = this->_mask.begin(); it != this->_mask.end(); it++) {
        std::fill(it->begin(), it->end(), 1);
    }
    this->_board.generate();
}

void GameScene::updatePosition() {
    te_point_t size;
    tea_window_size(&size, 0, 0);
    int w, h;
    w = this->_board.width() * 16;
    h = this->_board.height() * 16;
    this->_x = (size.x / 2) - ((w*this->_scale)/2);
    this->_y = (size.y / 2) - ((h*this->_scale)/2);

}

void GameScene::showCell(int x, int y) {
    char cell = this->_board.cell(x, y);
    if (cell < 0) return;
    if (cell == '*') return;
    if (this->_mask[y][x] == 0) return;
    this->_mask[y][x] = 0;
    this->_count--;
    if (cell != 0) return;

    this->showCell(x-1, y);
    this->showCell(x, y-1);
    this->showCell(x+1, y);
    this->showCell(x, y+1);

    this->showCell(x-1, y-1);
    this->showCell(x-1, y+1);
    this->showCell(x+1, y-1);
    this->showCell(x+1, y+1);
}

void GameScene::resize(int width, int height) {
    this->_screen = tea_texture(NULL, width*16, height*16, TEA_RGBA, TEA_TEXTURE_TARGET);
    this->_board.resize(width, height);
    this->_mask.resize(height);
    vector<vector<char> >::iterator it;
    for (it = this->_mask.begin(); it != this->_mask.end(); it++) {
        it->resize(width);
    }
    this->restart();
}

GameScene::~GameScene() {

}