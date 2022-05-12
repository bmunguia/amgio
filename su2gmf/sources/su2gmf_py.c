#include "su2gmf.h"
#include "Python.h"

int py_ConvertSU2toGMF( char *MshNam, char *SolNam, char *OutNam, char *FldNam  ) 
{
  
  Options *mshopt = AllocOptions();
  
  strcpy(mshopt->OutNam,OutNam);
  strcpy(mshopt->InpNam,MshNam);
  strcpy(mshopt->SolNam,SolNam);
  strcpy(mshopt->FldNam,FldNam);
  
  mshopt->clean = 0; // remove unconnected vertices
  
  if ( !CheckOptions(mshopt) ) {
    return 0;
  }
  
  return ConvertSU2SolToGMF (mshopt);
  
}

int py_ConvertGMFtoSU2( char *MshNam, char *SolNam, char *OutNam ) 
{
  
  Options *mshopt = AllocOptions();
  
  strcpy(mshopt->OutNam,OutNam);
  strcpy(mshopt->InpNam,MshNam);
  strcpy(mshopt->SolNam,SolNam);
  
  mshopt->clean = 0; // remove unconnected vertices
  
  if ( !CheckOptions(mshopt) ) {
    return 0;
  }
  
  return ConvertGMFtoSU2Sol (mshopt);
  
}

int py_ConvertGMFWithBoundtoSU2( char *MshNam, char *SolNam, char *BndMshNam, char *OutNam ) 
{
  
  Options *mshopt = AllocOptions();
  
  strcpy(mshopt->OutNam,OutNam);
  strcpy(mshopt->InpNam,MshNam);
  strcpy(mshopt->SolNam,SolNam);
  
  mshopt->clean = 0; // remove unconnected vertices
  
  if ( !CheckOptions(mshopt) ) {
    return 0;
  }
  
  return ConvertGMFWithBoundtoSU2Sol (mshopt, BndMshNam);
    
}

int py_ConvertGMFSoltoMet( char *MshNam, char *SolNam, char *OutNam ) 
{
  
  Options *mshopt = AllocOptions();
  
  strcpy(mshopt->InpNam,MshNam);
  strcpy(mshopt->OutNam,OutNam);
  strcpy(mshopt->SolNam,SolNam);
  
  mshopt->clean = 0; // remove unconnected vertices
  
  if ( !CheckOptions(mshopt) ) {
    return 0;
  }
  
  return ConvertGMFSoltoMet (mshopt);
    
}

void py_ReadMeshAndSol (char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, 
                        PyObject *pyEdg, PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol, 
                        PyObject *pySolHeader,  PyObject *pyMarkers)
{
  int i, j, d;
  
  Options *mshopt = AllocOptions();
  
  strcpy(mshopt->InpNam,MshNam);
  strcpy(mshopt->SolNam,SolNam);
  
  //--- Open mesh/solution file
  
  Mesh *Msh = NULL;
  Msh = SetupMeshAndSolution (mshopt->InpNam, mshopt->SolNam);
  
  for (i=1; i<=Msh->NbrVer; i++){
    for (d=0; d<3; d++)
      PyList_Append(pyVer, PyFloat_FromDouble(Msh->Ver[i][d]));
  }

  for (i=1; i<=Msh->NbrCor; i++){
    PyList_Append(pyCor, PyLong_FromLong(Msh->Cor[i]));
  }
  
  for (i=1; i<=Msh->NbrTri; i++){
    for (j=0; j<4; j++)
      PyList_Append(pyTri, PyLong_FromLong(Msh->Tri[i][j]));
  }
  
  for (i=1; i<=Msh->NbrTet; i++){
    for (j=0; j<5; j++)
      PyList_Append(pyTet, PyLong_FromLong(Msh->Tet[i][j]));
  }
  
  for (i=1; i<=Msh->NbrEfr; i++){
    for (j=0; j<3; j++)
      PyList_Append(pyEdg, PyLong_FromLong(Msh->Efr[i][j]));
  }
  
  for (i=1; i<=Msh->NbrHex; i++){
    for (j=0; j<9; j++)
      PyList_Append(pyHex, PyLong_FromLong(Msh->Hex[i][j]));
  }
  
  for (i=1; i<=Msh->NbrQua; i++){
    for (j=0; j<5; j++)
      PyList_Append(pyQua, PyLong_FromLong(Msh->Qua[i][j]));
  }
  
  for (i=1; i<=Msh->NbrPyr; i++){
    for (j=0; j<6; j++)
      PyList_Append(pyPyr, PyLong_FromLong(Msh->Pyr[i][j]));
  }
  
  for (i=1; i<=Msh->NbrPri; i++){
    for (j=0; j<7; j++)
      PyList_Append(pyPri, PyLong_FromLong(Msh->Pri[i][j]));
  }
  
  //--- First row of Markers contains dimension
  PyList_Append(pyMarkers, PyLong_FromLong(Msh->Dim));
  for (i=1; i<=Msh->NbrMarkers; i++){
    PyList_Append(pyMarkers, PyUnicode_FromString(Msh->Markers[i]));
  }
  
  for (i=0; i<Msh->SolSiz; i++){
    PyList_Append(pySolHeader, PyUnicode_FromString(Msh->SolTag[i]));
  }
  
  if ( Msh->Sol ) {
    
    //--- Output solution
    int iVer;
    for (iVer=1; iVer<=Msh->NbrVer; iVer++) {
      for (i=0; i<Msh->SolSiz; i++) {
        PyList_Append(pySol, PyFloat_FromDouble(Msh->Sol[iVer*Msh->SolSiz+i]));
      }
    }
    
  }
  
  if ( Msh )
     FreeMesh(Msh);
  
}

