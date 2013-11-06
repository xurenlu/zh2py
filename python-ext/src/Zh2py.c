/*
 *
 */
#include <Python.h>
#include "zh2py.h"

#define DD  printf("got here :%s,%d\n",__FILE__,__LINE__);
typedef struct {
	PyObject_HEAD	
	zh2py_table_root * root;
} Zh2PyIndex;

static PyObject *
Zh2PyIndex_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	DD
    Zh2PyIndex *self;

    self = (Zh2PyIndex *)type->tp_alloc(type, 0);

	self->root = NULL;
    return (PyObject *)self;

}

static int 
Zh2PyIndex_init(Zh2PyIndex *self, PyObject *args, PyObject *kwds)
{

	return 0;
}

static void
Zh2PyIndex_dealloc(Zh2PyIndex *self)
{
}

static PyObject *
Zh2PyIndex_load_table(Zh2PyIndex *self, PyObject *args)
{
	DD
	 char *fpath;
	if (!PyArg_ParseTuple(args, "s", &fpath)){
		PyErr_SetString(PyExc_TypeError, "load dict takes exactly one argument (0 given)");
		return NULL;	
	}

	self->root = zh2py_read_transform_table_file(fpath);
    printf("load table:%s\n",fpath);
	if (self->root==NULL){
		PyErr_SetString(PyExc_TypeError, "load dict takes exactly one argument (0 given)");
		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}
static PyObject * 
Zh2PyIndex_unload_table(Zh2PyIndex *self,PyObject *args){
	DD
 	if(self->root!=NULL){
        if(zh2py_free_table(self->root)!=0){
            return NULL;
        }
        zh2py_res_table_root = NULL;
    }
    Py_INCREF(Py_None);
    return Py_None;
 }

static PyObject *
Zh2PyIndex_get_pinyin(Zh2PyIndex *self, PyObject *args)
{
	DD
	 char * word;
	char * result;
	int len;
	PyObject *op;
	    DD
	if (!PyArg_ParseTuple(args, "s", &word)){
	    DD
		PyErr_SetString(PyExc_TypeError, "add word takes exactly one argument (0 given)");
		return NULL;	
	}
	DD
	result = zh2py_transform(self->root,word,&len);
	DD

	if(result == NULL){
	    DD
		return NULL;
	}else{
	    DD
		op  = calloc(sizeof(PyObject),1);
		op = PyString_FromString(result);
		Py_INCREF(op);
		return op;
	}
	
}


static PyMethodDef Zh2PyIndex_methods[] = {
	{
		"load_table", 
		(PyCFunction)Zh2PyIndex_load_table, 
		METH_VARARGS,
		"load dict from file"
	},
	{
		"unload_table", 
		(PyCFunction)Zh2PyIndex_unload_table, 
		METH_VARARGS,
		"unload table"
	},
	
	{
		"get_pinyin", 
		(PyCFunction)Zh2PyIndex_get_pinyin, 
		METH_VARARGS,
		"get pinyin from word"
	},

	{NULL}  /*  Sentinel */
};


static PyTypeObject Zh2PyIndexType = {
	PyObject_HEAD_INIT(NULL)
	0, /* ob_size*/
	"zh2py.Zh2Py", /* tp_name*/
	sizeof(Zh2PyIndex),           /* tp_basicsize*/
	0, /* tp_itemsize*/
	(destructor)Zh2PyIndex_dealloc, /* tp_dealloc*/
	0, /* tp_print*/
	0, /* tp_getattr*/
	0, /* tp_setattr*/
	0, /* tp_compare*/
	0, /* tp_repr*/
	0, /* tp_as_number*/
	0, /* tp_as_sequence*/
	0, /* tp_as_mapping*/
	0, /* tp_hash */
	0, /* tp_call*/
	0, /* tp_str*/
	0, /* tp_getattro*/
	0, /* tp_setattro*/
	0, /* tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags*/
	"Zh2Py objects",           /*  tp_doc */
	0, /*  tp_traverse */
	0, /*  tp_clear */
	0, /*  tp_richcompare */
	0, /*  tp_weaklistoffset */
	0, /*  tp_iter */
	0, /*  tp_iternext */
	Zh2PyIndex_methods, /*  tp_methods */
	0, /*  tp_members */
	0, /*  tp_getset */
	0, /*  tp_base */
	0, /*  tp_dict */
	0, /*  tp_descr_get */
	0, /*  tp_descr_set */
	0, /*  tp_dictoffset */
	(initproc)Zh2PyIndex_init, /*  tp_init */
	NULL, /*  tp_alloc */
	Zh2PyIndex_new, /*  tp_new */
};

static PyMethodDef zh2py_methods[] = {
	/*
	{
		"filter_keywords", 
		(PyCFunction)FkwDict_full_seg,
		METH_VARARGS,
		"alias to fkw_full_seg, just for fun"
	},
	*/

	{NULL}  /*  Sentinel */
};

#ifndef PyMODINIT_FUNC /*  declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC
initzh2py(void)
{
	PyObject* m;

    if (PyType_Ready(&Zh2PyIndexType) < 0)
        return;

	m = Py_InitModule3("zh2py", zh2py_methods,
			"create filter keywords extension");

	Py_INCREF(&Zh2PyIndexType);
	PyModule_AddObject(m, "Zh2Py", (PyObject *)&Zh2PyIndexType);
}
