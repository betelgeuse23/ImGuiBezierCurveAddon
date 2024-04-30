#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdio.h>
#include <fstream>

#include "DraggableBezierCurve.h"

/**
 * @file main.cpp
 * Файл, содержащий базовое демонтрационное приложение работы с аддоном
 * @brief Место входа в программу
 * @author Мельникова Анастасия
 */

/**
 * @brief Коллбек для ошибок GLFW.
 * Эта функция используется в качестве обратного вызова для ошибок GLFW. 
 * Она выводит код ошибки и описание в стандартный поток ошибок.
 * @param error Код ошибки, сообщаемый GLFW.
 * @param description Строка, описывающая ошибку.
 */
void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
/**
 * @brief Функция предназначенная для загрузки состояния кривых в файл
 * @param filename Имя файла сохранения.
 * @param bezierCurves Массив кривых.
 */
void SaveCurvesToFile(const std::string& filename, std::vector<ImGui::DraggableBezierCurve>& bezierCurves) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& curve : bezierCurves) {
            file << curve.Serialize() << std::endl;
        }
        file.close();
    }
}
/**
 * @brief Функция предназначенная для загрузки состояния кривых из файла
 * @param filename Имя файла сохранения.
 * @param bezierCurves Массив кривых.
 */
void LoadCurvesFromFile(const std::string& filename, std::vector<ImGui::DraggableBezierCurve>& bezierCurves) {
    std::vector<ImGui::DraggableBezierCurve> tempCurves;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                tempCurves.push_back(ImGui::DraggableBezierCurve::Deserialize(line));
            }
        }
        file.close();
    }
    if (!tempCurves.empty()) {
        bezierCurves.swap(tempCurves);
        tempCurves.clear();
    }
}

/**
 * main() - функция, с которой начинается выполнение программы 
 * @brief Точка входа
 * @return Состояние выхода из программы
 */
int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+GLEW example", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    std::vector<ImGui::DraggableBezierCurve> bezierCurves;
    bezierCurves.emplace_back();
    int selectedCurveIndex = 0;
    bool editMode = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save", "CTRL+S")) {
                    SaveCurvesToFile("curves.txt", bezierCurves);
                }
                if (ImGui::MenuItem("Load", "CTRL+L")) {
                    LoadCurvesFromFile("curves.txt", bezierCurves);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S)) && ImGui::GetIO().KeyCtrl) {
            SaveCurvesToFile("curves.txt", bezierCurves);
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L)) && ImGui::GetIO().KeyCtrl) {
            LoadCurvesFromFile("curves.txt", bezierCurves);
        }

        ImGui::Begin("Bezier Curve Editor");

        if (ImGui::Button("Add Curve")) {
            bezierCurves.emplace_back();
            selectedCurveIndex = bezierCurves.size() - 1;
            editMode = true;
        }
        ImGui::SameLine();

        if (ImGui::Button("Remove Curve") && selectedCurveIndex != 0) {
            bezierCurves[selectedCurveIndex].Clear();
            bezierCurves.erase(bezierCurves.begin() + selectedCurveIndex);
            selectedCurveIndex = std::max(selectedCurveIndex - 1, 0);
        }
        ImGui::SameLine();

        if (!bezierCurves.empty()) {
            static char currentCurveName[32] = "";
            if (selectedCurveIndex != 0) snprintf(currentCurveName, sizeof(currentCurveName), "Curve %d", selectedCurveIndex);
            else strcpy(currentCurveName, "Select a Curve");

            if (ImGui::BeginCombo("Curves", currentCurveName)) {
                for (int i = 0; i < bezierCurves.size(); ++i) {
                    if (i != 0) snprintf(currentCurveName, sizeof(currentCurveName), "Curve %d", i);
                    else snprintf(currentCurveName, sizeof(currentCurveName),  "Select a Curve");
                    const bool isSelected = (selectedCurveIndex == i);
                    if (ImGui::Selectable(currentCurveName, isSelected)) selectedCurveIndex = i;
                    if (isSelected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
        
        if (ImGui::RadioButton("Edit Mode", editMode)) editMode = !editMode;
        if (selectedCurveIndex == 0) editMode = false;

        ImVec4 color = ImU32toImVec4(*bezierCurves[selectedCurveIndex].GetColor());
        if (ImGui::ColorEdit4("Color", (float*)&color)) bezierCurves[selectedCurveIndex].SetColor(ImVec4toImU32(color));
        ImGui::SliderFloat("Thickness", bezierCurves[selectedCurveIndex].GetThickness(), 1.0f, 10.0f);
        ImGui::Checkbox("Closed Curve", bezierCurves[selectedCurveIndex].GetClosed());

        ImGui::Text("Intersects: %s", bezierCurves[selectedCurveIndex].IsSelfIntersecting() ? "true" : "false");

        ImGui::BeginChild(1, ImVec2(950, 450), true);
        ImGui::ZoneMapper::BeginZone();

        ImVec2 mPos = ImGui::GetMousePos();
        ImVec2 zonePos = ImGui::ZoneMapper::GetZonePos();
        ImVec2 zoneSize = ImGui::ZoneMapper::GetZoneSize();

        if (editMode) {
            if (ImGui::IsMouseClicked(0) && bezierCurves[selectedCurveIndex].dotIndex(12.0f) == -1 &&
                mPos.x >= zonePos.x && mPos.x < zoneSize.x + zonePos.x &&
                mPos.y >= zonePos.y && mPos.y < zoneSize.y + zonePos.y) {
                ImVec2 relativeMousePos = ImVec2(mPos.x - zonePos.x, mPos.y - zonePos.y);
                bezierCurves[selectedCurveIndex].AddPoint(relativeMousePos);
            }
            if (ImGui::IsMouseClicked(1)) {
                bezierCurves[selectedCurveIndex].DeletePoint(bezierCurves[selectedCurveIndex].dotIndex(12.0f));
            }
        }

        for (auto& bezierCurve : bezierCurves) {
            bezierCurve.Draw(&bezierCurve == &bezierCurves[selectedCurveIndex] && editMode);
        }

        ImGui::ZoneMapper::EndZone();
        ImGui::EndChild();
        
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
