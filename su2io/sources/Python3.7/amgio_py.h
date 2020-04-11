
int py_ConvertSU2toInria( char *MshNam, char *SolNam, char *OutNam );

int py_ConvertInriatoSU2( char *MshNam, char *SolNam, char *OutNam );

int py_ConvertInriaWithBoundtoSU2( char *MshNam, char *SolNam, char *BndMshNam, char *OutNam );

int py_ConvertInriaSoltoMet( char *MshNam, char *SolNam, char *OutNam );

int py_SplitSolution(char *SolNam, int dim, char *prefix, char *adap_sensor);

void py_ReadMeshAndSol (char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, 
     PyObject *pyEdg, PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol, 
     PyObject *pySolHeader,  PyObject *pyMarkers);

void py_ReadMesh (char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg, 
     PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers);

void py_ReadSol (char *SolNam, PyObject *pySol, PyObject *pySolHeader, int NbrVer);

void py_WriteMeshAndSol(char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, 
     PyObject *pyEdg,  PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol, 
     PyObject *pySolHeader, PyObject *pyMarkers, int Dim);

void py_WriteMesh(char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg,
     PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers, int Dim);

void py_WriteSol(char *SolNam, PyObject *pyVer, PyObject *pySol, PyObject *pySolHeader, int NbrVer, int Dim);
