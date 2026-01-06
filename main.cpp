#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include<bits/stdc++.h>
using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

//Utility Functions
void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glVertex2f(x4, y4);
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void drawCircle(float x, float y, float radius, int segments = 100) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * M_PI * float(i) / float(segments);
        glVertex2f(x + radius * cosf(angle), y + radius * sinf(angle));
    }
    glEnd();
}

// Global Variables
bool snowfall = true;
bool lightsOn = true;
bool moonVisible = true;
bool santaVisible = true;
bool dayMode = false;
float carPosition = -0.5f;
float carSpeed = 0.0f;
float santaPosition = -1.2f;
float santaBob = 0.0f;
float flameBob = 0.0f;
float timeCounter = 0.0f;  // For animations
bool fireOn = true;



// Arrays for snowflakes and stars
struct Snowflake {
    float x, y, speed, drift;
};
struct Star {
    float x, y, brightness, twinkleSpeed;
};


//Vectors
vector<Snowflake> snowflakes(300);
vector<Star> stars(150);



// Draw Functions for the objects
// Sky
void drawSky() {
    if (dayMode) {
        glBegin(GL_QUADS);
        glColor3ub(135, 206, 235);
        glVertex2f(-1.0, 1.0);
        glVertex2f(1.0, 1.0);
        glColor3ub(173, 216, 230);
        glVertex2f(1.0, -1.0);
        glVertex2f(-1.0, -1.0);
        glEnd();
    } else {
        glBegin(GL_QUADS);
        glColor3ub(5, 5, 38);
        glVertex2f(-1.0, 1.0);
        glVertex2f(1.0, 1.0);
        glColor3ub(26, 26, 77);
        glVertex2f(1.0, -1.0);
        glVertex2f(-1.0, -1.0);
        glEnd();
    }
}

// Stars
void drawStars() {
    if (dayMode) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // twinkling effect
    for (const auto& star : stars) {
        float twinkle = 0.5f + 0.5f * sin(timeCounter * star.twinkleSpeed);
        glColor4ub(255, 255, 255, static_cast<unsigned char>(180 * star.brightness * twinkle));
        glPointSize(1.5f + 0.5f * twinkle);
        glBegin(GL_POINTS);
        glVertex2f(star.x, star.y);
        glEnd();
    }

    glDisable(GL_BLEND);
}

//Sun Moon
void drawSunMoon() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (dayMode) {
        // Sun
        glColor4ub(255, 255, 200, 60);
        drawCircle(0.8, 0.8, 0.1);
        glColor3ub(255, 255, 150);
        drawCircle(0.8, 0.8, 0.06);
    } else if (moonVisible) {
        // Moon
        glColor4ub(255, 255, 230, 30);
        drawCircle(0.8, 0.8, 0.1);
        glColor3ub(242, 242, 217);
        drawCircle(0.8, 0.8, 0.06);
        glColor3ub(217, 217, 191);
        drawCircle(0.78, 0.82, 0.008);
        drawCircle(0.82, 0.78, 0.01);
        drawCircle(0.8, 0.77, 0.006);
    }

    glDisable(GL_BLEND);
}



//ground (snow)
void drawGround() {
    if (dayMode) {
        glColor3ub(235, 235, 245);
    } else {
        glColor3ub(230, 230, 240);
    }
    drawQuad(-1.0, -0.45, 1.0, -0.45, 1.0, -1.0, -1.0, -1.0);
}

//Roads
void drawRoad() {
    if (dayMode) {
        glColor3ub(50, 50, 60);
    } else {
        glColor3ub(38, 38, 46);
    }
    drawQuad(-1.0, -0.6, 1.0, -0.6, 1.0, -0.45, -1.0, -0.45);

    if (dayMode) {
        glColor3ub(240, 240, 180);
    } else {
        glColor3ub(230, 230, 179);
    }
    glLineWidth(1.2f);

    for (float x = -0.9; x <= 0.9; x += 0.2) {
        drawLine(x, -0.525, x + 0.1, -0.525);
    }
}

