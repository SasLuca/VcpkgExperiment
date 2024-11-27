#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <fmt/format.h>
#include <GLES3/gl3.h>

#include "gpu/ganesh/GrBackendSurface.h"
#include "gpu/ganesh/GrDirectContext.h"
#include "gpu/ganesh/gl/GrGLDirectContext.h"
#include "gpu/ganesh/gl/GrGLInterface.h"
#include "gpu/ganesh/gl/GrGLAssembleInterface.h"
#include "gpu/ganesh/SkSurfaceGanesh.h"
#include "gpu/ganesh/gl/GrGLBackendSurface.h"
#include "core/SkCanvas.h"
#include "core/SkColorSpace.h"
#include "core/SkSurface.h"
#include "core/SkPaint.h"
#include "core/SkRect.h"
#include "core/SkMaskFilter.h"
#include "core/SkBlurTypes.h"
#include "core/SkFont.h"
#include "core/SkTextBlob.h"
#include "core/SkFontMgr.h"
#include "gpu/ganesh/GrContextOptions.h"
#include "gpu/ganesh/GrDirectContext.h"
#include "gpu/ganesh/gl/GrGLInterface.h"

#include "include/ports/SkTypeface_win.h"

#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/TypefaceFontProvider.h"
#include "modules/skparagraph/include/FontCollection.h"

struct SkiaStuff
{
    sk_sp<SkSurface> surface;
    sk_sp<GrDirectContext> context;
    sk_sp<SkFontMgr> fontMgr;
    SkCanvas* canvas;

    sk_sp<SkTypeface> twemoji;
    SkString twemoji_family_name;
    sk_sp<SkTypeface> roboto;
    SkString roboto_family_name;
    sk_sp<skia::textlayout::FontCollection> font_collection;
    sk_sp<skia::textlayout::TypefaceFontProvider> typeface_provider;
};

struct AppState
{
    SDL_Window* window;
    SDL_GLContext gl_context;
    SkiaStuff skia;
    int w;
    int h;
};

// region Skia init crap
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
    return { sSurface, sContext, SkFontMgr_New_DirectWrite(), sSurface->getCanvas() };
}

//endregion

// region Skia Rendering Experiments
class AnimatedTextWithEmojis {
public:
    void Draw(float timeElapsed)
    {
        #if 0
        SkTextBlobBuilder textBlobBuilder;
        SkFont font = SkFont(fontMgr->matchFamilyStyle("Roboto", {}), 50);
        const SkTextBlobBuilder::RunBuffer& run = textBlobBuilder.allocRun(font, 1, 20, 100);
        run.glyphs[0] = 20;
        sk_sp<const SkTextBlob> blob = textBlobBuilder.make();
        SkPaint paint;
        paint.setColor(SK_ColorBLUE);
        canvas_->drawTextBlob(blob.get(), 0, 0, paint);
        #endif
        #if 1
        // Set up SkPaint for text
        SkPaint paint;
        paint.setColor(SK_ColorBLACK); // Set text color
        paint.setAntiAlias(true); // Anti-alias the text

        // Emojis and text to display
        const char* text = "Hello World! 🌍🎉🚀";

        // Create SkTextBlob for the text
        sk_sp<SkTextBlob> blob =
            SkTextBlob::MakeFromString(text, SkFont(fontMgr->matchFamilyStyle("Roboto", {}), 100 * scaleFactor_));

        // Draw the text at the center of the canvas
        canvas_->drawTextBlob(blob, 0, 200, paint);

        // Update the scale factor over time (for demonstration, we use timeElapsed)
        scaleFactor_ = 1.0f + 0.5f * sin(0.0001 * timeElapsed);  // Create a sine wave for scaling effect
        #endif
    }


    SkCanvas* canvas_ = nullptr;
    sk_sp<SkFontMgr> fontMgr;
    float scaleFactor_ = 1;
};

void drawRectangleWithDropShadow(SkCanvas* canvas)
{
    // Define the rectangle and its shadow properties
    SkRect rect = SkRect::MakeXYWH(100, 100, 200, 100); // x, y, width, height
    SkScalar shadowOffsetX = 10.0f;
    SkScalar shadowOffsetY = 10.0f;
    SkColor shadowColor = SK_ColorBLACK;
    SkScalar shadowBlurRadius = 15.0f;

    // Create a paint for the shadow
    SkPaint shadowPaint;
    shadowPaint.setColor(shadowColor);
    shadowPaint.setAntiAlias(true);
    shadowPaint.setMaskFilter(SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, shadowBlurRadius));

    // Draw the shadow
    SkRect shadowRect = rect.makeOffset(shadowOffsetX, shadowOffsetY);
    canvas->drawRect(shadowRect, shadowPaint);

    // Create a paint for the rectangle
    SkPaint rectPaint;
    rectPaint.setColor(SK_ColorBLUE); // Rectangle color
    rectPaint.setAntiAlias(true);

    // Draw the rectangle
    canvas->drawRect(rect, rectPaint);
}

