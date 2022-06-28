#include "AssetBrowserPanel.h"

#include "imgui.h"
const std::filesystem::path s_AssetsDirectory = "assets";

AssetBrowserPanel::AssetBrowserPanel()
	:m_CurrentDirectory(s_AssetsDirectory)
{
}

void AssetBrowserPanel::OnStart()
{
	m_FolderIcon = std::make_shared<Texture>("assets/textures/folderClosed.png");
	m_FolderOpenIcon = std::make_shared<Texture>("assets/textures/folderOpen.png");
}



void AssetBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Asset Browser");

    // Left
    {
        float h = ImGui::GetWindowHeight() - 40;
  

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        DrawSplitter(0, 8.0f, &w, &h, 8, 8);
        ImGui::BeginChild("File Hierarchy", ImVec2(w, h), true);
        {
            for (auto const& dir_entry : std::filesystem::directory_iterator(s_AssetsDirectory))
            {
                auto path = dir_entry.path();
                auto relativePath = std::filesystem::relative(path, s_AssetsDirectory);

                if (dir_entry.is_directory())
                {
	                if (ImGui::Button(relativePath.string().c_str()))
	                {
                        
                        m_CurrentDirectory = s_AssetsDirectory / relativePath.filename();
                        std::cout << m_CurrentDirectory << "\n";
	                }
	                
                }
            }
            ImGui::EndChild();
            ImGui::SameLine();
        }

        ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, h));
        {
            if (ImGui::IsItemActive())
                w += ImGui::GetIO().MouseDelta.x;
            ImGui::SameLine();
        }

        ImGui::SameLine();
        ImGui::PopStyleVar(1);
    }

    // Right
    {
        ImGui::BeginGroup();
        ImGui::BeginChild("right pane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us

        for (auto const& dir_entry : std::filesystem::directory_iterator(m_CurrentDirectory))
        {
            auto path = dir_entry.path();
            auto relativePath = std::filesystem::relative(path, m_CurrentDirectory);

            if (dir_entry.is_directory())
            {
                ImGui::BeginGroup();
                ImGui::ImageButton((ImTextureID)m_FolderOpenIcon->ID(), { 50,50 }, {0,1}, {1,0});
                ImGui::NewLine();
                ImGui::Text(dir_entry.path().filename().string().c_str());
                ImGui::EndGroup();
                ImGui::SameLine();
            }


            
        }

		ImGui::EndChild();
        ImGui::EndGroup();
    }





	ImGui::End();
}
