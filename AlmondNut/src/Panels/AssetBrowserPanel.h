#pragma once
#include <filesystem>

#include "imgui.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/FileSystem.h"
#include "Renderer/Texture.h"

class AssetBrowserPanel
{
public:
	AssetBrowserPanel();
	void OnStart();
	void DrawFileNode(std::shared_ptr<FileSystem::FileNode> dir_entry, std::filesystem::path relativeDirectory);
	const char* GetIcon(const std::string& string);
	void OnImGuiRender();

private:
    std::string searchString = "";
    const std::filesystem::path s_AssetsDirectory = "assets";
	std::filesystem::path m_CurrentDirectory;
    std::filesystem::path m_SelectedDirectory;
	float w = 200.0f;

    std::shared_ptr<FileSystem::FileNode>& RootNode = FileSystem::RootNode;
    std::shared_ptr<FileSystem::FileNode>& CurrentNode = FileSystem::CurrentNode;

    void DrawSplitter(int split_vertically, float thickness, float* size0, float* size1, float min_size0, float min_size1)
    {
        ImVec2 backup_pos = ImGui::GetCursorPos();
        if (split_vertically)
            ImGui::SetCursorPosY(backup_pos.y + *size0);
        else
            ImGui::SetCursorPosX(backup_pos.x + *size0);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));          // We don't draw while active/pressed because as we move the panes the splitter button will be 1 frame late
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.10f));
        ImGui::Button("##Splitter", ImVec2(!split_vertically ? thickness : -1.0f, split_vertically ? thickness : -1.0f));
        ImGui::PopStyleColor(3);

        ImGui::SetItemAllowOverlap(); // This is to allow having other buttons OVER our splitter. 

        if (ImGui::IsItemActive())
        {
            float mouse_delta = split_vertically ? ImGui::GetIO().MouseDelta.y : ImGui::GetIO().MouseDelta.x;

            // Minimum pane size
            if (mouse_delta < min_size0 - *size0)
                mouse_delta = min_size0 - *size0;
            if (mouse_delta > *size1 - min_size1)
                mouse_delta = *size1 - min_size1;

            // Apply resize
            *size0 += mouse_delta;
            *size1 -= mouse_delta;
        }
        ImGui::SetCursorPos(backup_pos);
    }
};
