
int py_ConvertSU2toGMF( char *MshNam, char *SolNam, char *OutNam, char *FldNam );

int py_ConvertGMFtoSU2( char *MshNam, char *SolNam, char *OutNam );

int py_ConvertGMFWithBoundtoSU2( char *MshNam, char *SolNam, char *BndMshNam, char *OutNam );

int py_ConvertGMFSoltoMet( char *MshNam, char *SolNam, char *OutNam );

void py_ReadMeshAndSol (char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, 
     PyObject *pyEdg, PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol, 
     PyObject *pySolHeader,  PyObject *pyMarkers);

void py_ReadMesh (char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg, 
     PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers);

void py_ReadSol (char *SolNam, PyObject *pySol, PyObject *pySolHeader, int NbrVer, int Dim);

void py_WriteMeshAndSol(char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, 
     PyObject *pyEdg,  PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol, 
     PyObject *pySolHeader, PyObject *pyMarkers, int Dim);

void py_WriteMesh(char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg,
     PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers, int Dim);

void py_WriteSol(char *SolNam, PyObject *pyVer, PyObject *pySol, PyObject *pySolHeader, int NbrVer, int Dim);
