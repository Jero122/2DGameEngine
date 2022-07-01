#include "AssetBrowserPanel.h"

#include <IconFontCppHeaders/IconsFontAwesome5.h>

#include "GUI/ImGuiCustom.h"


AssetBrowserPanel::AssetBrowserPanel()
	:m_CurrentDirectory(s_AssetsDirectory), m_SelectedDirectory(s_AssetsDirectory)
{
}

void AssetBrowserPanel::AddNode(FileNode& parentNode, std::filesystem::directory_entry const& dir_entry)
{
	FileNode node = FileNode(dir_entry);
	
	for (auto const& rec_dir_entry : std::filesystem::directory_iterator(dir_entry.path()))
	{
		if (rec_dir_entry.is_directory())
		{
			AddNode(node, rec_dir_entry);
		}
		else
		{
			node.childNodes.push_back(FileNode(rec_dir_entry));
		}
	}
	
	parentNode.childNodes.push_back(node);
}

void AssetBrowserPanel::OnStart()
{
	//TODO reconstruct tree every X seconds
	//Rootnode is always assets;
	for (auto& dir_entry : std::filesystem::directory_iterator(s_AssetsDirectory))
	{
		AddNode(RootNode, dir_entry);
	}

	CurrentNode = RootNode;
}


void AssetBrowserPanel::DrawFileNode(FileNode const& node, std::filesystem::path relativeDirectory)
{
	auto dir_entry = node.dir_entry;
	auto fileName = node.fileName;
	const auto path = dir_entry.path();

	
	if (dir_entry.is_directory())
	{
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags flags = (m_CurrentDirectory == path) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGui::PushID(fileName.c_str());

        auto nodeName = ICON_FA_FOLDER + std::string(" ") + fileName;
		bool node_open = ImGui::TreeNodeEx((void*)fileName.c_str(), flags, nodeName.c_str());


		if (ImGui::IsItemClicked())
		{
			m_CurrentDirectory = path;
			CurrentNode = node;
		}

		if (node_open)
		{
			//Recurse?
			for (auto const& rec_node : node.childNodes)
			{
				DrawFileNode(rec_node, path);
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
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, -0.1f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	// Left
	ImGui::Begin("Asset Browser");
    {
        float h = ImGui::GetContentRegionAvail().y;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));

        DrawSplitter(0, 8.0f, &w, &h, 8, 8);
        ImGui::BeginChild("File Hierarchy Tree", ImVec2(w, h), true);
        {
			//Assets parent node
			auto nodeName = ICON_FA_FOLDER + std::string(" ") + s_AssetsDirectory.string();
			ImGuiTreeNodeFlags flags = (m_CurrentDirectory == s_AssetsDirectory) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool node_open = ImGui::TreeNodeEx((void*)s_AssetsDirectory.string().c_str(), flags, nodeName.c_str());

			if (ImGui::IsItemClicked())
			{
				m_CurrentDirectory = s_AssetsDirectory;
				CurrentNode = RootNode;
			}
        	// If Parent open, expand Child nodes
			if (node_open)
			{
				for (auto const& node : RootNode.childNodes)
				{
					DrawFileNode(node, s_AssetsDirectory);
				}
				ImGui::TreePop();
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
		ImGui::BeginChild("Path", { 0, ImGui::GetFrameHeightWithSpacing() }, true);
		{
			ImGui::Text(m_CurrentDirectory.string().c_str());
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

			//Thumbnail Mode
			if (thumbnailSize > ListViewThreshold)
			{
				float panelWidth = ImGui::GetContentRegionAvail().x;
				int columnCount = (int)(panelWidth / cellSize);
				if (columnCount < 1)
					columnCount = 1;

				ImGui::Columns(columnCount, 0, false);
				//TODO handle clicking on directory nodes
				for (auto const& node : CurrentNode.childNodes)
				{
					icon = ICON_FA_FOLDER;
					auto dir_entry = node.dir_entry;
					auto filename = node.fileName;

					if (!dir_entry.is_directory())
					{
						icon = GetIcon(dir_entry.path().extension().string());
					}

					ImGui::PushID(filename.c_str());
					//Button
					ImGui::PushFont(font->Fonts[1]);
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::Button(icon, { thumbnailSize,thumbnailSize });
					ImGui::PopStyleColor(1);
					/*ImGui::ImageButton((ImTextureID)m_FolderIcon->ID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });*/
					ImGui::PopFont();
					//Text
					ImGui::TextWrapped(filename.c_str());
					ImGui::NextColumn();

					ImGui::PopID();
				}

				ImGui::Columns(1);
			}

			//TODO fix list mode, handle icons for leaf nodes
			//TODO leaf nodes should not be able to be clicked
			//Tree List Mode
			else
			{
				//TODO handle clicking on directory nodes
				for (auto const& node : CurrentNode.childNodes)
				{
					auto dir_entry = node.dir_entry;
					auto filename = node.fileName;

					auto nodeName = icon + std::string(" ") + filename;
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
					bool node_open = ImGui::TreeNodeEx((void*)filename.c_str(), flags, nodeName.c_str());
				}

				ImGui::Columns(1);
			}

			ImGui::PopStyleVar(2);
			ImGui::EndChild();
        }

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{ 64.0f / 255.0f, 64.0f / 255.0f, 64.0f / 255.0f, 1.0f });
		ImGui::BeginChild("Bar",ImVec2(0,0), true);
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
