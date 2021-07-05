#include "managers.h"

#include "tea.h"

TextureManager *TextureManager::_instance = NULL;

TextureManager::TextureManager() {
    this->_tex = tea_texture_load("textures.png", TEA_TEXTURE_STATIC);
    this->_rects = map<string, te_rect_t>();
    this->_rects["one"] = TEA_RECT(0, 0, 16, 16);
    this->_rects["two"] = TEA_RECT(16, 0, 16, 16);
    this->_rects["three"] = TEA_RECT(32, 0, 16, 16);
    this->_rects["bomb"] = TEA_RECT(48, 0, 16, 16);
    this->_rects["flag"] = TEA_RECT(64, 0, 16, 16);
    this->_rects["four"] = TEA_RECT(0, 16, 16, 16);
    this->_rects["five"] = TEA_RECT(16, 16, 16, 16);
    this->_rects["six"] = TEA_RECT(32, 16, 16, 16);
    this->_rects["card"] = TEA_RECT(48, 16, 16, 16);
    this->_rects["empty"] = TEA_RECT(64, 16, 16, 16);
    this->_rects["seven"] = TEA_RECT(0, 32, 16, 16);
    this->_rects["eight"] = TEA_RECT(16, 32, 16, 16);
    this->_rects["nine"] = TEA_RECT(32, 32, 16, 16);
    this->_rects["bg"] = TEA_RECT(48, 32, 16, 16);
}

te_texture_t *TextureManager::Texture() {
    return this->_tex;
}

TextureManager *TextureManager::Instance() {
    if (!_instance) _instance = new TextureManager();
    return _instance;
}

te_rect_t TextureManager::Rect(string name) { return this->_rects[name]; }