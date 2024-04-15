#pragma once

#include <imgui.h>
#include <cmath>
#include <algorithm>

/**
 * @file Helpers.h
 * Файл, содержащий вспомогательныные функции для вычислений и преобразования типов
 * @brief Вспомогательные функции
 * @author Мельникова Анастасия
 */

#ifdef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS_IMPLEMENTED
IM_MSVC_RUNTIME_CHECKS_OFF

/**
 * Ограничивает вектор ImVec2 'a' между двумя другими векторами ImVec2 'b' и 'c'.
 * @param a Вектор, который нужно ограничить.
 * @param b Минимальный граничный вектор.
 * @param c Максимальный граничный вектор.
 * @return Ограниченный вектор.
 */
static inline ImVec2 clampImVec2(const ImVec2& a, const ImVec2& b, const ImVec2& c) {return ImVec2(std::clamp(a.x, b.x, c.x), std::clamp(a.y, b.y, c.y));}
/**
 * Скалярное сложение значений двух ImVec2.
 * @param Первый вектор.
 * @param Второй вектор.
 * @return Новый вектор являющийся скалярной суммой двух векторов.
 */
static inline ImVec2  operator+(const ImVec2& lhs, const float& rhs)   { return ImVec2(lhs.x + rhs, lhs.y + rhs); }
/**
 * Скалярное вычитание значений двух ImVec2.
 * @param Первый вектор.
 * @param Второй вектор.
 * @return Новый вектор являющийся скалярной разностью двух векторов.
 */
static inline ImVec2  operator-(const ImVec2& lhs, const float& rhs)   { return ImVec2(lhs.x - rhs, lhs.y - rhs); }


/**
 * Вычисляет длину (амплитуду) вектора ImVec2.
 * @param Вектор.
 * @return Длина вектора.
 */
static inline float Len(const ImVec2& v) { return std::sqrt(v.x * v.x + v.y * v.y);}
/**
 * Вычисляет скалярное произведение значений двух ImVec2.
 * @param Первый вектор.
 * @param Второй вектор.
 * @return Скалярное произведение двух векторов.
 */
static inline float DotProduct(const ImVec2& a, const ImVec2& b) { return std::sqrt(a.x * b.x + a.y * b.y); }
/**
 * Вычисляет расстояние между двумя точками ImVec2.
 * @param a Первая точка.
 * @param b Вторая точка.
 * @return Расстояние между точками a и b.
 */
static inline float Dist(const ImVec2& a, const ImVec2& b) { return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)); }
/**
 * Вычисляет квадрат вектора ImVec2.
 * @param Вектор.
 * @return Квадрат вектора.
 */
static inline float Quad(const ImVec2& v) { return v.x * v.x + v.y * v.y; }
/**
 * Нормализует вектор ImVec2.
 * @param Вектор.
 * @return Нормализованный вектор.
 */
static inline ImVec2 Normalize(const ImVec2& v) { float len = Len(v); return (len > 0.0f) ? ImVec2(v.x / len, v.y / len) : ImVec2(0.0f, 0.0f); }

IM_MSVC_RUNTIME_CHECKS_RESTORE
#endif


/**
 * Преобразует цвет ImVec4 в цвет ImU32.
 * @param Цвет в формате ImVec4.
 * @return Цвет в формате ImU32.
 */
static inline ImU32 ImVec4toImU32(ImVec4 col) {
	return IM_COL32((int)(col.x * 255), (int)(col.y * 255), (int)(col.z * 255), (int)(col.w * 255));
}

/**
 * Преобразует цвет ImU32 в цвет ImVec4.
 * @param Цвет в формате ImU32.
 * @return Цвет в формате ImVec4.
 */
static inline ImVec4 ImU32toImVec4(ImU32 col) {
	return ImVec4((float)(col & 0xFF) / 255.0f, (float)((col >> 8) & 0xFF) / 255.0f, (float)((col >> 16) & 0xFF) / 255.0f, (float)((col >> 24) & 0xFF) / 255.0f);
}