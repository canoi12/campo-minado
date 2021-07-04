#include "game.h"

#include <iostream>
#include <cmath>

#include "managers.h"

int _window_resize(int wid, int w, int h) {
    return 0;
}

Game::Game() {
    te_config_t config = tea_config_init("Campo Minado", 640, 380);
    config.window_flags |= TEA_WINDOW_RESIZABLE;
    tea_init(&config);
    TextureManager::Instance();

    this->_sceneManager = SceneManager::Instance();
    this->_sceneManager->addScene("game", new GameScene());
    this->_sceneManager->gotoScene("game");
}

void Game::run() {
    float bg_time = 0.f;
    int bg_offset = 0;
    int bg_speed = 16;
    te_rect_t bg_part = TextureManager::Instance()->Rect("bg");
    while (!tea_should_close()) {
        tea_begin();
        te_point_t size;
        tea_window_size(&size, 0, 0);
        this->_sceneManager->update(tea_delta());

        tea_color(TEA_WHITE);
        tea_clear(TEA_COLOR(75, 90, 90, 255));
        bg_time += tea_delta()*bg_speed;
        bg_offset = (int)bg_time % 16;
        for (int yy = -16; yy < size.y; yy += 16) {
            for (int xx = -16; xx < size.x; xx += 16) {
                te_rect_t dest = TEA_RECT((TEA_TNUM)xx+bg_offset, (TEA_TNUM)yy+bg_offset, 16.f, 16.f);
                tea_texture_draw(TextureManager::Instance()->Texture(), &dest, &bg_part);
            }
        }
        this->_sceneManager->draw();
        tea_end();
    }
}

Game::~Game() {
    tea_deinit();
}

#if 0
Game::Game() : _board(10, 10, 12) {
    te_config_t config = tea_config_init("Campo Minado", 640, 380);
    config.window_flags |= TEA_WINDOW_RESIZABLE;
    tea_init(&config);
    TextureManager::Instance();
    this->_x = (640 / 2) - 80;
    this->_y = (380 / 2) - 80;
    this->_screen = tea_texture(NULL, this->_board.width()*16, this->_board.height()*16, TEA_RGBA, TEA_TEXTURE_TARGET);
    this->_count = this->_board.width()*this->_board.height();

    this->_textures = TextureManager::Instance()->Texture();
    this->_grid = new char *[this->_board.height()];
    for (int yy = 0; yy < this->_board.height(); yy++) {
        this->_grid[yy] = new char[this->_board.width()];
        memset(this->_grid[yy], 1, this->_board.width()*sizeof(char));
    }
}

void Game::run() {
    int w, h;
    w = this->_board.width() * 16;
    h = this->_board.height() * 16;
    int scale = 2;
    
    int bg_offset = 0;
    int bg_speed = 16;
    float bg_time = 0.f;
    te_rect_t bg_part = TextureManager::Instance()->Rect("bg");
    te_rect_t card = TextureManager::Instance()->Rect("card");
    while (!tea_should_close()) {
        tea_begin();
        te_point_t size;
        tea_window_size(&size, 0, 0);
        this->_x = (size.x / 2) - ((w * scale) / 2);
        this->_y = (size.y / 2) - ((h * scale) / 2);

        int mx, my;
        tea_mouse_pos(&mx, &my);
        int cx, cy;
        cx = floor((mx-this->_x) / (16*scale));
        cy = floor((my-this->_y) / (16*scale));
        if (tea_mouse_pressed(TEA_MOUSE_LEFT)) {
            this->CheckCell(cx, cy);
            if (this->_count == this->_board.bombs()) {
                cout << "You won!" << endl;
            }
        }

        tea_clear(TEA_COLOR(75, 90, 90, 255));
        bg_time += tea_delta()*bg_speed;
        bg_offset = (int)bg_time % 16;
        for (int yy = -16; yy < size.y; yy += 16) {
            for (int xx = -16; xx < size.x; xx += 16) {
                te_rect_t dest = TEA_RECT(xx+bg_offset, yy+bg_offset, 16, 16);
                tea_texture_draw(this->_textures, &dest, &bg_part);
            }
        }
        tea_set_target(this->_screen);
        tea_clear(0);
        this->_board.draw();

        for (int yy = 0; yy < this->_board.height(); yy++) {
            for (int xx = 0; xx < this->_board.width(); xx++) {
                TEA_TNUM dx, dy;
                dx = xx * 16;
                dy = yy * 16;

                tea_color(TEA_WHITE);
                te_rect_t dest = TEA_RECT(dx, dy, 16, 16);
                if (this->_grid[yy][xx] != 0) tea_texture_draw(this->_textures, &dest, &card);
            }
        }
        tea_set_target(NULL);

        te_rect_t dest = TEA_RECT(this->_x, this->_y, w*scale, h*scale);
        tea_texture_draw(this->_screen, &dest, NULL);
        tea_end();
    }
}

void Game::CheckCell(int xx, int yy) {
    char cell = this->_board.cell(xx, yy);
    if (cell < 0) return;
    if (cell == '*') return;
    if (this->_grid[yy][xx] == 0) return;
    this->_grid[yy][xx] = 0;
    this->_count--;
    if (cell != 0) return;

    this->CheckCell(xx-1, yy);
    this->CheckCell(xx, yy-1);
    this->CheckCell(xx+1, yy);
    this->CheckCell(xx, yy+1);

    this->CheckCell(xx-1, yy-1);
    this->CheckCell(xx-1, yy+1);
    this->CheckCell(xx+1, yy-1);
    this->CheckCell(xx+1, yy+1);
}

Game::~Game() {
    tea_deinit();
}
#endif