#include "Level1.h"
#define LEVEL1_WIDTH 50
#define LEVEL1_HEIGHT 50
#define LEVEL1_ENEMY_COUNT 50

unsigned int level1_data[2500] = { 0 };


GLuint level1FontTextureID;

void Level1::createEnemy(int enemyID) {
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(-1, 1);
    int where = (int)(4.0 * distr(eng));

    level1FontTextureID = Util::LoadTexture("font1.png");

    state.currScene = 1;

    state.enemies[state.spawned].entityType = ENEMY;
    if (enemyID < 49) {
        state.enemies[state.spawned].textureID = state.enemyTextureID;
    }
    else{
        state.enemies[state.spawned].textureID = state.bossTextureID;
    }
    state.enemies[state.spawned].speed = 1 + enemyID*.01;
    state.enemies[state.spawned].damage = 10 + enemyID;
    state.enemies[state.spawned].health = 1 + enemyID;
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

    state.enemies[state.spawned].height = 0.9f;
    state.enemies[state.spawned].width = 0.9f;
    state.enemies[state.spawned].aiType = WAITANDGO;
    state.enemies[state.spawned].aiState = IDLE;
    state.spawned++;
    state.active++;
    
}

void Level1::Initialize(Entity* player) {

    for (int i = 0; i < 50; i++) {
        level1_data[i] = 1;
        level1_data[2500 - i] = 1;
        level1_data[50 * i - 1] = 1;
        level1_data[50 * i] = 1;
    }

    state.active = 0;
    state.spawned = 0;

    state.secs = 0;
    state.currScene = 1;
    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tiles.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 1, 1);
    // Initialize Player
    if (player == NULL) {
        state.player = new Entity();
        state.player->entityType = PLAYER;
        state.player->position = glm::vec3(25, -25, 0);
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
    else {
        state.player = player;
        state.player->health = state.player->maxHealth;
        state.player->position = glm::vec3(25, -25, 0);
        state.player->isActive = true;
    }


    state.sword = new Entity();
    state.sword->entityType = SWORD;
    state.sword->position = glm::vec3(0);
    state.sword->isActive = false;
    state.sword->height = 0.7f;
    
    state.sword->textureID = Util::LoadTexture("sword.png");
    state.sword->animCols = 4;
    state.sword->animRows = 1;
    state.sword->animRight = new int[1]{ 3 };
    state.sword->animLeft = new int[1]{ 0 };
    state.sword->animIndices = state.player->animRight;
    state.sword->animFrames = 1;
    state.sword->animIndex = 0;
    state.sword->animTime = 0;
    state.sword->damage = state.player->damage;
       

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    state.bossTextureID = Util::LoadTexture("boss.png");
    state.enemyTextureID = Util::LoadTexture("enemy.png");

    createEnemy(0);
}
void Level1::Update(float deltaTime) { 
    state.secs += deltaTime;

    if (state.secs > 1) {
        state.secs = 0;
        if (state.spawned < 50) {
            createEnemy(state.spawned);
        }

    }
    if (state.player->attackCooldown > 0) {
        state.sword->isActive = false;
        state.attackTimer = 0;
        state.player->attackCooldown -= deltaTime;
        if (state.player->attackCooldown < 0) {
            state.player->attackCooldown = 0;
        }
    }
    if (state.player->isAttacking) {
        state.attackTimer += deltaTime;
        if (state.player->animIndices == state.player->animRight) {
            state.sword->position.x = state.player->position.x + 1;
            state.sword->position.y = state.player->position.y;
        }
        else {
            state.sword->position.x = state.player->position.x - 1;
            state.sword->position.y = state.player->position.y;
        }

        state.sword->isActive = true;
        if (state.attackTimer > state.player->attackDuration) {
            state.sword->isActive = false;
            state.player->isAttacking = false;
            state.player->attackCooldown = 3-state.player->attackSpeed;
        }
    }

	state.player->Update(deltaTime, state.player, state.enemies, state.spawned, state.map);
    for (int i = 0; i < state.spawned; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, state.spawned, state.map);
    }
    state.sword->Update(deltaTime, state.player, state.enemies, state.spawned, state.map);

    if (state.player->health <= 0) {
        state.player->money += state.player->kills;
        state.nextScene = 2;
    }

    if (state.player->kills >= 50) {
        state.player->state = 2;
    }

}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
    for (int i = 0; i < state.spawned; i++) {
        state.enemies[i].Render(program);
    }
    state.sword->Render(program);
	state.player->Render(program);
    Util::DrawText(program, level1FontTextureID, "Spacebar to attack, Arrow Keys to move", 0.5, -0.3f, glm::vec3(state.player->position.x-4, state.player->position.y-3.5, 0));
}