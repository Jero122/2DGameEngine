#include "AssetBrowserPanel.h"

#include <IconFontCppHeaders/IconsFontAwesome5.h>

#include "GUI/ImGuiCustom.h"
#include "GUI/imgui_stdlib.h"


AssetBrowserPanel::AssetBrowserPanel()
	:m_CurrentDirectory(s_AssetsDirectory), m_SelectedDirectory(s_AssetsDirectory)
{
}


void AssetBrowserPanel::OnStart()
{
	//TODO reconstruct tree every X seconds
	CurrentNode = RootNode;
}


void AssetBrowserPanel::DrawFileNode(std::shared_ptr<FileSystem::FileNode> node, std::filesystem::path relativeDirectory)
{
	auto dir_entry = node->dir_entry;
	auto fileName = node->fileName;
	const auto path = dir_entry.path();

	
	if (dir_entry.is_directory())
	{
		ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags node_flags = base_flags;
		const bool is_selected = (m_CurrentDirectory == s_AssetsDirectory);
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::PushID(fileName.c_str());

        auto nodeName = node->icon + std::string(" ") + fileName;
		bool node_open = ImGui::TreeNodeEx((void*)fileName.c_str(), node_flags, nodeName.c_str());


		if (ImGui::IsItemClicked())
		{
			m_CurrentDirectory = path;
			CurrentNode = node;
		}

		if (node_open)
		{
			node->icon = std::string(ICON_FA_FOLDER_OPEN);
			//Recurse?
			for (auto& rec_node : node->childNodes)
			{
				DrawFileNode(rec_node, path);
			}
			ImGui::TreePop();
		}
		else
		{
			node->icon = std::string(ICON_FA_FOLDER);
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
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, -0.1f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));


	ImGui::Begin("Asset Browser");
    {
        float h = ImGui::GetContentRegionAvail().y;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));
		// Left
        DrawSplitter(0, 8.0f, &w, &h, 8, 8);
        ImGui::BeginChild("File Hierarchy Tree", ImVec2(w, h), true);
        {
			//Assets parent node
			ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
			auto nodeName = RootNode->icon + std::string(" ") + s_AssetsDirectory.string();
			ImGuiTreeNodeFlags node_flags = base_flags;
			const bool is_selected = (m_CurrentDirectory == s_AssetsDirectory);
			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;

			bool node_open = ImGui::TreeNodeEx((void*)s_AssetsDirectory.string().c_str(), node_flags, nodeName.c_str());

			if (ImGui::IsItemClicked())
			{
				m_CurrentDirectory = s_AssetsDirectory;
				CurrentNode = RootNode;
			}
        	// If Parent open, expand Child nodes
			if (node_open)
			{
				RootNode->icon = ICON_FA_FOLDER_OPEN;
				for (auto& node : RootNode->childNodes)
				{
					DrawFileNode(node, s_AssetsDirectory);
				}
				ImGui::TreePop();
			}
			else
			{
				RootNode->icon = ICON_FA_FOLDER;
			}
            ImGui::EndChild();
            ImGui::SameLine();
        }
        ImGui::SameLine();
        ImGui::PopStyleVar(1);
    }

	static float padding = 16.0f;
	static float thumbnailSize = 65.0f;

	//Right
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::BeginGroup();
	{
		float h = ImGui::GetWindowHeight() - 40;

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{ 64.0f / 255.0f, 64.0f / 255.0f, 64.0f / 255.0f, 1.0f });
		ImGui::BeginChild("Top Bar", { 0, ImGui::GetFrameHeightWithSpacing() }, true);
		{
			
			//Current Directory
			ImGui::Text(m_CurrentDirectory.string().c_str());
			ImGui::SameLine();

			//Search Bar
			float width = 300;
			ImGuiStyle& style = ImGui::GetStyle();
			float size = width + style.FramePadding.x * 2.0f;
			float avail = ImGui::GetContentRegionAvail().x - 100.0f;
			float off = (avail - size) * 1.0f;

			

			if (off > 0.0f)
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
			
			ImGui::Text(ICON_FA_SEARCH);
			ImGui::SameLine();

			ImGui::PushItemWidth(width);
            ImGui::InputText(" ", &searchString);
			ImGui::PopItemWidth();

			ImGui::EndChild();
			ImGui::PopStyleColor(1);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.5,0.75f });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0,0 });
        ImGui::BeginChild("File Explorer", ImVec2(0,-ImGui::GetFrameHeightWithSpacing()), true); // Leave room for 1 line below us
        {
			auto font = ImGui::GetIO().Fonts;
			auto icon = ICON_FA_FOLDER;

			float ListViewThreshold = 35.0f;
			float cellSize = thumbnailSize + padding;

			//SEARCH LOGIC
			std::vector<std::shared_ptr<FileSystem::FileNode>> nodeList;
			nodeList = CurrentNode->childNodes;
			/*if (searchString.empty())
			{
				nodeList = CurrentNode->childNodes;
			}
			else
			{
				nodeList = Search(searchString, RootNode);
			}*/

			//Thumbnail Mode
			if (thumbnailSize > ListViewThreshold)
			{
				float panelWidth = ImGui::GetContentRegionAvail().x;
				int columnCount = (int)(panelWidth / cellSize);
				if (columnCount < 1)
					columnCount = 1;
				ImGui::Columns(columnCount, 0, false);

				//DISPLAY FILE NODES
				for (auto node : nodeList)
				{
					icon = ICON_FA_FOLDER;
					auto dir_entry = node->dir_entry;
					auto filename = node->fileName;

					if (!dir_entry.is_directory())
					{
						icon = GetIcon(dir_entry.path().extension().string());
					}

					ImGui::PushID(filename.c_str());
					//Button
					ImGui::PushFont(font->Fonts[1]);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					if(ImGui::Button(icon, { thumbnailSize,thumbnailSize }))
					{
						if (dir_entry.is_directory())
						{
							CurrentNode = node;
							searchString = "";
						}
						
					}
					ImGui::PopStyleColor(1);
					ImGui::PopFont();

					//Text
					ImGui::TextWrapped(filename.c_str());
					ImGui::NextColumn();

					ImGui::PopID();
				}

				ImGui::Columns(1);
			}

			//Tree List Mode
			else
			{
				for (auto node : nodeList)
				{
					auto dir_entry = node->dir_entry;
					auto filename = node->fileName;

					if (!dir_entry.is_directory())
					{
						icon = GetIcon(dir_entry.path().extension().string());
					}
					auto nodeName = icon + std::string(" ") + filename;
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
					bool node_open = ImGui::TreeNodeEx((void*)filename.c_str(), flags, nodeName.c_str());

					if (ImGui::IsItemClicked())
					{
						if (dir_entry.is_directory())
						{
							CurrentNode = node;
							searchString = "";
						}
					}

					if (node_open)
					{
						ImGui::TreePop();
					}

				}

				ImGui::Columns(1);
			}

			ImGui::PopStyleVar(2);
			ImGui::EndChild();
        }

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{ 64.0f / 255.0f, 64.0f / 255.0f, 64.0f / 255.0f, 1.0f });
		ImGui::BeginChild("Bottom Bar",ImVec2(0,0), true);
        {
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

			ImGui::EndChild();
			ImGui::PopStyleVar(1);
        }
        ImGui::EndGroup();
		ImGui::PopStyleColor(1);
	}

	ImGui::PopStyleVar(2);
	ImGui::End();
}