void py_ReadMesh (char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg, 
                  PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers)
{
  int i, j, d;
  
  Options *mshopt = AllocOptions();
  
  strcpy(mshopt->InpNam,MshNam);
  strcpy(mshopt->SolNam,"\0");
  
  //--- Open mesh/solution file
  
  Mesh *Msh = NULL;
  Msh = SetupMeshAndSolution (mshopt->InpNam, mshopt->SolNam);
  
  for (i=1; i<=Msh->NbrVer; i++){
    for (d=0; d<3; d++)
      PyList_Append(pyVer, PyFloat_FromDouble(Msh->Ver[i][d]));
  }

  for (i=1; i<=Msh->NbrCor; i++){
    PyList_Append(pyCor, PyLong_FromLong(Msh->Cor[i]));
  }
  
  for (i=1; i<=Msh->NbrTri; i++){
    for (j=0; j<4; j++)
      PyList_Append(pyTri, PyLong_FromLong(Msh->Tri[i][j]));
  }
  
  for (i=1; i<=Msh->NbrTet; i++){
    for (j=0; j<5; j++)
      PyList_Append(pyTet, PyLong_FromLong(Msh->Tet[i][j]));
  }
  
  for (i=1; i<=Msh->NbrEfr; i++){
    for (j=0; j<3; j++)
      PyList_Append(pyEdg, PyLong_FromLong(Msh->Efr[i][j]));
  }
  
  for (i=1; i<=Msh->NbrHex; i++){
    for (j=0; j<9; j++)
      PyList_Append(pyHex, PyLong_FromLong(Msh->Hex[i][j]));
  }
  
  for (i=1; i<=Msh->NbrQua; i++){
    for (j=0; j<5; j++)
      PyList_Append(pyQua, PyLong_FromLong(Msh->Qua[i][j]));
  }
  
  for (i=1; i<=Msh->NbrPyr; i++){
    for (j=0; j<6; j++)
      PyList_Append(pyPyr, PyLong_FromLong(Msh->Pyr[i][j]));
  }
  
  for (i=1; i<=Msh->NbrPri; i++){
    for (j=0; j<7; j++)
      PyList_Append(pyPri, PyLong_FromLong(Msh->Pri[i][j]));
  }
  
  //--- First row of Markers contains dimension
  PyList_Append(pyMarkers, PyLong_FromLong(Msh->Dim));
  for (i=1; i<=Msh->NbrMarkers; i++){
    PyList_Append(pyMarkers, PyUnicode_FromString(Msh->Markers[i]));
  }
  
  if ( Msh )
     FreeMesh(Msh);
  
}

