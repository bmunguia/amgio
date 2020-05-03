#include "amgio.h"

/*
Victorien Menier Feb 2016
Brian Munguía Apr 2020
*/

int AddGMFMeshSize (char *MshNam, int *SizMsh)
{
  int dim, FilVer, InpMsh, i; 
  
  for (i=0; i<GmfMaxKwd; i++)
    SizMsh[i] = 0;

  if ( !(InpMsh = GmfOpenMesh(MshNam,GmfRead,&FilVer,&dim)) ) {
    fprintf(stderr,"  ## ERROR: Mesh data file %s.mesh[b] not found ! ",MshNam);
    return 0;
  }
  
  //--- get number of entities
  SizMsh[GmfDimension]      = dim;
  SizMsh[GmfVertices]       = GmfStatKwd(InpMsh, GmfVertices);
  SizMsh[GmfTriangles]      = GmfStatKwd(InpMsh, GmfTriangles);
  SizMsh[GmfTetrahedra]     = GmfStatKwd(InpMsh, GmfTetrahedra);
  SizMsh[GmfEdges]          = GmfStatKwd(InpMsh, GmfEdges);
  SizMsh[GmfCorners]        = GmfStatKwd(InpMsh, GmfCorners);
  SizMsh[GmfPrisms]         = GmfStatKwd(InpMsh, GmfPrisms);
  SizMsh[GmfPyramids]       = GmfStatKwd(InpMsh, GmfPyramids);
  SizMsh[GmfHexahedra]      = GmfStatKwd(InpMsh, GmfHexahedra);
  SizMsh[GmfQuadrilaterals] = GmfStatKwd(InpMsh, GmfQuadrilaterals);
  
  if ( SizMsh[GmfVertices] <= 0 ) {
    fprintf(stderr,"\n  ## ERROR: NO VERTICES. IGNORED\n");
    return 0;
  }
  
  return 1;
}

