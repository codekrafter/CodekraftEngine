#include <string>
#include <cstring>
#include <iostream>
#include <g3log/logmessage.hpp>


#include "Core/EngineApp.hpp"
#include "Rendering/Display.hpp"
#include "Rendering/DisplayOpenGL.hpp"

#ifdef HAVE_CXA_DEMANGLE
const char* demangle(const char* name)
{
   char buf[1024];
    unsigned int size=1024;
    int status;
    char* res = abi::__cxa_demangle (name,
                                 buf,
                                 &size,
                                 &status);
    return res;
  }
#else
const char* demangle(const char* name)
{
  return name;
}
#endif  

int remove_whitesaces(char *p)
{
    int len = strlen(p);
    int new_len = 0;
    bool space = false;

    for (int i = 0; i < len; i++)
    {
        switch (p[i])
        {
        case ' ': space = true;  break;
        case '\t': space = true;  break;
        case '\n': break; // you could set space true for \r and \n
        case '\r': break; // if you consider them spaces, I just ignore them.
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

static std::string ckLogFormatter(const g3::LogMessage& msg)
{
    std::string out;
      out.append(msg.timestamp() + "\t"
                 + msg.level() 
                 + " [" 
                 + msg.file() 
                 + "->" 
                 + msg.function() 
                 + ":" + msg.line() + "]\t");
      return out;
}

struct CustomSink {

  enum FG_Color {YELLOW = 93, RED = 91, GREEN=32, WHITE = 97};

  FG_Color GetColor(const LEVELS level) const {
     if (level.value == WARNING.value) { return YELLOW; }
     if (level.value == DEBUG.value) { return GREEN; }
     if(level.value == ERROR.value) {return RED;}
     if (g3::internal::wasFatal(level)) { return RED; }

     return WHITE;
  }
  
  void ReceiveLogMessage(g3::LogMessageMover logEntry) {
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
    switch (c.display.type)
    {
    case DisplayType::OPENGL:
        display = new opengl::DisplayOpenGL(c.display);
        break;
    default:
        LOG(ERROR) << "Cannot Find Display for DisplayType " << demangle(typeid(c.display.type).name());
    }
};
CKEngine::~CKEngine()
{
    delete display;
    display = nullptr;
};
Display *CKEngine::getDisplay()
{
    return display;
};

void CKEngine::update()
{
    display->update();
}
}