void py_ReadSol(char *SolNam, PyObject *pySol, PyObject *pySolHeader, int NbrVer, int Dim)
{
  Options *mshopt = AllocOptions();
  
  strcpy(mshopt->InpNam,SolNam);

  //--- Open solution file
  
  Mesh *Sol = NULL;
  Sol = SetupSolution (mshopt->InpNam, NbrVer, Dim);

  int i, iVer;

  for (i=0; i<Sol->SolSiz; i++){
    PyList_Append(pySolHeader, PyUnicode_FromString(Sol->SolTag[i]));
  }

  for (iVer=1; iVer<=Sol->NbrVer; iVer++) {
    for (i=0; i<Sol->SolSiz; i++) {
      PyList_Append(pySol, PyFloat_FromDouble(Sol->Sol[iVer*Sol->SolSiz+i]));
    }
  }

  if ( Sol )
     FreeMesh(Sol);
}

void py_WriteMeshAndSol(char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, 
                        PyObject *pyEdg, PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol, 
                        PyObject *pySolHeader, PyObject *pyMarkers, int Dim)
{
  int i, j, NbrTag = 0;
  Mesh *Msh= NULL;
  int SizMsh[GmfMaxKwd+1];
  
  int is[5], ref, idx, siz=0;
  double crd[3];
  
  int NbrMarkers = 0;
  
  for (i=0; i<GmfMaxKwd; i++)
    SizMsh[i] = 0;
  
  //--- Get mesh size

  if ( PyList_Check(pyVer) )
    SizMsh[GmfVertices] = PyList_Size(pyVer);
  
  if ( PyList_Check(pyTri) )
    SizMsh[GmfTriangles] = PyList_Size(pyTri);
  
  if ( PyList_Check(pyTet) )
    SizMsh[GmfTetrahedra] = PyList_Size(pyTet);
  
  if ( PyList_Check(pyEdg) )
    SizMsh[GmfEdges] = PyList_Size(pyEdg);

  if ( PyList_Check(pyHex) )
    SizMsh[GmfHexahedra] = PyList_Size(pyHex);
  
  if ( PyList_Check(pyQua) )
    SizMsh[GmfQuadrilaterals] = PyList_Size(pyQua);
  
  if ( PyList_Check(pyPyr) )
    SizMsh[GmfPyramids] = PyList_Size(pyPyr);
  
  if ( PyList_Check(pyPri) )
    SizMsh[GmfPrisms] = PyList_Size(pyPri);

  if ( PyList_Check(pyCor) )
    SizMsh[GmfCorners] = PyList_Size(pyCor);

  if ( PyList_Check(pyMarkers) )
    NbrMarkers = PyList_Size(pyMarkers);
  
  //--- Allocate mesh
  
  Msh = AllocMesh(SizMsh);
  
  Msh->Dim = Dim;
  
  //--- Fill mesh

  if ( PyList_Check(pyCor) )
    {
    siz = PyList_Size(pyCor);
      
    for (i=0; i<siz; i++)
        {
        
           PyObject *oo = PyList_GetItem(pyCor,i);
           if ( PyLong_Check(oo) )
           {
        is[0] = (int) PyLong_AsLong(oo);
           }
        
        
      Msh->NbrCor++;
      AddCorner(Msh,Msh->NbrCor,is);
        }
    }
  
  if ( PyList_Check(pyTri) )
  {
      siz = PyList_Size(pyTri);
      
      for (i=0; i<siz/4; i++)
      {
        idx = 4*i;
        
        for (j=0; j<3; j++) {
           PyObject *oo = PyList_GetItem(pyTri,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyTri,idx+3);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrTri++;
        AddTriangle(Msh,Msh->NbrTri,is,ref);
      }
  }
  
  if ( PyList_Check(pyTet) )
  {
      siz = PyList_Size(pyTet);
      
      for (i=0; i<siz/5; i++)
      {
        idx = 5*i;
        
        for (j=0; j<5; j++) {
           PyObject *oo = PyList_GetItem(pyTet,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        Msh->NbrTet++;
        AddTetrahedron(Msh,Msh->NbrTet,is,is[4]);
        
      }
  }
    
  if ( PyList_Check(pyEdg) )
  {
      siz = PyList_Size(pyEdg);
      
      for (i=0; i<siz/3; i++)
      {
        idx = 3*i;
        
        for (j=0; j<2; j++) {
           PyObject *oo = PyList_GetItem(pyEdg,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyEdg,idx+2);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrEfr++;
        AddEdge(Msh,Msh->NbrEfr,is,ref);
      }
  }
  
  
  if ( PyList_Check(pyHex) )
  {
      siz = PyList_Size(pyHex);
      
      for (i=0; i<siz/9; i++)
      {
        idx = 9*i;
        
        for (j=0; j<8; j++) {
           PyObject *oo = PyList_GetItem(pyHex,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyHex,idx+8);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrHex++;
        AddHexahedron(Msh,Msh->NbrHex,is,ref);
      }
  }
  
  if ( PyList_Check(pyQua) )
  {
      siz = PyList_Size(pyQua);
      
      for (i=0; i<siz/5; i++)
      {
        idx = 5*i;
        
        for (j=0; j<4; j++) {
           PyObject *oo = PyList_GetItem(pyQua,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyQua,idx+4);
        ref = (int) PyLong_AsLong(oo);
                
        Msh->NbrQua++;
        AddQuadrilateral(Msh,Msh->NbrQua,is,ref);
      }
  }
    
    
  if ( PyList_Check(pyPyr) )
  {
      siz = PyList_Size(pyPyr);
      
      for (i=0; i<siz/6; i++)
      {
        idx = 6*i;
        
        for (j=0; j<5; j++) {
           PyObject *oo = PyList_GetItem(pyPyr,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyPyr,idx+5);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrPyr++;
        AddPyramid(Msh,Msh->NbrPyr,is,ref);
      }
  }
  
  if ( PyList_Check(pyPri) )
  {
      siz = PyList_Size(pyPri);
      
      for (i=0; i<siz/7; i++)
      {
        idx = 7*i;
        
        for (j=0; j<6; j++) {
           PyObject *oo = PyList_GetItem(pyPri,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyPri,idx+6);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrPri++;
        AddPrism(Msh,Msh->NbrPri,is,ref);
      }
  }  
  
  if ( PyList_Check(pyVer) )
  {
      siz = PyList_Size(pyVer);
      
      for (i=0; i<siz/3; i++)
      {
        idx = 3*i;
        
        for (j=0; j<3; j++) {
           PyObject *oo = PyList_GetItem(pyVer,idx+j);
           if ( PyFloat_Check(oo) )
           {
            crd[j] = (double) PyFloat_AS_DOUBLE(oo);
           }
        }
        Msh->NbrVer++;
        AddVertex(Msh,Msh->NbrVer,crd);
      }
  }
  
  if ( PyList_Check(pyMarkers) )
  {
    for (i=1; i<NbrMarkers; i++){
      PyObject *oo = PyList_GetItem(pyMarkers,i);
      strcpy(Msh->Markers[i], (char*) PyUnicode_AsUTF8(oo));
    }
    Msh->NbrMarkers = NbrMarkers-1;
  }
  
  
  //--- Get Solution size and check it matches the number of vertices
  
  if ( PyList_Check(pySol) )
    siz = PyList_Size(pySol);

  if ( PyList_Check(pySolHeader) )
    NbrTag = PyList_Size(pySolHeader);
  
  if ( siz > 0 ) {
      
    if ( siz%Msh->NbrVer == 0 ) {
      
      Msh->SolSiz = siz/Msh->NbrVer;
      Msh->NbrFld = Msh->SolSiz;
      Msh->FldTab = (int*) malloc(sizeof(int)*Msh->SolSiz);
      for (j=0; j<Msh->NbrFld; j++){
        Msh->FldTab[j] = GmfSca;

        if ( NbrTag == Msh->NbrFld  ) {
          PyObject *oo = PyList_GetItem(pySolHeader,j);
               if ( PyUnicode_Check(oo) ) {
                  sprintf(Msh->SolTag[j], "%s", (char*) PyUnicode_AsUTF8(oo));
          }
        }
        else {
          sprintf(Msh->SolTag[j], "scalar_%d", j);
        }
      }
      Msh->Sol = (double*) malloc(sizeof(double)*(Msh->NbrVer+1)*Msh->SolSiz);
      memset(Msh->Sol, 0, sizeof(double)*(Msh->NbrVer+1)*Msh->SolSiz);
      
      Msh->Sol[0] = 0.0;
      for (i=0; i<siz; i++) {
             PyObject *oo = PyList_GetItem(pySol,i);
             if ( PyFloat_Check(oo) ) {
          Msh->Sol[i+Msh->SolSiz] = (double) PyFloat_AS_DOUBLE(oo);
             }
      }
    }
    else {
      printf("  ## ERROR py_WriteMesh: Inconsistent solution provided. Skip.\n");
    }
    
  }
  
  //--- Write Mesh
  
  int FilTyp = GetInputFileType(MshNam);
  int SolTyp = GetInputFileType(SolNam);
  char *ptr = NULL;
  char BasNam[1024], BasNamSol[1024], OutSol[1024], FldNam[1024];
  
  // --- Get BasNam

  GetBasNam(MshNam, BasNam);
  GetBasNam(SolNam, BasNamSol);
  
  if ( FilTyp != FILE_SU2MSH ) {
    WriteGMFMesh(BasNam, Msh, 1);
    if ( Msh->Sol ) {
      sprintf(OutSol, "%s.solb", BasNamSol);
      strcpy(FldNam, "all");
      if ( ! WriteGMFSolutionItf(OutSol, FldNam, Msh) ) {
        printf("  ## ERROR : Output solution FAILED.\n");
      }
    }
  }
  else {
    WriteSU2Mesh(BasNam, Msh);
    if ( Msh->Sol ) {
      if ( SolTyp == FILE_SU2BIN ) {
        sprintf(OutSol, "%s.dat", BasNamSol);
        WriteSU2SolutionBin (OutSol, Msh->Dim, Msh->NbrVer, Msh->Ver, Msh->Sol, Msh->SolSiz, Msh->SolTag);
      }
      else {
        sprintf(OutSol, "%s.csv", BasNamSol);
        WriteSU2Solution (OutSol, Msh->Dim, Msh->NbrVer, Msh->Ver, Msh->Sol, Msh->SolSiz, Msh->SolTag);
      }
    }
  }    
}

void py_WriteMesh(char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg,
                  PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers, int Dim)
{
  int i, j, NbrTag = 0;
  Mesh *Msh= NULL;
  int SizMsh[GmfMaxKwd+1];
  
  int is[5], siz, ref, idx;
  double crd[3];
  
  int NbrMarkers = 0;
  
  for (i=0; i<GmfMaxKwd; i++)
    SizMsh[i] = 0;
  
  //--- Get mesh size

  if ( PyList_Check(pyVer) )
    SizMsh[GmfVertices] = PyList_Size(pyVer);
  
  if ( PyList_Check(pyTri) )
    SizMsh[GmfTriangles] = PyList_Size(pyTri);
  
  if ( PyList_Check(pyTet) )
    SizMsh[GmfTetrahedra] = PyList_Size(pyTet);
  
  if ( PyList_Check(pyEdg) )
    SizMsh[GmfEdges] = PyList_Size(pyEdg);

  if ( PyList_Check(pyHex) )
    SizMsh[GmfHexahedra] = PyList_Size(pyHex);
  
  if ( PyList_Check(pyQua) )
    SizMsh[GmfQuadrilaterals] = PyList_Size(pyQua);
  
  if ( PyList_Check(pyPyr) )
    SizMsh[GmfPyramids] = PyList_Size(pyPyr);
  
  if ( PyList_Check(pyPri) )
    SizMsh[GmfPrisms] = PyList_Size(pyPri);

  if ( PyList_Check(pyCor) )
    SizMsh[GmfCorners] = PyList_Size(pyCor);

  if ( PyList_Check(pyMarkers) )
    NbrMarkers = PyList_Size(pyMarkers);
  
  //--- Allocate mesh
  
  Msh = AllocMesh(SizMsh);
  
  Msh->Dim = Dim;
  
  //--- Fill mesh

  if ( PyList_Check(pyCor) )
    {
    siz = PyList_Size(pyCor);
      
    for (i=0; i<siz; i++)
        {
        
           PyObject *oo = PyList_GetItem(pyCor,i);
           if ( PyLong_Check(oo) )
           {
        is[0] = (int) PyLong_AsLong(oo);
           }
        
        
      Msh->NbrCor++;
      AddCorner(Msh,Msh->NbrCor,is);
        }
    }
  
  if ( PyList_Check(pyTri) )
  {
      siz = PyList_Size(pyTri);
      
      for (i=0; i<siz/4; i++)
      {
        idx = 4*i;
        
        for (j=0; j<3; j++) {
           PyObject *oo = PyList_GetItem(pyTri,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyTri,idx+3);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrTri++;
        AddTriangle(Msh,Msh->NbrTri,is,ref);
      }
  }
  
  if ( PyList_Check(pyTet) )
  {
      siz = PyList_Size(pyTet);
      
      for (i=0; i<siz/5; i++)
      {
        idx = 5*i;
        
        for (j=0; j<5; j++) {
           PyObject *oo = PyList_GetItem(pyTet,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        Msh->NbrTet++;
        AddTetrahedron(Msh,Msh->NbrTet,is,is[4]);
        
      }
  }
    
  if ( PyList_Check(pyEdg) )
  {
      siz = PyList_Size(pyEdg);
      
      for (i=0; i<siz/3; i++)
      {
        idx = 3*i;
        
        for (j=0; j<2; j++) {
           PyObject *oo = PyList_GetItem(pyEdg,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyEdg,idx+2);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrEfr++;
        AddEdge(Msh,Msh->NbrEfr,is,ref);
      }
  }
  
  
  if ( PyList_Check(pyHex) )
  {
      siz = PyList_Size(pyHex);
      
      for (i=0; i<siz/9; i++)
      {
        idx = 9*i;
        
        for (j=0; j<8; j++) {
           PyObject *oo = PyList_GetItem(pyHex,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyHex,idx+8);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrHex++;
        AddHexahedron(Msh,Msh->NbrHex,is,ref);
      }
  }
  
  if ( PyList_Check(pyQua) )
  {
      siz = PyList_Size(pyQua);
      
      for (i=0; i<siz/5; i++)
      {
        idx = 5*i;
        
        for (j=0; j<4; j++) {
           PyObject *oo = PyList_GetItem(pyQua,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyQua,idx+4);
        ref = (int) PyLong_AsLong(oo);
                
        Msh->NbrQua++;
        AddQuadrilateral(Msh,Msh->NbrQua,is,ref);
      }
  }
    
    
  if ( PyList_Check(pyPyr) )
  {
      siz = PyList_Size(pyPyr);
      
      for (i=0; i<siz/6; i++)
      {
        idx = 6*i;
        
        for (j=0; j<5; j++) {
           PyObject *oo = PyList_GetItem(pyPyr,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyPyr,idx+5);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrPyr++;
        AddPyramid(Msh,Msh->NbrPyr,is,ref);
      }
  }
  
  if ( PyList_Check(pyPri) )
  {
      siz = PyList_Size(pyPri);
      
      for (i=0; i<siz/7; i++)
      {
        idx = 7*i;
        
        for (j=0; j<6; j++) {
           PyObject *oo = PyList_GetItem(pyPri,idx+j);
           if ( PyLong_Check(oo) )
           {
            is[j] = (int) PyLong_AsLong(oo);
           }
        }
        
        PyObject *oo = PyList_GetItem(pyPri,idx+6);
        ref = (int) PyLong_AsLong(oo);
        
        Msh->NbrPri++;
        AddPrism(Msh,Msh->NbrPri,is,ref);
      }
  }  
  
  if ( PyList_Check(pyVer) )
  {
      siz = PyList_Size(pyVer);
      
      for (i=0; i<siz/3; i++)
      {
        idx = 3*i;
        
        for (j=0; j<3; j++) {
           PyObject *oo = PyList_GetItem(pyVer,idx+j);
           if ( PyFloat_Check(oo) )
           {
            crd[j] = (double) PyFloat_AS_DOUBLE(oo);
           }
        }
        Msh->NbrVer++;
        AddVertex(Msh,Msh->NbrVer,crd);
      }
  }
  
  if ( PyList_Check(pyMarkers) )
  {
    for (i=1; i<NbrMarkers; i++){
      PyObject *oo = PyList_GetItem(pyMarkers,i);
      strcpy(Msh->Markers[i], (char*) PyUnicode_AsUTF8(oo));
    }
    Msh->NbrMarkers = NbrMarkers-1;
  }
  
  //--- Write Mesh
  
  int FilTyp = GetInputFileType(MshNam);
  char *ptr = NULL;
  char BasNam[1024];
  
  // --- Get BasNam
  
  GetBasNam(MshNam, BasNam);
  
  if ( FilTyp != FILE_SU2MSH ) {
    WriteGMFMesh(BasNam, Msh, 1);
  }
  else {
    WriteSU2Mesh(BasNam, Msh);
  }    
}

void py_WriteSol(char *SolNam, PyObject *pyVer, PyObject *pySol, PyObject *pySolHeader, int NbrVer, int Dim)
{
  
  int i, j, idx, siz=0;
  int SolSiz=0, NbrFld=0, NbrTag=0;
  int *FldTab = NULL;
  
  double  *Sol = NULL;
  double3 *Ver = NULL;
  
  char SolTag[100][256];
  
  if ( PyList_Check(pySol) )
    siz = PyList_Size(pySol);
  
  if ( PyList_Check(pySolHeader) )
    NbrTag = PyList_Size(pySolHeader);
  
  int FilTyp = GetInputFileType(SolNam);
  
  if ( siz > 0 ) {
      
    if ( siz%NbrVer == 0 ) {
      
      SolSiz = siz/NbrVer;
      NbrFld = SolSiz;
      FldTab = (int*) malloc(sizeof(int)*SolSiz);
      for (j=0; j<NbrFld; j++){
        FldTab[j] = GmfSca;
        
        if ( NbrTag == NbrFld ) {
           PyObject *oo = PyList_GetItem(pySolHeader,j);
           if ( PyUnicode_Check(oo) )
           {
                  sprintf(SolTag[j], "%s", (char*) PyUnicode_AsUTF8(oo));
           }
        }
        else 
          sprintf(SolTag[j], "scalar_%d", j);
      }
      
      Sol = (double*) malloc(sizeof(double)*(NbrVer+1)*SolSiz);
      memset(Sol, 0, sizeof(double)*(NbrVer+1)*SolSiz);
      
      Sol[0] = 0.0;
      for (i=0; i<siz; i++)
      {
         PyObject *oo = PyList_GetItem(pySol,i);
         if ( PyFloat_Check(oo) )
         {
          Sol[i+SolSiz] = (double) PyFloat_AS_DOUBLE(oo);
         }
      }
    }
    else {
      printf("  ## ERROR py_WriteSolution: Inconsistent solution provided. Skip.\n");
      printf("siz %d NbrVer %d -> %d\n", siz, NbrVer, siz%NbrVer);
      exit(1);
    }
    
    
    if ( PyList_Check(pyVer) )
    {
        siz = PyList_Size(pyVer);
        
        if ( NbrVer != siz/3 ) {
          printf("  ## ERROR py_WriteSolution: Inconsistent number of vertices. Skip.\n");
          exit(1);
        }
        
        Ver = (double3*) malloc(sizeof(double3)*(NbrVer+1));
      
        for (i=0; i<siz/3; i++)
        {
          idx = 3*i;
        
          for (j=0; j<3; j++) {
             PyObject *oo = PyList_GetItem(pyVer,idx+j);
             if ( PyFloat_Check(oo) )
             {
              Ver[i+1][j] = (double) PyFloat_AS_DOUBLE(oo);
             }
            
          }
        }
    }
    
    if ( FilTyp == FILE_GMFSOL ) {
      WriteGMFSolution(SolNam, Sol, SolSiz, NbrVer, Dim, NbrFld, FldTab);
    }
    else if ( FilTyp == FILE_SU2BIN ) {
      WriteSU2SolutionBin (SolNam, Dim, NbrVer, Ver, Sol, SolSiz, SolTag);
    }
    else {
      WriteSU2Solution (SolNam, Dim, NbrVer, Ver, Sol, SolSiz, SolTag);
    }
    
  }  

  if ( FldTab )
    free(FldTab);
  
  if ( Sol )
    free(Sol);
  
  if ( Ver )
    free(Ver);
  
}

