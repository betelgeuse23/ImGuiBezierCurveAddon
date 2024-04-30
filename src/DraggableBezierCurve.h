#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <sstream>

#include "Helpers.h"
#include "ZoneMapper.h"
#include "DraggableDot.h"

/**
* @file DraggableBezierCurve.h
* Файл, содержащий объявления класса товечающего за перетаскиваемые кривые Безье
* @brief Класс перетаскиваемой кривой Безье
* @see DraggableBezierCurve.cpp
* @author Мельникова Анастасия
*/

namespace ImGui {

    /**
     * @brief Классе для создания и взаимодействия с перетаскиваемой кривой Безье в ImGui.
     * Этот класс позволяет пользователям создавать кривые Безье по перетаскиваемым точкм.
     * Кривую можно динамически корректировать, перемещая эти точки.
     * Дополнительный функционал включает проверку на самопересечение и динамическое добавление и удаление точек на кривой.
     */
    class DraggableBezierCurve {
    public:
        /**
         * Конструктор для создания кривой Безье.
         * @param cl Замкнутость кривой.
         * @param tic Толщина кривой.
         * @param col Цвет кривой.
         */
        DraggableBezierCurve(bool cl = false, float tic = 2.0f, ImU32 col = IM_COL32(255, 0, 0, 255));
         /**
          * Оператор присваивания копированием.
          * @param Ообъект DraggableBezierCurve.
          * @return Ссылка на текущий экземпляр DraggableBezierCurve.
          */
        DraggableBezierCurve& operator=(const DraggableBezierCurve& other);
        /**
         * Добавляет к кривой перетаскиваемую точку.
         * Точки добавляются таким образом, чтобы сохранить плавность кривой.
         * @param point Положение точки.
         * @param threshold Пороговое значение расстояния для добавления точки непосредственно на кривую.
         * @param rad Радиус точки.
         * @param col Цвет точки.
         */
        void AddPoint(const ImVec2& point, float threshold = 15.0f, float rad = 5.0f, ImU32 col = IM_COL32(255, 0, 0, 255));
        /**
         * Отображает кривую Безье и её точки.
         * @param editable Доступеность для редактирования, если значение равно true, точки на кривой можно перетаскивать.
         */
        void Draw(bool editable = true);
        /**
         * Проверяет пересекает ли кривая сама себя
         * @return Логическое значение, указывающее, пересекает ли кривая сама себя (true) или нет (false).
         */
        bool IsSelfIntersecting();
        /**
         * Определяет индекс точки, расположенной рядом с указателем мыши.
         * @param threshold Порог приближения для выбора точки.
         * @return Индекс точки, расположенной рядом с указателем мыши.
         */
        size_t dotIndex(float threshold = 5.0f);

        /**
         * Удаляет все точки с кривой.
         */
        void Clear() { points.clear(); }
        /**
         * Удаляет точку с кривой по ее индексу.
         * @param index Индекс удаляемой точки.
         */
        void DeletePoint(size_t index) { if (index < points.size()) points.erase(points.begin() + index); }

        /**
         * Устанавливает признак заскнутости кривой
         * @param cl Логическое значение, указывающее, является ли кривая замкнутой (true) или открытой (false).
         */
        void SetClosed(const bool& cl) { isClosed = cl; }
        /**
         * Задает цвет кривой.
         * @param col Цвет кривой.
         */
        void SetColor(const ImU32& col) { color = col; }
        /**
         * Задает толщину кривой.
         * @param tic Толщина кривой.
         */
        void SetThickness(const float& tic) { thickness = tic; }

        /**
         * Предоставляет указатель на свойство замкнутости кривой, позволяющее напрямую изменять его.
         * @return Указатель на логическое свойство замкнутости кривой.
         */
        bool* GetClosed() { return &isClosed; }
        /**
         * Предоставляет указатель на цвет кривой, позволяющее напрямую изменять его.
         * @return Указатель на цвет кривой.
         */
        ImU32* GetColor() { return &color; }
        /**
         * Предоставляет указатель на толщину кривой, позволяющее напрямую изменять его.
         * @return Указатель на толщину кривой.
         */
        float* GetThickness() { return &thickness; }
        /**
         * Преобразует кривую в строку.
         * @return Строка содержащая сериализованную кривую.
         */
        std::string Serialize() const;
        /**
         * Преобразует строку в кривую.
         * @param data Данные кривой.
         * @return Десериализованная кривая.
         */
        static DraggableBezierCurve Deserialize(const std::string& data);
    private:
        ImU32 color; ///< Цвет кривой.
        float thickness; ///< Толщина кривой.
        bool isClosed; ///< Замкнутость кривой.
        std::vector<DraggableDot> points; ///< Точки, составляющие кривую.
        /**
         * Вычисляет контрольные точки для заданного сегмента кривой.
         * @param i Индекс текущей точки в векторе точек.
         * @param cp1 Ссылка на первую контрольную точку, которая должна быть рассчитана.
         * @param cp2 Ссылка на вторую контрольную точку, которая должна быть рассчитана.
         */
        void CalculateControlPoints(size_t i, ImVec2& cp1, ImVec2& cp2);

        const float increment = 0.05f; ///< Приращение, используемое для выборки кривой Безье.

        /**
         * Вычисляет ближайшую точку на отрезке прямой, заданном символами "a" и "b", к заданной точке "p".
         * @param a Начальная точка отрезка прямой.
         * @param b Конечная точка отрезка прямой.
         * @param p Точка, к которой ищется ближайшая точка на отрезке прямой.
         * @return Координаты точки.
         */
        static inline ImVec2 ClosestPointOnLine(const ImVec2& a, const ImVec2& b, const ImVec2& p);
        /**
         * Вычисляет точку на кубической кривой Безье.
         * @param t Параметр, указывающее точку на кривой, которую необходимо вычислить.
         * @param p0 Первая контрольная точка кубической кривой Безье.
         * @param p1 Вторая контрольная точка кубической кривой Безье.
         * @param p2 Третья контрольная точка кубической кривой Безье.
         * @param p3 Четвертая контрольная точка кубической кривой Безье.
         * @return Координаты точки на кривой Безье.
         */
        static inline ImVec2 CalculateBezierPoint(float t, ImVec2 p0, ImVec2 p1, ImVec2 p2, ImVec2 p3);
        /**
         * Определяет, лежит ли точка 'q' на отрезке прямой 'pr'.
         * @param p Начальная точка отрезка прямой.
         * @param q Проверяемая точка.
         * @param r Конечная точка отрезка прямой.
         * @return Логическое значение, указывающее, лежит ли точка 'q' на отрезке прямой 'pr' (true) или нет (false).
         */
        static inline bool onSegment(ImVec2 p, ImVec2 q, ImVec2 r);
        /**
         * Проверяет, пересекаются ли два отрезка прямой ('p1q1' и 'p2q2').
         * @param p1 Начальная точка первого отрезка прямой.
         * @param q1 Конечная точка первого отрезка прямой.
         * @param p2 Начальная точка второго отрезка прямой.
         * @param q2 Конечная точка второго отрезка прямой.
         * @return Логическое значение, указывающее, пересекаются ли отрезки (true) или нет (false).
         */
        static bool SegmentsIntersect(const ImVec2& p1, const ImVec2& q1, const ImVec2& p2, const ImVec2& q2);
    };

}
