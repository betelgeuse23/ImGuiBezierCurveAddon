#include "DraggableBezierCurve.h"

/**
* @mainpage Документация по исходникам проекта ImGuiBezierCurveAddon
* В данной документации описываются классы и функии, используемые для создания аддона с кривыми Безье к Imgui 
*/

/**
* @file DraggableBezierCurve.cpp
* Файл, содержащий определение функций из файла DraggableBezierCurve.h
* @brief Класс перетаскиваемой кривой Безье
* @see DraggableBezierCurve.h
* @author Мельникова Анастасия
*/

namespace ImGui {
    DraggableBezierCurve::DraggableBezierCurve(bool cl, float tic, ImU32 col) : isClosed(cl), thickness(tic), color(col) {}

    DraggableBezierCurve& DraggableBezierCurve::operator=(const DraggableBezierCurve& other) {
        if (this != &other) {
            this->isClosed = other.isClosed;
            this->color = other.color;
            this->thickness = other.thickness;
            this->points = other.points;
        }
        return *this;
    }

    void DraggableBezierCurve::AddPoint(const ImVec2& newPoint, float threshold, float rad, ImU32 col) {
        size_t size = points.size();
        
        if (size < 2) {
            points.emplace_back(newPoint, rad, col);
            return;
        }

        size_t insertIndex = size;
        float minDistance = FLT_MAX;

        for (size_t i = 0; i < size - !isClosed; i++) {
            ImVec2 p1 = points[i].GetSimplePosition();
            ImVec2 p2 = points[(i + 1) % points.size()].GetSimplePosition();

            ImVec2 cp1, cp2;
            CalculateControlPoints(i, cp1, cp2);

            for (float t = 0; t <= 1; t += increment) {
                ImVec2 pointOnCurve = CalculateBezierPoint(t, p1, cp1, cp2, p2);
                float distance = Dist(newPoint, pointOnCurve);

                if (distance < minDistance) {
                    minDistance = distance;
                    insertIndex = (minDistance <= threshold) ? i + 1 : insertIndex;
                }
            }
        }

        if (minDistance > threshold) {
            if (Dist(newPoint, points.front().GetSimplePosition()) < Dist(newPoint, points.back().GetSimplePosition())) insertIndex = 0;
            else insertIndex = points.size();
        }

        points.insert(points.begin() + insertIndex, DraggableDot(newPoint, rad, col));
    }

    void DraggableBezierCurve::Draw(bool editable) {
        if (editable) for (auto& dot : points) {
            dot.SetColor(color);
            dot.Draw();
        }

        int size = points.size();
        if (size < 2) return;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 zonePos = ZoneMapper::GetZonePos();

        for (size_t i = 0; i < size - !isClosed; i++) {
            ImVec2 p1 = points[i].GetSimplePosition();
            ImVec2 p2 = points[(i + 1) % size].GetSimplePosition();

            ImVec2 cp1, cp2;
            CalculateControlPoints(i % size, cp1, cp2);

            draw_list->AddBezierCubic(p1 + zonePos, cp1 + zonePos, cp2 + zonePos, p2 + zonePos, color, thickness);
        }
    }

    void DraggableBezierCurve::CalculateControlPoints(size_t i, ImVec2& cp1, ImVec2& cp2) {
        int size = points.size();

        ImVec2 p0 = points[(size + i - 1) % size].GetSimplePosition();
        ImVec2 p1 = points[i].GetSimplePosition();
        ImVec2 p2 = points[(i + 1) % size].GetSimplePosition();
        ImVec2 p3 = points[(i + 2) % size].GetSimplePosition();

        if (!isClosed) {
            if (i == 0) p0 = p1;
            if (i == size - 2) p3 = p2;
        }

        ImVec2 tangentAtP1 = Normalize((p2 - p0) * 0.5f);
        ImVec2 tangentAtP2 = Normalize((p3 - p1) * 0.5f);

        cp1 = p1 + tangentAtP1 * (Len(p1 - p0) / 4.0f);
        cp2 = p2 - tangentAtP2 * (Len(p2 - p1) / 4.0f);
    }

