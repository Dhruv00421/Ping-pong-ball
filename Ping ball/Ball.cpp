#include "Ball.h"
#include "raymath.h" // Add this include for Clamp function
#include "raylib.h"

Ball::Ball()
	: x(screenWidth/2), y(screenHeight/2), speedx( -150.0f), speedy( 150.0f ), r(20), speedInc(1.1f)
{
}

Ball::~Ball()
{

}

void Ball::Update(Paddle& playerPaddle, Paddle& aiPaddle, Ball& ball)
{
    
	float deltaTime = GetFrameTime();
    //deltaTime = (deltaTime > 0.02f) ? 0.02f : deltaTime;
    deltaTime = Clamp(deltaTime, 0.001f, 0.016f);

	x += speedx * deltaTime;
	y += speedy * deltaTime;
    
    if (checkCollisionWithWall(ball))
    {
        if (y - r <= 0) {
            y = r;
            speedy = -speedy;
        }
        if (y + r >= screenHeight) {
            y = screenHeight - r;
            speedy = -speedy;
        }
    }
	//std::cout << deltaTime << std::endl;

    if (checkCollisionWithPaddle(playerPaddle))
    {
       /* if (x - r <= playerPaddle.getPaddleX() || x + r >= playerPaddle.getPaddleX() + playerPaddle.getPaddleWidth())
            speedx *= -1 * speedInc;

        if (y - r <= playerPaddle.getPaddleY() || y + r >= playerPaddle.getPaddleY() + playerPaddle.getPaddleHeight())
            speedy *= -1 * speedInc;*/

        speedx = -speedx;  // Reverse X direction
        speedx *= speedInc; // Increase speed
        speedy *= speedInc; // Increase speed
        speedx = Clamp(speedx, -600.0f, 600.0f);  // Prevent too fast movement
        speedy = Clamp(speedy, -600.0f, 600.0f);
    }
    if (checkCollisionWithPaddle(aiPaddle))
    {
        if (x - r <= aiPaddle.getPaddleX() || x + r >= aiPaddle.getPaddleX() + aiPaddle.getPaddleWidth())
        {
            speedx *= -1;
            float paddleVelocity = aiPaddle.getPaddleVelocity() * 1.1f;
            aiPaddle.setPaddleVelocity(paddleVelocity);
        }
        if (y - r <= aiPaddle.getPaddleY() || y + r >= aiPaddle.getPaddleY() + aiPaddle.getPaddleHeight())
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

bool Ball::checkCollisionWithPaddle( const Paddle& Paddle) const
{
    return CheckCollisionCircleRec({ x, y }, r, Paddle.getRect());
}

bool Ball::checkCollisionWithWall(const Ball& ball) const
{
    if (y - r <= 0 || y + r >= screenHeight) return true;
    else return false;
    
}
