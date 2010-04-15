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
 * File: CPythonPlugin.cpp
 * Created on March 27, 2010, 3:35 PM
 **/

#include <Python.h>
#include "CPythonPlugin.h"

CPythonPlugin::CPythonPlugin()
{
    is_loaded = false;
}

CPythonPlugin::~CPythonPlugin()
{
    if (is_loaded)
    {
	unload();
    }
}

/**
 *  Attempts to load/import a Python module and returns true if successful.
 **/
bool CPythonPlugin::load(char* name)
{
    PyObject *moduleName;    // Stores the name of our module as a Python string
    is_loaded = true;

    moduleName = PyString_FromString(name);

    if (moduleName == NULL)
    {
	PyErr_Print();
	is_loaded = false;
	return false;
    }

    m_oModule = PyImport_Import(moduleName);
    Py_DECREF(moduleName);

    if (m_oModule == NULL)
    {
	PyErr_Print();
	is_loaded = false;
	return false;
    }

    return is_loaded;
}

void CPythonPlugin::unload()
{
    if (m_oModule != NULL)
    {
	Py_DECREF(m_oModule);
	m_oModule = NULL;
    }

    is_loaded = false;
}
