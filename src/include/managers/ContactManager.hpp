/*
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
    along with this program.  If not, see http://www.gnu.org/licenses/.

    You can contact Chaotic Games at: chaoticgamesdev@gmail.com
*/

#ifndef CONTACT_MANAGER
#define CONTACT_MANAGER

#include <Box2D.h>

class ContactManager : public b2ContactListener{
public:
    void BeginContact(b2Contact* p_contact);
    void EndContact(b2Contact* p_contact);
    void PreSolve(b2Contact* p_contact, const b2Manifold* p_oldManifold);

    void checkCollision(b2Fixture* fixture1, b2Fixture* fixture2, b2Contact* p_contact);
    void checkLeave(b2Fixture* fixture1, b2Fixture* fixture2, b2Contact* p_contact);
    void checkPreSolve(b2Fixture* fixture1, b2Fixture* fixture2, b2Contact* p_contact, const b2Manifold* p_oldManifold);

    void setIdPlatforms(int p_id)   { idPlatform = p_id; }
    void setIdPlayer1(int p_id)     { idPlayer1 = p_id;  }
    void setIdPlayer2(int p_id)     { idPlayer2 = p_id;  }

private:
    int idPlatform;
    int idPlayer1;
    int idPlayer2;

};

#endif