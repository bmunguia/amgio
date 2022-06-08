
int SU2toGMF( char *MshNam, char *SolNam, char *OutNam, char *FldNam );

int GMFtoSU2( char *MshNam, char *SolNam, char *OutNam );

int GMFWithBoundtoSU2( char *MshNam, char *SolNam, char *BndMshNam, char *OutNam );

int GMFSoltoMet( char *MshNam, char *SolNam, char *OutNam );

void ReadMeshAndSol (char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, 
     PyObject *pyEdg, PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol, 
     PyObject *pySolHeader,  PyObject *pyMarkers);

void ReadMesh (char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg, 
     PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers);

void ReadSol (char *SolNam, PyObject *pySol, PyObject *pySolHeader, int NbrVer, int Dim);

void WriteMeshAndSol(char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, 
     PyObject *pyEdg,  PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol, 
     PyObject *pySolHeader, PyObject *pyMarkers, int Dim);

void WriteMesh(char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg,
     PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers, int Dim);

void WriteSol(char *SolNam, PyObject *pyVer, PyObject *pySol, PyObject *pySolHeader, int NbrVer, int Dim);
