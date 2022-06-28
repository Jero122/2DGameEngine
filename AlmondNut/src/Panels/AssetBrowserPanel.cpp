#include "AssetBrowserPanel.h"

#include "imgui.h"
const std::filesystem::path s_AssetsDirectory = "assets";

AssetBrowserPanel::AssetBrowserPanel()
	:m_CurrentDirectory(s_AssetsDirectory), m_SelectedDirectory(s_AssetsDirectory)
{
}

void AssetBrowserPanel::OnStart()
{
	m_FolderIcon = std::make_shared<Texture>("assets/textures/folderClosed.png");
	m_FolderOpenIcon = std::make_shared<Texture>("assets/textures/folderOpen.png");
}


void AssetBrowserPanel::DrawFileNode(std::filesystem::directory_entry const& dir_entry, std::filesystem::path relativeDirectory)
{
	const auto path = dir_entry.path();
    const auto relativePath = std::filesystem::relative(path, relativeDirectory);

	if (dir_entry.is_directory())
	{
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags flags = (m_CurrentDirectory == path) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGui::PushID(relativePath.string().c_str());
		bool node_open = ImGui::TreeNodeEx((void*)relativePath.string().c_str(), flags, relativePath.string().c_str());

		if (ImGui::IsItemClicked())
		{
			m_CurrentDirectory = path;
		}

		if (node_open)
		{
			if (!is_empty(dir_entry))
			{
				//Recurse?
                for (auto const& rec_dir_entry : std::filesystem::directory_iterator(path))
                {
                    if (rec_dir_entry.is_directory())
                    {
						DrawFileNode(rec_dir_entry,path);
                    }
                }
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}

void AssetBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Asset Browser");

    // Left
    {
        float h = ImGui::GetWindowHeight() - 40;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        DrawSplitter(0, 8.0f, &w, &h, 8, 8);
        ImGui::BeginChild("File Hierarchy Tree", ImVec2(w, h), true);
        {
            for (auto const& dir_entry : std::filesystem::directory_iterator(s_AssetsDirectory))
            {
                DrawFileNode(dir_entry, s_AssetsDirectory);
            }
            ImGui::EndChild();
            ImGui::SameLine();
        }

        ImGui::SameLine();
        ImGui::PopStyleVar(1);
    }

    // Right
    {
        ImGui::BeginGroup();
        ImGui::BeginChild("right pane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        {
	        if (exists(m_CurrentDirectory))
	        {
                for (auto const& dir_entry : std::filesystem::directory_iterator(m_CurrentDirectory))
                {
                    auto path = dir_entry.path();
                    auto relativePath = std::filesystem::relative(path, m_CurrentDirectory);

                    if (dir_entry.is_directory())
                    {
                        ImGui::BeginGroup();
                        ImGui::ImageButton((ImTextureID)m_FolderOpenIcon->ID(), { 50,50 }, { 0,1 }, { 1,0 });
                        ImGui::NewLine();
                        ImGui::Text(dir_entry.path().filename().string().c_str());
                        ImGui::EndGroup();
                        ImGui::SameLine();
                    }
                }
	        }

            ImGui::EndChild();
            ImGui::EndGroup();
        }
       
    }





	ImGui::End();
}
