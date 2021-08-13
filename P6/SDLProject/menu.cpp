#include "menu.h"
#define MENU_WIDTH 14
#define MENU_HEIGHT 8

#define MENU_ENEMY_COUNT 3
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

void Menu::Initialize(Entity* player) {
    state.currScene = 0;
    state.nextScene = -1;

    menuFontTextureID = Util::LoadTexture("font1.png");

    GLuint mapTextureID = Util::LoadTexture("tileset1.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 3, 3);
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.0f;
    state.player->width = 0.8f;
    state.player->height = 0.9f;
    state.player->health = state.player->maxHealth;
    state.player->damage = 20;

    state.player->textureID = Util::LoadTexture("playerSheet.png");
    state.player->animCols = 5;
    state.player->animRows = 2;
    state.player->animRight = new int[8]{ 0,1,2,3,4 };
    state.player->animLeft = new int[8]{ 5,6,7,8,9 };
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 5;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->attackDuration = 1;
}

void Menu::Update(float deltaTime) {

}

void Menu::Render(ShaderProgram* program) {
    Util::DrawText(program, menuFontTextureID, "Dungeon Upgrade", 1.0, -0.5f, glm::vec3(-4, 1, 0)); 
    Util::DrawText(program, menuFontTextureID, "press enter to begin", 1.0, -0.6f, glm::vec3(-4, -1, 0));
}