#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 1
unsigned int level2_data[] =
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

GLuint level2FontTextureID;

void Level2::Initialize(Entity* player) {
    state.currScene = 2;
    state.nextScene = -1;

    level2FontTextureID = Util::LoadTexture("font1.png");

    GLuint mapTextureID = Util::LoadTexture("tileset1.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 3, 3);
    state.player = player;
    state.player->position.x = 0;
    state.player->kills = 0;
    state.player->position.y = 0;
    state.player->isActive = false;


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
    state.sword->damage = 20;
}
void Level2::Update(float deltaTime) { 
}
void Level2::Render(ShaderProgram* program) {
    Util::DrawText(program, level2FontTextureID, "you have $" + std::to_string(state.player->money), 0.5, -0.3f, glm::vec3(-4.5, 3, 0));
    Util::DrawText(program, level2FontTextureID, "press 1 to increase damage by 10 [$10]", 0.5, -0.3f, glm::vec3(-4.5, 2, 0));
    Util::DrawText(program, level2FontTextureID, "press 2 to increase attack duration by 1 [$10]", 0.5, -0.3f, glm::vec3(-4.5, 1, 0));
    Util::DrawText(program, level2FontTextureID, "press 3 to increase Health by 100 [$10]", 0.5, -0.3f, glm::vec3(-4.5, 0, 0));
    if (state.player->attackSpeed < 3) {
        Util::DrawText(program, level2FontTextureID, "press 4 to reduce attack cooldown by 1 [$10]", 0.5, -0.3f, glm::vec3(-4.5, -1, 0));
    }
    Util::DrawText(program, level2FontTextureID, "press 5 to re enter the dungeon", 0.5, -0.3f, glm::vec3(-4.5, -2, 0));
}