void draw_paragraph(SkiaStuff& skia)
{
    // Create an SkFont using the loaded typeface
    SkFont font(skia.twemoji, 24); // 24 is the font size in points

    // Set up paint properties
    SkPaint paint;
    paint.setColor(SK_ColorBLACK);

    // The text you want to draw
    const char* text = "Hello World! 🌍🎉🚀";
    size_t textLen = strlen(text);

    // Create a text blob for efficient drawing
    sk_sp<SkTextBlob> textBlob = SkTextBlob::MakeFromText(text, textLen, font);

    // Assume you have an SkCanvas pointer named 'canvas'
    // Set the position where you want to draw the text
    float x = 100;
    float y = 100;

    // Draw the text blob onto the canvas
    skia.canvas->drawTextBlob(textBlob, x, y, paint);
}

void draw_paragraph2(SkiaStuff& skia)
{
    // Define paragraph style
    skia::textlayout::ParagraphStyle paragraphStyle;
    paragraphStyle.setTextAlign(skia::textlayout::TextAlign::kLeft);
    paragraphStyle.setMaxLines(10);

    // Create a ParagraphBuilder
    auto builder = skia::textlayout::ParagraphBuilder::make(paragraphStyle, skia.font_collection);

    // TextStyle for regular text
    skia::textlayout::TextStyle textStyle;
    textStyle.setFontFamilies({skia.roboto_family_name, skia.twemoji_family_name});
    textStyle.setFontSize(40);
    textStyle.setColor(SK_ColorBLACK);

    // TextStyle for emojis
//    skia::textlayout::TextStyle emojiStyle;
//    emojiStyle.setFontFamilies({skia.twemoji_family_name});
//    emojiStyle.setFontSize(24);
//    emojiStyle.setColor(SK_ColorBLACK);

    // Add text with styles
    builder->pushStyle(textStyle);
    builder->addText(u"Hello, 🌍");
    builder->pop();

//    builder->pushStyle(emojiStyle);
//    builder->addText(u"🌍"); // Earth emoji
//    builder->pop();
//
//    builder->pushStyle(textStyle);
//    builder->addText(u" Welcome to Skia!");
//    builder->pop();

    // Build and layout the paragraph
    auto paragraph = builder->Build();
    paragraph->layout(500);

    // Paint the paragraph onto the canvas
    //paragraph->paint(skia.canvas, 50, 100);

    // Save the canvas state
    skia.canvas->save();

    // Apply scaling transformation
    float canvasWidth = skia.canvas->getBaseLayerSize().width();
    float canvasHeight = skia.canvas->getBaseLayerSize().height();
    skia.canvas->translate(canvasWidth / 2, canvasHeight / 2); // Move to canvas center
    skia.canvas->scale(1 + 0.5 * sin(0.001 * SDL_GetTicks()), 1 + 0.5 * sin(0.001 * SDL_GetTicks()));
    skia.canvas->translate(-canvasWidth / 2, -canvasHeight / 2); // Move back

    // Paint the paragraph onto the canvas
    float x = (canvasWidth - paragraph->getMaxWidth()) / 2;
    float y = canvasHeight / 2;
    paragraph->paint(skia.canvas, x, y);

    // Restore the canvas state
    skia.canvas->restore();
}

// endregion

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
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
    auto skia = createSkiaSurface(app_state->w, app_state->h);
    skia.font_collection = sk_make_sp<skia::textlayout::FontCollection>();
    skia.typeface_provider = sk_make_sp<skia::textlayout::TypefaceFontProvider>();

    skia.twemoji = skia.fontMgr->makeFromFile(assets_path"twemoji.ttf");
    skia.twemoji->getFamilyName(&skia.twemoji_family_name);
    skia.typeface_provider->registerTypeface(skia.twemoji);

    skia.roboto = skia.fontMgr->matchFamilyStyle("Roboto", {});
    skia.roboto->getFamilyName(&skia.roboto_family_name);
    skia.typeface_provider->registerTypeface(skia.roboto);

    skia.font_collection->setAssetFontManager(skia.typeface_provider);
    app_state->skia = skia;

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
    //app_state->skia.canvas->clear(SK_ColorBLACK);

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(SK_ColorRED);
    paint.setStyle(SkPaint::kFill_Style);
    //app_state->skia.canvas->drawRect(SkRect::MakeXYWH(200, 150, 400, 300), paint);
    drawRectangleWithDropShadow(app_state->skia.canvas);

    static AnimatedTextWithEmojis emoji_text_scaling_experiment;
    emoji_text_scaling_experiment.canvas_ = app_state->skia.canvas;
    emoji_text_scaling_experiment.fontMgr = app_state->skia.fontMgr;
    //emoji_text_scaling_experiment.Draw(SDL_GetTicks());
    //draw_paragraph(app_state->skia);
    draw_paragraph2(app_state->skia);

    // Flush Skia commands and swap buffers
    app_state->skia.context->flush();

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
