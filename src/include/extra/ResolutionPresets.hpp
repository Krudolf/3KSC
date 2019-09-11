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

#ifndef RESOLUTION_PRESETS
#define RESOLUTION_PRESETS

#include <string>

struct ResolutionPreset{
    int         width;
    int         height;
    std::string resolutionString;
};

static ResolutionPreset g_resolutionPresets[] = {
      //Width       //Height        //String    
    { 800           , 600           , "800x600"     },
    { 1024          , 768           , "1024x768"    },
    { 1280          , 720           , "1280x720"    },
    { 1366          , 768           , "1366x768"    },
    { 1920          , 1080          , "1920x1080"   }
};

#endif