#include "amgio.h"

/*
Victorien Menier Feb 2016
Brian Munguía Apr 2016
*/

void AddCorner(Mesh *Msh, int idxCor, int *is)
{
  if ( idxCor > Msh->MaxNbrCor ) {
    printf("  ## ERROR : Max number of corners reached (%d, max %d).\n", idxCor, Msh->MaxNbrCor);
    exit(1);
  }
  Msh->Cor[idxCor] = is[0];
}

void AddVertex(Mesh *Msh, int idxVer, double *Crd)
{  
  Msh->Ver[idxVer][0] = Crd[0];
  Msh->Ver[idxVer][1] = Crd[1];
    
  if ( Msh->Dim == 3 )
    Msh->Ver[idxVer][2] = Crd[2];
}

void AddEdge(Mesh *Msh, int idx, int *is, int ref)
{
  if ( idx > Msh->MaxNbrEfr ) {
    printf("  ## ERROR : Max number of boundary edges reached.\n");
    printf("MAX = %d, idx = %d \n", Msh->MaxNbrEfr, idx);
    exit(1);
  }
  Msh->Efr[idx][0] = is[0];
  Msh->Efr[idx][1] = is[1];
  Msh->Efr[idx][2] = ref;
}

void AddTriangle(Mesh *Msh, int idxTri, int *is, int ref)
{
  if ( idxTri > Msh->MaxNbrTri ) {
    printf("  ## ERROR : Max number of triangles reached (%d, max %d).\n", idxTri, Msh->MaxNbrTri);
    exit(1);
  }
  Msh->Tri[idxTri][0] = is[0];
  Msh->Tri[idxTri][1] = is[1];
  Msh->Tri[idxTri][2] = is[2];
  Msh->Tri[idxTri][3] = ref;
}

void AddQuadrilateral(Mesh *Msh, int idx, int *is, int ref)
{
  if ( idx > Msh->MaxNbrQua ) {
    printf("  ## ERROR : Max number of quads reached.\n");
    exit(1);
  }
  Msh->Qua[idx][0] = is[0];
  Msh->Qua[idx][1] = is[1];
  Msh->Qua[idx][2] = is[2];
  Msh->Qua[idx][3] = is[3];
  Msh->Qua[idx][4] = ref;
}

void AddTetrahedron(Mesh *Msh, int idx, int *is, int ref)
{
  if ( idx > Msh->MaxNbrTet ) {
    printf("  ## ERROR : Max number of tetra reached.\n");
    exit(1);
  }
  Msh->Tet[idx][0] = is[0];
  Msh->Tet[idx][1] = is[1];
  Msh->Tet[idx][2] = is[2];
  Msh->Tet[idx][3] = is[3];
  Msh->Tet[idx][4] = ref;
}

void AddPyramid(Mesh *Msh, int idx, int *is, int ref)
{
  if ( idx > Msh->MaxNbrPyr ) {
    printf("  ## ERROR : Max number of pyramids reached.\n");
    exit(1);
  }
  Msh->Pyr[idx][0] = is[0];
  Msh->Pyr[idx][1] = is[1];
  Msh->Pyr[idx][2] = is[2];
  Msh->Pyr[idx][3] = is[3];
  Msh->Pyr[idx][4] = is[4];
  Msh->Pyr[idx][5] = ref;
}

void AddPrism(Mesh *Msh, int idx, int *is, int ref)
{
  if ( idx > Msh->MaxNbrPri ) {
    printf("  ## ERROR : Max number of prisms reached.\n");
    exit(1);
  }
  Msh->Pri[idx][0] = is[0];
  Msh->Pri[idx][1] = is[1];
  Msh->Pri[idx][2] = is[2];
  Msh->Pri[idx][3] = is[3];
  Msh->Pri[idx][4] = is[4];
  Msh->Pri[idx][5] = is[5];
  Msh->Pri[idx][6] = ref;
}

void AddHexahedron(Mesh *Msh, int idx, int *is, int ref)
{
  if ( idx > Msh->MaxNbrHex ) {
    printf("  ## ERROR : Max number of hexas reached.\n");
    exit(1);
  }
  Msh->Hex[idx][0] = is[0];
  Msh->Hex[idx][1] = is[1];
  Msh->Hex[idx][2] = is[2];
  Msh->Hex[idx][3] = is[3];
  Msh->Hex[idx][4] = is[4];
  Msh->Hex[idx][5] = is[5];
  Msh->Hex[idx][6] = is[6];
  Msh->Hex[idx][7] = is[7];
  Msh->Hex[idx][8] = ref;
}

void CheckTriangle(int4* Tri, double3* Ver, int iTri) {
  int Point0 = Tri[iTri][0],
      Point1 = Tri[iTri][1],
      Point2 = Tri[iTri][2];

  double2 a, b;
  for (int iDim = 0; iDim < 2; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
  }
  const double test = a[0]*b[1]-b[0]*a[1];

  if (test < 0.0) {
    Tri[iTri][0] = Point2;
    Tri[iTri][2] = Point0;
  }
}

void CheckQuadrilateral(int5* Qua, double3* Ver, int iQua) {
  int Point0 = Qua[iQua][0],
      Point1 = Qua[iQua][1],
      Point2 = Qua[iQua][2],
      Point3 = Qua[iQua][3];

  double2 a, b;
  for (int iDim = 0; iDim < 2; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
  }
  const double test1 = a[0]*b[1]-b[0]*a[1];

  for (int iDim = 0; iDim < 2; iDim++) {
    a[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point1][iDim]);
    b[iDim] = 0.5*(Ver[Point3][iDim]-Ver[Point1][iDim]);
  }
  const double test2 = a[0]*b[1]-b[0]*a[1];

  for (int iDim = 0; iDim < 2; iDim++) {
    a[iDim] = 0.5*(Ver[Point3][iDim]-Ver[Point2][iDim]);
    b[iDim] = 0.5*(Ver[Point0][iDim]-Ver[Point2][iDim]);
  }
  const double test3 = a[0]*b[1]-b[0]*a[1];

  for (int iDim = 0; iDim < 2; iDim++) {
    a[iDim] = 0.5*(Ver[Point0][iDim]-Ver[Point3][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point3][iDim]);
  }
  const double test4 = a[0]*b[1]-b[0]*a[1];

  if ((test1 < 0.0) && (test2 < 0.0) && (test3 < 0.0) && (test4 < 0.0)) {
    Qua[iQua][1] = Point3;
    Qua[iQua][3] = Point1;
  }
}

