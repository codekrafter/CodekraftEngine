#include "ThirdParty/IMGUI/imgui.h"
#include "Editor.hpp"
#include "Actor.hpp"
#include "Shader.hpp"
#include "WorldManager.hpp"
#include "StaticMeshActor.hpp"
#include "LightActor.hpp"
#include <algorithm>
#include <cxxabi.h>

namespace ck
{

std::string demangle(const char *name)
{

    int status = -4; // some arbitrary value to eliminate the compiler warning

    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void (*)(void *)> res{
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free};

    return (status == 0) ? res.get() : name;
}

Editor *Editor::inst;

Editor::Editor()
{
    showEditor = false;
};

Editor *Editor::getInstance()
{
    if (inst == nullptr)
    {
        inst = new Editor();
    }
    return inst;
};

void Editor::destroyInstance()
{
    if (inst == nullptr)
    {
        return;
    }
    else
    {
        delete inst;
        inst = nullptr;
    }
};

bool Editor::showCursor()
{
    return showEditor;
};

void Editor::toggleEditor()
{
    showEditor = !showEditor;
}

float a_mean(float *f, int size)
{
    if (size == 0 || f == nullptr)
        return 0;
    float total;
    for (int i = 0; i < size; ++i)
    {
        total = total + f[i];
    }
    return total / size;
}

void Editor::Draw()
{
    if (!showEditor)
        return;
    ImGui::ShowDemoWindow();
    ImGui::Begin("Scene Editor", &showEditor);
    std::vector<Actor *> c = WorldManager::getInstance()->getLevel()->contents;
    if (ImGui::CollapsingHeader("Level Actors"))
    {
        for (Actor *a : c)
        {
            if (ImGui::TreeNode(demangle(typeid(*a).name()).c_str()))
            {
                Transform t = a->getTransform();
                glm::vec3 l = t.location;
                glm::vec3 r = t.rotation;
                glm::vec3 s = t.scale;

                float ll[3] = {l.x, l.y, l.z};
                ImGui::DragFloat3("Location", ll);
                l.x = ll[0];
                l.y = ll[1];
                l.z = ll[2];

                float rr[3] = {r.x, r.y, r.z};
                ImGui::DragFloat3("Rotation", rr);
                r.x = rr[0];
                r.y = rr[1];
                r.z = rr[2];

                float ss[3] = {s.x, s.y, s.z};
                bool uni = false;
                ImGui::Checkbox("Uniform Scale", &map[a].first);
                if (map[a].first)
                {
                    float m = a_mean(ss, 3);
                    ImGui::DragFloat("Scale", &m, 0.01f, 0.01f, 10.0f);
                    s.x = m;
                    s.y = m;
                    s.z = m;
                }
                else
                {
                    ImGui::DragFloat3("Scale", ss);
                    s.x = ss[0];
                    s.y = ss[1];
                    s.z = ss[2];
                }

                t.location = l;
                t.rotation = r;
                t.scale = s;
                a->setTransform(t);

                ImGui::TreePop();
            }
        }
    }
    ImGui::Spacing();
    const char *items[] = {"StaticMesh", "Light"};
    static int item_current = 0;
    ImGui::Combo("Actor", &item_current, items, IM_ARRAYSIZE(items));
    if(ImGui::Button("Spawn In Actor"))
    {
        switch (item_current)
        {
            case 0:
            WorldManager::getInstance()->getLevel()->addActor(new StaticMeshActor());
            break;
            case 1:
                        WorldManager::getInstance()->getLevel()->addActor(new LightActor());
            break;
        }
    }
    ImGui::End();
};
}