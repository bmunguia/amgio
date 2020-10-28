#include "amgio.h"

int ConvertGMFtoSU2Sol (Options *mshopt)
{
  Mesh *Msh = NULL;
  char OutSol[1024];

  FILE *FilHdl=NULL;
  char *tok=NULL, *lin=NULL;

  int skip=0, SolSiz=0;
  size_t  len = 0;

  Msh = SetupMeshAndSolution (mshopt->InpNam, mshopt->SolNam);

  if ( Msh->FilTyp != FILE_GMF ) {
    printf("  ## ERROR : Input mesh file must be a .mesh (GMF) (FilTyp=%d)\n", Msh->FilTyp);
    return 0;
  }

  if ( mshopt->clean == 1 )
    RemoveUnconnectedVertices(Msh);
  
  WriteSU2Mesh(mshopt->OutNam, Msh);
  
  if ( Msh->Sol ) {
    if ( Msh->SolTyp == FILE_SU2BIN ) {
      sprintf(OutSol, "%s.dat", mshopt->OutNam);
      WriteSU2SolutionBin (OutSol, Msh->Dim, Msh->NbrVer, Msh->Ver, Msh->Sol, Msh->SolSiz, Msh->SolTag);
    }
    else {
      sprintf(OutSol, "%s.csv", mshopt->OutNam);
      WriteSU2Solution (OutSol, Msh->Dim, Msh->NbrVer, Msh->Ver, Msh->Sol, Msh->SolSiz, Msh->SolTag);
    }
  }

  if ( Msh )
     FreeMesh(Msh);

  return 1;
}

int ConvertGMFWithBoundtoSU2Sol (Options *mshopt, char* BndMshNam)
{
  Mesh *BndMsh = NULL, *Msh = NULL;
  char OutSol[1024];

  FILE *FilHdl=NULL;
  char *tok=NULL, *lin=NULL;

  int skip=0, SolSiz=0;
  size_t  len = 0;

  BndMsh = SetupMeshAndSolution (BndMshNam, "");
  Msh    = SetupMeshAndSolution (mshopt->InpNam, mshopt->SolNam);

  CopyBoundaryMarkers(Msh, BndMsh);

  if ( BndMsh )
    FreeMesh(BndMsh);
  else {
    printf("  ## ERROR : No BndMsh. \n");
    return 0;
  }

  if ( Msh->FilTyp != FILE_GMF ) {
    printf("  ## ERROR : Input mesh file must be a .mesh (GMF) (FilTyp=%d)\n", Msh->FilTyp);
    return 0;
  }

  if ( mshopt->clean == 1 )
    RemoveUnconnectedVertices(Msh);
  
  WriteSU2Mesh(mshopt->OutNam, Msh);
  
  if ( Msh->Sol ) {
    if ( Msh->SolTyp == FILE_SU2BIN ) {
      sprintf(OutSol, "%s.dat", mshopt->OutNam);
      WriteSU2SolutionBin (OutSol, Msh->Dim, Msh->NbrVer, Msh->Ver, Msh->Sol, Msh->SolSiz, Msh->SolTag);
    }
    else {
      sprintf(OutSol, "%s.csv", mshopt->OutNam);
      WriteSU2Solution (OutSol, Msh->Dim, Msh->NbrVer, Msh->Ver, Msh->Sol, Msh->SolSiz, Msh->SolTag);
    }
  }

  if ( Msh )
    FreeMesh(Msh);

  return 1;
}

int ConvertGMFSoltoMet (Options *mshopt)
{
  Mesh *Msh = NULL;
  char OutMet[1024];

  FILE *FilHdl=NULL;
  char *tok=NULL, *lin=NULL;

  int skip=0, SolSiz=0;
  size_t  len = 0;

  Msh = SetupMeshAndSolution (mshopt->InpNam, mshopt->SolNam);

  if ( Msh->FilTyp != FILE_GMF ) {
    printf("  ## ERROR : Input mesh file must be a .mesh (GMF) (FilTyp=%d)\n", Msh->FilTyp);
    return 0;
  }

  if ( mshopt->clean == 1 )
    RemoveUnconnectedVertices(Msh);
  
  sprintf(OutMet, "%s.solb", mshopt->OutNam);
  WriteGMFMetric(OutMet, Msh, 1);

  if ( Msh )
    FreeMesh(Msh);

  return 1;
}

int ConvertSU2SolToGMF (Options *mshopt)
{
  Mesh *Msh = NULL;
  char OutSol[1024];

  Msh = SetupMeshAndSolution (mshopt->InpNam, mshopt->SolNam);

  if ( Msh->FilTyp != FILE_SU2MSH ) {
    printf("  ## ERROR : Input mesh file must be a .su2.\n");
    return 0;
  }

    /*--- pyAMG can't currently handle quads, so split them ---*/
    // if ( Msh->Dim == 2 && Msh->NbrQua > 0) {
    //     SplitQuads(mshopt->InpNam, Msh);
    // }

  WriteGMFMesh(mshopt->OutNam, Msh, 1);

  if ( Msh->Sol ) {
    sprintf(OutSol, "%s.solb", mshopt->OutNam);
    if ( ! WriteGMFSolutionItf(OutSol, Msh) ) {
      printf("  ## ERROR : outputmach FAILED.\n");
    }
  }

  if ( Msh )
     FreeMesh(Msh);

  return 1;
}

int ConvertSU2SolToGMFSensor (Options *mshopt)
{
  Mesh *Msh = NULL;
  char OutSol[1024];

  Msh = SetupMeshAndSolution (mshopt->InpNam, mshopt->SolNam);

  if ( Msh->FilTyp != FILE_SU2MSH ) {
    printf("  ## ERROR : Input mesh file must be a .su2.\n");
    return 0;
  }

  WriteGMFMesh(mshopt->OutNam, Msh, 1);

  if ( Msh->Sol )
    SplitSolution (Msh, mshopt->OutNam, mshopt->FldNam);

  if ( Msh )
     FreeMesh(Msh);

  return 1;
}

int WriteGMFSensor (Mesh *Msh, char *prefix, char *sensor)
{
  int NbrFld = 1, i, iVer, idx;
  int FldTab[10]; 
  double *OutSol = NULL;
  char OutNam[256];
  
  for (i=0; i<NbrFld; i++) {
    FldTab[i] = 1;
  }
  
  OutSol = (double*)malloc(sizeof(double)*(Msh->NbrVer+1)*NbrFld);
  
  //--- Get fields indices

  int iSens = -1;
    
  for (i=0; i<Msh->NbrFld; i++) {
    if ( !strcasecmp(Msh->SolTag[i], sensor) ) {
      iSens = i;
      break;
    }
  }
  
  if ( iSens < 0 ) {
    printf("  ## ERROR: SplitSolution: Unknown sensor %s. Index not found.\n", sensor);
    return 0;
  }
  
  for (iVer=1; iVer<=Msh->NbrVer; iVer++) {    
    idx = iVer*NbrFld;
    OutSol[idx] = Msh->Sol[iVer*Msh->SolSiz+iSens];
  }
  
  sprintf(OutNam, "%s.solb", prefix);
  if ( ! WriteGMFSolution(OutNam, OutSol, NbrFld, Msh->NbrVer, Msh->Dim, NbrFld, FldTab) ) {
    printf("  ## ERROR : Output of solution failed.\n");
  }
  
  if ( OutSol )
    free(OutSol);
  
  return 1;
}
