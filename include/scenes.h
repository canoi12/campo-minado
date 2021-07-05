#pragma once
#include "board.h"

#include <string>
#include <vector>
#include <map>
#include "tea.h"

using namespace std;

class Scene {
    public:
    Scene() {}
    ~Scene() {}    

    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual void restart() = 0;
};

class MenuScreen : public Scene {
    public:
    MenuScreen();
    ~MenuScreen();

    void update(float dt);
    void draw();

    private:
    int _option;
    vector<string> _options;
};

enum {
    GAME_STATE = 0,
    WIN_STATE,
    GAMEOVER_STATE
};

class GameScene : public Scene {
    public:
    GameScene();
    ~GameScene();

    void update(float dt);
    void draw();

    void restart();

    private:
    float _x, _y;
    int _mouseX, _mouseY;
    int _panelHeight;
    float _scale;
    int _count;
    int _state;
    Board _board;
    vector<vector<char> > _mask;
    te_rect_t _rects[2]; // coordenadas das texturas de 'bloco virado' e 'bandeira'
    te_texture_t *_screen; // textura onde o tabuleiro será desenhado

    void updatePosition();
    void showCell(int x, int y);
    void resize(int width, int height);

    void showBombs();

    void gameplay(float dt);
    void win(float dt);
    void gameover(float dt);
};

class GameOverScene : public Scene {
    public:
    GameOverScene();
    ~GameOverScene();

    void update(float dt);
    void draw();
};