//Trees
void drawSingleTree()
{
    // trunk
    if (dayMode)
        glColor3ub(101, 84, 71);
    else
        glColor3ub(89, 64, 51);

    glBegin(GL_QUADS);
        glVertex2f(-0.015f, 0.0f);
        glVertex2f( 0.015f, 0.0f);
        glVertex2f( 0.012f, 0.09f);
        glVertex2f(-0.012f, 0.09f);
    glEnd();

    // bottom
    if (dayMode)
        glColor3ub(46, 122, 46);
    else
        glColor3ub(26, 102, 26);

    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.20f);
        glVertex2f(-0.08f, 0.09f);
        glVertex2f( 0.08f, 0.09f);
    glEnd();

    // mid
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.26f);
        glVertex2f(-0.065f, 0.14f);
        glVertex2f( 0.065f, 0.14f);
    glEnd();

    // top
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.32f);
        glVertex2f(-0.05f, 0.19f);
        glVertex2f( 0.05f, 0.19f);
    glEnd();

    // snow layer
    glColor3ub(245, 245, 250);
    glBegin(GL_QUADS);
        glVertex2f(-0.07f, 0.14f);
        glVertex2f( 0.07f, 0.14f);
        glVertex2f( 0.06f, 0.155f);
        glVertex2f(-0.06f, 0.155f);
    glEnd();
}

void drawTrees()
{
    // Left Tree
    glPushMatrix();
        glTranslatef(-0.9f, -0.45f, 0.0f);
        glScalef(1.6f, 1.6f, 1.0f);
        drawSingleTree();
    glPopMatrix();

    // Other trees
    float positions[] = { -0.6f, -0.3f, 0.0f, 0.3f, 0.6f, 0.85f };

    for (float x : positions)
    {
        glPushMatrix();
            glTranslatef(x, -0.45f, 0.0f);
            glScalef(1.05f, 1.05f, 1.0f);
            drawSingleTree();
        glPopMatrix();
    }
}



//Houses
void drawSingleHouse(float r, float g, float b)
{
    // base
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.25f, 0.0f);
        glVertex2f(0.25f, 0.2f);
        glVertex2f(0.0f, 0.2f);
    glEnd();

    // roof
    glColor3f(0.38f, 0.28f, 0.23f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.02f, 0.2f);
        glVertex2f(0.27f, 0.2f);
        glVertex2f(0.125f, 0.35f);
    glEnd();

    // snow layer
    glColor3f(0.95f, 0.95f, 0.98f);
    glBegin(GL_QUADS);
        glVertex2f(-0.01f, 0.2f);
        glVertex2f(0.26f, 0.2f);
        glVertex2f(0.25f, 0.21f);
        glVertex2f(0.0f, 0.21f);
    glEnd();

    // window
if (dayMode || lightsOn)
    glColor3f(1.0f, 0.95f, 0.7f);   // light ON
else
    glColor3f(0.1f, 0.1f, 0.15f);   // light OFF

glBegin(GL_QUADS);
    glVertex2f(0.08f, 0.08f);
    glVertex2f(0.17f, 0.08f);
    glVertex2f(0.17f, 0.14f);
    glVertex2f(0.08f, 0.14f);
glEnd();

    // door
    glColor3f(0.38f, 0.28f, 0.23f);
    glBegin(GL_QUADS);
        glVertex2f(0.03f, 0.0f);
        glVertex2f(0.08f, 0.0f);
        glVertex2f(0.08f, 0.12f);
        glVertex2f(0.03f, 0.12f);
    glEnd();
}

//chimney
void drawChimney() {
    glColor3ub(80, 80, 80);
    drawQuad(0.18f, 0.23f, 0.21f, 0.23f, 0.21f, 0.32f, 0.18f, 0.32f);

    // Snow layer
    glColor3ub(240, 240, 250);
    drawQuad(0.175f, 0.32f, 0.215f, 0.32f, 0.205f, 0.33f, 0.185f, 0.33f);
}

void drawHouses() {
    float xPos[5] = { -0.8f, -0.4f, 0.0f, 0.4f, 0.8f };

    float colors[5][3] = {
        {0.5f, 0.3f, 0.3f},
        {0.3f, 0.5f, 0.3f},
        {0.3f, 0.3f, 0.6f},
        {0.5f, 0.5f, 0.3f},
        {0.6f, 0.3f, 0.6f}
    };

    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glTranslatef(xPos[i], -0.45f, 0.0f);
        drawSingleHouse(colors[i][0], colors[i][1], colors[i][2]);
        drawChimney();
        glPopMatrix();
    }
}


