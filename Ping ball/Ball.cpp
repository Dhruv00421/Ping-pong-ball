#include "Ball.h"
#include "raymath.h" // Add this include for Clamp function

Ball::Ball()
	: x(screenWidth/2), y(screenHeight/2), speedx( 100.0f), speedy( 100.0f ), r(20)
{
}

Ball::~Ball()
{

}

void Ball::Update(Box& playerBox, Box& aiBox)
{
	float deltaTime = GetFrameTime();
    //deltaTime = (deltaTime > 0.02f) ? 0.02f : deltaTime;
    deltaTime = Clamp(deltaTime, 0.001f, 0.016f);

    if (IsWindowResized()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
    }

	x += speedx * deltaTime;
	y += speedy * deltaTime;

	/*if (x - r <= 0 || x + r >= 800) speedx = -speedx;
	if (x - r <= 0 || x + r >= 450) speedy = -speedy;*/
	/*if (x - r <= 0) {
        x = r;
        speedx = -speedx;
    }
    if (x + r >= 800) {
        x = 800 - r;
        speedx = -speedx;
    }*/
    
    if (y - r <= 0) {
        y = r;
        speedy = -speedy;
    }
    if (y + r >= screenHeight) {
        y = screenHeight - r;
        speedy = -speedy;
    }
	//std::cout << deltaTime << std::endl;

    if (checkCollisionWithBox(playerBox))
    {
        if (x - r <= playerBox.getBoxX() || x + r >= playerBox.getBoxX() + playerBox.getBoxWidth())
            speedx *= -1;

        if (y - r <= playerBox.getBoxY() || y + r >= playerBox.getBoxY() + playerBox.getBoxHeight())
            speedy *= -1;

        //std::cout << "Collision detected" << std::endl;
    }
    if (checkCollisionWithBox(aiBox))
    {
        if (x - r <= aiBox.getBoxX() || x + r >= aiBox.getBoxX() + aiBox.getBoxWidth())
            speedx *= -1;

        if (y - r <= aiBox.getBoxY() || y + r >= aiBox.getBoxY() + aiBox.getBoxHeight())
            speedy *= -1;

        //std::cout << "Collision detected" << std::endl;
    }

}

void Ball::Draw() const
{
	DrawCircle(static_cast<int>(x), static_cast<int>(y), r, BLACK);

}

void Ball::OnImguiRender()
{
    ImGui::SliderFloat("Speed", &speedx, -1000.0f, 1000.0f);
    //ImGui::Button
}

bool Ball::checkCollisionWithBox( const Box& box) const
{
    //std::cout << "Checking" << std::endl;
    //std::cout << CheckCollisionCircleRec({ x, y }, r, box.getRect()) << std::endl;
    return CheckCollisionCircleRec({ x, y }, r, box.getRect());
}
