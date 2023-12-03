/**
* Author: Jaden Thakur
* Assignment: Not-So-Super-Jaden
* Date due: 2023-12-2, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

// defines

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define LOG(argument) std::cout << argument << std::endl;
#define FIXED_TIMESTEP 0.0166666f
#define MAP_WIDTH 12
#define MAP_HEIGHT 12


// includes
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
//#include "stb_image.h"
#include <vector>
#include "Entity.h"
#include "Utility.h"
#include "Map.h"
#include "SDL_mixer.h"
#include "cmath"
#include <ctime>
#include <cstdlib>
#include <vector>
#include "Scene.h"
#include "Start.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"


// ****CONSTANTS****
const int WINDOW_WIDTH = 1000,
WINDOW_HEIGHT = 600;

const float BG_RED = 0.388f, // #63ADF2
BG_GREEN = 0.678f,
BG_BLUE = 0.949f,
BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;

// ****GLOBALS****

Scene* g_active_scene;
Start* g_start;
Level1* g_level_1;
Level2* g_level_2;
Level3* g_level_3;

Entity* player;
Entity* enemies;
Map* map;

int g_life_count = 3;

bool g_win = false,
g_go = false;

SDL_Window* g_display_window;
bool g_game_is_running = true;


ShaderProgram g_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

// UI stuff
const char HEART_FILEPATH[] = "assets/heart_sprite.png";
Entity* lives;


// ****ADDITIONAL FUNCTIONS****
void switch_to_scene(Scene* scene)
{
    g_active_scene = scene;
    g_active_scene->initialize();
}


// ****BASIC FUNCTIONS****
void initialize()
{
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("Not So Super Jaden",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    // Viewport Setup 
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(0.5f, 10.5f, -7.5f, 0.5f, -1.0f, 1.0f);

    g_program.set_projection_matrix(g_projection_matrix);
    g_program.set_view_matrix(g_view_matrix);

    // setting shader_program to use
    glUseProgram(g_program.get_program_id());

    // Colors the background as #63ADF2
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);

    

    

    // ————— LEVEL SETUP ————— //
    g_level_1 = new Level1();
    g_level_2 = new Level2();
    g_level_3 = new Level3();
    g_start = new Start();

    g_start->m_state.next_scene = g_level_1;
    g_level_1->m_state.next_scene = g_level_2;
    g_level_2->m_state.next_scene = g_level_3;
    g_level_3->m_state.next_scene = g_level_1;
    switch_to_scene(g_level_1);

    if (g_active_scene != g_start) {
        player = g_active_scene->m_state.player;
        enemies = g_active_scene->m_state.enemies;
    }
   
    map = g_active_scene->m_state.map;


    // LIVES SETUP
    lives = new Entity[3];
    for (int i = 0; i < g_life_count; i++) {
        lives[i].set_entity_type(LIFE);
        lives[i].set_position(glm::vec3( i * 0.5 + 1.0f, 0.0f, 0.0f));
        lives[i].m_texture_id = Utility::load_texture(HEART_FILEPATH);
        lives[i].set_height(0.4);
        lives[i].set_width(0.4);
        lives[i].scale();
    }

    // Blend Stuff
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    
    // Stop player from moving without input
    if (g_active_scene != g_start) {
        player->set_movement(glm::vec3(0.0f));
    }
    

    // One Click Events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_game_is_running = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                // Quit
                g_game_is_running = false;
                break;
            case SDLK_SPACE:
                if (player->m_collided_bottom)
                {
                    player->m_is_jumping = true;
                    Mix_PlayChannel(-1, g_active_scene->m_state.jump_sfx, 0);
                }
                break;
            case SDLK_l:
                switch_to_scene(g_active_scene->get_next_scene());
                if (g_active_scene != g_start) {
                    player = g_active_scene->m_state.player;
                    enemies = g_active_scene->m_state.enemies;
                    map = g_active_scene->m_state.map;
                }
                
                break;
            default:
                break;
            }
        default:
            break;
        }
    }


   

    if (g_active_scene != g_start) {

        // Holding Down Keys
        const Uint8* key_state = SDL_GetKeyboardState(NULL);

        if (key_state[SDL_SCANCODE_A])
        {
            if (player->get_position().x > 6) {
                for (int i = 0; i < g_life_count; i++) {
                    lives[i].set_position(glm::vec3(player->get_position().x - i * 0.5 - 4.0f, 0.0f, 0.0f));
                }
            }
            else {
                for (int i = 0; i < g_life_count; i++) {
                    lives[i].set_position(glm::vec3(i * 0.5 + 1.0f, 0.0f, 0.0f));
                }
            }
            
            player->move_left();
            player->m_animation_indices = player->m_animation[player->LEFT];
        }
        if (key_state[SDL_SCANCODE_D])
        {
            if (player->get_position().x > 6) {
                for (int i = 0; i < g_life_count; i++) {
                    lives[i].set_position(glm::vec3(player->get_position().x - i * 0.5 - 4.0f, 0.0f, 0.0f));
                }
            }
            else {
                for (int i = 0; i < g_life_count; i++) {
                    lives[i].set_position(glm::vec3(i * 0.5 + 1.0f, 0.0f, 0.0f));
                }
            }
            player->move_right();
            player->m_animation_indices = player->m_animation[player->RIGHT];
        }
        // normalize movement
        if (glm::length(player->get_movement()) > 1.0f)
        {
            player->set_movement(
                glm::normalize(
                    player->get_movement()
                )
            );
        }
    }
    
}

void update()
{
    // delta time
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    delta_time += g_accumulator;

    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }

    while (delta_time >= FIXED_TIMESTEP)
    {
        if (g_active_scene != g_start) {
            player->update(FIXED_TIMESTEP, player, enemies, 3, map);
            enemies[0].update(FIXED_TIMESTEP, player, NULL, 0, map);
            for (size_t i = 0; i < 3; i++) {
                lives[i].update(FIXED_TIMESTEP, player, NULL, 0, map);
            }
            delta_time -= FIXED_TIMESTEP;
        }
    }

    g_accumulator = delta_time;

    

    //if (player->dead) {
    //    //go = true;
    //}

    //for (size_t i = 0; i < 3; i++) {
    //    if (enemies[i].m_is_active) {
    //        //win = false;
    //        break;
    //    }
    //    //win = true;
    //}

    //if (g_win == true) {
    //    g_go = true;
    //}

    if (player->get_position().x >= 17) {
        player->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
        switch_to_scene(g_active_scene->get_next_scene());
        player = g_active_scene->m_state.player;
        enemies = g_active_scene->m_state.enemies;
        map = g_active_scene->m_state.map;
    }

    if (player->get_position().x >= 4 && player->get_position().x <= 5 && g_life_count > 0) {
        player->set_position(glm::vec3(player->get_position().x + 1, player->get_position().y, player->get_position().z));
        lives[g_life_count - 1].deactivate();
        g_life_count--;
    }
    
    if (g_life_count == 0) {
        switch_to_scene(g_start);
    }
    

    g_view_matrix = glm::mat4(1.0f);
    if (g_active_scene != g_start) {
        if (player->get_position().x > 5) {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-player->get_position().x + 6, 0.0f, 0.0f));
        }

        else {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        
    }
    

    

}

void render()
{
    g_program.set_view_matrix(g_view_matrix);
    glClear(GL_COLOR_BUFFER_BIT);

    // deactivate all of the platforms and player on game over
    /*if (go) {
        g_state.player->deactivate();
    }*/

    // render everything except UI
    /*else {*/
        /*map->render(&g_program);
        player->render(&g_program);

        for (size_t i = 0; i < 3; i++) {
            enemies[i].render(&g_program);
        }*/
    //}
    glUseProgram(g_program.get_program_id());
    g_active_scene->render(&g_program); 
    if (g_active_scene != g_start) {
        for (size_t i = 0; i < g_life_count; i++) {
            lives[i].render(&g_program);
        }
    }

    

    

    // render UI element based on win or lose
    /*if (win && go) {
        endscreen_text = "You Won!";
        Utility::draw_text(&g_program, font_texture_id, endscreen_text, 0.5f, 0.000001f, ui_element_position);
    }
    else if (go && !win) {
        endscreen_text = "You Lost!";
        Utility::draw_text(&g_program, font_texture_id, endscreen_text, 0.5f, 0.000001f, ui_element_position);
    }*/


    // Swap window
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    delete g_start;
    delete g_level_1;
    delete g_level_2;
    delete g_level_3;
    delete[] lives;
}

// ****GAME LOOP****
int main(int argc, char* argv[])
{
    initialize();

    while (g_game_is_running)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}