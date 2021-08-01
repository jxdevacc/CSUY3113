#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 1
unsigned int level2_data[] =
{
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 7,
 5, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 3, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 3, 0, 0
};

void Level2::Initialize() {

    state.currScene = 2;
    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tileset1.png");
	state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 3, 3);
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -5.0f, 0);
    state.player->speed = 1.0f;
    state.player->textureID = Util::LoadTexture("player.png");
    state.player->entityType = PLAYER;

    state.player->jumpPower = 5.0f;

    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].speed = 1;
    state.enemies[0].acceleration = glm::vec3(0, -5.0f, 0);
    state.enemies[0].isActive = true;

    state.enemies[0].position = glm::vec3(7, -2, 0);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;

}
void Level2::Update(float deltaTime) { 
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    state.enemies->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    if (state.player->position.x >= 12) {
        state.nextScene = 3;
    }
}
void Level2::Render(ShaderProgram* program) {
	state.map->Render(program);
    state.enemies->Render(program);
	state.player->Render(program);
}