// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package main

const ASSEMBLE_INTERFACE_GL_ES = `/*
 * Copyright 2019 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * THIS FILE IS AUTOGENERATED
 * Make edits to tools/gpu/gl/interface/templates.go or they will
 * be overwritten.
 */

#include "include/core/SkRefCnt.h"
#include "include/gpu/ganesh/gl/GrGLAssembleHelpers.h"
#include "include/gpu/ganesh/gl/GrGLAssembleInterface.h"
#include "include/gpu/ganesh/gl/GrGLExtensions.h"
#include "include/gpu/ganesh/gl/GrGLFunctions.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/gl/GrGLTypes.h"
#include "src/gpu/ganesh/gl/GrGLDefines.h"
#include "src/gpu/ganesh/gl/GrGLUtil.h"

#define GET_PROC(F) functions->f##F = (GrGL##F##Fn*)get(ctx, "gl" #F)
#define GET_PROC_SUFFIX(F, S) functions->f##F = (GrGL##F##Fn*)get(ctx, "gl" #F #S)
#define GET_PROC_LOCAL(F) GrGL##F##Fn* F = (GrGL##F##Fn*)get(ctx, "gl" #F)

#define GET_EGL_PROC_SUFFIX(F, S) functions->fEGL##F = (GrEGL##F##Fn*)get(ctx, "egl" #F #S)

#if SK_DISABLE_GL_ES_INTERFACE
sk_sp<const GrGLInterface> GrGLMakeAssembledGLESInterface(void *ctx, GrGLGetProc get) {
    return nullptr;
}
#else
sk_sp<const GrGLInterface> GrGLMakeAssembledGLESInterface(void *ctx, GrGLGetProc get) {
    GET_PROC_LOCAL(GetString);
    if (nullptr == GetString) {
        return nullptr;
    }

    const char* verStr = reinterpret_cast<const char*>(GetString(GR_GL_VERSION));
    GrGLVersion glVer = GrGLGetVersionFromString(verStr);

    if (glVer < GR_GL_VER(2,0)) {
        return nullptr;
    }

    GET_PROC_LOCAL(GetIntegerv);
    GET_PROC_LOCAL(GetStringi);
    GrEGLQueryStringFn* queryString;
    GrEGLDisplay display;
    GrGetEGLQueryAndDisplay(&queryString, &display, ctx, get);
    GrGLExtensions extensions;
    if (!extensions.init(kGLES_GrGLStandard, GetString, GetStringi, GetIntegerv, queryString,
                         display)) {
        return nullptr;
    }

    sk_sp<GrGLInterface> interface(new GrGLInterface);
    GrGLInterface::Functions* functions = &interface->fFunctions;

    // Autogenerated content follows
[[content]]
    // End autogenerated content
    // TODO(kjlubick): Do we want a feature that removes the extension if it doesn't have
    // the function? This is common on some low-end GPUs.

    if (extensions.has("GL_KHR_debug")) {
        // In general we have a policy against removing extension strings when the driver does
        // not provide function pointers for an advertised extension. However, because there is a
        // known device that advertises GL_KHR_debug but fails to provide the functions and this is
        // a debugging- only extension we've made an exception. This also can happen when using
        // APITRACE.
        if (!interface->fFunctions.fDebugMessageControl) {
            extensions.remove("GL_KHR_debug");
        }
    }
    interface->fStandard = kGLES_GrGLStandard;
    interface->fExtensions.swap(&extensions);

    return interface;
}
#endif
`

