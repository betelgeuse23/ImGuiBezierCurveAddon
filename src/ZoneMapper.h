#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <vector>

/**
* @file ZoneMapper.h
* ����, ���������� ���������� ������ ����������� �� ����������� ����������
* @brief ����� ����������� ����������
* @see ZoneMapper.cpp
* @author ���������� ���������
*/

namespace ImGui {

    /**
     * @brief ����� ��� ���������� ������ ����������������� ���������� ImGui.
     * ZoneMapper ��������� ��������� ���� ����������������� ���������� � ImGui � ��������� ���,
     * �������� ����������� � ���������� ��������� ����������������� ���������� ������������ ���� ���.
     */
    class ZoneMapper {
    public:
        /**
         * ����������� �� ���������.
         */
        ZoneMapper();

        /**
         * �������� �������� ����� ���� ����������������� ����������. 
         * ������ ����� ����� ���� ������ ����� ������� EndZone().
         */
        static void BeginZone();
        /**
        * ����������� �������� ����� ���� ����������������� ����������. 
        * ������ ������������ ������� ������� BeginZone().
        */
        static void EndZone();
        /**
         * ������������� ��������� ������� ����.
         * @return ���������� ������� ������ ���� ������� ���� � ���� ImVec2.
         */
        static ImVec2 GetZonePos();
        /**
         * ������������� ������ ������� ����.
         * @return ������ ������� ���� � ���� ImVec2.
         */
        static ImVec2 GetZoneSize();

    private:
        struct ZoneData {
            ImVec2 position; ///< ��������� ����.
            ImVec2 size; ///< ������ ����.
        };

        static std::vector<ZoneData> zoneStack; ///< ���� ���, ��� ��������� �����������.
    };

}