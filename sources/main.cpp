#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#include <fmt/format.h>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
}

struct AppState
{
    SDL_Window* window;
    SDL_Renderer* renderer;
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    fmt::println("FFmpeg Libraries:");
    fmt::println("libavcodec version: {}", avcodec_version());
    fmt::println("libavformat version: {}", avformat_version());
    fmt::println("libavutil version: {}", avutil_version());

    AppState* app_state = new AppState();
    *appstate = app_state;

    SDL_SetAppMetadata("VCPKG Experiment", "1.0", "com.example.renderer-clear");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("VCPKG Experiment", 640, 480, 0, &app_state->window, &app_state->renderer);
    SDL_SyncWindow(app_state->window);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* app_state = (AppState*) appstate;

    const double now = ((double)SDL_GetTicks()) / 1000.0;  // convert from milliseconds to seconds.

    // Choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly.
    const auto red = float(0.5 + 0.5 * SDL_sin(now));
    const auto green = float(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const auto blue = float(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat(app_state->renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);

    SDL_RenderClear(app_state->renderer);
    SDL_RenderPresent(app_state->renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    SDL_Quit();
}