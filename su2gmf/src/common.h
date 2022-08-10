#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <ctype.h>
#include <setjmp.h>

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

#include "mesh.h"
#include "option.h"

#define PI_NUMBER 3.14159265359
#define CGNS_STRING_SIZE 33

// Prototypes

//--- su2io.c
int  AddSU2MeshSize(char *FilNam, int *SizMsh) ;
int  GetSU2KeywordValue(FILE *FilHdl, char *Kwd);
int  GetSU2KeywordValueStr(FILE *FilHdl, char *Kwd, char *StrVal);
int  LoadSU2Elements(FILE *FilHdl, Mesh *Msh, Conn *Con);
int  LoadSU2ConnData(char *FilNam, Mesh *Msh, Conn *Con);
int  LoadSU2Mesh(char *FilNam, Mesh *Msh, Conn *Con);
int  LoadSU2Solution(char *SolNam, Mesh *Msh);
int  LoadSU2SolutionBin(char *SolNam, Mesh *Msh);
int  LoadSU2Vertices(FILE *FilHdl, Mesh *Msh);
void WriteSU2Mesh(char *nam, Mesh *Msh);
int  GetSU2SolSize(char *SolNam);
int  WriteSU2Solution(char *SolNam, int Dim, int NbrVer, double3 *Ver,  double *Sol, int SolSiz, char SolTag[100][256]);
int  WriteSU2SolutionBin(char *SolNam, int Dim, int NbrVer, double3 *Ver,  double *Sol, int SolSiz, char SolTag[100][256]);

//--- gmfio.c
int AddGMFMeshSize(char *MshNam, int *SizMsh);
int LoadGMFMesh(char *MshNam, Mesh *Msh, Conn *Con);
int LoadGMFConnData(char *MshNam, Mesh *Msh, Conn *Con);
int LoadGMFSolution(char *SolNam, Mesh *Msh);
int WriteGMFMesh(char *nam, Mesh *Msh, int OptBin);
int WriteGMFSolution(char *SolNam, double *Sol, int SolSiz, int NbrVer, int Dim, int NbrFld, int* FldTab);
int WriteGMFSolutionItf(char *SolNam, char *FldNam, Mesh *Msh);
int WriteGMFMetric(char *nam, Mesh *Msh, int OptBin);
int SplitQuads(char *nam, Mesh *Msh);

//--- option.c
Options* AllocOptions(void);
int      CheckOptions(Options *mshopt);
int      GetBasNam(char *InpNam, char *BasNam);
void     PrintOptions(Options *mshopt);

//---- mesh.c
Mesh* AllocMesh(int * SizMsh);
int   FreeMesh(Mesh *Msh);
int   FreeConn(Conn *Con);
int   GetMeshSize(char *MshNam, int *SizMsh);
Mesh *SetupMeshAndSolution(char *MshNam, char *SolNam, int Dim);
Mesh *SetupSolution(char *SolNam, int NbrVer, int Dim);
void  CopyBoundaryMarkers(Mesh *Msh, Mesh *BndMsh);
void  CheckVolumeElementOrientation(Mesh *Msh);
void  CheckSurfaceElementOrientation(Mesh *Msh, Conn *Con);
int   RemoveUnconnectedVertices(Mesh *Msh);
void  AddCorner(Mesh *Msh, int idxCor, int *is);
void  AddVertex(Mesh *Msh, int idxVer, double *Crd);
void  AddEdge(Mesh *Msh, int idx, int *is, int ref);
void  AddTriangle(Mesh *Msh, int idxTri, int *is, int ref);
void  AddQuadrilateral(Mesh *Msh, int idx, int *is, int ref);
void  AddPyramid(Mesh *Msh, int idx, int *is, int ref);
void  AddPrism(Mesh *Msh, int idx, int *is, int ref);
void  AddTetrahedron(Mesh *Msh, int idx, int *is, int ref);
void  AddHexahedron(Mesh *Msh, int idx, int *is, int ref);
void  CheckTriangle(int4* Tri, double3* Ver, int iTri);
void  CheckQuadrilateral(int5* Qua, double3* Ver, int iQua);
void  CheckTetrahedron(int5* Tet, double3* Ver, int iTet);
void  CheckPyramid(int6* Pyr, double3* Ver, int iPyr);
void  CheckPrism(int7* Pri, double3* Ver, int iPri);
void  CheckHexahedron(int9* Hex, double3* Ver, int iHex);
void  CheckBoundEdge(int3* Edg, double3* Ver, int iEdg, int iVol);
void  CheckBoundTriangle(int4* Tri, double3* Ver, int iTri, int iVol);
void  CheckBoundQuadrilateral(int5* Qua, double3* Ver, int iQua, int iVol);
void  PrintMeshInfo(Mesh *Msh);
int   GetFileType(char *FilNam);
void  StrRemoveChars(char* str, char c);
int   cmp_int2(const void *a, const void *b);

//--- convert.c
int ConvertGMFtoSU2Sol(Options *mshopt);
int ConvertGMFWithBoundtoSU2Sol(Options *mshopt, char* BndMshNam);
int ConvertGMFSoltoMet(Options *mshopt);
int ConvertSU2SolToGMF(Options *mshopt);
