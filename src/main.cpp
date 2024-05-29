//Justin Kim
//didn't get the scaling to work for both of the shapes

#include <include/raylib.h>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <imgui/imgui_stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


class Shape
{
public:
    std::string name;
    std::string newName = name;
    float posX;
    float posY;
    float sX;
    float sY;
    float color[3];
    float scaleFactor;
    bool active = true;
    bool selected = false;

    virtual void move(int windowWidth, int windowHeight) {};

    virtual void draw() {};

    virtual void drawText(Font font, int fontSize, float fontColor[3]) {};

    virtual void setGui() {};

    std::string getName() {
        return name;
    };
};

class Circle : public Shape {

    float radius;

public:
    Circle(std::string n, float x, float y, float speedX, float speedY, float rC, float gC, float bC, float r) {
        name = n;
        newName = name;
        posX = x;
        posY = y;
        sX = speedX;
        sY = speedY;
        color[0] = rC;
        color[1] = gC;
        color[2] = bC;
        radius = r;

        scaleFactor = 1.0f;
        active = true;
    };

    void move(int windowWidth, int windowHeight) override {
        if (active) {
            if (posX - radius <= 0 || posX + radius >= windowWidth) {
                sX *= -1;
            }
            if (posY - radius <= 0 || posY + radius >= windowHeight) {
                sY *= -1;
            }
            posX += sX;
            posY += sY;
        }
    }

    void draw() override {
        if (active) {
            DrawCircle((int)posX, (int)posY, radius * scaleFactor, ColorFromNormalized({ color[0],color[1],color[2],1.0f }));
        }   
     };

    void drawText(Font font, int fontSize, float fontColor[3]) override {
        if (active) {
            Vector2 textSize = MeasureTextEx(font, newName.c_str(), fontSize, 1);
            DrawTextEx(font, newName.c_str(), { posX - (textSize.x / 2), posY - (textSize.y / 2) }, fontSize, 1, ColorFromNormalized({ fontColor[0],fontColor[1],fontColor[2],1.0f }));
        }
    }

    void setGui() override {
        ImGui::Checkbox("Active", &active);
        ImGui::SliderFloat("Scale", &scaleFactor, 0.1f, 5.0f);
        ImGui::DragFloat2("Velocity", &sX, 0.1f);
        ImGui::ColorEdit3("Color", color);
        ImGui::InputText("Name", &newName);
    }
};

class Rectangles : public Shape {

    float width;
    float height;

public:
    Rectangles(std::string n, float x, float y, float speedX, float speedY, float rC, float gC, float bC, float w, float h) {
        name = n;
        newName = name;
        posX = x;
        posY = y;
        sX = speedX;
        sY = speedY;

        color[0] = rC;
        color[1] = gC;
        color[2] = bC;

        width = w;
        height = h;

        scaleFactor = 1.0f;
        active = true;
    };

    void move(int windowWidth, int windowHeight) override {

        if (active) {
            if (posX <= 0 || posX + width >= windowWidth) {
                sX *= -1;
            }
            if (posY <= 0 || posY + height >= windowHeight) {
                sY *= -1;
            }
            posX += sX;
            posY += sY;
        }   
    }

    void draw() override {
        if (active) {
            DrawRectangle((int)posX, (int)posY, width * scaleFactor, height * scaleFactor, ColorFromNormalized({ color[0],color[1],color[2],1.0f }));
        }
    };

    void drawText(Font font, int fontSize, float fontColor[3]) override {
        if (active) {
            Vector2 textSize = MeasureTextEx(font, newName.c_str(), fontSize, 1);
            DrawTextEx(font, newName.c_str(), { (posX + (width*scaleFactor) / 2) - (textSize.x / 2), (posY + (height*scaleFactor) / 2) - (textSize.y / 2) }, fontSize, 1, ColorFromNormalized({ fontColor[0],fontColor[1],fontColor[2],1.0f }));
        }
    }

    void setGui() override {
        ImGui::Checkbox("Active", &active);
        ImGui::SliderFloat("Scale", &scaleFactor, 0.1f, 5.0f);
        ImGui::DragFloat2("Velocity", &sX, 0.1f);
        ImGui::ColorEdit3("Color", color);
        ImGui::InputText("Name", &newName);
    }
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    int screenWidth;
    int screenHeight;

    std::string f;
    int fontSize;
    float fontColor[3] = { 0, 0, 0 };

    std::vector<Shape*> shapes;

    std::string line;

    std::fstream file("assets/input.txt");

    bool drawShapes = true;
    bool drawText = true;
    bool simulate = true;