    bool DraggableBezierCurve::IsSelfIntersecting() {
        int size = points.size();

        if (points.size() < 3) return false;

        std::vector<ImVec2> lineSegments;

        for (size_t i = 0; i < size - !isClosed; i++) {
            ImVec2 p1 = points[i % size].GetSimplePosition();
            ImVec2 p2 = points[(i + 1) % size].GetSimplePosition();

            ImVec2 cp1, cp2;
            CalculateControlPoints(i % size, cp1, cp2);

            for (float t = 0; t <= 1; t += increment) {
                ImVec2 pointOnCurve = CalculateBezierPoint(t, p1, cp1, cp2, p2);
                lineSegments.push_back(pointOnCurve);
            }
        }

        for (size_t i = 0; i < lineSegments.size() - 1; i++)
            for (size_t j = i + 2; j < lineSegments.size() - 1; j++)
                if (SegmentsIntersect(lineSegments[i], lineSegments[i + 1], lineSegments[j], lineSegments[j + 1])) return true;
        return false;
    }

    size_t DraggableBezierCurve::dotIndex(float threshold) {
        for (int i = 0; i < points.size(); i++) {
            ImVec2 mPos = ImGui::GetMousePos();
            ImVec2 pPos = points[i].GetSimplePosition();
            ImVec2 zonePos = ImGui::ZoneMapper::GetZonePos();
            
            if(Dist(mPos - zonePos, pPos) < threshold) return i;
        }
        return -1;
    }

    bool DraggableBezierCurve::onSegment(ImVec2 p, ImVec2 q, ImVec2 r) {
        return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
    }

    ImVec2 DraggableBezierCurve::ClosestPointOnLine(const ImVec2& a, const ImVec2& b, const ImVec2& p) {
        ImVec2 ab = b - a;
        return a + ab * std::clamp(DotProduct(p - a, ab) / Quad(ab), 0.0f, 1.0f);
    }

    ImVec2 DraggableBezierCurve::CalculateBezierPoint(float t, ImVec2 p0, ImVec2 p1, ImVec2 p2, ImVec2 p3) {
        float u = 1 - t;
        return p0 * (u * u * u) + p1 * (3 * u * u * t) + p2 * (3 * u * t * t) + p3 * (t * t * t);
    }

    bool DraggableBezierCurve::SegmentsIntersect(const ImVec2& p1, const ImVec2& q1, const ImVec2& p2, const ImVec2& q2) {
        auto orientation = [](const ImVec2& p, const ImVec2& q, const ImVec2& r) {
            float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
            if (val == 0.0f) return 0;
            return (val > 0.0f) ? 1 : 2;
        };

        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        if (o1 != o2 && o3 != o4) return true;

        if (o1 == 0 && onSegment(p1, p2, q1)) return true;
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;

        return false;
    }

    std::string DraggableBezierCurve::Serialize() const {
        std::ostringstream stream;
        stream << isClosed << "," << thickness << "," << color;
        for (DraggableDot dot : points) {
            stream << "," << dot.GetSimplePosition().x << "," << dot.GetSimplePosition().y
                << "," << *dot.GetRadius() << "," << *dot.GetColor();
        }
        return stream.str();
    }

    DraggableBezierCurve DraggableBezierCurve::Deserialize(const std::string& data) {
        std::istringstream stream(data);
        std::string segment;
        std::vector<std::string> seglist;

        while (std::getline(stream, segment, ',')) {
            seglist.push_back(segment);
        }

        DraggableBezierCurve curve(
            std::stoi(seglist[0]),
            std::stof(seglist[1]),
            static_cast<ImU32>(std::stoul(seglist[2])));

        for (size_t i = 3; i < seglist.size(); i += 4) {
            ImVec2 pos(std::stoi(seglist[i]), std::stoi(seglist[i + 1]));
            float rad = std::stof(seglist[i + 2]);
            ImU32 col = static_cast<ImU32>(std::stoul(seglist[i + 3]));
            curve.points.emplace_back(pos, rad, col);
        }

        return curve;
    }
}