//mailbox
void drawMailbox() {
    glPushMatrix();
    glTranslatef(-0.55f, -0.45f, 0.0f);

    // Pole
    glColor3ub(120, 90, 50);
    drawQuad(-0.008f, -0.15f, 0.008f, -0.15f, 0.008f, 0.0f, -0.008f, 0.0f);

    // Box
    glColor3ub(200, 30, 30);
    drawQuad(-0.04f, 0.0f, 0.04f, 0.0f, 0.04f, 0.035f, -0.04f, 0.035f);

    // Mail slot
    glColor3ub(20, 20, 20);
    drawQuad(-0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.025f, -0.02f, 0.025f);

    glPopMatrix();
}


void drawClouds() {
    if (!dayMode) return;

    // Cloud 1
    glColor3ub(245, 245, 245);
    drawCircle(-0.6f, 0.75f, 0.05f);
    drawCircle(-0.56f, 0.76f, 0.045f);
    drawCircle(-0.64f, 0.76f, 0.045f);
    drawCircle(-0.58f, 0.78f, 0.04f);

    // Cloud 2
    drawCircle(-0.1f, 0.8f, 0.05f);
    drawCircle(-0.06f, 0.81f, 0.045f);
    drawCircle(-0.14f, 0.81f, 0.045f);
    drawCircle(-0.08f, 0.83f, 0.04f);

    // Cloud 3
    drawCircle(0.5f, 0.72f, 0.05f);
    drawCircle(0.54f, 0.73f, 0.045f);
    drawCircle(0.46f, 0.73f, 0.045f);
    drawCircle(0.52f, 0.75f, 0.04f);
}



//Street Lamp
void drawSingleLamp() {
    // Pole
    if (dayMode) {
        glColor3ub(58, 58, 71);
    } else {
        glColor3ub(38, 38, 51);
    }
    drawQuad(-0.004, -0.5, 0.004, -0.5, 0.004, 0.0, -0.004, 0.0);

    // Lamp housing
    if (dayMode) {
        glColor3ub(84, 84, 97);
    } else {
        glColor3ub(64, 64, 77);
    }
    drawQuad(-0.012, 0.0, 0.012, 0.0, 0.013, 0.015, -0.013, 0.015);

    // Light
        //Day
         glColor3ub(255, 250, 179);
         drawCircle(0.0, 0.008, 0.01);


        //night
    if (!dayMode && lightsOn) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4ub(255, 250, 204, 20);
        drawTriangle(0.0, 0.015, -0.15, -0.5, 0.15, -0.5);

        glColor3ub(255, 250, 179);
        drawCircle(0.0, 0.008, 0.01);

        glDisable(GL_BLEND);
    }
}

void drawLamps() {
    float lampPositions[] = {-0.7, -0.4, 0.0, 0.4, 0.7};
    for (float pos : lampPositions) {
        glPushMatrix();
        glTranslatef(pos, -0.1, 0.0);
        drawSingleLamp();
        glPopMatrix();
    }
}

//Car
void drawCarBody() {
    // Car body
    if (dayMode) {
        glColor3ub(199, 46, 46);
    } else {
        glColor3ub(179, 26, 26);
    }
    glBegin(GL_POLYGON);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.15, 0.0);
    glVertex2f(0.16, 0.05);
    glVertex2f(-0.01, 0.05);
    glEnd();

    // Car top
    if (dayMode) {
        glColor3ub(173, 40, 40);
    } else {
        glColor3ub(153, 20, 20);
    }
    drawQuad(0.02, 0.05, 0.13, 0.05, 0.13, 0.07, 0.02, 0.07);

    // Windows
    if (dayMode) {
        glColor3ub(173, 224, 255);
    } else {
        glColor3ub(153, 204, 242);
    }
    drawQuad(0.025, 0.052, 0.065, 0.052, 0.065, 0.065, 0.025, 0.065);
    drawQuad(0.075, 0.052, 0.125, 0.052, 0.125, 0.065, 0.075, 0.065);

    // Wheels
    glColor3ub(13, 13, 13);
    drawCircle(0.03, 0.0, 0.012);
    drawCircle(0.12, 0.0, 0.012);

    // Wheel rims
    if (dayMode) {
        glColor3ub(122, 122, 122);
    } else {
        glColor3ub(102, 102, 102);
    }
    drawCircle(0.03, 0.0, 0.006);
    drawCircle(0.12, 0.0, 0.006);
}

