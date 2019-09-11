#include "../include/states/State.hpp"

void State::setDeltaTime(long long p_delta){
    m_deltaTime = p_delta / 1000000000.0;
}