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
* ����, ���������� ���������� ������ ����������� �� ��������������� ������ �����
* @brief ����� ��������������� ������ �����
* @see DraggableBezierCurve.cpp
* @author ���������� ���������
*/

namespace ImGui {

    /**
     * @brief ������ ��� �������� � �������������� � ��������������� ������ ����� � ImGui.
     * ���� ����� ��������� ������������� ��������� ������ ����� �� ��������������� �����.
     * ������ ����� ����������� ��������������, ��������� ��� �����.
     * �������������� ���������� �������� �������� �� ��������������� � ������������ ���������� � �������� ����� �� ������.
     */
    class DraggableBezierCurve {
    public:
        /**
         * ����������� ��� �������� ������ �����.
         * @param cl ����������� ������.
         * @param tic ������� ������.
         * @param col ���� ������.
         */
        DraggableBezierCurve(bool cl = false, float tic = 2.0f, ImU32 col = IM_COL32(255, 0, 0, 255));
         /**
          * �������� ������������ ������������.
          * @param ������� DraggableBezierCurve.
          * @return ������ �� ������� ��������� DraggableBezierCurve.
          */
        DraggableBezierCurve& operator=(const DraggableBezierCurve& other);
        /**
         * ��������� � ������ ��������������� �����.
         * ����� ����������� ����� �������, ����� ��������� ��������� ������.
         * @param point ��������� �����.
         * @param threshold ��������� �������� ���������� ��� ���������� ����� ��������������� �� ������.
         * @param rad ������ �����.
         * @param col ���� �����.
         */
        void AddPoint(const ImVec2& point, float threshold = 15.0f, float rad = 5.0f, ImU32 col = IM_COL32(255, 0, 0, 255));
        /**
         * ���������� ������ ����� � � �����.
         * @param editable ������������ ��� ��������������, ���� �������� ����� true, ����� �� ������ ����� �������������.
         */
        void Draw(bool editable = true);
        /**
         * ��������� ���������� �� ������ ���� ����
         * @return ���������� ��������, �����������, ���������� �� ������ ���� ���� (true) ��� ��� (false).
         */
        bool IsSelfIntersecting();
        /**
         * ���������� ������ �����, ������������� ����� � ���������� ����.
         * @param threshold ����� ����������� ��� ������ �����.
         * @return ������ �����, ������������� ����� � ���������� ����.
         */
        size_t dotIndex(float threshold = 5.0f);

        /**
         * ������� ��� ����� � ������.
         */
        void Clear() { points.clear(); }
        /**
         * ������� ����� � ������ �� �� �������.
         * @param index ������ ��������� �����.
         */
        void DeletePoint(size_t index) { if (index < points.size()) points.erase(points.begin() + index); }

        /**
         * ������������� ������� ����������� ������
         * @param cl ���������� ��������, �����������, �������� �� ������ ��������� (true) ��� �������� (false).
         */
        void SetClosed(const bool& cl) { isClosed = cl; }
        /**
         * ������ ���� ������.
         * @param col ���� ������.
         */
        void SetColor(const ImU32& col) { color = col; }
        /**
         * ������ ������� ������.
         * @param tic ������� ������.
         */
        void SetThickness(const float& tic) { thickness = tic; }

        /**
         * ������������� ��������� �� �������� ����������� ������, ����������� �������� �������� ���.
         * @return ��������� �� ���������� �������� ����������� ������.
         */
        bool* GetClosed() { return &isClosed; }
        /**
         * ������������� ��������� �� ���� ������, ����������� �������� �������� ���.
         * @return ��������� �� ���� ������.
         */
        ImU32* GetColor() { return &color; }
        /**
         * ������������� ��������� �� ������� ������, ����������� �������� �������� ���.
         * @return ��������� �� ������� ������.
         */
        float* GetThickness() { return &thickness; }
        /**
         * ����������� ������ � ������.
         * @return ������ ���������� ��������������� ������.
         */
        std::string Serialize() const;
        /**
         * ����������� ������ � ������.
         * @param data ������ ������.
         * @return ����������������� ������.
         */
        static DraggableBezierCurve Deserialize(const std::string& data);
    private:
        ImU32 color; ///< ���� ������.
        float thickness; ///< ������� ������.
        bool isClosed; ///< ����������� ������.
        std::vector<DraggableDot> points; ///< �����, ������������ ������.
        /**
         * ��������� ����������� ����� ��� ��������� �������� ������.
         * @param i ������ ������� ����� � ������� �����.
         * @param cp1 ������ �� ������ ����������� �����, ������� ������ ���� ����������.
         * @param cp2 ������ �� ������ ����������� �����, ������� ������ ���� ����������.
         */
        void DraggableBezierCurve::CalculateControlPoints(size_t i, ImVec2& cp1, ImVec2& cp2);

        const float increment = 0.05f; ///< ����������, ������������ ��� ������� ������ �����.

        /**
         * ��������� ��������� ����� �� ������� ������, �������� ��������� "a" � "b", � �������� ����� "p".
         * @param a ��������� ����� ������� ������.
         * @param b �������� ����� ������� ������.
         * @param p �����, � ������� ������ ��������� ����� �� ������� ������.
         * @return ���������� �����.
         */
        static inline ImVec2 ClosestPointOnLine(const ImVec2& a, const ImVec2& b, const ImVec2& p);
        /**
         * ��������� ����� �� ���������� ������ �����.
         * @param t ��������, ����������� ����� �� ������, ������� ���������� ���������.
         * @param p0 ������ ����������� ����� ���������� ������ �����.
         * @param p1 ������ ����������� ����� ���������� ������ �����.
         * @param p2 ������ ����������� ����� ���������� ������ �����.
         * @param p3 ��������� ����������� ����� ���������� ������ �����.
         * @return ���������� ����� �� ������ �����.
         */
        static inline ImVec2 CalculateBezierPoint(float t, ImVec2 p0, ImVec2 p1, ImVec2 p2, ImVec2 p3);
        /**
         * ����������, ����� �� ����� 'q' �� ������� ������ 'pr'.
         * @param p ��������� ����� ������� ������.
         * @param q ����������� �����.
         * @param r �������� ����� ������� ������.
         * @return ���������� ��������, �����������, ����� �� ����� 'q' �� ������� ������ 'pr' (true) ��� ��� (false).
         */
        static inline bool onSegment(ImVec2 p, ImVec2 q, ImVec2 r);
        /**
         * ���������, ������������ �� ��� ������� ������ ('p1q1' � 'p2q2').
         * @param p1 ��������� ����� ������� ������� ������.
         * @param q1 �������� ����� ������� ������� ������.
         * @param p2 ��������� ����� ������� ������� ������.
         * @param q2 �������� ����� ������� ������� ������.
         * @return ���������� ��������, �����������, ������������ �� ������� (true) ��� ��� (false).
         */
        static bool SegmentsIntersect(const ImVec2& p1, const ImVec2& q1, const ImVec2& p2, const ImVec2& q2);
    };

}