int LoadGMFMesh (char *MshNam, Mesh *Msh, Conn *Con)
{
  int i, j, idx;
  int dim, FilVer, InpMsh, ref; 
  double bufDbl[3];
  int bufInt[8], is[8];
  
  int NbrVer, NbrTri, NbrEfr, NbrCor, NbrTet, NbrHex, NbrQua, NbrPri, NbrPyr;
  
  if ( !(InpMsh = GmfOpenMesh(MshNam,GmfRead,&FilVer,&dim)) ) {
    printf("  ## ERROR: Mesh data file %s.mesh[b] not found ! \n",MshNam);
    return 0;
  }

  Msh->Dim = dim;
  strcpy(Msh->MshNam, MshNam);
  Msh->FilTyp = FILE_GMF;
    
  Msh->NbrVer = Msh->NbrTri = Msh->NbrEfr = 0;
  Msh->NbrTet = Msh->NbrHex = Msh->NbrQua = Msh->NbrPri = Msh->NbrPyr = 0;
  Msh->NbrCor = 0;
  
  NbrVer = NbrTri = NbrEfr = 0;
  NbrTet = NbrHex = NbrQua = NbrPri = NbrPyr = 0;
  NbrCor = 0;
  
  //--- Read vertices
  NbrVer = GmfStatKwd(InpMsh, GmfVertices);  
  
  GmfGotoKwd(InpMsh, GmfVertices);
  
  if ( Msh->Dim == 2 ) {
    for (i=1; i<=NbrVer; ++i) {
      GmfGetLin(InpMsh, GmfVertices, &bufDbl[0], &bufDbl[1], &ref);
      Msh->NbrVer++;
      AddVertex(Msh, Msh->NbrVer, bufDbl);
    }    
  }
  else {
    for (i=1; i<=NbrVer; ++i) {
      GmfGetLin(InpMsh, GmfVertices, &bufDbl[0], &bufDbl[1], &bufDbl[2], &ref);
      Msh->NbrVer++;
      AddVertex(Msh, Msh->NbrVer, bufDbl);
    }  
  }

  //--- Read corners
  NbrCor = GmfStatKwd(InpMsh, GmfCorners);  
  GmfGotoKwd(InpMsh, GmfCorners);
  for (i=1; i<=NbrCor; ++i) {
    GmfGetLin(InpMsh, GmfCorners, &bufInt[0]);
    Msh->NbrCor++;
    AddCorner(Msh,Msh->NbrCor,bufInt);
  }

  //--- Read boundary edges
  NbrEfr = GmfStatKwd(InpMsh, GmfEdges);  
  GmfGotoKwd(InpMsh, GmfEdges);
  for (i=1; i<=NbrEfr; ++i) {
    GmfGetLin(InpMsh, GmfEdges, &bufInt[0], &bufInt[1], &ref);
    Msh->NbrEfr++;
    AddEdge(Msh,Msh->NbrEfr,bufInt,ref);
    for (j = 0; j < 2; j++) Con->NbrEfr[bufInt[j]]++;
  }

  //--- Read Triangles
  NbrTri = GmfStatKwd(InpMsh, GmfTriangles);  
  GmfGotoKwd(InpMsh, GmfTriangles);
  for (i=1; i<=NbrTri; ++i) {
    GmfGetLin(InpMsh, GmfTriangles, &bufInt[0], &bufInt[1], &bufInt[2], &ref);
    Msh->NbrTri++;
    AddTriangle(Msh,Msh->NbrTri,bufInt,ref);
    for (j = 0; j < 3; j++) Con->NbrTri[bufInt[j]]++;
  }
  
  //--- Read Quads
  NbrQua = GmfStatKwd(InpMsh, GmfQuadrilaterals);  
  GmfGotoKwd(InpMsh, GmfQuadrilaterals);
  for (i=1; i<=NbrQua; ++i) {
    GmfGetLin(InpMsh, GmfQuadrilaterals, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3], &ref);
    Msh->NbrQua++;
    AddQuadrilateral(Msh,Msh->NbrQua,bufInt,ref);
    for (j = 0; j < 4; j++) Con->NbrQua[bufInt[j]]++;
  }

  //--- Read tetrahedra
  NbrTet = GmfStatKwd(InpMsh, GmfTetrahedra);  
  GmfGotoKwd(InpMsh, GmfTetrahedra);
  for (i=1; i<=NbrTet; ++i) {
    GmfGetLin(InpMsh, GmfTetrahedra, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3], &ref);
    Msh->NbrTet++;
    AddTetrahedron(Msh,Msh->NbrTet,bufInt,ref);
    for (j = 0; j < 4; j++) Con->NbrTet[bufInt[j]]++;
  }

  //--- Read Pyramids
  NbrPyr = GmfStatKwd(InpMsh, GmfPyramids);  
  GmfGotoKwd(InpMsh, GmfPyramids);
  for (i=1; i<=NbrPyr; ++i) {
    GmfGetLin(InpMsh, GmfPyramids, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3],  &bufInt[4], &ref);
    Msh->NbrPyr++;
    AddPyramid(Msh,Msh->NbrPyr,bufInt,ref);
    for (j = 0; j < 5; j++) Con->NbrPyr[bufInt[j]]++;
  }

  //--- Read Prisms
  NbrPri = GmfStatKwd(InpMsh, GmfPrisms);  
  GmfGotoKwd(InpMsh, GmfPrisms);
  for (i=1; i<=NbrPri; ++i) {
    GmfGetLin(InpMsh, GmfPrisms, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3],  &bufInt[4],  &bufInt[5], &ref);
    Msh->NbrPri++;
    AddPrism(Msh,Msh->NbrPri,bufInt,ref);
    for (j = 0; j < 6; j++) Con->NbrPri[bufInt[j]]++;
  }
  
  //--- Read Hexahedra
  NbrHex = GmfStatKwd(InpMsh, GmfHexahedra);  
  GmfGotoKwd(InpMsh, GmfHexahedra);
  for (i=1; i<=NbrHex; ++i) {
    GmfGetLin(InpMsh, GmfHexahedra, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3], &bufInt[4], &bufInt[5], &bufInt[6], &bufInt[7], &ref);
    Msh->NbrHex++;
    AddHexahedron(Msh,Msh->NbrHex,bufInt,ref);
    for (j = 0; j < 8; j++) Con->NbrHex[bufInt[j]]++;
  }
  
  if ( !GmfCloseMesh(InpMsh) ) {
    printf("  ## ERROR: Cannot close solution file %s ! \n",MshNam);
    return 0;
  }
    
  return 1;
}