void drawCar() {
    // car position based on speed
    carPosition += carSpeed * 0.05f;

    // car position on the boundary (-1 to 1)
    if (carPosition < -0.85f) carPosition = -0.85f;
    if (carPosition > 0.85f) carPosition = 0.85f;

    glPushMatrix();
    glTranslatef(carPosition, -0.53f, 0.0f);  // updated carPosition
    drawCarBody();

    // Headlights
    if (!dayMode && lightsOn) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPushMatrix();
        glTranslatef(0.15, 0.02, 0.0);
        glColor4ub(255, 255, 204, 40);
        drawTriangle(0.0, 0.0, 0.2, -0.01, 0.2, -0.07);

        glColor3ub(255, 255, 200);
        drawCircle(0.0, 0.0, 0.006);
        glPopMatrix();

        glDisable(GL_BLEND);
    }
    glPopMatrix();
}

//Santa
void drawSanta() {
    if (!santaVisible) return;

    // Update santa position
    santaPosition += 0.003f;
    santaBob += 0.05f;
    if (santaPosition > 2.0f) santaPosition = -1.2f;

    glPushMatrix();
    glTranslatef(santaPosition, 0.8f + sin(santaBob) * 0.03f, 0.0f);  // updated santaPosition

    // Reindeer
    if (dayMode) {
        glColor3ub(160, 109, 71);
    } else {
        glColor3ub(140, 89, 51);
    }
    drawCircle(-0.05, 0.02, 0.015);
    glLineWidth(1.0f);
    drawLine(-0.05, 0.02, -0.08, 0.0);
    drawLine(-0.05, 0.02, -0.08, 0.04);

    // Sleigh
    if (dayMode) {
        glColor3ub(224, 46, 46);
    } else {
        glColor3ub(204, 26, 26);
    }
    glBegin(GL_POLYGON);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.18, 0.0);
    glVertex2f(0.17, 0.05);
    glVertex2f(0.01, 0.05);
    glEnd();

    // Sledge runners
    if (dayMode) {
        glColor3ub(240, 240, 250);
    } else {
        glColor3ub(230, 230, 242);
    }
    glLineWidth(2.0f);
    drawLine(0.02, -0.005, 0.16, -0.005);

    // Santa body
    if (dayMode) {
        glColor3ub(255, 40, 40);
    } else {
        glColor3ub(255, 0, 0);
    }
    drawCircle(0.09, 0.06, 0.025);

    // Santa face
    if (dayMode) {
        glColor3ub(255, 234, 193);
    } else {
        glColor3ub(245, 214, 173);
    }
    drawCircle(0.09, 0.09, 0.018);

    // Beard
    glColor3ub(242, 242, 242);
    drawCircle(0.09, 0.075, 0.015);

    // Santa hat
    if (dayMode) {
        glColor3ub(255, 40, 40);
    } else {
        glColor3ub(255, 0, 0);
    }
    drawTriangle(0.09, 0.108, 0.06, 0.07, 0.12, 0.07);

    // Hat pom-pom
    glColor3ub(255, 255, 255);
    drawCircle(0.09, 0.115, 0.008);

    glPopMatrix();
}

