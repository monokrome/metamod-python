/**
 * Metamod Python Extension
 * Copyright(C) 2010, Brandon "monokrome" Stoner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * File: CMetaModPy.h
 * Created on March 31, 2010, 9:35 AM
 **/

#ifndef _CPYTHONMM_H
#define	_CPYTHONMM_H

#include <ISmmPlugin.h>

// Aliases snprintf for Windows systems without it
#if defined WIN32 && !defined snprintf
    #define snprintf _snprintf
#endif

class CMetaModPy : public ISmmPlugin
{
public:
    CMetaModPy();
    ~CMetaModPy();

    bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
    bool Unload(char *error, size_t maxlen);

    bool Pause(char *error, size_t maxlen);
    bool Unpause(char *error, size_t maxlen);

    void AllPluginsLoaded();

    const char *GetAuthor();
    const char *GetName();
    const char *GetDescription();
    const char *GetURL();
    const char *GetLicense();
    const char *GetVersion();
    const char *GetDate();
    const char *GetLogTag();
};

void InitializeHooks();

// This is a global reference to our plugin
extern CMetaModPy g_MetaModPy;

PLUGIN_GLOBALVARS();

#endif	/* _CPYTHONMM_H */

