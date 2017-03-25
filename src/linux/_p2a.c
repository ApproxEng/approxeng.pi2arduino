#include <Python.h>
#include "p2a.h"
#include "pigpio.h"

static char module_docstring[] = "This module handles I2C communication between the Pi and Arduino";
static char sendBytes_docstring[] = "Send some bytes over I2C from the Pi";
static char getBytes_docstring[] = "Get some bytes over I2C from the Arduino";
static char init_docstring[] = "Call gpioInitialise on the underlying library";
static char writeByteData_docstring[] = "Call SMBUS writeByteData(address, register, byteValue)";
static char writeBlockData_docstring[] = "Call SMBUS writeBlockData(address, register, bytes)";

static PyObject *p2a_sendBytes(PyObject *self, PyObject *args);

static PyObject *p2a_getBytes(PyObject *self, PyObject *args);

static PyObject *p2a_init(PyObject *self, PyObject *args);

static PyObject *p2a_writeByteData(PyObject *self, PyObject *args);

static PyObject *p2a_writeBlockData(PyObject *self, PyObject *args);

static PyMethodDef module_methods[] = {
        {"sendBytes",      p2a_sendBytes,      METH_VARARGS, sendBytes_docstring},
        {"getBytes",       p2a_getBytes,       METH_VARARGS, getBytes_docstring},
        {"init",           p2a_init,           METH_VARARGS, init_docstring},
        {"writeByteData",  p2a_writeByteData,  METH_VARARGS, writeByteData_docstring},
        {"writeBlockData", p2a_writeBlockData, METH_VARARGS, writeBlockData_docstring},
        {NULL, NULL,                           0, NULL}
};

PyMODINIT_FUNC initp2a(void) {
    PyObject *m = Py_InitModule3("_p2a", module_methods, module_docstring);
    if (m == NULL)
        return;
}

/**
 * Send a string of bytes to the I2C device using the function in p2a.c
 */
static PyObject *p2a_sendBytes(PyObject *self, PyObject *args) {
    unsigned address, length;
    char *buffer;
    if (!PyArg_ParseTuple(args, "is#", &address, &buffer, &length))
        return NULL;
    sendBytes(address, buffer, length);
    Py_INCREF(Py_None);
    return Py_None;
}

/**
 * Get a string of bytes from the specified device using the function in p2a.c
 */
static PyObject *p2a_getBytes(PyObject *self, PyObject *args) {
    unsigned address, length;
    PyObject *result;
    if (!PyArg_ParseTuple(args, "ii", &address, &length))
        return NULL;
    char *receiveBuffer;
    receiveBuffer = (char *) malloc(length);
    getBytes(address, receiveBuffer, length);
    result = Py_BuildValue("s#", receiveBuffer, length);
    free(receiveBuffer);
    return result;
}

/**
 * Initialise the pigpio library
 */
static PyObject *p2a_init(PyObject *self, PyObject *args) {
    gpioInitialise();
    Py_INCREF(Py_None);
    return Py_None;
}

/**
 * Call SMBUS i2cWriteByteData
 */
static PyObject *p2a_writeByteData(PyObject *self, PyObject *args) {
    unsigned address, reg, value;
    if (!PyArg_ParseTuple(args, "iii", &address, &reg, &value))
        return NULL;
    unsigned handle = (unsigned) i2cOpen(1, address, 0);
    i2cWriteByteData(handle, reg, value);
    i2cClose(handle);
    Py_INCREF(Py_None);
    return Py_None;
}

/**
 * Call SMBUS i2cWriteBlockData
 */
static PyObject *p2a_writeBlockData(PyObject *self, PyObject *args) {
    unsigned address, reg, length;
    char *buffer;
    if (!PyArg_ParseTuple(args, "iis#", &address, &reg, &buffer, &length))
        return NULL;
    unsigned handle = (unsigned) i2cOpen(1, address, 0);
    i2cWriteBlockData(handle, reg, buffer, length);
    i2cClose(handle);
    Py_INCREF(Py_None);
    return Py_None;
}