//Bench in the snow
void drawBench() {
    glPushMatrix();
    glTranslatef(0.6f, -0.9f, 0.0f);

    // Bench legs
    if (dayMode) {
        glColor3ub(90, 60, 35);
    } else {
        glColor3ub(80, 50, 25);
    }
    drawQuad(0.0, 0.0, 0.021, 0.0, 0.021, 0.084, 0.0, 0.084);
    drawQuad(0.189, 0.0, 0.21, 0.0, 0.21, 0.084, 0.189, 0.084);
    drawQuad(0.0, 0.0, 0.021, 0.0, 0.021, 0.14, 0.0, 0.14);
    drawQuad(0.189, 0.0, 0.21, 0.0, 0.21, 0.14, 0.189, 0.14);

    // Bench seat
    if (dayMode) {
        glColor3ub(100, 65, 40);
    } else {
        glColor3ub(90, 55, 30);
    }
    drawQuad(0.0, 0.084, 0.21, 0.084, 0.21, 0.105, 0.0, 0.105);

    // Bench backrest
    if (dayMode) {
        glColor3ub(90, 60, 35);
    } else {
        glColor3ub(80, 50, 25);
    }
    drawQuad(0.0, 0.105, 0.21, 0.105, 0.21, 0.175, 0.0, 0.175);

    // Armrests
    if (dayMode) {
        glColor3ub(100, 65, 40);
    } else {
        glColor3ub(90, 55, 30);
    }
    drawQuad(-0.014, 0.084, 0.0, 0.084, 0.0, 0.14, -0.014, 0.14);
    drawQuad(0.21, 0.084, 0.224, 0.084, 0.224, 0.14, 0.21, 0.14);

    // Backrest slats
    if (dayMode) {
        glColor3ub(70, 45, 25);
    } else {
        glColor3ub(60, 35, 15);
    }
    glLineWidth(2.5f);
    drawLine(0.0, 0.119, 0.21, 0.119);
    drawLine(0.0, 0.149, 0.21, 0.149);
    drawLine(0.0, 0.179, 0.21, 0.179);

    // Snow on bench
    if (dayMode) {
        glColor3ub(240, 240, 250);
    } else {
        glColor3ub(230, 230, 240);
    }
    glBegin(GL_POLYGON);
    glVertex2f(0.014, 0.105);
    glVertex2f(0.196, 0.105);
    glVertex2f(0.175, 0.107);
    glVertex2f(0.035, 0.107);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.014, 0.175);
    glVertex2f(0.196, 0.175);
    glVertex2f(0.182, 0.177);
    glVertex2f(0.028, 0.177);
    glEnd();

    // Snow drifts
    drawCircle(0.0105, 0.0, 0.015);
    drawCircle(0.1995, 0.0, 0.015);

    glPopMatrix();
}

//CampFire
void drawCampfire() {
    glPushMatrix();
    glTranslatef(0.45f, -0.9f, 0.0f);

    // Animate flame
    flameBob += 0.1f;

    // Logs
    if (dayMode) glColor3ub(120, 80, 40);
    else         glColor3ub(100, 60, 30);

    glLineWidth(3.0f);
    drawLine(-0.02f, 0.0f, 0.02f, 0.05f);
    drawLine(0.02f, 0.0f, -0.02f, 0.05f);
    drawLine(-0.015f, 0.01f, 0.015f, 0.01f);
    drawLine(0.0f, 0.0f, 0.0f, 0.06f);

    // Stones
    if (dayMode) glColor3ub(100, 100, 100);
    else         glColor3ub(80, 80, 80);
    drawCircle(-0.03f, 0.0f, 0.008f);
    drawCircle( 0.03f, 0.0f, 0.008f);
    drawCircle( 0.0f, -0.015f, 0.006f);
    drawCircle(-0.025f, -0.01f, 0.005f);
    drawCircle( 0.025f, -0.01f, 0.005f);

    // Flame
    if (fireOn) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float flameHeight = sin(flameBob) * 0.01f;

        glPushMatrix();
        glTranslatef(0.0f, flameHeight, 0.0f);

        glColor4ub(255, 100, 0, 180);
        drawTriangle(0.0f, 0.04f, -0.015f, 0.0f, 0.015f, 0.0f);

        glColor4ub(255, 150, 0, 200);
        drawTriangle(0.0f, 0.03f, -0.012f, 0.005f, 0.012f, 0.005f);

        glColor4ub(255, 220, 100, 220);
        drawTriangle(0.0f, 0.02f, -0.008f, 0.01f, 0.008f, 0.01f);

        glPopMatrix();
        glDisable(GL_BLEND);
    }

    glPopMatrix();
}


