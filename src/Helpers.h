#pragma once

#include <imgui.h>
#include <cmath>
#include <algorithm>

/**
 * @file Helpers.h
 * ����, ���������� ����������������� ������� ��� ���������� � �������������� �����
 * @brief ��������������� �������
 * @author ���������� ���������
 */

#ifdef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS_IMPLEMENTED
IM_MSVC_RUNTIME_CHECKS_OFF

/**
 * ������������ ������ ImVec2 'a' ����� ����� ������� ��������� ImVec2 'b' � 'c'.
 * @param a ������, ������� ����� ����������.
 * @param b ����������� ��������� ������.
 * @param c ������������ ��������� ������.
 * @return ������������ ������.
 */
static inline ImVec2 clampImVec2(const ImVec2& a, const ImVec2& b, const ImVec2& c) {return ImVec2(std::clamp(a.x, b.x, c.x), std::clamp(a.y, b.y, c.y));}
/**
 * ��������� �������� �������� ���� ImVec2.
 * @param ������ ������.
 * @param ������ ������.
 * @return ����� ������ ���������� ��������� ������ ���� ��������.
 */
static inline ImVec2  operator+(const ImVec2& lhs, const float& rhs)   { return ImVec2(lhs.x + rhs, lhs.y + rhs); }
/**
 * ��������� ��������� �������� ���� ImVec2.
 * @param ������ ������.
 * @param ������ ������.
 * @return ����� ������ ���������� ��������� ��������� ���� ��������.
 */
static inline ImVec2  operator-(const ImVec2& lhs, const float& rhs)   { return ImVec2(lhs.x - rhs, lhs.y - rhs); }


/**
 * ��������� ����� (���������) ������� ImVec2.
 * @param ������.
 * @return ����� �������.
 */
static inline float Len(const ImVec2& v) { return std::sqrt(v.x * v.x + v.y * v.y);}
/**
 * ��������� ��������� ������������ �������� ���� ImVec2.
 * @param ������ ������.
 * @param ������ ������.
 * @return ��������� ������������ ���� ��������.
 */
static inline float DotProduct(const ImVec2& a, const ImVec2& b) { return std::sqrt(a.x * b.x + a.y * b.y); }
/**
 * ��������� ���������� ����� ����� ������� ImVec2.
 * @param a ������ �����.
 * @param b ������ �����.
 * @return ���������� ����� ������� a � b.
 */
static inline float Dist(const ImVec2& a, const ImVec2& b) { return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)); }
/**
 * ��������� ������� ������� ImVec2.
 * @param ������.
 * @return ������� �������.
 */
static inline float Quad(const ImVec2& v) { return v.x * v.x + v.y * v.y; }
/**
 * ����������� ������ ImVec2.
 * @param ������.
 * @return ��������������� ������.
 */
static inline ImVec2 Normalize(const ImVec2& v) { float len = Len(v); return (len > 0.0f) ? ImVec2(v.x / len, v.y / len) : ImVec2(0.0f, 0.0f); }

IM_MSVC_RUNTIME_CHECKS_RESTORE
#endif


/**
 * ����������� ���� ImVec4 � ���� ImU32.
 * @param ���� � ������� ImVec4.
 * @return ���� � ������� ImU32.
 */
static inline ImU32 ImVec4toImU32(ImVec4 col) {
	return IM_COL32((int)(col.x * 255), (int)(col.y * 255), (int)(col.z * 255), (int)(col.w * 255));
}

/**
 * ����������� ���� ImU32 � ���� ImVec4.
 * @param ���� � ������� ImU32.
 * @return ���� � ������� ImVec4.
 */
static inline ImVec4 ImU32toImVec4(ImU32 col) {
	return ImVec4((float)(col & 0xFF) / 255.0f, (float)((col >> 8) & 0xFF) / 255.0f, (float)((col >> 16) & 0xFF) / 255.0f, (float)((col >> 24) & 0xFF) / 255.0f);
}