    if (file.is_open()) {
        while (std::getline(file, line)) {

            std::cout << line << "\n";
            
            std::istringstream i(line);

            std::string val;

            std::vector<std::string> v;
            
            while (std::getline(i, val, ' ')) {
                v.push_back(val);
            }
            
            if (v[0] == "Window") {
                screenWidth = stoi(v[2]);
                screenHeight = stoi(v[3]);
            }
            else if (v[0] == "Font") {
                f = v[1];
                fontSize = stoi(v[2]);
                fontColor[0] = std::stof(v[3]);
                fontColor[1] = std::stof(v[4]);
                fontColor[2] = std::stof(v[5]);
            }
            else {
                std::string name = v[1];
                float posX = std::stof(v[2]);
                float posY = std::stof(v[3]);
                float sX = std::stof(v[4]);
                float sY = std::stof(v[5]);
                float cR = std::stof(v[6]);
                float cG = std::stof(v[7]);
                float cB = std::stof(v[8]);

                if (v[0] == "Rectangle") {
                    float width = std::stof(v[9]);
                    float height = std::stof(v[10]);
                    shapes.push_back(new Rectangles(name, posX, posY, sX, sY, cR, cG, cB, width, height));
                }
                else if (v[0] == "Circle") {
                    float radius = std::stof(v[9]);
                    shapes.push_back(new Circle(name, posX, posY, sX, sY, cR, cG, cB, radius));
                }
            }
        }
    }
    else {
        std::cout << "couldn't open file";
        return 0;
    }

    for (auto val : shapes) {
        std::cout << val->getName() << "\n";
    }

    // Initialization
    //--------------------------------------------------------------------------------------

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Assignment 1 Starter Code");
    
    //initialize the raylib ImGui backend
    rlImGuiSetup(true);
    //increase ImGui item size to 2x
    ImGui::GetStyle().ScaleAllSizes(2);

    // Set raylib to target 60 frames-per-second (this does not mean it will actually RUN at 60 fps)
    SetTargetFPS(60);               

    // General variables
    //--------------------------------------------------------------------------------------

    //shape properties to draw on the screen (circle for this example)
    //units of size and speed are in pixels

    ////Let's draw some text to the screen too
    //bool drawText=true;
    //std::string strText= "Some Text";
    //std::string newText= strText;

    //load a font (Raylib gives warning if font can't be found, then uses default as fallback)
    Font font = LoadFont(f.c_str());

    // Main game loop
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        //move shapes
        if (simulate) {
            for (int i = 0; i < shapes.size(); i++) {
                shapes[i]->move(screenWidth, screenHeight);
            }
        }
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            //********** Raylib Drawing Content **********

            //draw shapes
            if (drawShapes) {
                for (int i = 0; i < shapes.size(); i++) {
                    shapes[i]->draw();
                    if (drawText) {
                        shapes[i]->drawText(font, fontSize, fontColor);
                    }
                }
            }

            //********** ImGUI Content *********
            
            //Draw imgui stuff last so it is over the top of everything else
            rlImGuiBegin();

                //sets the next window to be at this position
                //also uses the imgui.ini that gets created at first run
                ImGui::SetNextWindowSize(ImVec2(350, 450));
                //creates a new window
                ImGui::Begin("Control Panel",NULL,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse);
                    ImGui::Text("All Shapes");
                    //checkboxes, they directly modify the value (which is why we send a reference)
//                    ImGui::Checkbox("Draw Cricle",&drawCirc);
                    ImGui::Checkbox("Draw Shapes",&drawShapes);
                    ImGui::SameLine();
                    ImGui::Checkbox("Draw Text", &drawText);
                    ImGui::SameLine();
                    ImGui::Checkbox("Simulate", &simulate);

                    ImGui::Text("Select Shape");

                    static int currIndex = 0;

                    Shape* currShape = shapes[currIndex];
                    
                    if (ImGui::BeginCombo("Shape", currShape->getName().c_str())) 
                    {
                        for (int i = 0; i < shapes.size(); i++)
                        {
                            const bool is_selected = (currIndex == i);
                            if (ImGui::Selectable(shapes[i]->getName().c_str(), is_selected)) {
                                currIndex = i;
                                shapes[currIndex]->selected = true;
                            }

                            if (is_selected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::BeginGroup();
                    shapes[currIndex]->setGui();
                    ImGui::EndGroup();


                    //slider, again directly modifies the value and limites between 0 and 300 for this example
//                    ImGui::SliderFloat("Radius",&circRadius,0.0f,300.0f);
                    
                    //color picker button, directly modifies the color (3 element float array)
//                    ImGui::ColorEdit3("Circle Color",color);
                    
                    //text input field, directly modifies the string
 //                   ImGui::InputText("Text",&newText);
                    
                    //buttons, returns true if clicked on this frame
                    //if(ImGui::Button("Set Text")){
                    //    strText=newText;
                    //}

                    //The next item will be on the same line as the previous one
                //ends this window
                ImGui::End();

                //show ImGui Demo Content if you want to see it
                //bool open = true;
                //ImGui::ShowDemoWindow(&open);

            // end ImGui Content
            rlImGuiEnd();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // Clean Up
    //--------------------------------------------------------------------------------------
    rlImGuiShutdown();    // Shuts down the raylib ImGui backend
    UnloadFont(font);     // Remove font from memory
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}