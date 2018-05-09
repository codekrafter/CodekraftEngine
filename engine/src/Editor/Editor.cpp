#include <algorithm>
#include <cxxabi.h>

#include "ThirdParty/IMGUI/imgui.h"
#include "ThirdParty/ImGuizmo.hpp"
#include "Editor/Editor.hpp"
#include "ECS/Actor.hpp"
#include "Rendering/Shader.hpp"
#include "ECS/WorldManager.hpp"
#include "StaticMesh/StaticMeshActor.hpp"
#include "Lighting/LightActor.hpp"
#include "ECS/Level.hpp"

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
    //ImGui::ShowDemoWindow();
    ImGui::Begin("Scene Editor", &showEditor);
    std::vector<Actor *> c = WorldManager::getInstance()->getLevel()->contents;
    if (ImGui::CollapsingHeader("Level Actors"))
    {
        ImGui::InputInt("Selection ID", &si);
        if (si < 0 || si > c.size() + 1)
        {
            si = 0;
        }
        selected = c[si];
        for (int i = 0; i < c.size(); ++i)
        {
            Actor *a = c[i];
            if (a->getName() == "" || a->getName().empty() || a->getName().size() == 0)
            {
                a->setName(demangle(typeid(*a).name()).c_str());
            }
            ImGui::TreeNodeEx((void *)a, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, a->getName().c_str());
        }
    } /*
    else
    {
        selected = nullptr;
    }*/
    ImGui::Spacing();
    const char *items[] = {"StaticMesh", "Light"};
    static int item_current = 0;
    ImGui::Combo("Actor", &item_current, items, IM_ARRAYSIZE(items));
    if (ImGui::Button("Spawn In Actor"))
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

    // Draw Gizmo
    if (selected != nullptr)
    {
        glm::mat4 view = WorldManager::getInstance()->getLevel()->getCamera()->GetViewMatrix();
        glm::mat4 proj = WorldManager::getInstance()->getLevel()->getCamera()->getProjection();
        glm::mat4 model;

        Transform trans = selected->getTransform();
        ImGuizmo::RecomposeMatrixFromComponents(&trans.location.x, &trans.rotation.x, &trans.scale.x, &model[0][0]);

        ImGuiIO &io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

        glm::mat4 om = model;
        ImGuizmo::Manipulate(&view[0][0], &proj[0][0], ImGuizmo::TRANSLATE, ImGuizmo::WORLD, &model[0][0], NULL, NULL);

        float t[3], r[3], s[3];
        //ImGuizmo::DecomposeMatrixToComponents(&model[0][0], t, r, s);
        ImGuizmo::DecomposeMatrixToComponents(&model[0][0], &trans.location.x, &trans.rotation.x, &trans.scale.x);

        //trans.location = glm::vec3(t[0], t[1], t[2]);
        //trans.rotation = Math::Quaternion::euler(r[0], r[1], r[2]);
        //trans.scale = Math::Vector3(s[0], s[1], s[2]);

        selected->setTransform(trans);
    }
    ImGui::End();
};
} // namespace ck