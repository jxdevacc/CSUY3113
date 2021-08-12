#include "Level1.h"
#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 20
#define LEVEL1_ENEMY_COUNT 100

unsigned int level1_data[400] = { 0 };

void Level1::createEnemy(int enemyID) {
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(-1, 1);
    int where = (int)(4.0 * distr(eng));
    if (enemyID == 0) {
        state.enemies[state.spawned].entityType = ENEMY;
        state.enemies[state.spawned].textureID = state.enemyTextureID;
        state.enemies[state.spawned].speed = 1;
        state.enemies[state.spawned].isActive = true;
        if (where == 0) {
            state.enemies[state.spawned].position = glm::vec3(state.player->position.x + 6.0 * distr(eng), state.player->position.y + 5.0, 0);
        }
        else if (where == 1) {
            state.enemies[state.spawned].position = glm::vec3(state.player->position.x + 6.0 * distr(eng), state.player->position.y - 5.0, 0);
        }
        else if (where == 2) {
            state.enemies[state.spawned].position = glm::vec3(state.player->position.x + 6.0, state.player->position.y + 5.0 * distr(eng), 0);
        }
        else {
            state.enemies[state.spawned].position = glm::vec3(state.player->position.x - 6.0, state.player->position.y + 5.0 * distr(eng), 0);
        }


        state.enemies[state.spawned].aiType = WAITANDGO;
        state.enemies[state.spawned].aiState = IDLE;
        state.spawned++;
        state.active++;
    }
}

void Level1::Initialize() {

    for (int i = 0; i < 20; i++) {
        level1_data[i] = 1;
        level1_data[400 - i] = 1;
        level1_data[20 * i - 1] = 1;
        level1_data[20 * i] = 1;
    }

    state.active = 0;
    state.spawned = 0;

    state.secs = 0;
    state.currScene = 1;
    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tiles.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 1, 1);
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.0f;
    state.player->width = 0.8f;
    state.player->height = 0.9f;

    state.player->textureID = Util::LoadTexture("playerSheet.png");
    state.player->animCols = 5;
    state.player->animRows = 2;
    state.player->animRight = new int[8]{ 0,1,2,3,4 };
    state.player->animLeft = new int[8]{ 5,6,7,8,9 };
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 5;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->entityType = PLAYER;

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    state.enemyTextureID = Util::LoadTexture("enemy.png");

    createEnemy(0);
}
void Level1::Update(float deltaTime) { 
    state.secs += deltaTime;
    if (state.active < 15 and state.secs > 2) {
        state.secs = 0;
        createEnemy(0);
    }
	state.player->Update(deltaTime, state.player, state.enemies, state.spawned, state.map);
    for (int i = 0; i < state.spawned; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, state.spawned, state.map);
    }

    if (state.player->position.x >= 100) {
        state.nextScene = 2;
    }

}
void Level1::Render(ShaderProgram* program) {
    state.background->Render(program);
	state.map->Render(program);
    for (int i = 0; i < state.spawned; i++) {
        state.enemies[i].Render(program);
    }
	state.player->Render(program);
}