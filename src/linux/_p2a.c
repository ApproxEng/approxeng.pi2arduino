#include <Python.h>
#include "p2a.h"
#include "pigpio.h"

static char module_docstring[] = "This module handles I2C communication between the Pi and Arduino";

static char sendBytes_docstring[] = "Send some bytes over I2C from the Pi";

static char getBytes_docstring[] = "Get some bytes over I2C from the Arduino";

static PyObject *p2a_sendBytes(PyObject *self, PyObject *args);

static PyObject *p2a_getBytes(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
    {"sendBytes", p2a_sendBytes, METH_VARARGS, sendBytes_docstring},
    {"getBytes", p2a_getBytes, METH_VARARGS, getBytes_docstring},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC init_p2a(void) {
    PyObject *m = Py_InitModule3("_p2a", module_methods, module_docstring);
    if (m == NULL)
        return;
}

static PyObject *p2a_sendBytes(PyObject *self, PyObject *args) {
    int address, length;
    char *buffer;
    if (!PyArg_ParseTuple(args, "is#", &address, &buffer, &length))
        return NULL;
    sendBytes(address, buffer, length);
    return Py_BuildValue("i",0);
}

static PyObject *p2a_getBytes(PyObject *self, PyObject *args) {
    int address, length;
    PyObject *result;
    if (!PyArg_ParseTuple(args, "ii", &address, &length))
        return NULL;
    char *receiveBuffer;
    receiveBuffer = (char *)malloc(length);
    getBytes(address, receiveBuffer, length);
    result = Py_BuildValue("s#", receiveBuffer, length);
    free(receiveBuffer);
    return result;
}