int LoadGMFConnData (char *MshNam, Mesh *Msh, Conn *Con)
{
  int i, j, idx;
  int dim, FilVer, InpMsh, ref; 
  double bufDbl[3];
  int bufInt[8], is[8];
  
  int NbrVer, NbrTri, NbrEfr, NbrCor, NbrTet, NbrHex, NbrQua, NbrPri, NbrPyr;

  for (int iVer = 1; iVer <= Msh->NbrVer; iVer++) {
    if (Con->NbrEfr[iVer] > 0)
      Con->Efr[iVer] = (int*)malloc(sizeof(int)*(Con->NbrEfr[iVer]));
    if (Con->NbrTri[iVer] > 0)
      Con->Tri[iVer] = (int*)malloc(sizeof(int)*(Con->NbrTri[iVer]));
    if (Con->NbrTet[iVer] > 0)
      Con->Tet[iVer] = (int*)malloc(sizeof(int)*(Con->NbrTet[iVer]));
    if (Con->NbrQua[iVer] > 0)
      Con->Qua[iVer] = (int*)malloc(sizeof(int)*(Con->NbrQua[iVer]));
    if (Con->NbrHex[iVer] > 0)
      Con->Hex[iVer] = (int*)malloc(sizeof(int)*(Con->NbrHex[iVer]));
    if (Con->NbrPri[iVer] > 0)
      Con->Pri[iVer] = (int*)malloc(sizeof(int)*(Con->NbrPri[iVer]));
    if (Con->NbrPyr[iVer] > 0)
      Con->Pyr[iVer] = (int*)malloc(sizeof(int)*(Con->NbrPyr[iVer]));

    Con->NbrEfr[iVer] = Con->NbrTri[iVer] = Con->NbrTet[iVer] = Con->NbrQua[iVer] = 0;
    Con->NbrHex[iVer] = Con->NbrPri[iVer] = Con->NbrPyr[iVer] = 0;
  }
  
  if ( !(InpMsh = GmfOpenMesh(MshNam,GmfRead,&FilVer,&dim)) ) {
    printf("  ## ERROR: Mesh data file %s.mesh[b] not found ! \n",MshNam);
    return 0;
  }
    
  Msh->NbrTri = Msh->NbrEfr = 0;
  Msh->NbrTet = Msh->NbrHex = Msh->NbrQua = Msh->NbrPri = Msh->NbrPyr = 0;
  Msh->NbrCor = 0;
  
  NbrTri = NbrEfr = 0;
  NbrTet = NbrHex = NbrQua = NbrPri = NbrPyr = 0;
  NbrCor = 0;

  //--- Read boundary edges
  NbrEfr = GmfStatKwd(InpMsh, GmfEdges);  
  GmfGotoKwd(InpMsh, GmfEdges);
  for (i=1; i<=NbrEfr; ++i) {
    GmfGetLin(InpMsh, GmfEdges, &bufInt[0], &bufInt[1], &ref);
    Msh->NbrEfr++;
    for (j = 0; j < 2; j++) {
      Con->Efr[bufInt[j]][Con->NbrEfr[bufInt[j]]] = Msh->NbrEfr;
      Con->NbrEfr[bufInt[j]]++;
    }
  }

  //--- Read Triangles
  NbrTri = GmfStatKwd(InpMsh, GmfTriangles);  
  GmfGotoKwd(InpMsh, GmfTriangles);
  for (i=1; i<=NbrTri; ++i) {
    GmfGetLin(InpMsh, GmfTriangles, &bufInt[0], &bufInt[1], &bufInt[2], &ref);
    Msh->NbrTri++;
    for (j = 0; j < 3; j++) {
      Con->Tri[bufInt[j]][Con->NbrEfr[bufInt[j]]] = Msh->NbrTri;
      Con->NbrTri[bufInt[j]]++;
    }
  }
  
  //--- Read Quads
  NbrQua = GmfStatKwd(InpMsh, GmfQuadrilaterals);  
  GmfGotoKwd(InpMsh, GmfQuadrilaterals);
  for (i=1; i<=NbrQua; ++i) {
    GmfGetLin(InpMsh, GmfQuadrilaterals, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3], &ref);
    Msh->NbrQua++;
    for (j = 0; j < 4; j++) {
      Con->Qua[bufInt[j]][Con->NbrQua[bufInt[j]]] = Msh->NbrQua;
      Con->NbrQua[bufInt[j]]++;
    }
  }

  //--- Read tetrahedra
  NbrTet = GmfStatKwd(InpMsh, GmfTetrahedra);  
  GmfGotoKwd(InpMsh, GmfTetrahedra);
  for (i=1; i<=NbrTet; ++i) {
    GmfGetLin(InpMsh, GmfTetrahedra, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3], &ref);
    Msh->NbrTet++;
    for (j = 0; j < 4; j++) {
      Con->Tet[bufInt[j]][Con->NbrTet[bufInt[j]]] = Msh->NbrTet;
      Con->NbrTet[bufInt[j]]++;
    }
  }

  //--- Read Pyramids
  NbrPyr = GmfStatKwd(InpMsh, GmfPyramids);  
  GmfGotoKwd(InpMsh, GmfPyramids);
  for (i=1; i<=NbrPyr; ++i) {
    GmfGetLin(InpMsh, GmfPyramids, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3],  &bufInt[4], &ref);
    Msh->NbrPyr++;
    for (j = 0; j < 5; j++) {
      Con->Pyr[bufInt[j]][Con->NbrPyr[bufInt[j]]] = Msh->NbrPyr;
      Con->NbrPyr[bufInt[j]]++;
    }
  }

  //--- Read Prisms
  NbrPri = GmfStatKwd(InpMsh, GmfPrisms);  
  GmfGotoKwd(InpMsh, GmfPrisms);
  for (i=1; i<=NbrPri; ++i) {
    GmfGetLin(InpMsh, GmfPrisms, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3],  &bufInt[4],  &bufInt[5], &ref);
    Msh->NbrPri++;
    for (j = 0; j < 6; j++) {
      Con->Pri[bufInt[j]][Con->NbrPri[bufInt[j]]] = Msh->NbrPri;
      Con->NbrPri[bufInt[j]]++;
    }
  }
  
  //--- Read Hexahedra
  NbrHex = GmfStatKwd(InpMsh, GmfHexahedra);  
  GmfGotoKwd(InpMsh, GmfHexahedra);
  for (i=1; i<=NbrHex; ++i) {
    GmfGetLin(InpMsh, GmfHexahedra, &bufInt[0], &bufInt[1], &bufInt[2], &bufInt[3], &bufInt[4], &bufInt[5], &bufInt[6], &bufInt[7], &ref);
    Msh->NbrHex++;
    for (j = 0; j < 8; j++) {
      Con->Hex[bufInt[j]][Con->NbrHex[bufInt[j]]] = Msh->NbrHex;
      Con->NbrHex[bufInt[j]]++;
    }
  }
  
  if ( !GmfCloseMesh(InpMsh) ) {
    printf("  ## ERROR: Cannot close solution file %s ! \n",MshNam);
    return 0;
  }
    
  return 1;
}

