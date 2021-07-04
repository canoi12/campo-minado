#pragma once
#include <iostream>
#include <string>
#include <map>
#include <typeindex>
#include <typeinfo>

using namespace std;

#include "tea.h"
#include "scenes.h"

class ResourceManager {
    public:
    ~ResourceManager();
    static ResourceManager *Instance();

    te_texture_t *Texture(string name);
    
    private:
    static ResourceManager *_instance;
    map<string, te_texture_t*> _tex;

    ResourceManager();
};

class TextureManager {
    public:
    ~TextureManager();
    static TextureManager *Instance();

    te_texture_t *Texture();
    te_rect_t Rect(string name);

    private:
    te_texture_t *_tex;
    static TextureManager *_instance;
    map<string, te_rect_t> _rects;
    TextureManager();
};

class SceneManager {
    public:
    ~SceneManager();
    static SceneManager *Instance();

    void update(float dt);
    void draw();

    void addScene(string name, Scene *scene);
    void gotoScene(string name);

    private:
    static SceneManager *_instance;
    map<string, Scene*> _scenes;
    Scene *_current;

    SceneManager();
};