#include "scenes.h"

#include "managers.h"
#include <cmath>

GameScene::GameScene() : _board(10, 10) {
    this->_x = 0;
    this->_y = 0;
    this->_scale = 2.f;
    this->_screen = nullptr;
    this->_panelHeight = 48;
    this->_mouseX = 0;
    this->_mouseY = 0;
    this->_state = GAME_STATE;

    this->_count = this->_board.width()*this->_board.height();
    this->_rects[0] = TextureManager::Instance()->Rect("card");
    this->_rects[1] = TextureManager::Instance()->Rect("flag");

    this->_mask = vector<vector<char> >();
    this->resize(this->_board.width(), this->_board.height());
}

void GameScene::update(float dt) {
    if (tea_key_pressed(TEA_KEY_R)) this->restart();
    if (this->_state == GAME_STATE) this->gameplay(dt);
}

void GameScene::draw() {
    te_point_t size;
    tea_window_size(&size, 0, 0);
    tea_set_target(this->_screen);
    tea_clear(0);
    this->_board.draw();

    if (this->_state != GAME_STATE) this->updatePosition();

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
    tea_color(TEA_WHITE);
    tea_mode(0);
    if (this->_board.cell(this->_mouseX, this->_mouseY) >= 0) tea_rect(this->_mouseX*16, this->_mouseY*16, 16, 16);
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
    // string bombs = to_string(this->_board.bombs()) + " minas";
    // tea_print(bombs.c_str(), 16, 8);

    // string field = "Campo: " + to_string(this->_board.width()) + "x" + to_string(this->_board.height());
    // tea_print(field.c_str(), 128, 8);

    // string remain = "Restantes: " + to_string(this->_count - this->_board.bombs());
    // tea_print(remain.c_str(), 256, 8);

    tea_line(0, 24, size.x, 24);

    string info;
    info += to_string(this->_board.bombs().size()) + " minas";
    info += "     ";
    info += "Campo: " + to_string(this->_board.width()) + "x" + to_string(this->_board.height());
    info += "     ";
    info += "Restam: " + to_string(this->_count - this->_board.bombs().size());
    tea_print(info.c_str(), 8, 8);

    string controls;
    controls += "A/D: -/+ largura";
    controls += "      ";
    controls += "S/W: -/+ altura";
    controls += "      ";
    controls += "X/C: -/+ minas";
    controls += "      ";
    controls += "R: reiniciar";

    tea_print(controls.c_str(), 8, 32);

    if (this->_state == WIN_STATE) this->win(tea_delta());
    else if (this->_state == GAMEOVER_STATE) this->gameover(tea_delta());
}

void GameScene::restart() {
    this->_state = GAME_STATE;
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
    this->_y = this->_panelHeight + ((size.y-this->_panelHeight) / 2) - ((h*this->_scale)/2);
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

void GameScene::showBombs() {
    vector<Pos>::iterator bomb;
    for (bomb = this->_board.bombs().begin(); bomb != this->_board.bombs().end(); bomb++) {
        this->_mask[bomb->y][bomb->x] = 0;
    }
}

void GameScene::gameplay(float dt) {
    te_point_t size;
    tea_window_size(&size, 0, 0);
    int w, h;
    w = this->_board.width() * 16;
    h = this->_board.height() * 16;

    // this->_x = (size.x / 2.f) - ((w * this->_scale) / 2.f);
    // this->_y = this->_panelHeight + ((size.y-this->_panelHeight) / 2.f) - ((h * this->_scale) / 2.f);
    this->updatePosition();

    if (tea_key_pressed(TEA_KEY_A)) this->resize(this->_board.width()-1, this->_board.height());
    else if (tea_key_pressed(TEA_KEY_D)) this->resize(this->_board.width()+1, this->_board.height());

    if (tea_key_pressed(TEA_KEY_S)) this->resize(this->_board.width(), this->_board.height()-1);
    else if (tea_key_pressed(TEA_KEY_W)) this->resize(this->_board.width(), this->_board.height()+1);

    if (tea_key_pressed(TEA_KEY_X)) {
        this->_board.setBombs(this->_board.bombs().size()-1);
        this->restart();
    } else if (tea_key_pressed(TEA_KEY_C)) {
        this->_board.setBombs(this->_board.bombs().size()+1);
        this->restart();
    }

    int mx, my;
    tea_mouse_pos(&mx, &my);
    int cx, cy;
    cx = floor((mx-this->_x) / (16*this->_scale));
    cy = floor((my-this->_y) / (16*this->_scale));
    this->_mouseX = cx;
    this->_mouseY = cy;
    if (this->_board.cell(cx, cy) >= 0 && this->_mask[cy][cx] != 0) {
        if (tea_mouse_pressed(TEA_MOUSE_LEFT)) {
            if (this->_mask[cy][cx] == 1) this->showCell(cx, cy);
            if (this->_board.cell(cx, cy) == '*') {
                this->showBombs();
                this->_state = GAMEOVER_STATE;
            }
            if (this->_count == this->_board.bombs().size()) this->_state = WIN_STATE;
        } else if (tea_mouse_pressed(TEA_MOUSE_RIGHT)) {
            if (this->_mask[cy][cx] == 1) this->_mask[cy][cx] = 2;
            else if (this->_mask[cy][cx] == 2) this->_mask[cy][cx] = 1;
        }
    }
}

void GameScene::win(float dt) {
    te_point_t size;
    tea_window_size(&size, 0, 0);
    int cx = size.x / 2 - 64;
    int cy = size.y / 2 - 4;
    tea_color(TEA_BLACK);
    tea_mode(1);
    tea_rect(0, size.y/2 - 32, size.x, 64);
    tea_color(TEA_WHITE);
    tea_print("PARABÉNS BOE!", cx, cy);
}

void GameScene::gameover(float dt) {
    te_point_t size;
    tea_window_size(&size, 0, 0);
    int cx = size.x / 2 - 64;
    int cy = size.y / 2 - 4;
    tea_color(TEA_BLACK);
    tea_mode(1);
    tea_rect(0, size.y/2 - 32, size.x, 64);
    tea_color(TEA_WHITE);
    tea_print("PERDEU GALADO!", cx, cy);
}

GameScene::~GameScene() {

}