const ASSEMBLE_INTERFACE_GL = `/*
 * Copyright 2019 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * THIS FILE IS AUTOGENERATED
 * Make edits to tools/gpu/gl/interface/templates.go or they will
 * be overwritten.
 */

#include "include/core/SkRefCnt.h"
#include "include/gpu/ganesh/gl/GrGLAssembleHelpers.h"
#include "include/gpu/ganesh/gl/GrGLAssembleInterface.h"
#include "include/gpu/ganesh/gl/GrGLExtensions.h"
#include "include/gpu/ganesh/gl/GrGLFunctions.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/gl/GrGLTypes.h"
#include "src/gpu/ganesh/gl/GrGLDefines.h"
#include "src/gpu/ganesh/gl/GrGLUtil.h"

#define GET_PROC(F) functions->f##F = (GrGL##F##Fn*)get(ctx, "gl" #F)
#define GET_PROC_SUFFIX(F, S) functions->f##F = (GrGL##F##Fn*)get(ctx, "gl" #F #S)
#define GET_PROC_LOCAL(F) GrGL##F##Fn* F = (GrGL##F##Fn*)get(ctx, "gl" #F)

#define GET_EGL_PROC_SUFFIX(F, S) functions->fEGL##F = (GrEGL##F##Fn*)get(ctx, "egl" #F #S)

#if SK_DISABLE_GL_INTERFACE
sk_sp<const GrGLInterface> GrGLMakeAssembledGLInterface(void *ctx, GrGLGetProc get) {
    return nullptr;
}
#else
sk_sp<const GrGLInterface> GrGLMakeAssembledGLInterface(void *ctx, GrGLGetProc get) {
    GET_PROC_LOCAL(GetString);
    GET_PROC_LOCAL(GetStringi);
    GET_PROC_LOCAL(GetIntegerv);

    // GetStringi may be nullptr depending on the GL version.
    if (nullptr == GetString || nullptr == GetIntegerv) {
        return nullptr;
    }

    const char* versionString = (const char*) GetString(GR_GL_VERSION);
    GrGLVersion glVer = GrGLGetVersionFromString(versionString);

    if (glVer < GR_GL_VER(2,0) || GR_GL_INVALID_VER == glVer) {
        // This is our minimum for non-ES GL.
        return nullptr;
    }

    GrEGLQueryStringFn* queryString;
    GrEGLDisplay display;
    GrGetEGLQueryAndDisplay(&queryString, &display, ctx, get);
    GrGLExtensions extensions;
    if (!extensions.init(kGL_GrGLStandard, GetString, GetStringi, GetIntegerv, queryString,
                         display)) {
        return nullptr;
    }

    sk_sp<GrGLInterface> interface(new GrGLInterface());
    GrGLInterface::Functions* functions = &interface->fFunctions;

    // Autogenerated content follows
[[content]]
    // End autogenerated content
    interface->fStandard = kGL_GrGLStandard;
    interface->fExtensions.swap(&extensions);

    return interface;
}
#endif
`

const ASSEMBLE_INTERFACE_WEBGL = `/*
 * Copyright 2019 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * THIS FILE IS AUTOGENERATED
 * Make edits to tools/gpu/gl/interface/templates.go or they will
 * be overwritten.
 */
#include "include/core/SkRefCnt.h"
#include "include/gpu/ganesh/gl/GrGLAssembleInterface.h"

#if SK_DISABLE_WEBGL_INTERFACE || !defined(__EMSCRIPTEN__)
struct GrGLInterface;
sk_sp<const GrGLInterface> GrGLMakeAssembledWebGLInterface(void *ctx, GrGLGetProc get) {
    return nullptr;
}
#else

#include "include/gpu/ganesh/gl/GrGLAssembleHelpers.h"
#include "src/gpu/ganesh/gl/GrGLUtil.h"

// Located https://github.com/emscripten-core/emscripten/tree/7ba7700902c46734987585409502f3c63beb650f/system/include/webgl
#include <webgl/webgl1.h>
#include <webgl/webgl1_ext.h>
#include <webgl/webgl2.h>
#include <webgl/webgl2_ext.h>

#define GET_PROC(F) functions->f##F = emscripten_gl##F
#define GET_PROC_SUFFIX(F, S) functions->f##F = emscripten_gl##F##S

sk_sp<const GrGLInterface> GrGLMakeAssembledWebGLInterface(void *ctx, GrGLGetProc get) {
    const char* verStr = reinterpret_cast<const char*>(glGetString(GR_GL_VERSION));
    GrGLVersion glVer = GrGLGetVersionFromString(verStr);
    if (glVer < GR_GL_VER(1,0)) {
        return nullptr;
    }

    GrGLExtensions extensions;
    if (!extensions.init(kWebGL_GrGLStandard, glGetString, glGetStringi, glGetIntegerv)) {
        return nullptr;
    }

    sk_sp<GrGLInterface> interface(new GrGLInterface);
    GrGLInterface::Functions* functions = &interface->fFunctions;

    // Autogenerated content follows
[[content]]
    // End autogenerated content

    interface->fStandard = kWebGL_GrGLStandard;
    interface->fExtensions.swap(&extensions);

    return interface;
}
#endif
`