//Snowman
void drawSnowman() {
    glPushMatrix();
    glTranslatef(-0.8f, -0.9f, 0.0f);

    // Snow drift
    if (dayMode) {
        glColor3ub(230, 230, 240);
    } else {
        glColor3ub(220, 220, 230);
    }
    drawCircle(0.0, 0.12f, 0.052f);

    // Bottom snowball
    if (dayMode) {
        glColor3ub(245, 245, 250);
    } else {
        glColor3ub(240, 240, 245);
    }
    drawCircle(0.0, 0.12f, 0.048);

    // Middle snowball
    drawCircle(0.0, 0.212f, 0.036);

    // Top snowball
    drawCircle(0.0, 0.272f, 0.024);

    // Eyes
    glColor3ub(20, 20, 20);
    drawCircle(-0.006, 0.276f, 0.003);
    drawCircle(0.006, 0.276f, 0.003);

    // Nose
    glColor3ub(255, 140, 0);
    drawTriangle(0.0, 0.272f, 0.01, 0.268f, 0.0, 0.264f);

    // Mouth
    glColor3ub(20, 20, 20);
    for (int i = 0; i < 5; i++) {
        float x = -0.0048f + i * 0.0024f;
        float y = 0.264f - i * i * 0.00012f;
        drawCircle(x, y, 0.0015f);
    }

    // Buttons
    drawCircle(0.0, 0.212f - 0.012, 0.004);
    drawCircle(0.0, 0.212f, 0.004);
    drawCircle(0.0, 0.212f + 0.012, 0.004);

    // Hat
    glColor3ub(20, 20, 20);
    drawQuad(-0.016, 0.292f, 0.016, 0.292f, 0.016, 0.296f, -0.016, 0.296f);
    drawQuad(-0.01, 0.296f, 0.01, 0.296f, 0.01, 0.32f, -0.01, 0.32f);

    // Hat band
    glColor3ub(220, 60, 60);
    drawQuad(-0.0104, 0.304f, 0.0104, 0.304f, 0.0104, 0.308f, -0.0104, 0.308f);

    // Arms
    if (dayMode) {
        glColor3ub(100, 80, 60);
    } else {
        glColor3ub(90, 70, 50);
    }
    glLineWidth(3.5f);
    drawLine(-0.024, 0.212f, -0.072, 0.192f);
    drawLine(0.024, 0.212f, 0.072, 0.192f);

    // Scarf
    glColor3ub(220, 60, 60);
    drawQuad(-0.012, 0.248f, 0.012, 0.248f, 0.012, 0.26f, -0.012, 0.26f);
    drawTriangle(-0.012, 0.248f, -0.024, 0.232f, 0.0, 0.232f);
    drawTriangle(0.012, 0.248f, 0.0, 0.232f, 0.024, 0.232f);

    // Snow on hat
    if (dayMode) {
        glColor3ub(240, 240, 250);
    } else {
        glColor3ub(230, 230, 240);
    }
    glBegin(GL_POLYGON);
    glVertex2f(-0.012, 0.32f);
    glVertex2f(0.012, 0.32f);
    glVertex2f(0.008, 0.322f);
    glVertex2f(-0.008, 0.322f);
    glEnd();

    glPopMatrix();
}

//Initialize Snowflakes
void initializeSnowflakes() {

    for (auto& flake : snowflakes) {
        flake.x = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        flake.y = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        flake.speed = 0.0005f + (rand() / (float)RAND_MAX) * 0.0015f;
        flake.drift = (rand() / (float)RAND_MAX) * 0.0003f - 0.00015f;
    }
}

//Update Snowflakes
void updateSnowflakes() {
    if (!snowfall) return;

    for (auto& flake : snowflakes) {
        // Move snowflake
        flake.y -= flake.speed;
        flake.x += flake.drift;

        // Reset if fallen off screen
        if (flake.y < -1.0f) {
            flake.y = 1.0f;
            flake.x = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        }

        // Wrap around horizontally
        if (flake.x < -1.0f) flake.x = 1.0f;
        if (flake.x > 1.0f) flake.x = -1.0f;
    }
}

void drawSnow() {
    if (!snowfall) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Snow
    if (dayMode) {
        glColor4ub(255, 255, 255, 200);
    } else {
        glColor4ub(255, 255, 255, 180);
    }

    glPointSize(2.0f);
    glBegin(GL_POINTS);

    for (const auto& flake : snowflakes) {
        glVertex2f(flake.x, flake.y);
    }

    glEnd();
    glDisable(GL_BLEND);
}

void drawSignboard() {
    glPushMatrix();
    glTranslatef(0.2f, -0.35f, 0.0f);

    // Poles
    glColor3ub(120, 80, 50);
    drawQuad(-0.04f, -0.25f, -0.03f, -0.25f, -0.03f, 0.0f, -0.04f, 0.0f);
    drawQuad(0.03f, -0.25f, 0.04f, -0.25f, 0.04f, 0.0f, 0.03f, 0.0f);

    // Board
    glColor3ub(30, 60, 100);
    drawQuad(-0.18f, 0.0f, 0.18f, 0.0f, 0.18f, 0.08f, -0.18f, 0.08f);

    // border
    glColor3ub(255, 215, 0);
    glLineWidth(1.5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.18f, 0.0f);
    glVertex2f(0.18f, 0.0f);
    glVertex2f(0.18f, 0.08f);
    glVertex2f(-0.18f, 0.08f);
    glEnd();

    // Text
    glColor3ub(255, 255, 255);
    glRasterPos2f(-0.175f, 0.03f);
    string text = "WELCOME TO THE NEIGHBORHOOD";

    // font
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);

    // elements
    glColor3ub(255, 215, 0);
    drawQuad(-0.05f, 0.0f, -0.02f, 0.0f, -0.02f, 0.01f, -0.05f, 0.01f);
    drawQuad(0.02f, 0.0f, 0.05f, 0.0f, 0.05f, 0.01f, 0.02f, 0.01f);

    glPopMatrix();
}


