#include <stdlib.h>
#include <iostream>
#include <vector>
#include <Python.h>
#include "math.h"

void generate_data(std::vector<double> &xvec, std::vector<double> &yvec)
{
    long N = 1000000;
    for (double t = 0; t < 2*M_PI; t+= 2*M_PI/(N-1))
    {
        xvec.push_back(t);
        yvec.push_back(sin(t*4)*exp(-0.7*t));
    }
}
PyObject *vec_to_object(std::vector<double> &x)
{
    PyObject *pXVec = PyList_New(0);

    for (std::size_t i = 0; i < x.size(); ++i)
    {
        PyObject *pValue = PyFloat_FromDouble(x[i]);
        if (!pValue) { fprintf(stderr, "Cannot convert array value\n"); }
        PyList_Append(pXVec, pValue);
    }
    return pXVec;
}
int main(int argc, char * argv[]) {

	PyObject *pArgTuple, *pXVec, *pYVec;

	Py_Initialize();

    // Import the plotting stuff
	PyObject *plt = PyImport_ImportModule("matplotlib.pyplot");
    PyObject *plt_plot = PyObject_GetAttrString(plt, "plot");
    PyObject *plt_show = PyObject_GetAttrString(plt, "show");

    pArgTuple = PyTuple_New(2);

    std::vector<double> xvec, yvec;
    generate_data(xvec,yvec);
    pXVec = vec_to_object(xvec);
    pYVec = vec_to_object(yvec);

    //Set the argument tuple to contain the two input tuples
    PyTuple_SetItem(pArgTuple, 0, pXVec);
    PyTuple_SetItem(pArgTuple, 1, pYVec);
    PyObject_CallObject(plt_plot, pArgTuple);
    PyObject_CallObject(plt_show, NULL);

    // Cleanup
    Py_DECREF(pXVec);
    Py_DECREF(pYVec);
    Py_DECREF(pArgTuple);
    Py_DECREF(plt);
    Py_DECREF(plt_plot);
    Py_DECREF(plt_show);

    Py_Finalize();

	return EXIT_SUCCESS;
}
