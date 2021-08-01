#include "Level3.h"
#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 8

#define LEVEL3_ENEMY_COUNT 3
unsigned int level3_data[] =
{
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 7,
 5, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 1, 1, 1,
 1, 1, 2, 0, 0, 9, 1, 2, 0, 0, 3, 0, 0, 0,
 0, 0, 5, 0, 0, 3, 0, 5, 0, 0, 3, 0, 0, 0
};

void Level3::Initialize() {

    state.currScene = 3;
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset1.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 3, 3);
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -4, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -5.0f, 0);
    state.player->speed = 1.0f;
    state.player->textureID = Util::LoadTexture("player.png");
    state.player->entityType = PLAYER;

    state.player->jumpPower = 5.0f;

    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].speed = 1;
    state.enemies[0].acceleration = glm::vec3(0, -5.0f, 0);
    state.enemies[0].isActive = true;

    state.enemies[0].position = glm::vec3(5, -5, 0);
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].jumpPower = 5.0f;
}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    state.enemies->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    if (state.player->position.x >= 12) {
        state.player->state = 2;
    }

}
void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies->Render(program);
}