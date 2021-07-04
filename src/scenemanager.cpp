#include "managers.h"

SceneManager *SceneManager::_instance = nullptr;

SceneManager::SceneManager() {
    this->_scenes = map<string, Scene*>();
    this->_current = nullptr;
}

void SceneManager::update(float dt) {
    if (this->_current) this->_current->update(dt);
}

void SceneManager::draw() {
    if (this->_current) this->_current->draw();
}

void SceneManager::addScene(string name, Scene *scene) {
    this->_scenes[name] = scene;
}

void SceneManager::gotoScene(string name) {
    Scene *scn = this->_scenes[name];
    if (scn) this->_current = scn;
}

SceneManager *SceneManager::Instance() {
    if (!_instance) _instance = new SceneManager();
    return _instance;
}