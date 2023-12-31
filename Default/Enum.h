#pragma once

enum DIRECTION { DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LT, DIR_RT, DIR_END };

enum OBJID { OBJ_PLAYER, OBJ_BULLET, OBJ_MONSTER, OBJ_MOUSE, OBJ_UI, OBJ_SHIELD, OBJ_SKILL, OBJ_BUTTON, OBJ_DECO, OBJ_END };

enum SCENEID { SC_LOGO, SC_MENU, SC_EDIT, SC_STAGE, SC_ENDING, SC_END };

enum STAGE { STAGE_TUTORIAL, STAGE_1, STAGE_2, STAGE_END };

enum MONSTERID { MONSTER_GENERAL, MONSTER_BOSS, MONSTER_END};

enum RENDERID { RENDER_BACKGROUND, RENDER_GAMEOBJECT, RENDER_EFFECT, RENDER_UI, RENDER_END };

enum CHANNELID { SOUND_BGM, SOUND_EFFECT, SOUND_SKILL, SOUND_ENEMY,SOUND_BOSS, SOUND_BOSSSKILL, MAXCHANNEL };

enum UIID {UI_TUTORIAL, UI_CHARACTER, UI_ENEMY, UI_MAP, UI_SKILL, UI_END};

enum SKILLID {SKILL_STORM, SKILL_EXPLOSION, SKILL_TORNADO, SKILL_HEAL, SKILL_NONE};