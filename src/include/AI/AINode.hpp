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
#ifndef AI_NODE
#define AI_NODE

#include <string>

class AINode{
    private:                            // v----------------v  0  1  2   3   4
        int      m_type;                // Type of comparison (>, <, ==, >=, <=)
        
    public:
        AINode* m_true_children;
        AINode* m_false_children;
        AINode(int p_id, int p_type, float p_comparison_value, float *p_data, int action);
        ~AINode();
        AINode* makeDecision(AINode* node);
        int      m_action;
        float    m_comparison_value;
        float*   m_data;
        int      m_id;
};

#endif