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
 * File: CMetaModPy.cpp
 * Created on March 31, 2010, 9:35 AM
 **/

#include "CPythonManager.h"
#include "CMetaModPy.h"

    // TODO: Can we determine this at runtime?
#define MAXIMUM_PATH_LENGTH 4096

SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int, int);

// This is a global reference to our plugin
CMetaModPy g_MetaModPy;

// This is a reference to our server
IServerGameDLL *server = NULL;

// A basic manager for loading/unloading Python plugins, and secondarily for finding
// loaded plugins.
CPythonManager* g_pythonManager;

// Expose our plugin to the metamod subsystem
PLUGIN_EXPOSE(CMetaModPy, g_MetaModPy);

/**
 *  Called when our plugin is first loaded in the system, during the execution of
 * DLLInit() - prior to initializing the GameDLL.
 **/
bool CMetaModPy::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
    char *directoryBuffer = new char[MAXIMUM_PATH_LENGTH];
    PyObject *loaderModule;

    PLUGIN_SAVEVARS();

    /* Make sure we build on MM:S 1.4 */
#if defined METAMOD_PLAPI_VERSION
    GET_V_IFACE_ANY(GetServerFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
#else
    GET_V_IFACE_ANY(serverFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
#endif

    // Set up our Python environment
    setenv("PYTHONNOUSERSITE", "1", 1);

    g_SMAPI->PathFormat(directoryBuffer, MAXIMUM_PATH_LENGTH, "%s/%s", g_SMAPI->GetBaseDir(), "addons/metamodpy");
    setenv("PYTHONHOME", directoryBuffer, 1);

    g_SMAPI->PathFormat(directoryBuffer, MAXIMUM_PATH_LENGTH, "%s/%s", directoryBuffer, "plugins");
    setenv("PYTHONPATH", directoryBuffer, 1);

    // We don't need this any more.
    delete directoryBuffer;

    // Create our initialize the Python subsystem
    g_pythonManager = new CPythonManager();

    InitializeHooks();

    return true;
}

/**
 * DLLInit() has now been called, and all metamod plugins have been loaded. We
 * can move on to interacting with them.
 **/
void CMetaModPy::AllPluginsLoaded()
{
}

/**
 * Used for any "clean up" needed to properly shut down our plugin
 **/
bool CMetaModPy::Unload(char* error, size_t maxlen)
{
    delete g_pythonManager;
    return true;
}

bool CMetaModPy::Pause(char* error, size_t maxlen)
{
    return true;
}

bool CMetaModPy::Unpause(char* error, size_t maxlen)
{
    return true;
}

CMetaModPy::CMetaModPy()
{
}

CMetaModPy::~CMetaModPy()
{
}

/*************
 *  Describe our plugin.
 *************/
const char *CMetaModPy::GetLicense()
{
	return "Public Domain";
}

const char *CMetaModPy::GetVersion()
{
	return "0.0.1.0";
}

const char *CMetaModPy::GetDate()
{
	return __DATE__;
}

const char *CMetaModPy::GetLogTag()
{
	return "MMPY";
}

const char *CMetaModPy::GetAuthor()
{
	return "Brandon \"monokrome\" Stoner";
}

const char *CMetaModPy::GetDescription()
{
	return "Abstracts the power of SourceHook to Python.";
}

const char *CMetaModPy::GetName()
{
	return "MetaModPy";
}

const char *CMetaModPy::GetURL()
{
	return "http://metamodpy.monokro.me/";
}