const VALIDATE_INTERFACE = `/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * THIS FILE IS AUTOGENERATED
 * Make edits to tools/gpu/gl/interface/templates.go or they will
 * be overwritten.
 */

#include "include/gpu/ganesh/gl/GrGLConfig.h"
#include "include/gpu/ganesh/gl/GrGLExtensions.h"
#include "include/gpu/ganesh/gl/GrGLFunctions.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/gl/GrGLTypes.h"
#include "include/private/base/SkDebug.h"
#include "src/gpu/ganesh/gl/GrGLDefines.h"
#include "src/gpu/ganesh/gl/GrGLUtil.h"

GrGLInterface::GrGLInterface() {
    fStandard = kNone_GrGLStandard;
}

#if GR_GL_CHECK_ERROR
static const char* get_error_string(GrGLenum err) {
    switch (err) {
        case GR_GL_NO_ERROR:
            return "";
        case GR_GL_INVALID_ENUM:
            return "Invalid Enum";
        case GR_GL_INVALID_VALUE:
            return "Invalid Value";
        case GR_GL_INVALID_OPERATION:
            return "Invalid Operation";
        case GR_GL_OUT_OF_MEMORY:
            return "Out of Memory";
        case GR_GL_CONTEXT_LOST:
            return "Context Lost";
    }
    return "Unknown";
}

GrGLenum GrGLInterface::checkError(const char* location, const char* call) const {
    GrGLenum error = fFunctions.fGetError();
    if (error != GR_GL_NO_ERROR && !fSuppressErrorLogging) {
        SkDebugf("---- glGetError 0x%x(%s)", error, get_error_string(error));
        if (location) {
            SkDebugf(" at\n\t%s", location);
        }
        if (call) {
            SkDebugf("\n\t\t%s", call);
        }
        SkDebugf("\n");
        if (error == GR_GL_OUT_OF_MEMORY) {
            fOOMed = true;
        }
    }
    return error;
}

bool GrGLInterface::checkAndResetOOMed() const {
    if (fOOMed) {
        fOOMed = false;
        return true;
    }
    return false;
}

void GrGLInterface::suppressErrorLogging() { fSuppressErrorLogging = true; }
#endif

#define RETURN_FALSE_INTERFACE                                                 \
    SkDEBUGF("%s:%d GrGLInterface::validate() failed.\n", __FILE__, __LINE__); \
    return false

bool GrGLInterface::validate() const {

    if (kNone_GrGLStandard == fStandard) {
        RETURN_FALSE_INTERFACE;
    }

    if (!fExtensions.isInitialized()) {
        RETURN_FALSE_INTERFACE;
    }

    GrGLVersion glVer = GrGLGetVersion(this);
    if (GR_GL_INVALID_VER == glVer) {
        RETURN_FALSE_INTERFACE;
    }
    // Autogenerated content follows
[[content]]
    // End autogenerated content
    return true;
}

#if defined(GPU_TEST_UTILS)

void GrGLInterface::abandon() const {
    const_cast<GrGLInterface*>(this)->fFunctions = GrGLInterface::Functions();
}

#endif // defined(GPU_TEST_UTILS)
`
