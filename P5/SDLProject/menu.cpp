#include "menu.h"
#define MENU_WIDTH 14
#define MENU_HEIGHT 8

#define MENY_ENEMY_COUNT 3
unsigned int menu_data[] =
{
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
 0, 9, 1, 1, 1, 1, 1, 1, 1, 2, 0, 2, 2, 2,
 0, 3, 0, 0, 0, 0, 0, 0, 0, 5, 0, 2, 2, 2
};

GLuint menuFontTextureID;

void Menu::Initialize() {
    state.currScene = 0;
    state.nextScene = -1;

    menuFontTextureID = Util::LoadTexture("font1.png");

    GLuint mapTextureID = Util::LoadTexture("tileset1.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 3, 3);
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -5.0f, 0);
    state.player->speed = 1.0f;
    state.player->textureID = Util::LoadTexture("player.png");
    state.player->entityType = PLAYER;

    state.player->jumpPower = 5.0f;
}

void Menu::Update(float deltaTime) {

}

void Menu::Render(ShaderProgram* program) {
    Util::DrawText(program, menuFontTextureID, "Platformer Guy", 1.0, -0.5f, glm::vec3(1.3, -3.4, 0)); 
    Util::DrawText(program, menuFontTextureID, "press enter to begin", 1.0, -0.6f, glm::vec3(1, -4.4, 0));
}