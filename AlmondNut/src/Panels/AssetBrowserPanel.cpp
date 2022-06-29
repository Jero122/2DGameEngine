#include "AssetBrowserPanel.h"

#include <IconFontCppHeaders/IconsFontAwesome5.h>

#include "GUI/ImGuiCustom.h"
const std::filesystem::path s_AssetsDirectory = "assets";

AssetBrowserPanel::AssetBrowserPanel()
	:m_CurrentDirectory(s_AssetsDirectory), m_SelectedDirectory(s_AssetsDirectory)
{
}

void AssetBrowserPanel::OnStart()
{

}


void AssetBrowserPanel::DrawFileNode(std::filesystem::directory_entry const& dir_entry, std::filesystem::path relativeDirectory)
{
	const auto path = dir_entry.path();
	auto relativePath = std::filesystem::relative(path, relativeDirectory);

	if (dir_entry.is_directory())
	{
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags flags = (m_CurrentDirectory == path) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGui::PushID(relativePath.string().c_str());

        auto nodeName = ICON_FA_FOLDER + std::string(" ") + relativePath.string();
		bool node_open = ImGui::TreeNodeEx((void*)relativePath.string().c_str(), flags, nodeName.c_str());


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

const char* AssetBrowserPanel::GetIcon(const std::string& string)
{
	if (string == ".png" || string == ".jpg" || string == ".jpeg")
	{
		return ICON_FA_FILE_IMAGE;
	}
	else if (string == ".obj" || string == "" || string == ".fbx")
	{
		return ICON_FA_CUBE;
	}
	else if (string == ".txt")
	{
		return ICON_FA_FILE_ALT;
	}
	else if (string == ".ttf")
	{
		return ICON_FA_FONT;
	}
	return ICON_FA_WRENCH;
}

void AssetBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Asset Browser");
    // Left
    {
        float h = ImGui::GetWindowHeight() - 40;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        //DrawSplitter(0, 8.0f, &w, &h, 8, 8);
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
		{
			float h = ImGui::GetWindowHeight() - 40;
        	ImGui::BeginChild("right pane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true); // Leave room for 1 line below us
	        {
				auto font = ImGui::GetIO().Fonts;

		        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.5,0.75f});
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
				ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0,0 });

				auto icon = ICON_FA_FOLDER;

				float ListViewThreshold = 35.0f;
				static float padding = 16.0f;
				static float thumbnailSize = 65.0f;
				float cellSize = thumbnailSize + padding;

				//Thumbnail Mode
				if (thumbnailSize > ListViewThreshold)
				{
					float panelWidth = ImGui::GetContentRegionAvail().x;
					int columnCount = (int)(panelWidth / cellSize);
					if (columnCount < 1)
						columnCount = 1;

					ImGui::Columns(columnCount, 0, false);

					for (auto& dir_entry : std::filesystem::directory_iterator(m_CurrentDirectory))
					{
						icon = ICON_FA_FOLDER;
						auto path = dir_entry.path();
						auto relativePath = std::filesystem::relative(path, m_CurrentDirectory);
						std::string filenameString = relativePath.filename().string();

						if (!dir_entry.is_directory())
						{
							icon = GetIcon(dir_entry.path().extension().string());
						}
						

						ImGui::PushID(filenameString.c_str());
						//Button
						ImGui::PushFont(font->Fonts[1]);
						ImGui::Button(icon, { thumbnailSize,thumbnailSize });
						/*ImGui::ImageButton((ImTextureID)m_FolderIcon->ID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });*/
						ImGui::PopFont();
						//Text
						ImGui::TextWrapped(filenameString.c_str());
						ImGui::NextColumn();

						ImGui::PopID();
					}

					ImGui::Columns(1);
				}
				//Tree List Mode
				else
				{
					for (auto& dir_entry : std::filesystem::directory_iterator(m_CurrentDirectory))
					{
						auto path = dir_entry.path();
						auto relativePath = std::filesystem::relative(path, m_CurrentDirectory);
						std::string filenameString = relativePath.filename().string();

						auto nodeName = icon + std::string(" ") + relativePath.string();
						ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
						bool node_open = ImGui::TreeNodeEx((void*)relativePath.string().c_str(), flags, nodeName.c_str());
					}

					ImGui::Columns(1);
				}
				

				ImGui::PopStyleVar(3);
				ImGui::EndChild();

				// Thumnail Size Slider
				ImGuiStyle& style = ImGui::GetStyle();
				float size = 100 + style.FramePadding.x * 2.0f;
				float avail = ImGui::GetContentRegionAvail().x;

				float off = (avail - size) * 1.0f;
				if (off > 0.0f)
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
				ImGui::PushItemWidth(100);
				ImGui::SliderFloat(" ", &thumbnailSize, 16, 150, "%.0f");
				ImGui::PopItemWidth();
	        }
        	ImGui::EndGroup();
		}
    }
	ImGui::End();
}