int LoadGMFSolution(char *SolNam, Mesh *Msh)
{
  int    SolMsh,FilVer=0,dim=0,SolTyp,iVer,i, idxVer;
  int    NbrLin,NbrTyp,SolSiz,TypTab[ GmfMaxTyp ];
  double *bufDbl = NULL;
    
  if ( Msh->Sol )
  {
    printf("  ## ERROR LoadGMFSolution : Msh->Sol already allocated.\n");
    return 0;
  }
  
  if ( (Msh == NULL) || (SolNam == NULL) ) {
    printf("  ## ERROR: LoadGMFSolution : MESH/FILE NAME NOT ALLOCATED \n");
    return 0; 
  }
  
  if ( !(SolMsh = GmfOpenMesh(SolNam,GmfRead,&FilVer,&dim)) ) {
    printf(" Solution data file %s.sol[b] not found ! \n",SolNam);
    return 0;
  }
  
  strcpy(Msh->SolNam, SolNam);

  if ( dim != 2 && dim != 3 ) {
    printf("  ## ERROR: WRONG DIMENSION NUMBER. IGNORED\n");
    return 0;
  }
  
  SolTyp = GmfSolAtVertices;
  NbrLin = GmfStatKwd(SolMsh, SolTyp, &NbrTyp, &SolSiz, TypTab);
  
  if ( NbrLin == 0 ) {
    printf("  ## ERROR LoadGMFSolution : No SolAtVertices in the solution file !\n");
    return 0;
  }
  
  if ( NbrLin != Msh->NbrVer ) {
    printf("  ## ERROR LoadGMFSolution : The number of vertices does not match (NbrLin %d, NbrVer %d).\n", NbrLin, Msh->NbrVer);
    return 0;
  } 
  
  //--- Allocate Msh->Sol
  
  Msh->Sol = (double*) malloc(sizeof(double)*(Msh->NbrVer+1)*SolSiz);
  memset(Msh->Sol, 0, sizeof(double)*(Msh->NbrVer+1)*SolSiz);
  
  Msh->SolSiz = SolSiz;
  
  Msh->NbrFld = NbrTyp;
  Msh->FldTab = (int*) malloc(sizeof(int)*SolSiz);
  for (i=0; i<NbrTyp; i++) {
    Msh->FldTab[i] = TypTab[i];
    switch (TypTab[i]) {
      case GmfSca:
      sprintf(Msh->SolTag[i], "scalar_%d", i);
      break;
            
      case GmfVec:
      sprintf(Msh->SolTag[i], "vector_%d", i);
      break;
      
      case GmfSymMat:
      sprintf(Msh->SolTag[i], "SymMatrix_%d", i);
      break;
      
      case GmfMat:
      sprintf(Msh->SolTag[i], "Matrix_%d", i);
      break;
      
      default:
      sprintf(Msh->SolTag[i], "field_%d", i);  
    }
  }
  //--- Read solution
  GmfGotoKwd(SolMsh, SolTyp);
  
  bufDbl = (double*)malloc(sizeof(double)*SolSiz);
  for (iVer=1; iVer<=Msh->NbrVer; ++iVer) {
    GmfGetLin(SolMsh, SolTyp, bufDbl);
    idxVer = iVer*SolSiz;
    for (i=0; i<SolSiz; i++)
      Msh->Sol[idxVer+i] = bufDbl[i];
  }
  free(bufDbl);
  
  if ( !GmfCloseMesh(SolMsh) ) {
    printf("  ## ERROR: Cannot close solution file %s ! \n",SolNam);
    return 0;
  }
  
  return 1;
}

