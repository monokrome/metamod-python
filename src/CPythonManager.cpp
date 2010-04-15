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
 * File: CPythonManager.cpp
 * Created on March 27, 2010, 3:04 PM
 **/

#include <Python.h>
#include <vector>

#include <stdlib.h>

#include "CPythonManager.h"
#include "CPythonPlugin.h"

CPythonManager::CPythonManager()
{
	// Initializes the "global" Python interpretter without unneeded signals.
    if (!Py_IsInitialized())
    {
	printf("Loading Python.\n");
	Py_InitializeEx(0);
	printf("Python loaded.\n");
    }

	// Get the global thread state, and assign it to our reference.
    threadState = PyThreadState_Get();
}

CPythonManager::~CPythonManager()
{
    std::vector<plugin_t*>::iterator itr;

	// Iterate through all plugins and unload each of them.
    for (itr = m_oPlugins.begin(); itr < m_oPlugins.end(); ++itr)
    {
	unload((*itr)->id);
    }

	// Clean up and get rid of our Python interpretter.
    if (Py_IsInitialized())
    {
	printf("Unloading Python.\n");
	Py_Finalize();
	printf("Python unloaded.\n");
    }
}

/**
 * Load a plugin into the system with the given name. The name should be the same
 * as the name of the Python module.
 **/
int CPythonManager::load(char* name)
{
	// Creates and returns a new plugin_t reference to a structure
    plugin_t* this_plugin = get_next_plugin();

	// Create a separate interpreter for this plugin, and activate it
    this_plugin->threadState = Py_NewInterpreter();
    PyThreadState_Swap(this_plugin->threadState);

	// Create our new plugin, and load it into Python
    this_plugin->plugin = new CPythonPlugin();
    this_plugin->plugin->load(name);

	// If the plugin didn't load properly, we delete it. It's useless now.
    if (this_plugin->plugin->is_loaded != true)
    {
	this_plugin->plugin->unload();
	delete this_plugin->plugin;
	delete this_plugin;

	return false;
    }

	// Swap back to the global thread state and await the next command
    PyThreadState_Swap(threadState);

	// Append this plugin to the manager's list of plugins
    m_oPlugins.insert(m_oPlugins.begin(), this_plugin);
    return this_plugin->id;
}

/**
 * Unload a plugin by searching for it's identifier.
 **/
void CPythonManager::unload(int plugin_id)
{
    std::vector<plugin_t*>::iterator itr;

	// Iterate through all plugins in the list, and unload any with the
	// provided plugin ID.
    for (itr = m_oPlugins.begin(); itr < m_oPlugins.end(); ++itr)
    {
	if ((*itr)->id == plugin_id)
	{
	    PyThreadState_Swap((*itr)->threadState);
	    (*itr)->plugin->unload();
	    Py_EndInterpreter((*itr)->threadState);
	    PyThreadState_Swap(threadState);

	    delete (*itr)->plugin;
	    delete (*itr);

	    m_oPlugins.erase(itr);

	    /**
	     * TODO: Check if there is any possibility of having
	     *   multiple plugins with matching IDs. I don't think
	     *   it is, so it's possible that we can modify the
	     *   code to `break` here and save us some processing.
	     **/
	}
    }
}

/***
 * Returns a plugin_t structure with an `id` that has not been used by any
 * plugins yet.
 *
 *     NOTE: Is this possibly unstable? Could we have issues with long-standing
 *           servers? I don't think so, but I want to look into this a bit more.
 ***/
plugin_t* CPythonManager::get_next_plugin()
{
	// Create a pointer to a new plugin object
    plugin_t* next_plugin = new plugin_t;
    std::vector<plugin_t*>::iterator itr;

	// Increment through all current plugins and find the next highest ID
	// for assignign to the new plugin.
    for (itr = m_oPlugins.begin(); itr < m_oPlugins.end(); ++itr)
    {
	if ((*itr)->id > next_plugin->id)
	{
	    next_plugin->id = (*itr)->id + 1;
	}
    }

    return next_plugin;
}
