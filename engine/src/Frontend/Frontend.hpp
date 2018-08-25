#pragma once

namespace ck
{
    // A frontend, what actually runs the game, and the actual executable
    class Frontend
    {
        virtual bool init() = 0;
        virtual bool shutdown() = 0;
    };
}