int WriteGMFMesh(char *nam, Mesh *Msh, int OptBin)
{
  int       OutMsh,FilVer,i, j;
  int       iVer,iTri,iEfr,iCor,iTet,iQua; 
  long long idx[8];
  char      OutFil[512];
  
  int Dim = Msh->Dim;
  int NbrVer  = Msh->NbrVer;
  int NbrTri  = Msh->NbrTri;
  int NbrEfr  = Msh->NbrEfr;
  int NbrCor  = Msh->NbrCor;
  int NbrQua  = Msh->NbrQua;
  int NbrTet  = Msh->NbrTet;
  int NbrHex  = Msh->NbrHex;
  int NbrPri  = Msh->NbrPri;
  int NbrPyr  = Msh->NbrPyr;
  double3*Ver = Msh->Ver;
  int4*Tri    = Msh->Tri;
  int3*Efr    = Msh->Efr;
  int*Cor     = Msh->Cor;
  int5*Tet    = Msh->Tet;
  int5*Qua    = Msh->Qua;
  int9*Hex    = Msh->Hex;
  int7*Pri    = Msh->Pri;
  int6*Pyr    = Msh->Pyr;
  
  
  //--- Define file name extension 
  strcpy(OutFil,nam);

  if ( OptBin == 1 )
    strcat(OutFil,".meshb");
  else
    strcat(OutFil,".mesh");
 
  FilVer = GmfDouble;
  
  //--- Open file
  if ( !(OutMsh = GmfOpenMesh(OutFil,GmfWrite,FilVer,Dim)) ) {
    printf("  ## ERROR: Cannot open mesh file %s ! \n",OutFil);
    return 0;
  }
  
  //--- Write vertices
  GmfSetKwd(OutMsh, GmfVertices, NbrVer);

  if ( Msh->Dim == 2 ) {
    for (iVer=1; iVer<=NbrVer; ++iVer) {
      GmfSetLin(OutMsh, GmfVertices,Ver[iVer][0],Ver[iVer][1],0);  
    }
  }
  else {
    
    
    for (iVer=1; iVer<=NbrVer; ++iVer) {
      GmfSetLin(OutMsh, GmfVertices,Ver[iVer][0],Ver[iVer][1],Ver[iVer][2],0);  
    }
  }

  if ( Msh->NbrCor ) {
    //--- Write corners
    GmfSetKwd(OutMsh, GmfCorners, NbrCor);
    for (iCor=1; iCor<=NbrCor; ++iCor) {
       idx[0] = (long long)(Cor[iCor]);
      GmfSetLin(OutMsh, GmfCorners,idx[0]);  
    }
  }

  if ( Msh->NbrEfr ) {
    //--- Write Edges
    GmfSetKwd(OutMsh, GmfEdges, NbrEfr);
    for (iEfr=1; iEfr<=NbrEfr; ++iEfr) {
      for (i=0; i<2; ++i) {
        idx[i] = (long long)(Efr[iEfr][i]);
      }
      GmfSetLin(OutMsh, GmfEdges,idx[0],idx[1],Efr[iEfr][2]);  
    }
  }
  
  
  if ( Msh->Tri > 0 ) {
    //--- Write triangles
    GmfSetKwd(OutMsh, GmfTriangles, NbrTri);
    for (iTri=1; iTri<=NbrTri; ++iTri) {
      for (i=0; i<3; ++i) {
        idx[i] = (long long)(Tri[iTri][i]);
      }
      GmfSetLin(OutMsh, GmfTriangles,idx[0],idx[1],idx[2],Tri[iTri][3]);  
    }
  }
  
  if ( Msh->Qua > 0 ) {
    //--- Write quads
    GmfSetKwd(OutMsh, GmfQuadrilaterals, NbrQua);
    for (iQua=1; iQua<=NbrQua; ++iQua) {
      for (i=0; i<4; ++i) {
        idx[i] = (long long)(Qua[iQua][i]);
      }
      GmfSetLin(OutMsh, GmfQuadrilaterals,idx[0],idx[1],idx[2],idx[3],Qua[iQua][4]);  
    }
  }

  if ( Msh->Tet > 0 ) {
    //--- Write tetrahedra
    GmfSetKwd(OutMsh, GmfTetrahedra, Msh->NbrTet);
    for (iTet=1; iTet<=Msh->NbrTet; ++iTet) {
      for (i=0; i<4; ++i) {
        idx[i] = (long long)(Tet[iTet][i]);
      }
      GmfSetLin(OutMsh, GmfTetrahedra,idx[0],idx[1],idx[2],idx[3],Tet[iTet][4]);  
    }
  }

  if ( Msh->Pyr > 0 ) {
    //--- Write pyr
    GmfSetKwd(OutMsh, GmfPyramids, Msh->NbrPyr);
    for (i=1; i<=Msh->NbrPyr; ++i) {
      for (j=0; j<5; ++j) {
        idx[j] = (long long)(Msh->Pyr[i][j]);
      }
      GmfSetLin(OutMsh, GmfPyramids,idx[0],idx[1],idx[2],idx[3],idx[4],Msh->Pyr[i][5]);  
    }
  }  

  if ( Msh->Pri > 0  ) {
    //--- Write prisms
    GmfSetKwd(OutMsh, GmfPrisms, Msh->NbrPri);
    for (i=1; i<=Msh->NbrPri; ++i) {
      for (j=0; j<6; ++j) {
        idx[j] = (long long)(Msh->Pri[i][j]);
      }
            
      GmfSetLin(OutMsh, GmfPrisms,idx[0],idx[1],idx[2],idx[3],idx[4],idx[5],Msh->Pri[i][6]);  
    }
  }

  if ( Msh->Hex > 0 ) {
    //--- Write hexahedra
    GmfSetKwd(OutMsh, GmfHexahedra, NbrHex);
    for (i=1; i<=NbrHex; ++i) {
      for (j=0; j<8; ++j) {
        idx[j] = (long long)(Hex[i][j]);
      }
            
      GmfSetLin(OutMsh, GmfHexahedra,idx[0],idx[1],idx[2],idx[3],idx[4],idx[5],idx[6],idx[7],Hex[i][8]);  
    }
  }

  //--- close mesh file
  if ( !GmfCloseMesh(OutMsh) ) {
    printf("  ## ERROR: Cannot close mesh file %s ! \n",OutFil);
    return 0;
  }
  
  return 1;
}