//Initialize Stars
void initializeStars() {

    for (auto& star : stars) {
        star.x = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        star.y = (rand() / (float)RAND_MAX) * 1.0f;  // Stars only in upper half
        star.brightness = 0.5f + (rand() / (float)RAND_MAX) * 0.5f;
        star.twinkleSpeed = 0.5f + (rand() / (float)RAND_MAX) * 2.0f;
    }
}

//Update Function
void update(int value) {
    // Update time for animations
    timeCounter += 0.016f;  // Approximately 60 FPS

    // Update snowflakes
    updateSnowflakes();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

//Keyboard event control
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 's':
        case 'S':
            snowfall = !snowfall;
            break;
        case 'l':
        case 'L':
            lightsOn = !lightsOn;
            break;
        case 'm':
        case 'M':
            moonVisible = !moonVisible;
            break;
        case 'a':
        case 'A':
            santaVisible = !santaVisible;
            break;
        case 'r':
        case 'R':
            carPosition = -0.5f;
            carSpeed = 0.0f;
            break;
        case '+':
            if (carSpeed < 0) carSpeed = 0.0f;
            else carSpeed += 0.002f;
            if (carSpeed > 0.03f) carSpeed = 0.03f;
            break;
        case '-':
            if (carSpeed > 0) carSpeed = 0.0f;
            else carSpeed -= 0.002f;
            if (carSpeed < -0.03f) carSpeed = -0.03f;
            break;
        case '0':
        case 'd':
        case 'D':
            dayMode = !dayMode;
            if (dayMode) {
                lightsOn = false;
            }
            break;
        case 27:  // ESC key
            exit(0);
            break;
    }
}

void specialKeys(int key, int x, int y) {

    switch (key) {
        case GLUT_KEY_LEFT:
            carSpeed = -0.125f;  // Move left
            break;
        case GLUT_KEY_RIGHT:
            carSpeed = 0.15f;   // Move right
            break;
    }
}

void specialKeysUp(int key, int x, int y) {

    switch (key) {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            carSpeed = 0.0f;
            break;
    }
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float mouseX = (x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 2.0f - 1.0f;
        float mouseY = 1.0f - (y / (float)glutGet(GLUT_WINDOW_HEIGHT)) * 2.0f;

        float dx = mouseX - 0.8f;
        float dy = mouseY - 0.8f;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= 0.15f) {
            dayMode = !dayMode;
            if (dayMode) {
                lightsOn = false;
            }
        }

        // Campfire toggle
            float fx = mouseX - 0.45f;
            float fy = mouseY + 0.9f;

            if (fx * fx + fy * fy <= 0.02f) {
                     fireOn = !fireOn;
               }

    }
}

//Display
void display() {
    glClearColor(0.02f, 0.02f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawStars();
    drawSunMoon();
    drawGround();
    //drawSnow();
    drawRoad();
    drawTrees();
    drawHouses();
    //drawTrees();
    //drawLamps();
    drawCar();
    drawLamps();
    drawClouds();
    drawSanta();
    drawBench();
    drawCampfire();
    drawSnowman();
    drawSnow();
    //drawClouds();
    drawMailbox();
    drawSignboard();

    // Display controls
    glColor3f(1.0f, 1.0f, 1.0f);

    // day/night indicator
    glRasterPos2f(0.7f, 0.95f);
    if (dayMode) {
        string mode = "Day Mode";
        for (char c : mode) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    } else {
        string mode = "Night Mode";
        for (char c : mode) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }

    glutSwapBuffers();
}

//Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Winter In The Neighborhood");


    // glutFullScreen();

    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    // Initialize random seed
    srand(static_cast<unsigned int>(time(NULL)));

    // Initialize snowflakes and stars
    initializeSnowflakes();
    initializeStars();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);
    glutMouseFunc(mouseClick);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
