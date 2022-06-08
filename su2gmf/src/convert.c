#include "common.h"

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

  WriteGMFMesh(mshopt->OutNam, Msh, 1);

  if ( Msh->Sol ) {
    sprintf(OutSol, "%s.solb", mshopt->OutNam);
    if ( ! WriteGMFSolutionItf(OutSol, mshopt->FldNam, Msh) ) {
      printf("  ## ERROR : outputmach FAILED.\n");
    }
  }

  if ( Msh )
     FreeMesh(Msh);

  return 1;
}