int WriteGMFSolution(char *SolNam, double *Sol, int SolSiz, int NbrVer, int Dim, int NbrFld, int* FldTab)
{
  int       OutSol, iVer;
  double   *dbl=NULL;
  
  if ( !Sol ) {
    printf("  ## ERROR WriteGMFSolution : Sol not allocated.\n");
    return 0;  
  }
  
  if ( SolSiz < 1 ) {
    printf("  ## ERROR WriteGMFSolution : SolSiz < 1.\n");
    return 0;
  }
  
  //--- Open solution file
  if ( !(OutSol = GmfOpenMesh(SolNam, GmfWrite, GmfDouble, Dim)) ) {
    fprintf(stderr,"  ## ERROR: Cannot open solution file %s ! \n",SolNam);
    exit(1);
  }

  GmfSetKwd(OutSol, GmfSolAtVertices, NbrVer, NbrFld, FldTab);
  
  for (iVer=1; iVer<=NbrVer; ++iVer) {
    dbl = &Sol[iVer*SolSiz];
    GmfSetLin(OutSol, GmfSolAtVertices, dbl);
  }
    
  if ( !GmfCloseMesh(OutSol) ) {
    printf("  ## ERROR: Cannot close solution file %s ! \n",SolNam);
    return 0;
  }
  
  return 1;
}

