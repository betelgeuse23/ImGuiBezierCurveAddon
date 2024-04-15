#include "ZoneMapper.h"

/**
* @file ZoneMapper.cpp
* ����, ���������� ����������� ������� �� ����� ZoneMapper.h
* @brief ����� ����������� ����������
* @see ZoneMapper.h
* @author ���������� ���������
*/

namespace ImGui {

    std::vector<ZoneMapper::ZoneData> ZoneMapper::zoneStack;

    ZoneMapper::ZoneMapper() {}

    void ZoneMapper::BeginZone() {
        ZoneData zone;
        zone.position = ImGui::GetCursorScreenPos();
        zone.size = ImGui::GetContentRegionAvail();
        zoneStack.push_back(zone);
    }

    void ZoneMapper::EndZone() {
        zoneStack.pop_back();
    }

    ImVec2 ZoneMapper::GetZonePos() {
        if (!zoneStack.empty()) return zoneStack.back().position;
        else return ImVec2(0, 0);
    }

    ImVec2 ZoneMapper::GetZoneSize() {
        if (!zoneStack.empty()) return zoneStack.back().size;
        else return ImVec2(0, 0);
    }

}
