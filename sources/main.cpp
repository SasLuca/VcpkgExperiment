#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <fmt/format.h>
#include <GLES3/gl3.h>

//#define SK_GANESH
//#define SK_GL
#include "gpu/GrBackendSurface.h"
#include "gpu/GrDirectContext.h"
#include "gpu/ganesh/gl/GrGLDirectContext.h"
#include "gpu/gl/GrGLInterface.h"
#include "gpu/gl/GrGLAssembleInterface.h"
#include "gpu/ganesh/SkSurfaceGanesh.h"
#include "gpu/ganesh/gl/GrGLBackendSurface.h"
#include "core/SkCanvas.h"
#include "core/SkColorSpace.h"
#include "core/SkSurface.h"
#include "gpu/GrContextOptions.h"
#include "gpu/GrDirectContext.h"
#include "gpu/gl/GrGLInterface.h"

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
}

struct SkiaStuff
{
    sk_sp<SkSurface> surface;
    sk_sp<GrDirectContext> context;
    SkCanvas* canvas;
};

GrGLFuncPtr skia_gl_get_proc(void*, const char* p)
{
    return (GrGLFuncPtr) SDL_GL_GetProcAddress(p);
}

// Function to initialize Skia with OpenGL backend
SkiaStuff createSkiaSurface(int width, int height)
{
    auto interface = GrGLMakeNativeInterface();
    if (interface == nullptr) {
        //backup plan. see https://gist.github.com/ad8e/dd150b775ae6aa4d5cf1a092e4713add?permalink_comment_id=4680136#gistcomment-4680136
        interface = GrGLMakeAssembledInterface(nullptr, skia_gl_get_proc);
    }
    auto sContext = GrDirectContexts::MakeGL(interface);

    GrGLFramebufferInfo framebufferInfo;
    framebufferInfo.fFBOID = 0; // assume default framebuffer
    // We are always using OpenGL and we use RGBA8 internal format for both RGBA and BGRA configs in OpenGL.
    //(replace line below with this one to enable correct color spaces) framebufferInfo.fFormat = GL_SRGB8_ALPHA8;
    framebufferInfo.fFormat = GL_RGBA8;

    SkColorType colorType = kRGBA_8888_SkColorType;
    GrBackendRenderTarget backendRenderTarget = GrBackendRenderTargets::MakeGL(width, height,
                                                                               0, // sample count
                                                                               0, // stencil bits
                                                                               framebufferInfo);

    //(replace line below with this one to enable correct color spaces) sSurface = SkSurfaces::WrapBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, SkColorSpace::MakeSRGB(), nullptr).release();
    auto sSurface = SkSurfaces::WrapBackendRenderTarget(sContext.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, nullptr);
    if (sSurface == nullptr) abort();
    return { sSurface, sContext, sSurface->getCanvas() };
}


struct AppState
{
    SDL_Window* window;
    SDL_GLContext gl_context;
    SkiaStuff skia;
    int w;
    int h;
};

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    fmt::println("FFmpeg Libraries:");
    fmt::println("libavcodec version: {}", avcodec_version());
    fmt::println("libavformat version: {}", avformat_version());
    fmt::println("libavutil version: {}", avutil_version());

    AppState* app_state = new AppState();
    *appstate = app_state;

    SDL_SetAppMetadata("VCPKG Experiment", "1.0", "com.lucasas.vcpkgexperiment");
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
    SDL_SetHint(SDL_HINT_VIDEO_FORCE_EGL, "1");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    app_state->window = SDL_CreateWindow("Experiment", 800, 600, SDL_WINDOW_OPENGL);
    SDL_SyncWindow(app_state->window);
    SDL_GetWindowSize(app_state->window, &app_state->w, &app_state->h);

    app_state->gl_context = SDL_GL_CreateContext(app_state->window);
    SDL_GL_MakeCurrent(app_state->window, app_state->gl_context);
    SDL_GL_SetSwapInterval(1);

    // Initialize Skia
    app_state->skia = createSkiaSurface(app_state->w, app_state->h);

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

    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a square with Skia
    app_state->skia.canvas->clear(SK_ColorBLACK);
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(SK_ColorRED);
    paint.setStyle(SkPaint::kFill_Style);
    app_state->skia.canvas->drawRect(SkRect::MakeXYWH(200, 150, 400, 300), paint);
    app_state->skia.context->flush();

    // Flush Skia commands and swap buffers
    //app_state->skia_surface.get().

    // Swap buffers
    SDL_GL_SwapWindow(app_state->window);

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