/*
  Interface to function WriteGMFSolution()
*/
int WriteGMFSolutionItf(char *SolNam, Mesh *Msh)
{
  double *Sol      = Msh->Sol;
  int     SolSiz   = Msh->SolSiz;
  int     NbrVer   = Msh->NbrVer;
  int     Dim      = Msh->Dim; 
  int     NbrFld   = Msh->NbrFld; 
  int    *FldTab   = Msh->FldTab; 
  
  return WriteGMFSolution(SolNam, Sol, SolSiz, NbrVer, Dim, NbrFld, FldTab);
}

int WriteGMFMetric(char *MetNam, Mesh *Msh, int OptBin)
{
  double *Sol       = Msh->Sol;
  int     SolSiz    = Msh->SolSiz;
  int     NbrVer    = Msh->NbrVer;
  int     Dim       = Msh->Dim; 
  int     NbrFld    = 1; 
  int     FldTab[1] = {GmfSymMat}; 
    int     OutMet, iVer;
  double   *dbl=NULL;
  
  if ( !Sol ) {
    printf("  ## ERROR WriteGMFSolution : Sol not allocated.\n");
    return 0; 
  }
  
  if ( SolSiz < 1 ) {
    printf("  ## ERROR WriteGMFSolution : SolSiz < 1.\n");
    return 0;
  }
  
  //--- Open solution file
  if ( !(OutMet = GmfOpenMesh(MetNam, GmfWrite, GmfDouble, Dim)) ) {
    fprintf(stderr,"  ## ERROR: Cannot open solution file %s ! \n",MetNam);
    exit(1);
  }
  //printf("  %%%% %s OPENED (WRITE)\n",SolNam);

  GmfSetKwd(OutMet, GmfSolAtVertices, NbrVer, NbrFld, FldTab);
  
  for (iVer=1; iVer<=NbrVer; ++iVer) {

    dbl = &Sol[iVer*SolSiz+(SolSiz-(3*(Dim-1)))];
    GmfSetLin(OutMet, GmfSolAtVertices, dbl);
  }
    
  if ( !GmfCloseMesh(OutMet) ) {
    printf("  ## ERROR: Cannot close solution file %s ! \n",MetNam);
    return 0;
  }
  
  return 1;
}

int SplitQuads(char *nam, Mesh *Msh)
{
  int ref = 0;
  int iQua, is[3];
  int NbrQua = Msh->NbrQua;

  /*--- TODO: store old tris if mixed element mesh ---*/

  /*--- Allocate space for tris ---*/
  Msh->MaxNbrTri = Msh->MaxNbrTri+Msh->NbrQua*2;
  Msh->Tri = (int4*)malloc(sizeof(int4)*(Msh->MaxNbrTri+1));
  for (iQua = 1; iQua <= NbrQua; iQua++) {
    /*--- First triangle ---*/
    Msh->NbrTri++;
    is[0] = Msh->Qua[iQua][0];
    is[1] = Msh->Qua[iQua][1];
    is[2] = Msh->Qua[iQua][2];
    AddTriangle(Msh,Msh->NbrTri,is,ref);
    /*--- Second triangle ---*/
    Msh->NbrTri++;
    is[0] = Msh->Qua[iQua][2];
    is[1] = Msh->Qua[iQua][3];
    is[2] = Msh->Qua[iQua][0];
    AddTriangle(Msh,Msh->NbrTri,is,ref);
  }

  /*--- Set NbrQua = 0 ---*/
  Msh->NbrQua = 0;

  return 1;
}
