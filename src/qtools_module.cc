#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <vector>

#include "transform.h"
#include "base.h"
#include "pricing.h"


std::vector<double> doublevec_from_floatlist(PyObject *fl_list) {
    std::vector<double> v_double = {} ;

    if (!PyList_Check(fl_list)) 
        return v_double;            // return empty vector if input is not a python list

    int list_length = PyObject_Length(fl_list);

    for (int i = 0 ; i < list_length ; i++) {
        PyObject *x;
        x = PyList_GetItem(fl_list, i);
        if (!PyFloat_Check(x))
            v_double.push_back(0);  // if list entry is not a float, 
        else
            v_double.push_back(PyFloat_AsDouble(x));
    }

    return v_double;
}

PyObject * floatlist_from_doublevec(std::vector<double> &v) {
    PyObject * pylist = PyTuple_New(v.size());

    int idx =0;
    for (double x: v) {
        PyObject * item = PyFloat_FromDouble(x);
        PyTuple_SetItem(pylist, idx++, item);
    }
    return pylist;
}

static PyObject* PriceVanillaEuCallWrapper(PyObject *self, PyObject *args) {
    double spot, time_to_expiry, strike, rate, vol, price;
    PyObject *retobj;
    long num_rounds;


    if (!PyArg_ParseTuple(args, "dddddl", &spot, &time_to_expiry, &strike, &rate, &vol, &num_rounds))
        return nullptr;
    price = PriceVanillaEuCall(spot, time_to_expiry, strike, rate, vol, num_rounds);
    retobj = PyFloat_FromDouble(price);
    return retobj;
}

static PyObject* PriceAmericanCallWrapper(PyObject *self, PyObject *args) {
    double spot, time_to_expiry, strike, rate, vol, price;
    PyObject *retobj;
    long tree_depth;


    if (!PyArg_ParseTuple(args, "dddddl", &spot, &time_to_expiry, &strike, &rate, &vol, &tree_depth))
        return nullptr;
    price = PriceAmericanCall(spot, time_to_expiry, strike, rate, vol, tree_depth);
    retobj = PyFloat_FromDouble(price);
    return retobj;
}

static PyObject* PriceAmericanPutWrapper(PyObject *self, PyObject *args) {
    double spot, time_to_expiry, strike, rate, vol, price;
    PyObject *retobj;
    long tree_depth;


    if (!PyArg_ParseTuple(args, "dddddl", &spot, &time_to_expiry, &strike, &rate, &vol, &tree_depth))
        return nullptr;
    price = PriceAmericanPut(spot, time_to_expiry, strike, rate, vol, tree_depth);
    retobj = PyFloat_FromDouble(price);
    return retobj;
}

static PyObject* PriceVanillaEuPutWrapper(PyObject *self, PyObject *args) {
    double spot, time_to_expiry, strike, rate, vol, price;
    PyObject *retobj;
    long num_rounds;


    if (!PyArg_ParseTuple(args, "dddddl", &spot, &time_to_expiry, &strike, &rate, &vol, &num_rounds))
        return nullptr;
    price = PriceVanillaEuPut(spot, time_to_expiry, strike, rate, vol, num_rounds);
    retobj = PyFloat_FromDouble(price);
    return retobj;
}

static PyObject* PriceDigitalEuCallWrapper(PyObject *self, PyObject *args) {
    double spot, time_to_expiry, strike, rate, vol, price;
    PyObject *retobj;
    long num_rounds;


    if (!PyArg_ParseTuple(args, "dddddl", &spot, &time_to_expiry, &strike, &rate, &vol, &num_rounds))
        return nullptr;
    price = PriceDigitalEuCall(spot, time_to_expiry, strike, rate, vol, num_rounds);
    retobj = PyFloat_FromDouble(price);
    return retobj;
}

static PyObject* PriceDigitalEuPutWrapper(PyObject *self, PyObject *args) {
    double spot, time_to_expiry, strike, rate, vol, price;
    PyObject *retobj;
    long num_rounds;


    if (!PyArg_ParseTuple(args, "dddddl", &spot, &time_to_expiry, &strike, &rate, &vol, &num_rounds))
        return nullptr;
    price = PriceDigitalEuPut(spot, time_to_expiry, strike, rate, vol, num_rounds);
    retobj = PyFloat_FromDouble(price);
    return retobj;
}

static PyMethodDef qtools_methods[] = {
    { "PriceVanillaEuCall", PriceVanillaEuCallWrapper, METH_VARARGS, "Price a vanilla European call with simple Monte Carlo" },
    { "PriceVanillaEuPut", PriceVanillaEuPutWrapper, METH_VARARGS, "Price a vanilla European put with simple Monte Carlo" },
    { "PriceDigitalEuCall", PriceDigitalEuCallWrapper, METH_VARARGS, "Price a digital European call with simple Monte Carlo" },
    { "PriceDigitalEuPut", PriceDigitalEuPutWrapper, METH_VARARGS, "Price a digital European put with simple Monte Carlo" },
    { "PriceAmericanCall", PriceAmericanCallWrapper, METH_VARARGS, "Price an American call option with a binomial tree model" },
    { "PriceAmericanPut", PriceAmericanPutWrapper, METH_VARARGS, "Price an American put option with a binomial tree model" },
 { NULL, NULL, 0, NULL }
};

static struct PyModuleDef qtools = {
    PyModuleDef_HEAD_INIT,
    "qtools",
    "Some tools for quantitative finance",
    -1,
    qtools_methods
};

PyMODINIT_FUNC PyInit_qtools(void) {
    Py_Initialize();
    std::cout << "init\n";
    return PyModule_Create(&qtools);
}

