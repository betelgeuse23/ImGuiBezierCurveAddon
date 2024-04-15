#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <vector>

/**
* @file ZoneMapper.h
* Файл, содержащий объявления класса отвечающего за зонирование интерфейса
* @brief Класс зонирования интерфейса
* @see ZoneMapper.cpp
* @author Мельникова Анастасия
*/

namespace ImGui {

    /**
     * @brief Класс для управления зонами пользовательского интерфейса ImGui.
     * ZoneMapper позволяет создавать зоны пользовательского интерфейса в ImGui и управлять ими,
     * упрощает организацию и размещение элементов пользовательского интерфейса относительно этих зон.
     */
    class ZoneMapper {
    public:
        /**
         * Конструктор по умолчанию.
         */
        ZoneMapper();

        /**
         * Начинает разметку новой зоны пользовательского интерфейса. 
         * Должно иметь после себя парный вызов функции EndZone().
         */
        static void BeginZone();
        /**
        * Заканчивает разметку новой зоны пользовательского интерфейса. 
        * Должно предваряться вызовом функции BeginZone().
        */
        static void EndZone();
        /**
         * Предоставляет положение текущей зоны.
         * @return Координаты верхего левого угла текущей зоны в виде ImVec2.
         */
        static ImVec2 GetZonePos();
        /**
         * Предоставляет размер текущей зоны.
         * @return Размер текущей зоны в виде ImVec2.
         */
        static ImVec2 GetZoneSize();

    private:
        struct ZoneData {
            ImVec2 position; ///< Положение зоны.
            ImVec2 size; ///< Размер зоны.
        };

        static std::vector<ZoneData> zoneStack; ///< Стак зон, для поддержки вложенности.
    };

}