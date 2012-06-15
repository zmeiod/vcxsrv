/*
 * Copyright © 2011 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "glxserver.h"
#include "indirect_dispatch.h"
#include "glxbyteorder.h"
#include "unpack.h"

int
__glXDisp_SetClientInfoARB(__GLXclientState * cl, GLbyte * pc)
{
    xGLXSetClientInfoARBReq *req = (xGLXSetClientInfoARBReq *) pc;
    char *gl_extensions;
    char *glx_extensions;

    /* Verify that the size of the packet matches the size inferred from the
     * sizes specified for the various fields.
     */
    const unsigned expected_size = sz_xGLXSetClientInfoARBReq
        + (req->numVersions * 8)
        + __GLX_PAD(req->numGLExtensionBytes)
        + __GLX_PAD(req->numGLXExtensionBytes);

    if (req->length != (expected_size / 4))
        return BadLength;

    /* Verify that the actual length of the GL extension string matches what's
     * encoded in protocol packet.
     */
    gl_extensions = (char *) (req + 1) + (req->numVersions * 8);
    if (req->numGLExtensionBytes != 0
        && memchr(gl_extensions, 0,
                  __GLX_PAD(req->numGLExtensionBytes)) == NULL)
        return BadLength;

    /* Verify that the actual length of the GLX extension string matches
     * what's encoded in protocol packet.
     */
    glx_extensions = gl_extensions + __GLX_PAD(req->numGLExtensionBytes);
    if (req->numGLXExtensionBytes != 0
        && memchr(glx_extensions, 0,
                  __GLX_PAD(req->numGLXExtensionBytes)) == NULL)
        return BadLength;

    free(cl->GLClientextensions);
    cl->GLClientextensions = strdup(gl_extensions);

    return 0;
}

int
__glXDispSwap_SetClientInfoARB(__GLXclientState * cl, GLbyte * pc)
{
    xGLXSetClientInfoARBReq *req = (xGLXSetClientInfoARBReq *) pc;

    req->length = bswap_16(req->length);
    req->numVersions = bswap_32(req->numVersions);
    req->numGLExtensionBytes = bswap_32(req->numGLExtensionBytes);
    req->numGLXExtensionBytes = bswap_32(req->numGLXExtensionBytes);

    return __glXDisp_SetClientInfoARB(cl, pc);
}

int
__glXDisp_SetClientInfo2ARB(__GLXclientState * cl, GLbyte * pc)
{
    return BadRequest;
}

int
__glXDispSwap_SetClientInfo2ARB(__GLXclientState * cl, GLbyte * pc)
{
    return BadRequest;
}
