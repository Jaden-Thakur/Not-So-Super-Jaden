/**
* Author: Jaden Thakur
* Assignment: Platformer
* Date due: 2023-12-2, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Level0.h"
#include "Utility.h"

std::string m_text;
std::string m_title;
glm::mat4 m_title_matrix;
glm::mat4 m_text_matrix;
glm::vec3 m_title_position;
glm::vec3 m_text_position;
GLuint m_font_texture_id;
const char m_FONT_FILEPATH[] = "assets/font.png";

Level0::~Level0()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Level0::initialize()
{
    m_state.player = new Entity();
    m_state.enemies = new Entity[ENEMY_COUNT];

    m_font_texture_id = Utility::load_texture(m_FONT_FILEPATH);

    /**
     BGM
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/mario.mp3");
    Mix_PlayMusic(m_state.bgm, 1);
    Mix_VolumeMusic(7.5f);
}

void Level0::update(float delta_time)
{
    
}


void Level0::render(ShaderProgram* program)
{
    m_title = "Not So Super Jaden";
    m_text = "Press \"enter\" to Start";
    m_title_position = glm::vec3(0.5f, -1.0f, 0.0f);
    m_text_position = glm::vec3(0.5f, -3.0f, 0.0f);
    Utility::draw_text(program, m_font_texture_id, m_title, 0.5f, 0.000001f, m_title_position);
    Utility::draw_text(program, m_font_texture_id, m_text, 0.4f, 0.000001f, m_text_position);
}
