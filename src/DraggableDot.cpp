#include "DraggableDot.h"

/**
* @file DraggableDot.cpp
* Файл, содержащий определение функций из файла DraggableDot.h
* @brief Класс перетаскиваемой точки
* @see DraggableDot.h
* @author Мельникова Анастасия
*/

namespace ImGui {

    DraggableDot::DraggableDot(const ImVec2& pos, float rad, ImU32 col)
        : position(pos), radius(rad), color(col) {}

    void DraggableDot::Draw() {
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec2 zonePos = ZoneMapper::GetZonePos();
        ImVec2 zoneSize = ZoneMapper::GetZoneSize();
        ImVec2 dotPos = zonePos + position;

        ImGui::SetCursorScreenPos(dotPos - radius);
        ImGui::PushID(this);
        ImGui::InvisibleButton("##dot", ImVec2(2 * radius, 2 * radius));
        ImGui::PopID();

        draw_list->AddCircleFilled(dotPos, radius, color);

        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
            position = ImVec2(std::clamp(position.x + mouseDelta.x, 0.0f, zoneSize.x), std::clamp(position.y + mouseDelta.y, 0.0f, zoneSize.y));
        }
    }
}
