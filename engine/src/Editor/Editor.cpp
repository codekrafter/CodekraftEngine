#include <algorithm>
#include <cxxabi.h>

#include "ThirdParty/IMGUI/imgui.h"
#include "ThirdParty/ImGuizmo.hpp"
#include "Editor/Editor.hpp"
#include "ECS/Actor.hpp"
#include "Rendering/Shader.hpp"
#include "ECS/WorldManager.hpp"
#include "StaticMesh/StaticMeshActor.hpp"
#include "StaticMesh/StaticMeshComponent.hpp"
#include "Lighting/LightActor.hpp"
#include "ECS/Level.hpp"
#include "Logging.hpp"
#include "Camera/ACamera.hpp"
#include "Camera/CameraUtils.hpp"

namespace ck
{

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
    std::vector<Actor *> &c = WorldManager::getInstance()->getLevel()->contents;
    ImGui::Separator();
    selected = c[si];
    for (int i = 0; i < c.size(); ++i)
    {
        Actor *a = c[i];
        if (a->getName() == "" || a->getName().empty() || a->getName().size() == 0)
        {
            a->setName(demangle(typeid(*a).name()));
        }
        map[a] = map[a];
        bool activated = (selected == a);
        bool pre = activated;
        ImGui::Selectable(a->getName().c_str(), &activated);
        if (!(pre) && (activated))
        {
            ptrdiff_t pos = std::find(c.begin(), c.end(), a) - c.begin();
            si = pos;
            selected = a;
        }
        //ImGui::TreeNodeEx("a", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", a->getName().c_str());
    }
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    // TODO: Property Editor

    // Draw Gizmo
    if (selected != nullptr)
    {
        RenderingConfig rc;
        glm::mat4 proj = getProjection(rc);
        Transform cam_trans = WorldManager::getInstance()->getLevel()->getCamera()->getTransform();
        glm::vec3 pos = cam_trans.location;
        DirVectors dv = getVectors(cam_trans.rotation.y, cam_trans.rotation.x);
        glm::mat4 view = getViewMatrix(pos, dv);

        glm::mat4 model;

        Transform trans = selected->getTransform();
        ImGuizmo::RecomposeMatrixFromComponents(&trans.location.x, &trans.rotation.x, &trans.scale.x, &model[0][0]);

        ImGuiIO &io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

        glm::mat4 om = model;
        ImGuizmo::Manipulate(&view[0][0], &proj[0][0], ImGuizmo::TRANSLATE, ImGuizmo::WORLD, &model[0][0], NULL, NULL);

        ImGuizmo::DecomposeMatrixToComponents(&model[0][0], &trans.location.x, &trans.rotation.x, &trans.scale.x);

        selected->setTransform(trans);
    }
    ImGui::End();

    ImGui::Begin("Shader Editor", &s_open);
    if (ImGui::Button("Reload Selected"))
    {

        bool found = false;
        if (!(selected == nullptr))
        {

            for (ActorComponent *ac : selected->components)
            {
                if (typeid(StaticMeshComponent) == typeid(*ac))
                {
                    found = true;
                    StaticMeshComponent *smc = dynamic_cast<StaticMeshComponent *>(ac);
                    smc->mat->getShader()->reload();
                    break;
                }
            }

            if (!found)
            {
                LOG(INFO) << "Didnt find Static Mesh Component";
            }
        }
    }
    ImGui::End();
};
} // namespace ck