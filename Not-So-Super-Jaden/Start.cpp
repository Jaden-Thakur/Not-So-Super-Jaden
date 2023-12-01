/**
* Author: Jaden Thakur
* Assignment: Not-So-Super-Jaden
* Date due: 2023-12-2, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Start.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

const char PLAYER_SPRITESHEET_FILEPATH[] = "assets/Player_Spritesheet.png";
const char SPIKY_SPRITESHEET_FILEPATH[] = "assets/Spiky_Spritesheet.png";
const char MAP_FILEPATH[] = "assets/tileset.png";

//unsigned int LEVEL_DATA[] =
//{
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
//    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
//    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
//};

Start::~Start()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Start::initialize()
{
    //GLuint map_texture_id = Utility::load_texture("assets/images/tileset.png");
    /*m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA, map_texture_id, 1.0f, 4, 1);*/

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/dooblydoo.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(0.0f);

}

void Start::update(float delta_time)
{
   /* m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
    }*/
}


void Start::render(ShaderProgram* program)
{
    /*m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }*/
}
