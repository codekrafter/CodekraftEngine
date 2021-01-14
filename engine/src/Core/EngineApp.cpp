#include <string>
#include <cstring>
#include <iostream>
#include <chrono>

#include "Core/EngineApp.hpp"
#include "Assets/AssetManager.hpp"
#include "Rendering/Core/RenderingManager.hpp"
#include "ECS/WorldManager.hpp"
#include "ECS/Level.hpp"
#include "Logging.hpp"

int remove_whitesaces(char *p)
{
    int len = strlen(p);
    int new_len = 0;
    bool space = false;

    for (int i = 0; i < len; i++)
    {
        switch (p[i])
        {
        case ' ':
            space = true;
            break;
        case '\t':
            space = true;
            break;
        case '\n':
            break; // you could set space true for \r and \n
        case '\r':
            break; // if you consider them spaces, I just ignore them.
        default:
            if (space && new_len > 0)
                p[new_len++] = ' ';
            p[new_len++] = p[i];
            space = false;
        }
    }

    p[new_len] = '\0';

    return new_len;
}

inline int remove_whitesaces(std::string &str)
{
    int len = remove_whitesaces(&str[0]);
    str.resize(len);
    return len; // returning len for consistency with the primary function
                // but u can return std::string instead.
}

static std::string ckLogFormatter(const g3::LogMessage &msg)
{
    std::string out;
    out.append(msg.timestamp() + "\t" + msg.level() + " [" + msg.file() + "->" + msg.function() + ":" + msg.line() + "]\t");
    return out;
}

struct CustomSink
{

    enum FG_Color
    {
        YELLOW = 93,
        RED = 91,
        GREEN = 32,
        WHITE = 97
    };

    FG_Color GetColor(const LEVELS level) const
    {
        if (level.value == WARNING.value)
        {
            return YELLOW;
        }
        if (level.value == DEBUG.value)
        {
            return GREEN;
        }
        if (level.value == LOG_ERROR.value)
        {
            return RED;
        }
        if (g3::internal::wasFatal(level))
        {
            return RED;
        }

        return WHITE;
    }

    void ReceiveLogMessage(g3::LogMessageMover logEntry)
    {
        auto level = logEntry.get()._level;
        auto color = GetColor(level);
        std::string m = logEntry.get().toString(ckLogFormatter);
        remove_whitesaces(m);

        std::cout << "\033[" << color << "m"
                  << m << "\033[m" << std::endl;
    }
};
namespace ck
{
CKEngine::CKEngine(EngineConfig c)
{
    config = c;
    auto sinkHandle = logworker->addSink(std::make_unique<CustomSink>(), &CustomSink::ReceiveLogMessage);
    initializeLogging(logworker.get());
    /*switch (c.rendering.draw)
    {
    case DrawType::OPENGL:
        display = new opengl::DisplayOpenGL(c.rendering);
        break;
    default:
        LOG(LOG_ERROR) << "Cannot Find Display for DisplayType " << demangle(typeid(c.rendering.draw).name());
    }*/
    //display = new opengl::DisplayOpenGL(c.rendering);

    am = new AssetManager();

    rm = new RenderingManager(c.rendering);
};
CKEngine::~CKEngine()
{
    game->onShutdown();
    delete game;
    game = nullptr;

    delete am;
    am = nullptr;

    delete rm;
    rm = nullptr;
};

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsec;

void CKEngine::init()
{
    LOG(INFO) << "Initialized Engine";
    Level *level = WorldManager::getInstance()->getLevel();
    RenderingManager::inst()->getWindow()->init();
    RenderingManager::inst()->getRenderer()->init();

    level->init();

    lastFrame = Time::now();
}

void CKEngine::update()
{
    Level *level = WorldManager::getInstance()->getLevel();
    tunit currentFrame = Time::now();
    fsec deltaTime = currentFrame - lastFrame;
    float dt = deltaTime.count();
    lastFrame = currentFrame;

    RenderingManager::inst()->preRender();
    level->tick(dt);
    std::vector<Actor *> actors = level->getContents();
    for (Actor *a : actors)
    {
        a->render();
        for (ActorComponent *ac : a->components)
        {
            ac->render();
        }
    }

    RenderingManager::inst()->postRender();
}
} // namespace ck