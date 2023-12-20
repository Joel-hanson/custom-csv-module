#include <Python.h>

static PyObject *read_csv_file(PyObject *self, PyObject *args)
{
    // Parse the Python arguments: a single string representing the file path
    const char *file_path;
    if (!PyArg_ParseTuple(args, "s", &file_path))
    {
        // PyErr_SetString already sets the error indicator, so no need to return NULL here
        PyErr_SetString(PyExc_TypeError, "Invalid argument. Expected a string.");
        return NULL;
    }

    // Open the CSV file for reading
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        // PyErr_SetFromErrnoWithFilenameObject sets the error indicator, includes the file path,
        // and is preferable for file-related errors
        PyErr_SetFromErrnoWithFilenameObject(PyExc_FileNotFoundError, PyUnicode_DecodeFSDefault(file_path));
        return NULL;
    }

    // Create a Python list to store the rows from the CSV file
    PyObject *result = PyList_New(0);
    if (!result)
    {
        // Handle memory allocation failure
        PyErr_NoMemory();
        fclose(file);
        return NULL;
    }

    // Buffer to store each line read from the CSV file
    char line[4096];

    // Read each line from the CSV file
    while (fgets(line, sizeof(line), file))
    {
        // Create a Python list to store the elements of the current row
        PyObject *row = PyList_New(0);
        if (!row)
        {
            // Handle memory allocation failure
            PyErr_NoMemory();
            fclose(file);
            Py_XDECREF(result);
            return NULL;
        }

        // Tokenize the line using ',' as the delimiter
        char *token = strtok(line, ",");
        while (token != NULL)
        {
            // Calculate the length of the token
            size_t token_len = strlen(token);

            // Strip newline characters from the end of the token
            while (token_len > 0 && (token[token_len - 1] == '\n' || token[token_len - 1] == '\r'))
            {
                token[--token_len] = '\0';
            }

            // Decode the token into a Python Unicode string and append it to the row list
            PyObject *item = PyUnicode_DecodeUTF8(token, token_len, "strict");
            if (!item)
            {
                // Handle decoding failure
                PyErr_SetString(PyExc_UnicodeError, "Failed to decode token");
                fclose(file);
                Py_XDECREF(result);
                Py_XDECREF(row);
                return NULL;
            }

            // Append the item to the row list
            if (PyList_Append(row, item) == -1)
            {
                // Handle list append failure
                PyErr_SetString(PyExc_RuntimeError, "Failed to append item to row");
                fclose(file);
                Py_XDECREF(result);
                Py_XDECREF(row);
                return NULL;
            }

            Py_XDECREF(item);

            // Move to the next token
            token = strtok(NULL, ",");
        }

        // Append the row list to the result list
        if (PyList_Append(result, row) == -1)
        {
            // Handle list append failure
            PyErr_SetString(PyExc_RuntimeError, "Failed to append row to result");
            fclose(file);
            Py_XDECREF(result);
            Py_XDECREF(row);
            return NULL;
        }

        Py_XDECREF(row);
    }

    // Close the CSV file
    fclose(file);

    // Return the final result, which is a Python list of lists representing the CSV data
    return result;
}

// Method definitions
static PyMethodDef customcsv_methods[] = {
    {"read_csv_file", read_csv_file, METH_VARARGS, "Read a CSV file."},
    {NULL, NULL, 0, NULL} // Sentinel
};

// Module definition
static struct PyModuleDef customcsvmodule = {
    PyModuleDef_HEAD_INIT,
    "customcsv", // Module name
    NULL,
    -1,
    customcsv_methods // Method definitions
};

// Module initialization function
PyMODINIT_FUNC PyInit_customcsv(void)
{
    return PyModule_Create(&customcsvmodule);
}
