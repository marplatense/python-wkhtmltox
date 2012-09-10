#include "Python.h"
#include "structmember.h"
#include "wkhtmltox/pdf.h"

// Web page specific settings
typedef struct {
    PyObject_HEAD
    PyObject *background; // Should we print the background? Must be either "true" or "false".
    PyObject *loadImages; // Should we load images? Must be either "true" or "false".
    PyObject *enableJavaScript; // Should we enable javascript? Must be either "true" or "false".
    PyObject *enableIntelligentShrinking; // Should we enable intelligent shrinkng to fit more content on one page? Must be either "true" or "false". Has no effect for wkhtmltoimage.
    int minimumFontSize; // The minimum font size allowed. E.g. "9"
    PyObject *printMediaType; // Should the content be printed using the print media type instead of the screen media type. Must be either "true" or "false". Has no effect for wkhtmltoimage.
    PyObject *defaultEncoding; // What encoding should we guess content is using if they do not specify it propperly? E.g. "utf-8"
    PyObject *userStyleSheet; // Url er path to a user specified style sheet.
    PyObject *enablePlugins; // Should we enable NS plugins, must be either "true" or "false". Enabling this will have limited success.
} wkhtmltox_WebObject;

static void
wkhtmltox_WebObject_dealloc(wkhtmltox_WebObject* self)
{
    Py_XDECREF(self->background);
    Py_XDECREF(self->loadImages);
    Py_XDECREF(self->enableJavaScript);
    Py_XDECREF(self->enableIntelligentShrinking);
    Py_XDECREF(self->printMediaType);
    Py_XDECREF(self->defaultEncoding);
    Py_XDECREF(self->userStyleSheet);
    Py_XDECREF(self->enablePlugins);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
wkhtmltox_WebObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    wkhtmltox_WebObject *self;
    self = (wkhtmltox_WebObject *)type->tp_alloc(type, 0);
    if (self!=NULL) {
        self->background = Py_False;
        if (self->background == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->loadImages = Py_False;
        if (self->loadImages == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->enableJavaScript = Py_False;
        if (self->enableJavaScript == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->enableIntelligentShrinking = Py_False;
        if (self->enableIntelligentShrinking == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->minimumFontSize = PyInt_FromSize_t(9);
        if (self->minimumFontSize == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->printMediaType = Py_False;
        if (self->printMediaType == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->defaultEncoding = PyString_FromString("utf-8");
        if (self->defaultEncoding == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->userStyleSheet = PyString_FromString("");
        if (self->userStyleSheet == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
        self->enablePlugins = Py_False;
        if (self->enablePlugins == NULL)
        {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject *)self;
}

static int
wkhtmltox_WebObject_init(wkhtmltox_WebObject *self, PyObject *args, PyObject *kwds)
{

    PyObject *background, *loadImages, 
             *enableJavaScript, *enableIntelligentShrinking,
             *printMediaType, *enablePlugins,
             *defaultEncoding, *userStyleSheet=NULL, *tmp;

    static char *kwlist[] = {"background", "loadImages", "enableJavascript",
                             "enableIntelligentShrinking", "minimumFontSize",
                             "printMediaType", "defaultEncoding", 
                             "userStyleSheet", "enablePlugins", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|iiiiiissi", kwlist,
                                     &background,
                                     &loadImages,
                                     &enableJavaScript,
                                     &enableIntelligentShrinking,
                                     &self->minimumFontSize,
                                     &printMediaType,
                                     &defaultEncoding,
                                     &userStyleSheet,
                                     &enablePlugins))
       return -1;
    if (background) {
       tmp = self->background;
       Py_INCREF(background);
       self->background = background;
       Py_XDECREF(tmp);
    }
    if (loadImages) {
       tmp = self->loadImages;
       Py_INCREF(loadImages);
       self->loadImages = loadImages;
       Py_XDECREF(tmp);
    }
    if (enableJavaScript) {
       tmp = self->enableJavaScript;
       Py_INCREF(enableJavaScript);
       self->enableJavaScript = enableJavaScript;
       Py_XDECREF(tmp);
    }
    if (enableIntelligentShrinking) {
       tmp = self->enableIntelligentShrinking;
       Py_INCREF(enableIntelligentShrinking);
       self->enableIntelligentShrinking = enableIntelligentShrinking;
       Py_XDECREF(tmp);
    }
    if (printMediaType) {
       tmp = self->printMediaType;
       Py_INCREF(printMediaType);
       self->printMediaType = printMediaType;
       Py_XDECREF(tmp);
    }
    if (defaultEncoding) {
       tmp = self->defaultEncoding;
       Py_INCREF(defaultEncoding);
       self->defaultEncoding = defaultEncoding;
       Py_XDECREF(tmp);
    }
    if (userStyleSheet) {
       tmp = self->userStyleSheet;
       Py_INCREF(userStyleSheet);
       self->userStyleSheet = userStyleSheet;
       Py_XDECREF(tmp);
    }
    if (enablePlugins) {
       tmp = self->enablePlugins;
       Py_INCREF(enablePlugins);
       self->enablePlugins = enablePlugins;
       Py_XDECREF(tmp);
    }
    return 0;
}

static PyMemberDef wkhtmltox_WebObject_members[] = {
    {"background", T_OBJECT_EX, offsetof(wkhtmltox_WebObject, background), 0,
     "Boolean to indicate if the background should be printed. Default False"},
    {"loadImages", T_OBJECT_EX, offsetof(wkhtmltox_WebObject, loadImages), 0,
     "Boolean to indicate if images should be loaded. Default False"},
    {"enableJavaScript", T_OBJECT_EX, offsetof(wkhtmltox_WebObject, enableJavaScript), 0,
     "Boolean to indicate if javascript should be enabled. Default False"},
    {"enableIntelligentShrinking", T_OBJECT_EX, offsetof(wkhtmltox_WebObject, enableIntelligentShrinking), 0,
     "Boolean to indicate if intelligent shrinking should be enable in order to fit more content in one page. Default False. It has no effect for wkhtmltoimage"},
    {"minimumFontSize", T_INT, offsetof(wkhtmltox_WebObject, minimumFontSize), 0,
     "The minimum font size allowed. Default 9"},
    {"printMediaType", T_OBJECT_EX, offsetof(wkhtmltox_WebObject, printMediaType), 0,
     "Boolean to indicate if the content should be printed using the print media type instead of the screen media type. Default False"},
    {"defaultEncoding", T_OBJECT_EX, offsetof(wkhtmltox_WebObject, defaultEncoding), 0,
     "Encoding to be use to guess what encoding the content is using in case it is not specified properly. Defaul utf-8"},
    {"userStyleSheet", T_OBJECT_EX, offsetof(wkhtmltox_WebObject, userStyleSheet), 0,
     "URL or path to a user specified style sheet"},
    {"enablePlugins", T_OBJECT_EX, offsetof(wkhtmltox_WebObject, enablePlugins), 0,
     "Boolean ro indicate if NS plugins should be enabled. Default False. Enabling this will have limited success"},
    {NULL}
};

static PyMethodDef wkhtmltox_WebObject_methods[] = {
            {NULL}  /* Sentinel */
};

static PyTypeObject wkhtmltox_WebType = {
    PyObject_HEAD_INIT(NULL)
    0,                                                  /*ob_size*/
    "wkhtmltox.web",                                    /*tp_name*/
    sizeof(wkhtmltox_WebObject),                        /*tp_basicsize*/
    0,                                                  /*tp_itemsize*/
    (destructor)wkhtmltox_WebObject_dealloc,            /*tp_dealloc*/
    0,                                                  /*tp_print*/
    0,                                                  /*tp_getattr*/
    0,                                                  /*tp_setattr*/
    0,                                                  /*tp_compare*/
    0,                                                  /*tp_repr*/
    0,                                                  /*tp_as_number*/
    0,                                                  /*tp_as_sequence*/
    0,                                                  /*tp_as_mapping*/
    0,                                                  /*tp_hash */
    0,                                                  /*tp_call*/
    0,                                                  /*tp_str*/
    0,                                                  /*tp_getattro*/
    0,                                                  /*tp_setattro*/
    0,                                                  /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,           /*tp_flags*/
    "Web page specific settings",                       /*tp_doc */
    0,                                                  /*tp_traverse */
    0,                                                  /*tp_clear */
    0,                                                  /*tp_richcompare */
    0,                                                  /*tp_weaklistoffset */
    0,                                                  /*tp_iter */
    0,                                                  /*tp_iternext */
    wkhtmltox_WebObject_methods,                        /*tp_methods */
    wkhtmltox_WebObject_members,                        /*tp_members */
    0,                                                  /*tp_getset */
    0,                                                  /*tp_base */
    0,                                                  /*tp_dict */
    0,                                                  /*tp_descr_get */
    0,                                                  /*tp_descr_set */
    0,                                                  /*tp_dictoffset */
    (initproc)wkhtmltox_WebObject_init,                 /*tp_init */
    0,                                                  /*tp_alloc */
    wkhtmltox_WebObject_new,                            /*tp_new */
};

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initwkhtmltox(void) 
{
    PyObject* m;

    if (PyType_Ready(&wkhtmltox_WebType) < 0)
        return;

    m = Py_InitModule3("wkhtmltox", wkhtmltox_WebObject_methods,
                       "Example module that creates an extension type.");

    if (m == NULL) {
        return;
    }
    Py_INCREF(&wkhtmltox_WebType);
    PyModule_AddObject(m, "Web", (PyObject *)&wkhtmltox_WebType);
}
