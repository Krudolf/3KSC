/********************************************************************************
*********************************************************************************
	3KSC: A fighting game
    Copyright (C) 2018  Chaotic Games

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You can contact Chaotic Games at: chaoticgamesdev@gmail.com
*********************************************************************************
*********************************************************************************/

#ifndef ARENA
#define ARENA

class Game;
class EngineManager;
class PhysicsManager;
class Item;

#include "characters/Character.hpp"
#include "../include/entities/items/Portal.hpp"
#include <Box2D/Common/b2Math.h>

class Arena : public Entity {
public:
    Arena(float p_position[3], float p_scale, const char* p_modelURL);
    ~Arena();

    static Arena*   getInstance();
    void            spawnPlayers();
    void            addPlayer(bool p_online = false);
    int             getPlayerCount();
    Character*      getPlayer(int p_index);
    void            catchItem(int p_owner, float p_where[3]);
	void            finishRound();
    void            movePlatforms();
    void            animateBackground();
    void            restart();
    void            setSpawnPositions(float p_spawnPositions[4][3]); 
    void            update(float p_delta);
    void            portalSpawner();
    void            portalSpawnerOnline();
    void            spawnPortal();
    void            hidePortal();
    bool            portalIsActive();
    float*          getPortalPosition();
    bool            spawnRandomItem();
    void            onlineUpdate(float p_time);
    void            onlineUpdateClient(float p_time);
    void            spawnItemAt(int p_type, int x, int y);
    b2Vec2          getClosestItemPosition(b2Vec2 p_position);
    void            setOnline(bool p_state);
    bool            getOnline();
    void            setRespawnPositions(float p_respawnPosition[3]){ for(int i = 0; i < 3; i++) m_respawnPosition[i] = p_respawnPosition[i]; }
    void            setItemRange(float p_itemRange[3]) { for(int i = 0; i < 3; i++) m_spawnItemRange[i] = p_itemRange[i]; }

    float*          getRespawnPosition();

    void            pleaseKill(int p_playerIndex);
    void            spawnPlayer(bool p_online = false);
    void            spawnPlayerOnline(bool p_online, int p_index);

private: 
    static Arena*       m_instance;
    Game*               m_game;
    PhysicsManager*     m_physicsManager;
    
    static const char*  m_modelURLs[3];
    static const char*  m_skyboxURLs[3][6];
    float               m_time;
    std::vector<Item*>  m_items;
    float               m_offsetSpawnTime;
    float               m_nextSpawnTime;
    int                 m_spawnedItems;
    int                 m_usedItems;
    int                 m_maxItemsOnScreen = 5;
    int                 m_currentItems;
    int                 m_lastItemType = 0;
    int                 m_spawningPortalTime;
    Portal*             m_portal;

    int                 m_playerCount;
    Character**         m_players;

    float               m_spawnPositions[4][3];
    float               m_respawnPosition[3]; // First []: index. Second []: [0] for x, [1] for y, [2] for z
    float               m_spawnItemRange[3];

    bool                m_online = false;

    bool                m_portalSpawned;
    float               m_portalDuration;
    float               m_portalTime;
    float               m_portalOffsetTime;
    float               m_portalSpawnTime;

    float               m_musicIntensity[4];


};

#endif