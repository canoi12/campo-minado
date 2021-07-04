#pragma once
#include "scenes.h"
#include "managers.h"

class Game {
    public:
	Game();
	Game(int width, int height);
	~Game();
	void run();

    private:
#if 0
	Board _board;
	int _x, _y;
	te_texture_t *_screen;
	te_texture_t *_textures;
	char **_grid;
	int _count;
#endif
	SceneManager *_sceneManager;
};