void CheckTetrahedron(int5* Tet, double3* Ver, int iTet) {
  int Point0 = Tet[iTet][0],
      Point1 = Tet[iTet][1],
      Point2 = Tet[iTet][2],
      Point3 = Tet[iTet][3];

  double3 a, b, c, n;
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    c[iDim] = Ver[Point3][iDim]-Ver[Point0][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  if (test < 0.0) {
    Tet[iTet][0] = Point1;
    Tet[iTet][1] = Point0;
  }
}

void CheckPyramid(int6* Pyr, double3* Ver, int iPyr) {
  int Point0 = Pyr[iPyr][0],
      Point1 = Pyr[iPyr][1],
      Point2 = Pyr[iPyr][2],
      Point3 = Pyr[iPyr][4];

  double3 a, b, c, n;
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    c[iDim] = Ver[Point3][iDim]-Ver[Point0][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test1 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  Point0 = Pyr[iPyr][2];
  Point1 = Pyr[iPyr][3];
  Point2 = Pyr[iPyr][0];
  Point3 = Pyr[iPyr][4];
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    c[iDim] = Ver[Point3][iDim]-Ver[Point0][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test2 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  if ((test1 < 0.0) || (test2 < 0.0)) {
    Point1 = Pyr[iPyr][1];
    Point3 = Pyr[iPyr][3];
    Pyr[iPyr][1] = Point3;
    Pyr[iPyr][3] = Point1;
  }
}

void CheckPrism(int7* Pri, double3* Ver, int iPri) {
  int Point0 = Pri[iPri][0],
      Point1 = Pri[iPri][1],
      Point2 = Pri[iPri][2],
      Point3 = Pri[iPri][3],
      Point4 = Pri[iPri][4],
      Point5 = Pri[iPri][5];

  double3 a, b, c, n;
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    c[iDim] = (Ver[Point3][iDim]-Ver[Point0][iDim])+
              (Ver[Point4][iDim]-Ver[Point1][iDim])+
              (Ver[Point5][iDim]-Ver[Point2][iDim]);
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test1 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point4][iDim]-Ver[Point3][iDim]);
    b[iDim] = 0.5*(Ver[Point5][iDim]-Ver[Point3][iDim]);
    c[iDim] = (Ver[Point0][iDim]-Ver[Point3][iDim])+
              (Ver[Point1][iDim]-Ver[Point4][iDim])+
              (Ver[Point2][iDim]-Ver[Point5][iDim]);
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test2 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  if ((test1 < 0.0) || (test2 < 0.0)) {
    Pri[iPri][0] = Point1;
    Pri[iPri][1] = Point0;
    Pri[iPri][3] = Point4;
    Pri[iPri][4] = Point3;
  }
}

void CheckHexahedron(int9* Hex, double3* Ver, int iHex) {
  int Point0 = Hex[iHex][0],
      Point1 = Hex[iHex][1],
      Point2 = Hex[iHex][2],
      Point3 = Hex[iHex][5];

  double3 a, b, c, n;
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    c[iDim] = Ver[Point3][iDim]-Ver[Point0][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test1 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  Point0 = Hex[iHex][2];
  Point1 = Hex[iHex][3];
  Point2 = Hex[iHex][0];
  Point3 = Hex[iHex][7];
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    c[iDim] = Ver[Point3][iDim]-Ver[Point0][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test2 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  Point0 = Hex[iHex][1];
  Point1 = Hex[iHex][2];
  Point2 = Hex[iHex][3];
  Point3 = Hex[iHex][6];
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    c[iDim] = Ver[Point3][iDim]-Ver[Point0][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test3 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  Point0 = Hex[iHex][3];
  Point1 = Hex[iHex][0];
  Point2 = Hex[iHex][1];
  Point3 = Hex[iHex][4];
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    c[iDim] = Ver[Point3][iDim]-Ver[Point0][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test4 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  if ((test1 < 0.0) || (test2 < 0.0) || (test3 < 0.0) || (test4 < 0.0)) {
    Point0 = Hex[iHex][1];
    Point1 = Hex[iHex][3];
    Point2 = Hex[iHex][5];
    Point3 = Hex[iHex][7];
    Hex[iHex][1] = Point0;
    Hex[iHex][3] = Point1;
    Hex[iHex][5] = Point2;
    Hex[iHex][7] = Point3;
  }
}

void CheckBoundEdge(int3* Edg, double3* Ver, int iEdg, int iVol) {
  int Point0 = Edg[iEdg][0],
      Point1 = Edg[iEdg][1];

  double2 a, b;
  for (int iDim = 0; iDim < 2; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[iVol][iDim]-Ver[Point0][iDim]);
  }
  const double test = a[0]*b[1]-b[0]*a[1];

  if (test < 0.0) {
    Edg[iEdg][0] = Point1;
    Edg[iEdg][1] = Point0;
  }
}

void CheckBoundTriangle(int4* Tri, double3* Ver, int iTri, int iVol) {
  int Point0 = Tri[iTri][0],
      Point1 = Tri[iTri][1],
      Point2 = Tri[iTri][2];

  double3 a, b, c, n;
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    c[iDim] = Ver[iVol][iDim]-Ver[Point0][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  if (test < 0.0) {
    Tri[iTri][0] = Point2;
    Tri[iTri][2] = Point0;
  }
}

void CheckBoundQuadrilateral(int5* Qua, double3* Ver, int iQua, int iVol) {
  int Point0 = Qua[iQua][0],
      Point1 = Qua[iQua][1],
      Point2 = Qua[iQua][2],
      Point3 = Qua[iQua][3];

  double3 a, b, c, n;
  for (int iDim = 0; iDim < 3; iDim++) {
    a[iDim] = 0.5*(Ver[Point1][iDim]-Ver[Point0][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point0][iDim]);
    c[iDim] = Ver[iVol][iDim]-Ver[Point0][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test1 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  for (int iDim = 0; iDim < 2; iDim++) {
    a[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point1][iDim]);
    b[iDim] = 0.5*(Ver[Point3][iDim]-Ver[Point1][iDim]);
    c[iDim] = Ver[iVol][iDim]-Ver[Point1][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test2 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  for (int iDim = 0; iDim < 2; iDim++) {
    a[iDim] = 0.5*(Ver[Point3][iDim]-Ver[Point2][iDim]);
    b[iDim] = 0.5*(Ver[Point0][iDim]-Ver[Point2][iDim]);
    c[iDim] = Ver[iVol][iDim]-Ver[Point2][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test3 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  for (int iDim = 0; iDim < 2; iDim++) {
    a[iDim] = 0.5*(Ver[Point0][iDim]-Ver[Point3][iDim]);
    b[iDim] = 0.5*(Ver[Point2][iDim]-Ver[Point3][iDim]);
    c[iDim] = Ver[iVol][iDim]-Ver[Point3][iDim];
  }
  n[0] = a[1]*b[2]-b[1]*a[2];
  n[1] = b[0]*a[2]-a[0]*b[2];
  n[2] = a[0]*b[1]-b[0]*a[1];
  const double test4 = n[0]*c[0]+n[1]*c[1]+n[2]*c[2];

  if ((test1 < 0.0) && (test2 < 0.0) && (test3 < 0.0) && (test4 < 0.0)) {
    Qua[iQua][1] = Point3;
    Qua[iQua][3] = Point1;
  }
}

Mesh* AllocMesh (int * SizMsh)
{
  Mesh *Msh=NULL;
  Msh = (Mesh*)malloc(sizeof(struct S_Mesh));
  
  Msh->MaxNbrVer = SizMsh[GmfVertices];
  Msh->MaxNbrCor = SizMsh[GmfCorners];
  Msh->MaxNbrEfr = SizMsh[GmfEdges];
  Msh->MaxNbrTri = SizMsh[GmfTriangles];
  Msh->MaxNbrTet = SizMsh[GmfTetrahedra];
  Msh->MaxNbrQua = SizMsh[GmfQuadrilaterals];
  Msh->MaxNbrHex = SizMsh[GmfHexahedra];
  Msh->MaxNbrPyr = SizMsh[GmfPyramids];
  Msh->MaxNbrPri = SizMsh[GmfPrisms];
  Msh->Dim       = SizMsh[GmfDimension];
  
  Msh->NbrVer = 0;
  Msh->NbrCor = 0;
  Msh->NbrEfr = 0;
  Msh->NbrTri = 0;
  Msh->NbrTet = 0;
  Msh->NbrQua = 0;
  Msh->NbrHex = 0;
  Msh->NbrPri = 0;
  Msh->NbrPyr = 0;
  
  Msh->Ver = NULL;
  Msh->Cor = NULL;
  Msh->Efr = NULL;
  Msh->Tri = NULL;
  Msh->Tet = NULL;
  Msh->Qua = NULL;
  Msh->Hex = NULL;
  Msh->Pri = NULL;
  Msh->Pyr = NULL;

  Msh->SolSiz = 0;
  Msh->Sol = NULL;
  for (int i = 0; i < 100; i++) {
    strcpy(Msh->SolTag[i],"\0");
  }
  
  Msh->NbrFld = 0;
  Msh->FldTab = NULL;
  
  Msh->NbrMarkers = 0;
  for (int i = 0; i < 10000; i++) {
    strcpy(Msh->Markers[i],"\0");
  }
  
  if ( Msh->MaxNbrVer > 0 ) {
    Msh->Ver = (double3*)malloc(sizeof(double3)*(Msh->MaxNbrVer+1));
  }
  
  if ( Msh->MaxNbrCor > 0 ) {
    Msh->Cor = (int*)malloc(sizeof(int)*(Msh->MaxNbrCor+1));
  }

  if ( Msh->MaxNbrEfr > 0 ) {
    Msh->Efr = (int3*)malloc(sizeof(int3)*(Msh->MaxNbrEfr+1));
  }
  
  if ( Msh->MaxNbrTri > 0 ) {
    Msh->Tri = (int4*)malloc(sizeof(int4)*(Msh->MaxNbrTri+1));
  }
  
  if ( Msh->MaxNbrTet > 0 ) {
    Msh->Tet = (int5*)malloc(sizeof(int5)*(Msh->MaxNbrTet+1));
  }
  
  if ( Msh->MaxNbrHex > 0 ) {
    Msh->Hex = (int9*)malloc(sizeof(int9)*(Msh->MaxNbrHex+1));
  }
  
  if ( Msh->MaxNbrQua > 0 ) {
    Msh->Qua = (int5*)malloc(sizeof(int5)*(Msh->MaxNbrQua+1));
  }
  
  if ( Msh->MaxNbrPri > 0 ) {
    Msh->Pri = (int7*)malloc(sizeof(int7)*(Msh->MaxNbrPri+1));
  }
  
  if ( Msh->MaxNbrPyr > 0 ) {
    Msh->Pyr = (int6*)malloc(sizeof(int6)*(Msh->MaxNbrPyr+1));
  }

  Msh->FilTyp = -1;
  
  return Msh;    
}

Conn* AllocConn (int NbrVer)
{
  Conn* Con=NULL;
  Con = (Conn*)malloc(sizeof(struct S_Conn));

  Con->NbrVer = NbrVer;
  
  Con->Efr = NULL;
  Con->Tri = NULL;
  Con->Tet = NULL;
  Con->Qua = NULL;
  Con->Hex = NULL;
  Con->Pri = NULL;
  Con->Pyr = NULL;

  Con->NbrEfr = NULL;
  Con->NbrTri = NULL;
  Con->NbrTet = NULL;
  Con->NbrQua = NULL;
  Con->NbrHex = NULL;
  Con->NbrPri = NULL;
  Con->NbrPyr = NULL;

  Con->Efr    = (int**)malloc(sizeof(int*)*(NbrVer+1));
  Con->Tri    = (int**)malloc(sizeof(int*)*(NbrVer+1));
  Con->Tet    = (int**)malloc(sizeof(int*)*(NbrVer+1));
  Con->Qua    = (int**)malloc(sizeof(int*)*(NbrVer+1));
  Con->Hex    = (int**)malloc(sizeof(int*)*(NbrVer+1));
  Con->Pri    = (int**)malloc(sizeof(int*)*(NbrVer+1));
  Con->Pyr    = (int**)malloc(sizeof(int*)*(NbrVer+1));

  Con->NbrEfr = (int*)malloc(sizeof(int)*(NbrVer+1));
  Con->NbrTri = (int*)malloc(sizeof(int)*(NbrVer+1));
  Con->NbrTet = (int*)malloc(sizeof(int)*(NbrVer+1));
  Con->NbrQua = (int*)malloc(sizeof(int)*(NbrVer+1));
  Con->NbrHex = (int*)malloc(sizeof(int)*(NbrVer+1));
  Con->NbrPri = (int*)malloc(sizeof(int)*(NbrVer+1));
  Con->NbrPyr = (int*)malloc(sizeof(int)*(NbrVer+1));

  for (int iVer = 0; iVer < NbrVer; iVer++) {
    Con->NbrEfr[iVer] = 0;
    Con->NbrTri[iVer] = 0;
    Con->NbrTet[iVer] = 0;
    Con->NbrQua[iVer] = 0;
    Con->NbrHex[iVer] = 0;
    Con->NbrPri[iVer] = 0;
    Con->NbrPyr[iVer] = 0;
  }
  
  return Con;    
}

int cmp_int2(const void *a, const void *b)
{
  int *ia = (int*) a;
  int *ib = (int*) b;
  
  if ( ia[0] < ib[0] ) {
    return -1;
  }
  else if ( ia[0] > ib[0] ) {
    return 1;
  }
  else {
    
    if ( ia[1] < ib[1] ) {
      return -1;
    }
    else if ( ia[1] > ib[1 ] ) {
      return 1;
    }
    else {
      return 0;
    }
  }
  
}

double Dist (double *crd0, double *crd1)
{
  double len;
  len = (crd1[0]-crd0[0])*(crd1[0]-crd0[0]) + (crd1[1]-crd0[1])*(crd1[1]-crd0[1]);
  len = sqrt(len);
  return len;
}

int FreeMesh(Mesh *Msh)
{
  
  if ( !Msh ) return 0;
  
  if ( Msh->Ver ) {
    free(Msh->Ver);
  }
  
  if ( Msh->Cor ) {
    free(Msh->Cor);
  }

  if ( Msh->Efr ) {
    free(Msh->Efr);
  }
  
  if ( Msh->Pyr ) {
    free(Msh->Pyr);
  }
  
  if ( Msh->Pri ) {
    free(Msh->Pri);
  }
  
  if ( Msh->Tri ) {
    free(Msh->Tri);
  }
  
  if ( Msh->Tet ) {
    free(Msh->Tet);
  }
  
  if ( Msh->Sol ) {
    free(Msh->Sol);
  }
  
  if ( Msh->FldTab ) {
    free(Msh->FldTab);
  }
  
  free(Msh);
  
  return 1;  
}

int FreeConn(Conn *Con)
{
  
  if ( !Con ) return 0;

  for (int i = 1; i <= Con->NbrVer; i++) {
    if (Con->Efr[i]) free(Con->Efr[i]);
    if (Con->Tri[i]) free(Con->Tri[i]);
    if (Con->Tet[i]) free(Con->Tet[i]);
    if (Con->Qua[i]) free(Con->Qua[i]);
    if (Con->Hex[i]) free(Con->Hex[i]);
    if (Con->Pri[i]) free(Con->Pri[i]);
    if (Con->Pyr[i]) free(Con->Pyr[i]);
  }

  if (Con->Efr) free(Con->Efr);
  if (Con->Tri) free(Con->Tri);
  if (Con->Tet) free(Con->Tet);
  if (Con->Qua) free(Con->Qua);
  if (Con->Hex) free(Con->Hex);
  if (Con->Pri) free(Con->Pri);
  if (Con->Pyr) free(Con->Pyr);

  if (Con->NbrEfr) free(Con->NbrEfr);
  if (Con->NbrTri) free(Con->NbrTri);
  if (Con->NbrTet) free(Con->NbrTet);
  if (Con->NbrQua) free(Con->NbrQua);
  if (Con->NbrHex) free(Con->NbrHex);
  if (Con->NbrPri) free(Con->NbrPri);
  if (Con->NbrPyr) free(Con->NbrPyr);
  
  free(Con);
  
  return 1;  
}

int imin(int n, int *idx)
{
  int i;
  int im   = 0;
  for(i=1; i<n; i++) {
    if ( idx[i] < idx[im] ) {
      im = i;
    }
  }
  return im;  
}

void outwardNormalCrd(double2 crd0, double2 crd1, double *vno)
{
  vno[0]  = crd1[1]-crd0[1];  // define outward normal, how to verify this ? 
  vno[1]  = crd0[0]-crd1[0];
  vno[2]  = sqrt(vno[0]*vno[0]+vno[1]*vno[1]);
}

void PrintMeshInfo (Mesh *Msh)
{
  int i;
  printf("\n-------------------------\n");
  printf("Mesh: %s\n", Msh->MshNam);
  printf("  Vertices   : %d\n", Msh->NbrVer);
  if ( Msh->NbrTri > 0 ) printf("  Triangles  : %d\n", Msh->NbrTri);
  if ( Msh->NbrQua > 0 ) printf("  Quads      : %d\n", Msh->NbrQua);
  if ( Msh->NbrEfr > 0 ) printf("  Edges      : %d\n", Msh->NbrEfr);
  if ( Msh->NbrTet > 0 ) printf("  Tetrahedra : %d\n", Msh->NbrTet);
  if ( Msh->NbrPri > 0 ) printf("  Prisms     : %d\n", Msh->NbrPri);
  if ( Msh->NbrHex > 0 ) printf("  Hexahedra  : %d\n", Msh->NbrHex);
  if ( Msh->NbrPyr > 0 ) printf("  Pyramids   : %d\n", Msh->NbrPyr);
  
  if ( Msh->Sol ){
    printf("Solution: %s\n", Msh->SolNam);
    printf("  Solution fields: %d\n", Msh->NbrFld);
    for (i=0; i<Msh->NbrFld; i++)
      printf("    %s\n", Msh->SolTag[i]);
  }
  
  printf("-------------------------\n\n");
    
}

int RemoveUnconnectedVertices(Mesh *Msh)
{

  int iVer, iTri, NbrVer, iEfr, j;
  
  int *Tag = (int*)malloc(sizeof(int)*(Msh->NbrVer+1));
  memset(Tag,0,sizeof(int)*(Msh->NbrVer+1));
  
  for (iTri=1; iTri<=Msh->NbrTri; iTri++) {
    for (j=0; j<3; j++) 
      Tag[Msh->Tri[iTri][j]] = 1;
  }
  
  NbrVer=0;
  for (iVer=1; iVer<=Msh->NbrVer; iVer++) {  
    if ( Tag[iVer] != 1 ) continue;
    
    NbrVer++;
    Tag[iVer] = NbrVer;
    
    Msh->Ver[NbrVer][0] = Msh->Ver[iVer][0];
    Msh->Ver[NbrVer][1] = Msh->Ver[iVer][1];
    Msh->Ver[NbrVer][2] = Msh->Ver[iVer][2];
    
  }

  for (iTri=1; iTri<=Msh->NbrTri; iTri++) {
    for (j=0; j<3; j++) 
      Msh->Tri[iTri][j] = Tag[Msh->Tri[iTri][j]];
  }
  
  for (iEfr=1; iEfr<=Msh->NbrEfr; iEfr++) {
    for (j=0; j<2; j++) 
      Msh->Efr[iEfr][j] = Tag[Msh->Efr[iEfr][j]];
  }  
  
  printf(" -- Info : %d unconnected vertices removed.\n", Msh->NbrVer-NbrVer);
  
  Msh->NbrVer = NbrVer;
  
  if ( Tag )
    free(Tag);
  
  return 1;
}

Mesh *SetupMeshAndSolution (char *MshNam, char *SolNam)
{
  Mesh *Msh = NULL;
  Conn *Con = NULL;
  int SizMsh[GmfMaxSizMsh+1];
  int FilTyp = GetInputFileType(MshNam);
  int val=0;
  
  if ( !FilTyp ) {
    printf("  ## ERROR SetupMeshAndSolution : Unknown mesh format.\n");
    return NULL;
  }
  
  if ( FilTyp == FILE_SU2MSH )
    AddSU2MeshSize(MshNam, SizMsh);
  else if ( FilTyp == FILE_GMF )
    AddGMFMeshSize(MshNam, SizMsh);
  
  Msh = AllocMesh(SizMsh);
  Con = AllocConn(SizMsh[GmfVertices]);
  
  if ( FilTyp == FILE_SU2MSH ) {
    LoadSU2Mesh(MshNam, Msh, Con);
    LoadSU2ConnData(MshNam, Msh, Con);
    if ( strcmp(SolNam,"") ) {
      FilTyp = GetInputFileType(SolNam);
      if (FilTyp == FILE_SU2BIN)
        val = LoadSU2SolutionBin(SolNam, Msh);
      else
        val = LoadSU2Solution(SolNam, Msh);
    }
    
    if ( val != 1 ) {
      Msh->Sol = NULL;
    }
    
  }  
  else if ( FilTyp == FILE_GMF ){
    LoadGMFMesh(MshNam, Msh, Con);
    LoadGMFConnData(MshNam, Msh, Con);
    if ( strcmp(SolNam,"") )
      LoadGMFSolution(SolNam, Msh);
  }

  CheckVolumeElementOrientation(Msh);
  CheckSurfaceElementOrientation(Msh, Con);

  if (Con) FreeConn(Con);

  return Msh;
}

Mesh *SetupSolution (char *SolNam, int NbrVer, int Dim)
{
  Mesh *Sol = NULL;
  int SizSol[GmfMaxSizMsh+1];
  int FilTyp = GetInputFileType(SolNam);
  int val=0;
  
  if ( !FilTyp ) {
    printf("  ## ERROR SetupMeshAndSolution : Unknown mesh format.\n");
    return NULL;
  }
  
  for (int i = 0; i < GmfMaxSizMsh+1; i++) SizSol[i] = 0;
  SizSol[GmfVertices] = NbrVer;
  
  Sol = AllocMesh(SizSol);
  Sol->NbrVer = NbrVer;
  Sol->Dim    = Dim;
  
  if ( FilTyp == FILE_SU2BIN ) {
    val = LoadSU2SolutionBin(SolNam, Sol);  
    if ( val != 1 ) {
      Sol->Sol = NULL;
    }
  }
  else if ( FilTyp == FILE_SU2CSV ) {   
    val = LoadSU2Solution(SolNam, Sol);  
    if ( val != 1 ) {
      Sol->Sol = NULL;
    }
  }
  else if ( FilTyp == FILE_GMFSOL ){
    LoadGMFSolution(SolNam, Sol);
  }
  return Sol;
}

void CheckVolumeElementOrientation(Mesh *Msh) {

  double3* Ver = Msh->Ver;
  int i;
  
  /*--- Check tri and quad orientation ---*/
  if (Msh->Dim == 2) {
    for (i = 1; i <= Msh->NbrTri; i++) {
      CheckTriangle(Msh->Tri, Msh->Ver, i);
    }
    for (i = 1; i <= Msh->NbrQua; i++) {
      CheckQuadrilateral(Msh->Qua, Msh->Ver, i);
    }
  }
  /*--- Check tet, pyr, pri, and hex orientation ---*/
  else {
    for (i = 1; i <= Msh->NbrTet; i++) {
      CheckTetrahedron(Msh->Tet, Msh->Ver, i);
    }
    for (i = 1; i <= Msh->NbrPyr; i++) {
      CheckPyramid(Msh->Pyr, Msh->Ver, i);
    }
    for (i = 1; i <= Msh->NbrPri; i++) {
      CheckPrism(Msh->Pri, Msh->Ver, i);
    }
    for (i = 1; i <= Msh->NbrHex; i++) {
      CheckHexahedron(Msh->Hex, Msh->Ver, i);
    }
  }

}

void CheckSurfaceElementOrientation(Mesh *Msh, Conn *Con) {

  double3* Ver = Msh->Ver;
  int i, j, k, l, idx;
  int nbrcheck = 0;
  int check[8];

  for (k = 0; k < 8; k++) check[k] = 0;
  
  /*--- Check edge orientation ---*/
  if (Msh->Dim == 2) {
    for (i = 1; i <= Msh->NbrEfr; i++) {
      nbrcheck = 0;
      for (k = 0; k < 4; k++) check[k] = 0;

      /*--- Check if edge is part of quad ---*/
      for (j = 0; j < 2; j++) {
        for (k = 0; k < Con->NbrQua[Msh->Efr[i][j]]; k++) {
          idx = Con->Qua[Msh->Efr[i][j]][k];
          for (l = 0; l < 4; l++) {
            if ((Msh->Qua[idx][l] == Msh->Efr[i][0]) ||
                (Msh->Qua[idx][l] == Msh->Efr[i][1])) {
              check[l] = 1;
              nbrcheck++;
            }
          }
          if (nbrcheck == 2) {
            for (l = 0; l < 4; l++) {
              if (check[l]==0) {
                CheckBoundEdge(Msh->Efr, Msh->Ver, i, Msh->Qua[idx][l]);
                break;
              }
            }
            break;
          }
          else {
            nbrcheck = 0;
            for (l = 0; l < 4; l++) check[l] = 0;
          }
        }
      }

      /*--- Check if edge is part of triangles ---*/
      if (nbrcheck < 2) {
        for (j = 0; j < 2; j++) {
          for (k = 0; k < Con->NbrTri[Msh->Efr[i][j]]; k++) {
            idx = Con->Tri[Msh->Efr[i][j]][k];
            for (l = 0; l < 3; l++) {
              if ((Msh->Tri[idx][l] == Msh->Efr[i][0]) ||
                  (Msh->Tri[idx][l] == Msh->Efr[i][1])) {
                check[l] = 1;
                nbrcheck++;
              }
            }
            if (nbrcheck == 2) {
              for (l = 0; l < 3; l++) {
                if (check[l]==0) {
                  CheckBoundEdge(Msh->Efr, Msh->Ver, i, Msh->Tri[idx][l]);
                  break;
                }
              }
              break;
            }
            else {
              nbrcheck = 0;
              for (l = 0; l < 3; l++) check[l] = 0;
            }
          }
        }
      }
    }
  }

  /*--- Check tri and quad orientation ---*/
  else {
    for (i = 1; i <= Msh->NbrTri; i++) {
      nbrcheck = 0;
      for (k = 0; k < 4; k++) check[k] = 0;

      /*--- Check if tri is part of tets ---*/
      for (j = 0; j < 3; j++) {
        for (k = 0; k < Con->NbrTet[Msh->Tri[i][j]]; k++) {
          idx = Con->Tet[Msh->Tri[i][j]][k];
          for (l = 0; l < 4; l++) {
            if ((Msh->Tet[idx][l] == Msh->Tri[i][0]) ||
                (Msh->Tet[idx][l] == Msh->Tri[i][1]) ||
                (Msh->Tet[idx][l] == Msh->Tri[i][2])) {
              check[l] = 1;
              nbrcheck++;
            }
          }
          if (nbrcheck == 3) {
            for (l = 0; l < 4; l++) {
              if (check[l]==0) {
                CheckBoundTriangle(Msh->Tri, Msh->Ver, i, Msh->Tet[idx][l]);
                break;
              }
            }
            break;
          }
          else {
            nbrcheck = 0;
            for (l = 0; l < 4; l++) check[l] = 0;
          }
        }
      }

      /*--- Check if tri is part of pris ---*/
      if (nbrcheck < 3) {
        for (j = 0; j < 3; j++) {
          for (k = 0; k < Con->NbrPri[Msh->Tri[i][j]]; k++) {
            idx = Con->Pri[Msh->Tri[i][j]][k];
            for (l = 0; l < 6; l++) {
              if ((Msh->Pri[idx][l] == Msh->Tri[i][0]) ||
                  (Msh->Pri[idx][l] == Msh->Tri[i][1]) ||
                  (Msh->Pri[idx][l] == Msh->Tri[i][2])) {
                check[l] = 1;
                nbrcheck++;
              }
            }
            if (nbrcheck == 3) {
              for (l = 0; l < 6; l++) {
                if (check[l]==0) {
                  CheckBoundTriangle(Msh->Tri, Msh->Ver, i, Msh->Pri[idx][l]);
                  break;
                }
              }
              break;
            }
            else {
            nbrcheck = 0;
            for (l = 0; l < 6; l++) check[l] = 0;
          }
          }
        }
      }

      /*--- Check if tri is part of pyrs ---*/
      if (nbrcheck < 3) {
        for (j = 0; j < 3; j++) {
          for (k = 0; k < Con->NbrPyr[Msh->Tri[i][j]]; k++) {
            idx = Con->Pyr[Msh->Tri[i][j]][k];
            for (l = 0; l < 5; l++) {
              if ((Msh->Pyr[idx][l] == Msh->Tri[i][0]) ||
                  (Msh->Pyr[idx][l] == Msh->Tri[i][1]) ||
                  (Msh->Pyr[idx][l] == Msh->Tri[i][2])) {
                check[l] = 1;
                nbrcheck++;
              }
            }
            if (nbrcheck == 3) {
              for (l = 0; l < 5; l++) {
                if (check[l]==0) {
                  CheckBoundTriangle(Msh->Tri, Msh->Ver, i, Msh->Pyr[idx][l]);
                  break;
                }
              }
              break;
            }
            else {
            nbrcheck = 0;
            for (l = 0; l < 5; l++) check[l] = 0;
          }
          }
        }
      }
    }

    for (i = 1; i <= Msh->NbrQua; i++) {
      nbrcheck = 0;
      for (k = 0; k < 8; k++) check[k] = 0;

      /*--- Check if qua is part of hexs ---*/
      for (j = 0; j < 4; j++) {
        for (k = 0; k < Con->NbrHex[Msh->Qua[i][j]]; k++) {
          idx = Con->Hex[Msh->Qua[i][j]][k];
          for (l = 0; l < 8; l++) {
            if ((Msh->Hex[idx][l] == Msh->Qua[i][0]) ||
                (Msh->Hex[idx][l] == Msh->Qua[i][1]) ||
                (Msh->Hex[idx][l] == Msh->Qua[i][2]) ||
                (Msh->Hex[idx][l] == Msh->Qua[i][3])) {
              check[l] = 1;
              nbrcheck++;
            }
          }
          if (nbrcheck == 4) {
            for (l = 0; l < 8; l++) {
              if (check[l]==0) {
                CheckBoundQuadrilateral(Msh->Qua, Msh->Ver, i, Msh->Hex[idx][l]);
                break;
              }
            }
            break;
          }
          else {
            nbrcheck = 0;
            for (l = 0; l < 8; l++) check[l] = 0;
          }
        }
      }

      /*--- Check if qua is part of pris ---*/
      if (nbrcheck < 4) {
        for (j = 0; j < 4; j++) {
          for (k = 0; k < Con->NbrPri[Msh->Qua[i][j]]; k++) {
            idx = Con->Pri[Msh->Qua[i][j]][k];
            for (l = 0; l < 6; l++) {
              if ((Msh->Pri[idx][l] == Msh->Qua[i][0]) ||
                  (Msh->Pri[idx][l] == Msh->Qua[i][1]) ||
                  (Msh->Pri[idx][l] == Msh->Qua[i][2]) ||
                  (Msh->Pri[idx][l] == Msh->Qua[i][3])) {
                check[l] = 1;
                nbrcheck++;
              }
            }
            if (nbrcheck == 4) {
              for (l = 0; l < 6; l++) {
                if (check[l]==0) {
                  CheckBoundQuadrilateral(Msh->Qua, Msh->Ver, i, Msh->Pri[idx][l]);
                  break;
                }
              }
              break;
            }
            else {
              nbrcheck = 0;
              for (l = 0; l < 6; l++) check[l] = 0;
            }
          }
        }
      }

      /*--- Check if tri is part of pyrs ---*/
      if (nbrcheck < 4) {
        for (j = 0; j < 4; j++) {
          for (k = 0; k < Con->NbrPyr[Msh->Qua[i][j]]; k++) {
            idx = Con->Pyr[Msh->Qua[i][j]][k];
            for (l = 0; l < 5; l++) {
              if ((Msh->Pyr[idx][l] == Msh->Qua[i][0]) ||
                  (Msh->Pyr[idx][l] == Msh->Qua[i][1]) ||
                  (Msh->Pyr[idx][l] == Msh->Qua[i][2]) ||
                  (Msh->Pyr[idx][l] == Msh->Qua[i][3])) {
                check[l] = 1;
                nbrcheck++;
              }
            }
            if (nbrcheck == 4) {
              for (l = 0; l < 5; l++) {
                if (check[l]==0) {
                  CheckBoundQuadrilateral(Msh->Qua, Msh->Ver, i, Msh->Pyr[idx][l]);
                  break;
                }
              }
              break;
            }
            else {
              nbrcheck = 0;
              for (l = 0; l < 5; l++) check[l] = 0;
            }
          }
        }
      }
    }
  }

}

void CopyBoundaryMarkers (Mesh *Msh, Mesh *BndMsh)
{
  int iMark;
  for (iMark = 0; iMark <= BndMsh->NbrMarkers; iMark++) {
    strcpy(Msh->Markers[iMark], BndMsh->Markers[iMark]);
  }
}

void switchHexIdx(int *idx, int *swi)
{
  int im;
  im = imin(8,idx);  
    
  switch( im ) { 
  
  case 0:
    swi[0] = idx[0];
    swi[1] = idx[1];
    swi[2] = idx[2];
    swi[3] = idx[3];
    swi[4] = idx[4];
    swi[5] = idx[5];
    swi[6] = idx[6];
    swi[7] = idx[7];
    break;
    
  case 1:
    swi[0] = idx[1];
    swi[1] = idx[2];
    swi[2] = idx[3];
    swi[3] = idx[0];
    swi[4] = idx[5];
    swi[5] = idx[6];
    swi[6] = idx[7];
    swi[7] = idx[4];
    break;
  
  case 2:
    swi[0] = idx[2];
    swi[1] = idx[3];
    swi[2] = idx[0];
    swi[3] = idx[1];
    swi[4] = idx[6];
    swi[5] = idx[7];
    swi[6] = idx[4];
    swi[7] = idx[5];
    break;
  
  case 3:
    swi[0] = idx[3];
    swi[1] = idx[0];
    swi[2] = idx[1];
    swi[3] = idx[2];
    swi[4] = idx[7];
    swi[5] = idx[4];
    swi[6] = idx[5];
    swi[7] = idx[6];
    break; 
    
  case 4:
    swi[0] = idx[4];
    swi[1] = idx[7];
    swi[2] = idx[6];
    swi[3] = idx[5];
    swi[4] = idx[0];
    swi[5] = idx[3];
    swi[6] = idx[2];
    swi[7] = idx[1];
    break; 
    
  case 5:
    swi[0] = idx[5];
    swi[1] = idx[4];
    swi[2] = idx[7];
    swi[3] = idx[6];
    swi[4] = idx[1];
    swi[5] = idx[0];
    swi[6] = idx[3];
    swi[7] = idx[2];
    break;      
   
  case 6:
    swi[0] = idx[6];
    swi[1] = idx[5];
    swi[2] = idx[4];
    swi[3] = idx[7];
    swi[4] = idx[2];
    swi[5] = idx[1];
    swi[6] = idx[0];
    swi[7] = idx[3];
    break;
    
  case 7:
    swi[0] = idx[7];
    swi[1] = idx[6];
    swi[2] = idx[5];
    swi[3] = idx[4];
    swi[4] = idx[3];
    swi[5] = idx[2];
    swi[6] = idx[1];
    swi[7] = idx[0];
    break;         
  }   
}

void switchQuaIdx(int *idx, int *swi)
{
  int im = 0;
  
  im = imin(4,idx);  
  
  switch( im ) { 
  
  case 0:
    swi[0] = idx[0];
    swi[1] = idx[1];
    swi[2] = idx[2];
    swi[3] = idx[3];
    break;
    
  case 1:
    swi[0] = idx[1];
    swi[1] = idx[2];
    swi[2] = idx[3];
    swi[3] = idx[0];
    break;
  
  case 2:
    swi[0] = idx[2];
    swi[1] = idx[3];
    swi[2] = idx[0];
    swi[3] = idx[1];
    break;
  
  case 3:
    swi[0] = idx[3];
    swi[1] = idx[0];
    swi[2] = idx[1];
    swi[3] = idx[2];
    break;  
   
  }
  
}

void switchTetIdx(int *idx, int *swi)
{
  
  if ( idx[1] < idx[0] ) {  
    
    if ( idx[2] < idx[1] ) {   
      if ( idx[3] < idx[2] ) {  
        swi[0] = idx[3];   swi[1] = idx[2];
        swi[2] = idx[1];   swi[3] = idx[0];
      }
      else if ( idx[0] < idx[3] ){  
        swi[0] = idx[2];   swi[1] = idx[0];
        swi[2] = idx[1];   swi[3] = idx[3];
      }
      else { 
        swi[0] = idx[2];   swi[1] = idx[1];
        swi[2] = idx[3];   swi[3] = idx[0];
      }
    }   
    else if ( idx[0] < idx[2] ) {   
      if ( idx[3] < idx[1] ) {  
        swi[0] = idx[3];   swi[1] = idx[1];
        swi[2] = idx[0];   swi[3] = idx[2];
      }
      else if ( idx[2] < idx[3] ){ 
        swi[0] = idx[1];   swi[1] = idx[2];
        swi[2] = idx[0];   swi[3] = idx[3];
      }
      else {  
        swi[0] = idx[1];   swi[1] = idx[0];
        swi[2] = idx[3];   swi[3] = idx[2];
      }
    }
    
    else { 
      if ( idx[3] < idx[1] ) {  
        swi[0] = idx[3];   swi[1] = idx[2];
        swi[2] = idx[1];   swi[3] = idx[0];
      }
      else if ( idx[0] < idx[3] ){  
        swi[0] = idx[1];   swi[1] = idx[2];
        swi[2] = idx[0];   swi[3] = idx[3];
      }
      else {  
        swi[0] = idx[1];   swi[1] = idx[3];
        swi[2] = idx[2];   swi[3] = idx[0];
      }
    }   

  }
  
  else {   
    
    if ( idx[2] < idx[0] ) {  
      if ( idx[3] < idx[2] ) { 
        swi[0] = idx[3];   swi[1] = idx[0];
        swi[2] = idx[2];   swi[3] = idx[1];
      }
      else if ( idx[1] < idx[3] ){  
        swi[0] = idx[2];   swi[1] = idx[0];
        swi[2] = idx[1];   swi[3] = idx[3];
      }
      else { 
        swi[0] = idx[2];   swi[1] = idx[3];
        swi[2] = idx[0];   swi[3] = idx[1];
      }
    }   
    else if ( idx[1] < idx[2] ) {   
      if ( idx[3] < idx[0] ) { 
        swi[0] = idx[3];   swi[1] = idx[1];
        swi[2] = idx[0];   swi[3] = idx[2];
      }
      else if ( idx[2] < idx[3] ){  
        swi[0] = idx[0];   swi[1] = idx[1];
        swi[2] = idx[2];   swi[3] = idx[3];
      }
      else {  
        swi[0] = idx[0];   swi[1] = idx[3];
        swi[2] = idx[1];   swi[3] = idx[2];
      }
    }
    
    else {  
      if ( idx[3] < idx[0] ) { 
        swi[0] = idx[3];   swi[1] = idx[0];
        swi[2] = idx[2];   swi[3] = idx[1];
      }
      else if ( idx[1] < idx[3] ){ 
        swi[0] = idx[0];   swi[1] = idx[1];
        swi[2] = idx[2];   swi[3] = idx[3];
      }
      else { 
        swi[0] = idx[0];   swi[1] = idx[2];
        swi[2] = idx[3];   swi[3] = idx[1];
      }
    }   
    
  }
  
}

void switchTriIdx(int *idx, int *swi)
{
  if ( idx[1] < idx[0] ) { 
    if ( idx[2] < idx[1] ) {  
      swi[0] = idx[2];
      swi[1] = idx[0];
      swi[2] = idx[1];
    }   
    else { 
      swi[0] = idx[1];
      swi[1] = idx[2];
      swi[2] = idx[0];
    }   
  }
  
  else {
    if ( idx[2] < idx[0] ) {  
        swi[0] = idx[2];
        swi[1] = idx[0];
        swi[2] = idx[1];
    }
    else {  
      swi[0] = idx[0];
      swi[1] = idx[1];
      swi[2] = idx[2];
    }
  }

}

int GetInputFileType (char *FilNam) 
{
  char *ext=NULL;  
  
  if ( !FilNam ) {
    printf("\n ## ERROR GetInputFileType : No file name provided.\n\n");
     exit(1);
  }
  
  ext = strrchr(FilNam, '.');
  
  if (!ext) {
    return 0;    
  } else {      
      ext = ext+1;
      if ( strcmp(ext,"su2") == 0  ) {
        return FILE_SU2MSH;
      }
      else if ( strcmp(ext,"csv") == 0  ) {
        return FILE_SU2CSV;
      }
      else if ( strcmp(ext,"dat") == 0  ) {
        return FILE_SU2BIN;
      }
      else if ( strcmp(ext,"mesh") == 0 || strcmp(ext,"meshb") == 0 ) {
        return FILE_GMF;
      }
      else if ( strcmp(ext,"sol") == 0 || strcmp(ext,"solb") == 0 ) {
        return FILE_GMFSOL;
      }
      else if ( strcmp(ext,"geo") == 0  ) {
        return FILE_GEO;
      }
      else {
        return 0;
      }
  }
}

//--- Transforms all letters to lower case
int Str2Lower(char *buff)
{
  unsigned int iChr;
  
  for (iChr=0; iChr<strlen(buff); iChr++) 
    buff[iChr] = tolower( buff[iChr] );

  return 1;
}

//--- Removes all occurences of char c from str
void StrRemoveChars (char* s, char ch) {
  char *p = s;
  while (*s) {
      if (*s != ch)
          *p++ = *s;
      s++;
  }
  *p = 0;
}
