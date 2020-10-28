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

int SplitSolution (Mesh *Msh, char *prefix, char *sensor)
{
  int NbrFld = 1, i, iVer, idx;
  int FldTab[10]; 
  double *OutSol = NULL;
  char OutNam[256];
  
  int PresFlag=0, MachFlag=0, TempFlag=0;
    
  if (!strcmp(sensor, "MACH")) {
    NbrFld = 1;
    MachFlag = 1;
  }
  else if (!strcmp(sensor, "PRESSURE")) {
    NbrFld = 1;
    PresFlag = 1;
  }
  else if (!strcmp(sensor, "TEMPERATURE")) {
    NbrFld = 1;
    PresFlag = 1;
  }
  // else if (!strcmp(sensor, "MACH_PRES")) {
  //   NbrFld = 2;
  //   PresFlag = MachFlag = 1;
  // }
  else {
    printf("## ERROR: SplitSolution: Unknown sensor.\n");
    exit(1);
  }
  
  for (i=0; i<NbrFld; i++) {
    FldTab[i] = 1;
  }
  
  OutSol = (double*)malloc(sizeof(double)*(Msh->NbrVer+1)*NbrFld);
  
  //--- Get fields indices

  int iMach = -1;
  int iPres = -1;
  int iTemp = -1;
    
  for (i=0; i<Msh->NbrFld; i++) {
    if ( !strcmp(Msh->SolTag[i], "Mach") && (MachFlag == 1) ) {
      iMach = i;
    }
    if ( !strcmp(Msh->SolTag[i], "Pressure") && (PresFlag == 1) ) {
      iPres = i;
    }
    if ( !strcmp(Msh->SolTag[i], "Temperature") && (TempFlag == 1) ) {
      iTemp = i;
    }
  }
  
  if ( (iMach < 0) && (MachFlag == 1) ) {
    printf("  ## ERROR: SplitSolution: Mach index not found.\n");
    return 0;
  }
  
  if ( (iPres < 0) && (PresFlag == 1) ) {
    printf("  ## ERROR SplitSolution: Pres index not found.\n");
    return 0;
  }
  
  for (iVer=1; iVer<=Msh->NbrVer; iVer++) {    
    
    idx = iVer*NbrFld-1;
    
    if ( MachFlag == 1 ){
      idx++;
      OutSol[idx] = Msh->Sol[iVer*Msh->SolSiz+iMach];
    }
    
    if ( PresFlag == 1 ){
      idx++;
      OutSol[idx] = Msh->Sol[iVer*Msh->SolSiz+iPres];
    }
  }
  
  sprintf(OutNam, "%s.solb", prefix);
  if ( ! WriteGMFSolution(OutNam, OutSol, NbrFld, Msh->NbrVer, Msh->Dim, NbrFld, FldTab) ) {
    printf("  ## ERROR : Output of solution failed.\n");
  }
  
  if ( OutSol )
    free(OutSol);
  
  return 1;
}