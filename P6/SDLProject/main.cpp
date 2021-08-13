#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <string>
#include <random>

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "util.h"
#include "Entity.h"
#include <vector>
#include "map.h"

#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "menu.h"

GLuint fontTextureID;
SDL_Window* displayWindow;
bool gameIsRunning = true;

Mix_Music* music;
Mix_Chunk* click;
Mix_Chunk* death;
Mix_Chunk* slash;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[3];

int money = 0;
float lifePosX = 0.0;
float lifePosY = 0.0;

void SwitchToScene(Scene* scene, Entity* player = NULL) {
    currentScene = scene;
    currentScene->Initialize(player);
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("calm.mp3");
    Mix_PlayMusic(music, -1);

    click = Mix_LoadWAV("click.wav");
    slash = Mix_LoadWAV("slash.wav");
    death = Mix_LoadWAV("death.wav");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontTextureID = Util::LoadTexture("font1.png");

    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    SwitchToScene(sceneList[0]);

}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {

            case SDLK_RETURN:
                if (currentScene->state.currScene < 1) {
                    currentScene->state.nextScene = 1;
                    Mix_PlayChannel(-1, click, 0);
                }
                break;

            case SDLK_1:
                if (currentScene->state.player->money > 10 && currentScene->state.currScene == 2) {
                    currentScene->state.player->money -= 10;
                    currentScene->state.player->damage += 10;
                    Mix_PlayChannel(-1, click, 0);
                }
                break;

            case SDLK_2:
                if (currentScene->state.player->money > 10 && currentScene->state.currScene == 2) {
                    currentScene->state.player->money -= 10;
                    currentScene->state.player->attackDuration += 1;
                    Mix_PlayChannel(-1, click, 0);
                }
                break;

            case SDLK_3:
                if (currentScene->state.player->money > 10 && currentScene->state.currScene == 2) {
                    currentScene->state.player->money -= 10;
                    currentScene->state.player->maxHealth += 100;
                    Mix_PlayChannel(-1, click, 0);
                }
                break;

            case SDLK_4:
                if (currentScene->state.player->money > 10 && currentScene->state.currScene == 2) {
                    currentScene->state.player->money -= 10;
                    currentScene->state.player->attackSpeed += 1;
                    Mix_PlayChannel(-1, click, 0);
                }
                break;

            case SDLK_5:
                if (currentScene->state.currScene == 2) {
                    Mix_PlayChannel(-1, click, 0);
                    currentScene->state.nextScene = 1;
                }
                break;


            case SDLK_SPACE:
                if (currentScene->state.player->attackCooldown == 0 && currentScene->state.currScene == 1) {
                    currentScene->state.player->isAttacking = true;
                    Mix_PlayChannel(-1, slash, 0);
                }

                break;
                

            }


            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -3.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        currentScene->state.sword->animIndices = currentScene->state.sword->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 3.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
        currentScene->state.sword->animIndices = currentScene->state.sword->animRight;

    }
    if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->movement.y = 3.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->movement.y = -3.0f;
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, -currentScene->state.player->position.y, 0));
    lifePosX = currentScene->state.player->position.x - 4.5;
    lifePosY = currentScene->state.player->position.y + 3.25;
}



void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);
    currentScene->state.map->Render(&program);

    if (currentScene->state.currScene == 1) {
        std::string health = std::to_string(currentScene->state.player->health);
        Util::DrawText(&program, fontTextureID, health, 1.0, -0.6f, glm::vec3(currentScene->state.player->position.x - (1 - (1.0/health.length())), currentScene->state.player->position.y + 1, 0));

        for (int i = 0; i < currentScene->state.spawned; i++) {
            if (currentScene->state.enemies[i].isActive) {
                health = std::to_string(currentScene->state.enemies[i].health);
                Util::DrawText(&program, fontTextureID, health, 1.0, -0.6f, glm::vec3(currentScene->state.enemies[i].position.x - (1 - (1.0/health.length())), currentScene->state.enemies[i].position.y + 1, 0));
            }
        }
    }

    currentScene->Render(&program);

    if (currentScene->state.player->state == 2) {
        Util::DrawText(&program, fontTextureID, "You win!", 1.0, -0.6f, glm::vec3(currentScene->state.player->position.x - 2, currentScene->state.player->position.y, 0));
        currentScene->state.player->isActive = false;
    }

    if (currentScene->state.currScene == 1) {
        Util::DrawText(&program, fontTextureID, "Kills: " + std::to_string(currentScene->state.player->kills), 0.75, -0.5f, glm::vec3(lifePosX, lifePosY, 0));
        Util::DrawText(&program, fontTextureID, "Damage: " + std::to_string(currentScene->state.player->damage), 0.75, -0.5f, glm::vec3(lifePosX, lifePosY - .75, 0));
        Util::DrawText(&program, fontTextureID, "Cooldown: " + std::to_string(currentScene->state.player->attackCooldown), 0.75, -0.5f, glm::vec3(lifePosX, lifePosY - 1.5, 0));
    }

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene], currentScene->state.player);

        Render();

    }

    Shutdown();
    return 0;
}