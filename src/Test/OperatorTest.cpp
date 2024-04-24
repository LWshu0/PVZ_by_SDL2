#include "Core/ExtSDL.h"
#include <iostream>

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_FPoint p1{ 0.1f, 0.2f };
    SDL_FPoint p2{ 0.3f, 0.4f };
    float factor = 7.0f;

    SDL_FPoint p_sum = p1 + p2;
    SDL_FPoint p_sub = p1 - p2;
    SDL_FPoint p_mul = p1 * p2;
    SDL_FPoint p_div = p1 / p2;
    SDL_FPoint p_neg = -p1;

    SDL_FPoint p_fsum = p1 + factor;
    SDL_FPoint p_fsub = p1 - factor;
    SDL_FPoint p_fmul = p1 * factor;
    SDL_FPoint p_fdiv = p1 / factor;

    SDL_Log("p1: (%f, %f), p2: (%f, %f)\n", p1.x, p1.y, p2.x, p2.y);

    SDL_Log("p1 + p2: %f, %f\n", p_sum.x, p_sum.y);
    SDL_Log("p1 - p2: %f, %f\n", p_sub.x, p_sub.y);
    SDL_Log("p1 * p2: %f, %f\n", p_mul.x, p_mul.y);
    SDL_Log("p1 / p2: %f, %f\n", p_div.x, p_div.y);
    SDL_Log("-p1: %f, %f\n", p_neg.x, p_neg.y);

    SDL_Log("p1 + factor: %f, %f\n", p_fsum.x, p_fsum.y);
    SDL_Log("p1 - factor: %f, %f\n", p_fsub.x, p_fsub.y);
    SDL_Log("p1 * factor: %f, %f\n", p_fmul.x, p_fmul.y);
    SDL_Log("p1 / factor: %f, %f\n", p_fdiv.x, p_fdiv.y);

    SDL_Quit();
    return 0;
}