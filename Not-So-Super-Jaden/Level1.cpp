/**
* Author: Jaden Thakur
* Assignment: Not-So-Super-Jaden
* Date due: 2023-12-2, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Level1.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

const char PLAYER_SPRITESHEET_FILEPATH[] = "assets/Player_Spritesheet.png";
const char SPIKY_SPRITESHEET_FILEPATH[] = "assets/Spiky_Spritesheet.png";
const char MAP_FILEPATH[] = "assets/tileset.png";

unsigned int LEVEL1_DATA[] =
{
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

Level1::~Level1()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Level1::initialize()
{
    GLuint map_texture_id = Utility::load_texture(MAP_FILEPATH);
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL1_DATA, map_texture_id, 1.0f, 4, 1);
    
    // Player Stuff
    // Creation
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture(PLAYER_SPRITESHEET_FILEPATH);
    
    // Walking
    m_state.player->m_animation[m_state.player->LEFT] = new int[4] { 1, 3, 5, 7 };
    m_state.player->m_animation[m_state.player->RIGHT] = new int[4] { 0, 2, 4, 6 };

    m_state.player->m_animation_indices = m_state.player->m_animation[m_state.player->LEFT]; // start Jaden looking left
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_cols = 2;
    m_state.player->m_animation_rows = 4;

   /* m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);*/
    
    // Jumping
    m_state.player->m_jumping_power = 5.0f;
    

    // Enemy Stuff
    GLuint spiky_texture_id = Utility::load_texture(SPIKY_SPRITESHEET_FILEPATH);
    
    m_state.enemies = new Entity[ENEMY_COUNT];

    //Spiky Stuff
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_enemy_type(SPIKY);
    m_state.enemies[0].set_mode(IDLE);
    m_state.enemies[0].m_texture_id = spiky_texture_id;
    m_state.enemies[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(1.0f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    m_state.enemies[0].m_animation[m_state.enemies[0].LEFT] = new int[1] {0};
    m_state.enemies[0].m_animation[m_state.enemies[0].RIGHT] = new int[1] {1};
    m_state.enemies[0].m_animation_indices = m_state.enemies[0].m_animation[m_state.enemies[0].LEFT];
    m_state.enemies[0].m_animation_time = 0.0f;
    m_state.enemies[0].m_animation_frames = 1;
    m_state.enemies[0].m_animation_index = 0;
    m_state.enemies[0].m_animation_cols = 2;
    m_state.enemies[0].m_animation_rows = 1;
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_state.bgm = Mix_LoadMUS("assets/dooblydoo.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(0.5f);
    
    m_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}

void Level1::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
    }
}


void Level1::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
}
