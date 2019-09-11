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

#include "../include/AI/AINode.hpp"

AINode::AINode(int p_id, int p_type, float p_comparison_value, float *p_data, int p_action){
    m_type              = p_type;
    m_comparison_value  = p_comparison_value;
    m_data              = p_data;
    m_action            = p_action;
    m_id                = p_id;
}

AINode::~AINode(){}

AINode* AINode::makeDecision(AINode* node){
    if(node->m_type==0){
        if(node->m_data != 0){
            if(*(node->m_data) > node->m_comparison_value){
                return(makeDecision(node->m_true_children));
            }
            else{
                return(makeDecision(node->m_false_children));
            }
        }
        else
            return(node);
    }
    else if(node->m_type==1){
        if(node->m_data != 0){
            if(*(node->m_data) < node->m_comparison_value){
                return(makeDecision(node->m_true_children));
            }
            else{
                return(makeDecision(node->m_false_children));
            }
        }
        else
            return(node);
    }
    else if(node->m_type==2){
        if(node->m_data != 0){
            if(*(node->m_data) == node->m_comparison_value){
                return(makeDecision(node->m_true_children));
            }
            else{
                return(makeDecision(node->m_false_children));
            }
        }
        else
            return(node);
    }
    else if(node->m_type==3){
        if(node->m_data != 0){
            if(*(node->m_data) >= node->m_comparison_value){
                return(makeDecision(node->m_true_children));
            }
            else{
                return(makeDecision(node->m_false_children));
            }
        }
        else
            return(node);
    }
    else if(node->m_type==4){
        if(node->m_data != 0){
            if(*(node->m_data) <= node->m_comparison_value){
                return(makeDecision(node->m_true_children));
            }
            else{
                return(makeDecision(node->m_false_children));
            }
        }
        else{
            return(node);
        }
    }
    else{
        return(node);
    }
}

