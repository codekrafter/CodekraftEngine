#include "ThirdParty/IMGUI/imgui.h"
#include "Editor.hpp"
#include "AssetManager.hpp"
#include "ThirdParty/easylogging/easylogging++.h"
#include "Shader.hpp"
#include <algorithm>

namespace ck
{

Editor *Editor::inst;

Editor::Editor()
{
};

Editor::~Editor()
{
};

Editor *Editor::getInstance()
{
    if (inst == nullptr)
    {
        inst = new Editor();
        inst->assetMenu = false;
        inst->assetLoader = false;
        inst->assetSaver = false;
        inst->showEditor = false;
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
    //return (assetMenu || assetLoader);
    return showEditor;
};

void Editor::drawassetMenus()
{
    /*for (AssetFile file : openEditors)
    {
        bool openEditor = true;
        if (file.type == "SHADER")
        {
            if (!openEditor)
            {
                return;
            }
            if (vt[0] == '\0' && ft[0] == '\0' && gt[0] == '\0')
            {
                Shader *shader = dynamic_cast<Shader *>(file.asset());
                std::copy(shader->vertex.begin(), shader->vertex.end(), vt);
                std::copy(shader->fragment.begin(), shader->fragment.end(), ft);
                std::copy(shader->geometry.begin(), shader->geometry.end(), gt);
            }
            ImGui::Begin("Shader Editor", &openEditor);
            ImGui::Text("Vertex");
            ImGui::SameLine();
            ImGui::InputTextMultiline("v", vt, IM_ARRAYSIZE(vt));
            ImGui::Text("Fragment");
            ImGui::SameLine();
            ImGui::InputTextMultiline("f", ft, IM_ARRAYSIZE(ft));
            ImGui::Text("Geometry");
            ImGui::SameLine();
            ImGui::InputTextMultiline("g", gt, IM_ARRAYSIZE(gt));
            ImGui::End();
        }
        else
        {
            LOG(ERROR) << "Could not find editor for asset of type '" << file.type << "'";
        }
        if (!openEditor)
        {
            openEditors.erase(std::remove(openEditors.begin(), openEditors.end(), file), openEditors.end());
        }
    }*/
};

void Editor::toggleEditor()
{
    if (true) //TODO check if debug build
    {
        if (showEditor)
        {
            showEditor = false;
            /*assetMenu = false;
            assetLoader = false;*/
        }
        else
        {
            showEditor = true;
            assetMenu = true;
        }
    }
}

void Editor::Draw()
{
    if (!showEditor)
    {
        return;
    }
    if (assetMenu)
    {
        ImGui::Begin("Asset Menu", &assetMenu, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open.."))
                {
                    assetLoader = true;
                }
                if (ImGui::MenuItem("Save"))
                {
                    //am->close();
                }
                if (ImGui::MenuItem("Save As.."))
                {
                    assetSaver = true;
                }
                if (ImGui::MenuItem("Close"))
                {
                    assetMenu = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        //ImGui::ShowDemoWindow();
        /*for (std::pair<std::string, AssetFile> pair : am->getMap())
        {
            /*if (ImGui::BeginMenu(s.c_str()))
        {
            ImGui::Text("Child");
            ImGui::EndMenu();
        };/
            ImGui::Text(pair.first.c_str(), "NULL");
            ImGui::SameLine();
            ImGui::Text("(");
            ImGui::SameLine();
            ImGui::Text(pair.second.type.c_str(), "NULL");
            ImGui::SameLine();
            ImGui::Text(")");
            ImGui::SameLine();
            if (ImGui::Button("Open Editor"))
            {
                openEditors.push_back(pair.second);
            };
        }*/
        ImGui::End();
    }

    if (assetLoader)
    {
        ImGui::Begin("Asset Loader", &assetLoader);
        ImGui::SetWindowFocus();
        ImGui::Text("File Name: ");
        ImGui::SameLine();
        ImGui::InputText("", fn, IM_ARRAYSIZE(fn));
        ImGui::Text(fn, "NULL");
        if (ImGui::Button("Load"))
        {
            //am->open(fn);
            assetLoader = false;
        }
        ImGui::End();
    }
    if (assetSaver)
    {
        ImGui::Begin("Asset Saver", &assetSaver);
        ImGui::SetWindowFocus();
        ImGui::Text("File Name: ");
        ImGui::SameLine();
        ImGui::InputText("", fn, IM_ARRAYSIZE(fn));
        ImGui::Text(fn, "NULL");
        if (ImGui::Button("Save"))
        {
            //am->close(fn);
            assetSaver = false;
        }
        ImGui::End();
    }

    drawassetMenus();
};
}