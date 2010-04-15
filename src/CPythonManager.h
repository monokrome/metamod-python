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
 * File: CPythonManager.h
 * Created on March 27, 2010, 3:04 PM
 **/

#ifndef _PYTHONMANAGER_H
#define	_PYTHONMANAGER_H

#include "CPythonPlugin.h"
#include <vector>

typedef struct
{
    int id;                     // Our plugin's reference ID
    char* name;                 // This plugin's name
    CPythonPlugin* plugin;      // A reference to our CPythonPlugin
    PyThreadState* threadState; // The thread that this plugin exists within
} plugin_t;

class CPythonManager
{
private:
    PyThreadState* threadState;
    std::vector<plugin_t*> m_oPlugins;
    plugin_t* get_next_plugin();

public:
    CPythonManager();
    ~CPythonManager();

    int load(char* name);
    void unload(int plugin_id);
};

extern CPythonManager* g_pythonManager;

#endif	/* _PYTHONMANAGER_H */
