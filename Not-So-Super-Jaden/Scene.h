#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include "SDL_mixer.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"

class Scene;

struct GameState
{
    // ————— GAME OBJECTS ————— //
    Map *map;
    Entity *player;
    Entity *enemies;
    
    // ————— AUDIO ————— //
    Mix_Music *bgm;
    Mix_Chunk *jump_sfx;
    
    // ————— POINTERS TO OTHER SCENES ————— //
    Scene* next_scene;
};

class Scene {
public:
    // ————— ATTRIBUTES ————— //
    int m_number_of_enemies = 1;
    
    GameState m_state;
    
    // ————— METHODS ————— //
    virtual void initialize() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program) = 0;
    
    // ————— GETTERS ————— //
    GameState const get_state()             const { return m_state;             }
    int       const get_number_of_enemies() const { return m_number_of_enemies; }

    void set_next_scene(Scene* new_scene) { m_state.next_scene = new_scene; }
    Scene* get_next_scene() { return m_state.next_scene; }
};
