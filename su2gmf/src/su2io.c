#include "common.h"

/*
Victorien Menier Feb 2016
Brian Munguía Feb 2020
*/

int AddSU2MeshSize(char *FilNam, int *SizMsh)
{
  int i, NbrElt, iElt, typ, CptElt;
  int NbrTri, NbrTet, NbrHex, NbrPyr, NbrRec, NbrLin, NbrCor, NbrWed, NbrP2Tri, NbrP2Lin;
  int NbrMark, iMark;
  FILE *FilHdl = NULL;
  char str[1024];


  for (i=0; i<GmfMaxSizMsh; i++)
    SizMsh[i] = 0;


  if ( GetFileType(FilNam) != FILE_SU2MSH )
  {
    printf("NOT SU2, return\n");
    return 1;
  }


  FilHdl = fopen(FilNam, "r");

  if ( !FilHdl )
  {
    fprintf(stderr, "  -- Info : Tried to open %s. Failed.\n", FilNam );
    return 1;
  }

  //--- Dim ?

  rewind(FilHdl);
  SizMsh[GmfDimension] = GetSU2KeywordValue(FilHdl, "NDIME=");

  //--- Elements?

  NbrTri = NbrTet = NbrHex = NbrPyr = NbrLin = NbrCor = NbrRec = NbrWed = NbrP2Tri = NbrP2Lin =  0;

  NbrElt = GetSU2KeywordValue(FilHdl, "NELEM=");

  for (iElt=0; iElt<NbrElt; iElt++)
  {
    fscanf(FilHdl, "%d", &typ);

    if ( typ == SU2_TRIANGLE )
      NbrTri++;
    else if ( typ == SU2_TETRAHEDRAL )
      NbrTet++;
    else if ( typ == SU2_HEXAHEDRAL )
      NbrHex++;
    else if ( typ == SU2_PYRAMID )
      NbrPyr++;
    else if ( typ == SU2_RECTANGLE )
      NbrRec++;
    else if ( typ == SU2_WEDGE )
      NbrWed++;
    else if ( typ == SU2_LINE )
      NbrLin++;
    else if ( typ == SU2_TRIANGLEP2 )
      NbrP2Tri++;
    else if ( typ == SU2_LINEP2 )
      NbrP2Lin++;
    else
    {
      printf("  ## ERROR: AddSU2MeshSize: Unknown element type %d\n", typ);
      return 1;
    }
    fgets (str, sizeof str, FilHdl);
  }//for iElt

  rewind(FilHdl);
  SizMsh[GmfVertices] = GetSU2KeywordValue(FilHdl, "NPOIN=");

  rewind(FilHdl);
  NbrCor = GetSU2KeywordValue(FilHdl, "NCORNERS=");

  //--- Boundary Elements?
  NbrMark = 0;

  rewind(FilHdl);
  NbrMark = GetSU2KeywordValue(FilHdl, "NMARK=");



  for (iMark=1; iMark<=NbrMark; iMark++)
  {

    GetSU2KeywordValueStr(FilHdl, "MARKER_TAG=", str);

    if ( !strcmp(str,"SEND_RECEIVE") )
    {
      printf("      Tag %s was ignored.\n", str);
      continue;
    }

    CptElt = GetSU2KeywordValue(FilHdl, "MARKER_ELEMS=");

    for (iElt=0; iElt<CptElt; iElt++)
    {
      fscanf(FilHdl, "%d", &typ);
      if ( typ == SU2_TRIANGLE )
        NbrTri++;
      else if ( typ == SU2_RECTANGLE )
        NbrRec++;
      else if ( typ == SU2_LINE )
        NbrLin++;
      else if ( typ == SU2_LINEP2 )
        NbrP2Lin++;
      else
      {
        printf("  ## ERROR: AddSU2MeshSize : Unknown boundary element type %d\n", typ);
        return 1;
      }
      fgets (str, sizeof str, FilHdl);
    }

  }

  SizMsh[GmfTriangles]      = NbrTri+NbrP2Tri;
  SizMsh[GmfTetrahedra]     = NbrTet;
  SizMsh[GmfCorners]        = NbrCor;
  SizMsh[GmfEdges]          = NbrLin+NbrP2Lin;
  SizMsh[GmfPrisms]         = NbrWed;
  SizMsh[GmfQuadrilaterals] = NbrRec;
  SizMsh[GmfPyramids]       = NbrPyr;
  SizMsh[GmfHexahedra]      = NbrHex;
  SizMsh[GmfTrianglesP2]    = NbrP2Tri;
  SizMsh[GmfEdgesP2]        = NbrP2Lin;

  if ( FilHdl ) fclose(FilHdl);

  return 0;
}

int GetSU2KeywordValue(FILE *FilHdl, char *Kwd)
{

  size_t lenKwd=0, len=0;
  int buf=0, res=0;
  char str[1024], str2[1024], kwd[1024];

  if ( !FilHdl || !Kwd ) return 1;

  sprintf(kwd,"%s",Kwd);

  lenKwd = strlen(kwd);

  do
  {
    res = fscanf(FilHdl, "%s", str);
  } while( (res != EOF) && strncmp(str, kwd, lenKwd) );

  if (res == EOF)
  {
    fprintf(stderr,"  ## WARNING: MISSING SU2 MESH FILE KEYWORD: %s.\n", Kwd);
    return 1;
  }

  len = strlen(str);

  if ( len > lenKwd )
  {
    strncpy(str2, &str[lenKwd], len-lenKwd+1);
    buf = atoi(str2);
  }
  else
    fscanf(FilHdl, "%d", &buf);

  fgets(str, sizeof str, FilHdl);


  return buf;
}

int GetSU2KeywordValueStr(FILE *FilHdl, char *Kwd, char *StrVal)
{

  size_t lenKwd=0, len=0;
  int  res=0;
  char str[1024], kwd[1024], buf[1024];

  if ( !FilHdl || !Kwd ) return 1;

  strcpy(kwd,Kwd);
  lenKwd = strlen(kwd);

  do
  {
    res = fscanf(FilHdl, "%s", str);
  } while( (res != EOF) && strncmp(str, kwd, lenKwd) );

  if (res == EOF)
  {
    fprintf(stderr,"  ## ERROR: Invalid SU2 file (Check kwd: %s).\n", Kwd);
    return 1;
  }

  len = strlen(str);

  if ( len > lenKwd )
    strncpy(StrVal, &str[lenKwd], len-lenKwd);
  else
  {
    fscanf(FilHdl, "%s", buf);
    sprintf(StrVal, "%s", buf);
  }

  fgets(str, sizeof str, FilHdl);

  return 0;
}

int LoadSU2Elements(FILE *FilHdl, Mesh *Msh, Conn *Con)
{
  int  ref=1;
  char str[1024];

  int iMark, NbrMark=0, CptElt;
  int iElt, NbrElt=0, typ, is[8], swi[8], buf, s, idx, res;

  Msh->NbrTri = Msh->NbrTet = Msh->NbrHex = Msh->NbrEfr = Msh->NbrQua = Msh->NbrPyr = Msh->NbrPri = 0;

  rewind(FilHdl);
  do
  {
    res = fscanf(FilHdl, "%s", str);
  } while( (res != EOF) && strcmp(str, "NELEM=") );

  fscanf(FilHdl, "%d", &NbrElt);
  fgets(str, sizeof str, FilHdl);

  idx=0;

  for (iElt=0; iElt<NbrElt; iElt++)
  {
    fscanf(FilHdl, "%d", &typ);

    if ( typ == SU2_TRIANGLE )
    {

      for (s=0; s<3; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        swi[s] = buf+1;
        is[s]  = buf+1;
        if ( swi[s] > Msh->NbrVer )
        {
          printf("  ## ERROR: LoadSU2Elements: vertex out of bound (vid=%d)\n", swi[s]);
          return 1;
        }
        Con->NbrTri[is[s]]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrTri++;

      if ( Msh->NbrTri > Msh->MaxNbrTri )
      {
        printf("  ## ERROR: LoadSU2Elements: triangle out of bound (tid=%d, max=%d)\n", Msh->NbrTri, Msh->MaxNbrTri);
        return 1;
      }

      AddTriangle(Msh,Msh->NbrTri,is,ref);
    }
    else if ( typ == SU2_RECTANGLE )
    {
      for (s=0; s<4; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        swi[s] = buf+1;
        is[s]  = buf+1;
        Con->NbrQua[is[s]]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrQua++;

      if ( Msh->NbrQua > Msh->MaxNbrQua )
      {
        printf("  ## ERROR: LoadSU2Elements: quad out of bound (id=%d)\n", Msh->NbrQua);
        return 1;
      }

      AddQuadrilateral(Msh,Msh->NbrQua,is,ref);
    }
    else if ( typ == SU2_TETRAHEDRAL )
    {
      for (s=0; s<4; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        swi[s] = buf+1;
        is[s]  = buf+1;
        Con->NbrTet[is[s]]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrTet++;

      if ( Msh->NbrTet > Msh->MaxNbrTet )
      {
        printf("  ## ERROR: LoadSU2Elements: tetra out of bound (tid=%d)\n", Msh->NbrTet);
        return 1;
      }

      AddTetrahedron(Msh,Msh->NbrTet,is,ref);
    }
    else if ( typ == SU2_PYRAMID )
    {
      for (s=0; s<5; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        swi[s] = buf+1;
        is[s]  = buf+1;
        Con->NbrPyr[is[s]]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrPyr++;

      if ( Msh->NbrPyr > Msh->MaxNbrPyr )
      {
        printf("  ## ERROR: LoadSU2Elements: pyramid out of bound (id=%d)\n", Msh->NbrPyr);
        return 1;
      }

      AddPyramid(Msh,Msh->NbrPyr,is,ref);
    }
    else if ( typ == SU2_WEDGE )
    {
      for (s=0; s<6; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        swi[s] = buf+1;
        is[s]  = buf+1;
        Con->NbrPri[is[s]]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrPri++;

      if ( Msh->NbrPri > Msh->MaxNbrPri )
      {
        printf("  ## ERROR: LoadSU2Elements: prism out of bound (id=%d)\n", Msh->NbrPri);
        return 1;
      }

      AddPrism(Msh,Msh->NbrPri,is,ref);
    }
    else if ( typ == SU2_HEXAHEDRAL )
    {
      for (s=0; s<8; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        swi[s] = buf+1;
        is[s]  = buf+1;
        Con->NbrHex[is[s]]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrHex++;

      if ( Msh->NbrHex > Msh->MaxNbrHex )
      {
        printf("  ## ERROR: LoadSU2Elements: hexahedron out of bound (hid=%d)\n", Msh->NbrHex);
        return 1;
      }

      AddHexahedron(Msh,Msh->NbrHex,is,ref);
    }
    else
    {
      printf("  ## ERROR: LoadSU2Elements: Unknown element type %d\n", typ);
      return 1;
    }

    fgets (str, sizeof str, FilHdl);

  }//for iElt

  //--- Read boundary elements

  rewind(FilHdl);
  NbrMark = GetSU2KeywordValue(FilHdl, "NMARK=");

  Msh->NbrMarkers = NbrMark;

  for (iMark=1; iMark<=NbrMark; iMark++)
  {

    GetSU2KeywordValueStr(FilHdl, "MARKER_TAG=", str);

    if ( iMark < 10000-1 ) strcpy(Msh->Markers[iMark], str);

    if ( !strcmp(str,"SEND_RECEIVE") )
    {
      printf("      Tag %s was ignored.\n", str);
      continue;
    }

    CptElt = GetSU2KeywordValue(FilHdl, "MARKER_ELEMS=");

    for (iElt=0; iElt<CptElt; iElt++)
    {

      fscanf(FilHdl, "%d", &typ);

      if ( typ == SU2_TRIANGLE )
      {
        for (s=0; s<3; s++)
        {
          fscanf(FilHdl, "%d", &buf);
          swi[s] = buf+1;
          Con->NbrTri[swi[s]]++;
        }

        Msh->NbrTri++;

        if ( Msh->NbrTri > Msh->MaxNbrTri )
        {
          printf("  ## ERROR: LoadSU2Elements: triangle out of bound (tid=%d, max=%d)\n", Msh->NbrTri, Msh->MaxNbrTri);
          return 1;
        }

        AddTriangle(Msh,Msh->NbrTri,swi,iMark+1);
      }
      else if ( typ == SU2_RECTANGLE )
      {
        for (s=0; s<4; s++)
        {
          fscanf(FilHdl, "%d", &buf);
          swi[s] = buf+1;
          Con->NbrQua[swi[s]]++;
        }

        Msh->NbrQua++;

        if ( Msh->NbrQua > Msh->MaxNbrQua )
        {
          printf("  ## ERROR: LoadSU2Elements: quad out of bound (id=%d)\n", Msh->NbrQua);
          return 1;
        }

        AddQuadrilateral(Msh,Msh->NbrQua,swi,iMark+1);
      }
      else if ( typ == SU2_LINE )
      {
        for (s=0; s<2; s++)
        {
          fscanf(FilHdl, "%d", &buf);
          swi[s] = buf+1;
          Con->NbrEfr[swi[s]]++;
        }

        Msh->NbrEfr++;

        if ( Msh->NbrEfr > Msh->MaxNbrEfr )
        {
          printf("  ## ERROR: LoadSU2Elements: boundary edge out of bound (id=%d, max=%d)\n", Msh->NbrEfr, Msh->MaxNbrEfr);
          return 1;
        }

        AddEdge(Msh,Msh->NbrEfr,swi,iMark+1);
      }
      else
      {
        printf("  ## ERROR: LoadSU2Elements: Unknown element type %d\n", typ);
        return 1;
      }
      fgets(str, sizeof str, FilHdl);

    }//for iElt
  }//for iMark

  return 0;
}

int LoadSU2ConnData(char *FilNam, Mesh *Msh, Conn *Con)
{
  int  ref=1;
  char str[1024];

  int iMark, NbrMark=0, CptElt;
  int iElt, NbrElt=0, typ, is[8], swi[8], buf, s, idx, res;

  for (int iVer = 1; iVer <= Msh->NbrVer; iVer++)
  {
    if (Con->NbrEfr[iVer] > 0) Con->Efr[iVer] = (int*)malloc(sizeof(int)*(Con->NbrEfr[iVer]));
    if (Con->NbrTri[iVer] > 0) Con->Tri[iVer] = (int*)malloc(sizeof(int)*(Con->NbrTri[iVer]));
    if (Con->NbrTet[iVer] > 0) Con->Tet[iVer] = (int*)malloc(sizeof(int)*(Con->NbrTet[iVer]));
    if (Con->NbrQua[iVer] > 0) Con->Qua[iVer] = (int*)malloc(sizeof(int)*(Con->NbrQua[iVer]));
    if (Con->NbrHex[iVer] > 0) Con->Hex[iVer] = (int*)malloc(sizeof(int)*(Con->NbrHex[iVer]));
    if (Con->NbrPri[iVer] > 0) Con->Pri[iVer] = (int*)malloc(sizeof(int)*(Con->NbrPri[iVer]));
    if (Con->NbrPyr[iVer] > 0) Con->Pyr[iVer] = (int*)malloc(sizeof(int)*(Con->NbrPyr[iVer]));

    Con->NbrEfr[iVer] = Con->NbrTri[iVer] = Con->NbrTet[iVer] = Con->NbrQua[iVer] = 0;
    Con->NbrHex[iVer] = Con->NbrPri[iVer] = Con->NbrPyr[iVer] = 0;
  }

  Msh->NbrTri = Msh->NbrTet = Msh->NbrHex = Msh->NbrEfr = Msh->NbrQua = Msh->NbrPyr = Msh->NbrPri = 0;

  FILE *FilHdl = fopen(FilNam, "r");

  do
  {
    res = fscanf(FilHdl, "%s", str);
  } while( (res != EOF) && strcmp(str, "NELEM=") );

  fscanf(FilHdl, "%d", &NbrElt);
  fgets(str, sizeof str, FilHdl);

  idx=0;

  for (iElt=0; iElt<NbrElt; iElt++)
  {
    fscanf(FilHdl, "%d", &typ);

    if ( typ == SU2_TRIANGLE )
    {

      for (s=0; s<3; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        Con->Tri[buf+1][Con->NbrTri[buf+1]] = Msh->NbrTri+1;
        Con->NbrTri[buf+1]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrTri++;

      if ( Msh->NbrTri > Msh->MaxNbrTri )
      {
        printf("  ## ERROR: LoadSU2ConnData: triangle out of bound (tid=%d, max=%d)\n", Msh->NbrTri, Msh->MaxNbrTri);
        return 1;
      }
    }
    else if ( typ == SU2_RECTANGLE )
    {
      for (s=0; s<4; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        Con->Qua[buf+1][Con->NbrQua[buf+1]] = Msh->NbrQua+1;
        Con->NbrQua[buf+1]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrQua++;

      if ( Msh->NbrQua > Msh->MaxNbrQua )
      {
        printf("  ## ERROR: LoadSU2ConnData: quad out of bound (id=%d)\n", Msh->NbrQua);
        return 1;
      }
    }
    else if ( typ == SU2_TETRAHEDRAL )
    {
      for (s=0; s<4; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        Con->Tet[buf+1][Con->NbrTet[buf+1]] = Msh->NbrTet+1;
        Con->NbrTet[buf+1]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrTet++;

      if ( Msh->NbrTet > Msh->MaxNbrTet )
      {
        printf("  ## ERROR: LoadSU2ConnData: tetra out of bound (tid=%d)\n", Msh->NbrTet);
        return 1;
      }
    }
    else if ( typ == SU2_PYRAMID )
    {
      for (s=0; s<5; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        Con->Pyr[buf+1][Con->NbrPyr[buf+1]] = Msh->NbrPyr+1;
        Con->NbrPyr[buf+1]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrPyr++;

      if ( Msh->NbrPyr > Msh->MaxNbrPyr )
      {
        printf("  ## ERROR: LoadSU2ConnData: pyramid out of bound (id=%d)\n", Msh->NbrPyr);
        return 1;
      }
    }
    else if ( typ == SU2_WEDGE )
    {
      for (s=0; s<6; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        Con->Pri[buf+1][Con->NbrPri[buf+1]] = Msh->NbrPri+1;
        Con->NbrPri[buf+1]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrPri++;

      if ( Msh->NbrPri > Msh->MaxNbrPri )
      {
        printf("  ## ERROR: LoadSU2ConnData: prism out of bound (id=%d)\n", Msh->NbrPri);
        return 1;
      }
    }
    else if ( typ == SU2_HEXAHEDRAL )
    {
      for (s=0; s<8; s++)
      {
        fscanf(FilHdl, "%d", &buf);
        Con->Hex[buf+1][Con->NbrHex[buf+1]] = Msh->NbrHex+1;
        Con->NbrHex[buf+1]++;
      }

      fscanf(FilHdl, "%d", &buf);

      Msh->NbrHex++;

      if ( Msh->NbrHex > Msh->MaxNbrHex )
      {
        printf("  ## ERROR: LoadSU2ConnData: hexahedron out of bound (hid=%d)\n", Msh->NbrHex);
        return 1;
      }
    }
    else
    {
      printf("  ## ERROR: Unknown element type %d\n", typ);
      return 1;
    }

    fgets(str, sizeof str, FilHdl);

  }//for iElt

  //--- Read boundary elements

  rewind(FilHdl);
  NbrMark = GetSU2KeywordValue(FilHdl, "NMARK=");

  for (iMark=1; iMark<=NbrMark; iMark++)
  {

    GetSU2KeywordValueStr(FilHdl, "MARKER_TAG=", str);

    if ( !strcmp(str,"SEND_RECEIVE") )
    {
      printf("      Tag %s was ignored.\n", str);
      continue;
    }

    CptElt = GetSU2KeywordValue(FilHdl, "MARKER_ELEMS=");

    for (iElt=0; iElt<CptElt; iElt++)
    {

      fscanf(FilHdl, "%d", &typ);

      if ( typ == SU2_TRIANGLE )
      {
        for (s=0; s<3; s++)
        {
          fscanf(FilHdl, "%d", &buf);
          Con->Tri[buf+1][Con->NbrTri[buf+1]] = Msh->NbrTri+1;
          Con->NbrTri[buf+1]++;
        }

        Msh->NbrTri++;

        if ( Msh->NbrTri > Msh->MaxNbrTri )
        {
          printf("  ## ERROR: LoadSU2ConnData: triangle out of bound (tid=%d, max=%d)\n", Msh->NbrTri, Msh->MaxNbrTri);
          return 1;
        }
      }
      else if ( typ == SU2_RECTANGLE )
      {
        for (s=0; s<4; s++)
        {
          fscanf(FilHdl, "%d", &buf);
          Con->Qua[buf+1][Con->NbrQua[buf+1]] = Msh->NbrQua+1;
          Con->NbrQua[buf+1]++;
        }

        Msh->NbrQua++;

        if ( Msh->NbrQua > Msh->MaxNbrQua )
        {
          printf("  ## ERROR: LoadSU2ConnData: quad out of bound (id=%d)\n", Msh->NbrQua);
          return 1;
        }
      }
      else if ( typ == SU2_LINE )
      {
        for (s=0; s<2; s++)
        {
          fscanf(FilHdl, "%d", &buf);
          Con->Efr[buf+1][Con->NbrEfr[buf+1]] = Msh->NbrEfr+1;
          Con->NbrEfr[buf+1]++;
        }

        Msh->NbrEfr++;

        if ( Msh->NbrEfr > Msh->MaxNbrEfr )
        {
          printf("  ## ERROR: LoadSU2ConnData: boundary edge out of bound (id=%d, max=%d)\n", Msh->NbrEfr, Msh->MaxNbrEfr);
          return 1;
        }
      }
      else
      {
        printf("  ## ERROR: LoadSU2ConnData: Unknown element type %d\n", typ);
        return 1;
      }
      fgets(str, sizeof str, FilHdl);

    }//for iElt
  }//for iMark

  return 0;
}

int LoadSU2Corners(FILE *FilHdl, Mesh *Msh)
{
  char   str[1024];

  int iCor, NbrCor=0, typ, is[8], swi[8], buf, s, idx, res;
  Msh->NbrCor = 0;

  rewind(FilHdl);
  do
  {
    res = fscanf(FilHdl, "%s", str);
  } while( (res != EOF) && strcmp(str, "NCORNERS=") );

  fscanf(FilHdl, "%d", &NbrCor);
  fgets(str, sizeof str, FilHdl);

  for (iCor=0; iCor<NbrCor; iCor++)
  {
    fscanf(FilHdl, "%d", &typ);

    if ( typ == SU2_CORNER )
    {
      fscanf(FilHdl, "%d", &buf);
      swi[0] = buf+1;
      if ( swi[0] > Msh->NbrVer )
      {
        printf("  ## ERROR: LoadSU2Corners: vertex out of bound (vid=%d)\n", swi[0]);
        return 1;
      }

      Msh->NbrCor++;

      if ( Msh->NbrCor > Msh->MaxNbrCor )
      {
        printf("  ## ERROR: LoadSU2Corners: corner (id=%d, max=%d)\n", Msh->NbrCor, Msh->MaxNbrCor);
        return 1;
      }

      AddCorner(Msh,Msh->NbrCor,swi);

    }
    else
    {
      printf("  ## ERROR: LoadSU2Corners: Unknown element type %d\n", typ);
      return 1;
    }

    fgets(str, sizeof str, FilHdl);

  }//for iCor

  return 0;
}

int LoadSU2Mesh(char *FilNam, Mesh *Msh, Conn *Con)
{

  FILE *FilHdl=NULL;

  if ( (Msh == NULL) || (FilNam == NULL) )
  {
    printf("  ## ERROR: LoadSU2Mesh: Mesh file name not allocated. \n");
    return 1;
  }

  if ( GetFileType(FilNam) != FILE_SU2MSH )
    return 1;

  FilHdl = fopen(FilNam, "r");

  if ( !FilHdl )
  {
    fprintf(stderr, "  -- Info : Tried to open %s. Failed.\n", FilNam );
    return 1;
  }

  strcpy(Msh->MshNam, FilNam);
  Msh->FilTyp = FILE_SU2MSH;

  rewind(FilHdl);
  Msh->Dim = GetSU2KeywordValue(FilHdl, "NDIME=");

  rewind(FilHdl);

  if ( Msh->Dim != 2 && Msh->Dim != 3 )
  {
    fprintf(stderr, "  ## ERROR: LoadSU2Mesh: Invalid dimension number for mesh %s (dim=%d).\n", FilNam, Msh->Dim);
    return 1;
  }

  //--- Read vertices
  LoadSU2Vertices(FilHdl, Msh);

  //--- Read corners
  LoadSU2Corners(FilHdl, Msh);

  //--- Read Elements
  LoadSU2Elements(FilHdl, Msh, Con);

  if ( FilHdl ) fclose(FilHdl);

  return 0;
}

int GetSU2SolSize(char *SolNam)
{
  int NbrLin=0;
  char *tok=NULL, *lin=NULL;
  char str[1024];

  int i, iVer, iVerMax;
  size_t  len = 0;
  FILE *FilHdl=NULL;

  FilHdl = fopen(SolNam,"r");

  // Skip header
  getline(&lin, &len, FilHdl);

  //--- Count

  iVerMax = -1;

  NbrLin=0;
  while ( getline(&lin, &len, FilHdl) != -1 )
  {
    NbrLin++;
    tok = strtok(lin, "  ,");
    iVer = atoi(tok)+1;
    iVerMax = max(iVer, iVerMax);
  }
  return iVerMax;
}

int LoadSU2Solution(char *SolNam, Mesh *Msh)
{
  int NbrLin=0;
  char *tok=NULL, *lin=NULL;
  char str[1024];

  int i, iVer, skip=0, SolSiz=0, idx, idxVer;
  size_t  len = 0;
  FILE *FilHdl=NULL;

  double *Sol = NULL;

  if ( Msh->Sol )
  {
    printf("  ## ERROR: LoadSU2Solution : Msh->Sol already allocated.\n");
    return 1;
  }

  if ( (Msh == NULL) || (SolNam == NULL) )
  {
    printf("  ## ERROR: LoadSU2Solution : mesh/solution file not allocated.\n");
    return 1;
  }

  FilHdl = fopen(SolNam,"r");

  if ( !FilHdl )
  {
    fprintf(stderr,"  ## ERROR: LoadSU2Solution: Unable to open %s.\n", SolNam);
    return 1;
  }

  strcpy(Msh->SolNam, SolNam);
  Msh->SolTyp = FILE_SU2CSV;

  if ( getline(&lin, &len, FilHdl) != -1 )
  {
    tok = strtok(lin, "  ,");
    skip = 0;
    SolSiz = 0;
    while ( tok )
    {
      if ( !strcmp(tok,"\"PointID\"") || !strcmp(tok,"\"x\"") || !strcmp(tok,"\"y\"") || !strcmp(tok,"\"z\"")   )
      {
        tok = strtok(NULL, "  ,");
        skip++;
        continue;
      }

      strcpy(Msh->SolTag[SolSiz], tok);
      StrRemoveChars(Msh->SolTag[SolSiz], '\"');
      StrRemoveChars(Msh->SolTag[SolSiz], '\n');
      SolSiz++;
      tok = strtok(NULL, "  ,");
    }
  }

  //--- Allocate Msh->Sol

  Msh->Sol = (double*) malloc(sizeof(double)*(Msh->NbrVer+1)*SolSiz);
  memset(Msh->Sol, 0, sizeof(double)*(Msh->NbrVer+1)*SolSiz);

  Sol = Msh->Sol;
  Msh->SolSiz = SolSiz;

  //--- Set Msh->FldTab
  // Note: Set all fields as scalars (later: vectors for velocity)
  Msh->FldTab = (int*) malloc(sizeof(int)*SolSiz);
  Msh->NbrFld = SolSiz;
  for (i=0; i<SolSiz; i++)
    Msh->FldTab[i] = GmfSca;

  //--- Load vertex solution

  NbrLin=0;
  while ( getline(&lin, &len, FilHdl) != -1 )

  {

    NbrLin++;
    tok = strtok(lin, "  ,");

    i=0, idx=0;
    while ( tok )

    {

      if ( i == 0 )

      {
        iVer = atoi(tok)+1;
        idxVer = iVer*SolSiz;
        if ( iVer > Msh->NbrVer )

        {
          fprintf(stderr,"  ## ERROR: LoadSU2Solution: Vertex out of bound (vid=%d).\n", iVer);
          return 1;
        }
      }
      else if ( i >= skip )
      {
        Sol[idxVer+idx] = atof(tok);
        if ( Sol[idxVer+idx] != Sol[idxVer+idx] )
          Sol[idxVer+idx] = 0;
        idx++;
      }

      tok = strtok(NULL, "  ,");
      i++;

      if ( idx == SolSiz )
        break;
    }

    if ( NbrLin == Msh->NbrVer )
      break;

  }


  if ( FilHdl ) fclose(FilHdl);

  if ( NbrLin != Msh->NbrVer )
  {
    fprintf(stderr,"  ## ERROR: LoadSU2Sol: Inconsistent number of vertices. \n");
    return 1;
  }

  return 0;
}

int LoadSU2SolutionBin(char *SolNam, Mesh *Msh)
{
  int NbrLin=0;
  char *tok=NULL, *lin=NULL;
  char str_buf[CGNS_STRING_SIZE], str[1024];

  int i, iVer, skip=0, SolSiz=0, idx, idxVer;
  size_t  len = 0;
  size_t ret;
  int nRestart_Vars = 5, Restart_Vars[5];
  FILE *FilHdl=NULL;

  double *Sol = NULL;
  double Coord[3];

  if ( Msh->Sol )
  {
    printf("  ## ERROR: LoadSU2SolutionBin : Msh->Sol already allocated.\n");
    return 1;
  }

  if ( (Msh == NULL) || (SolNam == NULL) )
  {
    printf("  ## ERROR: LoadSU2SolutionBin : mesh/solution file not allocated.\n");
    return 1;
  }

  FilHdl = fopen(SolNam,"rb");

  if ( !FilHdl )
  {
    fprintf(stderr,"  ## ERROR: LoadSU2SolutionBin : Unable to open %s.\n", SolNam);
    return 1;
  }

  strcpy(Msh->SolNam, SolNam);
  Msh->SolTyp = FILE_SU2BIN;

  //--- Read the number of variables and points
  ret = fread(Restart_Vars, sizeof(int), nRestart_Vars, FilHdl);
  if (ret != (unsigned long)nRestart_Vars)
  {
    printf("  ## ERROR: LoadSU2SolutionBin : Incorrect number of restart vars.\n");
    return 1;
  }
  if (Restart_Vars[0] != 535532)
  {
    printf("  ## ERROR: LoadSU2SolutionBin : file is not a binary SU2 restart file.\n");
    printf("SU2 reads/writes binary restart files by default.\n");
    printf("Note that backward compatibility for ASCII restart files is\n");
    printf("possible with the WRT_BINARY_RESTART / READ_BINARY_RESTART options.\n");
    return 1;
  }

  SolSiz = Restart_Vars[1]-Msh->Dim;

  //--- Store SolTags except coordinates

  for (i = 0; i < Msh->Dim; i++) ret = fread(str_buf, sizeof(char), CGNS_STRING_SIZE, FilHdl);
  for (i = 0; i < SolSiz; i++)
  {
    ret = fread(str_buf, sizeof(char), CGNS_STRING_SIZE, FilHdl);
    strcpy(Msh->SolTag[i], str_buf);
  }

  //--- Allocate Msh->Sol

  Msh->Sol = (double*) malloc(sizeof(double)*(Msh->NbrVer+1)*SolSiz);
  memset(Msh->Sol, 0, sizeof(double)*(Msh->NbrVer+1)*SolSiz);

  Sol = Msh->Sol;
  Msh->SolSiz = SolSiz;

  //--- Set Msh->FldTab
  // Note: Set all fields as scalars (later: vectors for velocity)
  Msh->FldTab = (int*) malloc(sizeof(int)*SolSiz);
  Msh->NbrFld = SolSiz;
  for (i=0; i<SolSiz; i++)
    Msh->FldTab[i] = GmfSca;

  //--- Load vertex solution except coordinates
  for (i = 1; i <= Msh->NbrVer; i++)
  {
    ret = fread(Coord, sizeof(double), Msh->Dim, FilHdl);
    ret = fread(&Sol[i*SolSiz], sizeof(double), SolSiz, FilHdl);
  }

  if ( FilHdl ) fclose(FilHdl);

  return 0;
}

int LoadSU2Vertices(FILE *FilHdl, Mesh *Msh)
{
  int iVer, d, ref;
  double crd[3], bufDbl;
  char str[1024];

  rewind(FilHdl);

  Msh->NbrVer = GetSU2KeywordValue(FilHdl, "NPOIN=");

  if ( Msh->NbrVer > Msh->MaxNbrVer )
  {
    printf("  ## ERROR: LoadSU2Vertices : INCONSISTENT NUMBER OF VERTICES.\n");
    return 1;
  }

  for (iVer=1; iVer<=Msh->NbrVer; iVer++)
  {

    crd[2] = 0;

    for (d=0; d<Msh->Dim; d++)
    {
      fscanf(FilHdl, "%lf", &bufDbl);
      crd[d] = bufDbl;
    }

    fscanf(FilHdl, "%d", &ref);
    fgets(str, sizeof str, FilHdl);

    AddVertex(Msh, iVer, crd);
  }

  return 0;
}

void WriteSU2Mesh(char *nam, Mesh *Msh)
{
  int  i, j, s, idx;
  int  iVer,iCor,iTri,iEfr, iTet, iHex, iPri, iPyr, iQua, NbrElt=0;
  char OutNam[512];

  int Dim = Msh->Dim;

  int3 *BdrTag=NULL;
  int NbrBdr, NbrTag, start, iTag, cpt;
  int NbrCor;

  FILE *OutFil=NULL;

  GetBasNam(nam, OutNam);
  strcat(OutNam, ".su2");

  OutFil = fopen(OutNam, "wb");

  if ( !OutFil )
  {
    printf("  ## ERROR: WriteSU2Mesh : Can't open %s\n", OutNam);
    return 1;
  }

  fprintf(OutFil, "NDIME= %d\n", Dim);

  if ( Msh->Dim == 2 )
    NbrElt = Msh->NbrTri+Msh->NbrQua;
  else
    NbrElt = Msh->NbrTet+Msh->NbrHex+Msh->NbrPri+Msh->NbrPyr;

  fprintf(OutFil, "NELEM= %d\n", NbrElt);

  if ( Msh->Dim == 2 )
  {
    //--- Write triangles
    for (iTri=1; iTri<=Msh->NbrTri; iTri++)
    {
      fprintf(OutFil, "%d ", SU2_TRIANGLE);
      for (i=0; i<3; ++i)
      {
        fprintf(OutFil, "%d ",Msh->Tri[iTri][i]-1);
      }
      fprintf(OutFil, "%d\n", iTri-1);
    }

    //--- Write quads
    for (iQua=1; iQua<=Msh->NbrQua; iQua++)
    {
      fprintf(OutFil, "%d ", SU2_RECTANGLE);
      for (i=0; i<4; ++i)
      {
        fprintf(OutFil, "%d ",Msh->Qua[iQua][i]-1);
      }
      fprintf(OutFil, "%d\n", iQua-1);
    }

  }

  //--- Write tets
  for (iTet=1; iTet<=Msh->NbrTet; iTet++)
  {
    fprintf(OutFil, "%d ", SU2_TETRAHEDRAL);
    for (i=0; i<4; ++i)
    {
      fprintf(OutFil, "%d ",Msh->Tet[iTet][i]-1);
    }
    fprintf(OutFil, "%d\n", iTet-1);
  }

  //--- Write pyramids
  for (i=1; i<=Msh->NbrPyr; i++)
  {
    fprintf(OutFil, "%d ", SU2_PYRAMID);
    for (j=0; j<5; ++j)
    {
      fprintf(OutFil, "%d ",Msh->Pyr[i][j]-1);
    }

    fprintf(OutFil, "%d\n", i-1);
  }

  //--- Write prisms
  for (i=1; i<=Msh->NbrPri; i++)
  {
    fprintf(OutFil, "%d ", SU2_WEDGE);
    for (j=0; j<6; ++j)
    {
      fprintf(OutFil, "%d ",Msh->Pri[i][j]-1);
    }
    fprintf(OutFil, "%d\n", i-1);
  }

  //--- Write hexes
  for (i=1; i<=Msh->NbrHex; i++)
  {
    fprintf(OutFil, "%d ", SU2_HEXAHEDRAL);
    for (j=0; j<8; ++j)
    {
      fprintf(OutFil, "%d ",Msh->Hex[i][j]-1);
    }
    fprintf(OutFil, "%d\n", i-1);
  }

  //--- Write vertices
  fprintf(OutFil, "NPOIN= %d\n", Msh->NbrVer);

  if ( Msh->Dim == 2 )
  {
    for (iVer=1; iVer<=Msh->NbrVer; iVer++)
      fprintf(OutFil, "%.16le %.16le %d \n", Msh->Ver[iVer][0], Msh->Ver[iVer][1], iVer-1);
  }
  else
  {
    for (iVer=1; iVer<=Msh->NbrVer; iVer++)
      fprintf(OutFil, "%.16le %.16le %.16le %d \n", Msh->Ver[iVer][0], Msh->Ver[iVer][1], Msh->Ver[iVer][2], iVer-1);
  }

  //--- Write bdry elements

  if ( Msh->Dim == 2 )
  {
    BdrTag = (int3*)malloc(sizeof(int3)*Msh->NbrEfr);
    for (iEfr=1; iEfr<=Msh->NbrEfr; iEfr++)
    {
      BdrTag[iEfr-1][0] = Msh->Efr[iEfr][2];
      BdrTag[iEfr-1][1] = iEfr;
    }
    NbrBdr = Msh->NbrEfr;
  }
  else
  {
    BdrTag = (int3*)malloc(sizeof(int3)*(Msh->NbrTri+Msh->NbrQua));

    idx = 0;

    for (iTri=1; iTri<=Msh->NbrTri; iTri++)
    {
      BdrTag[idx][0] = Msh->Tri[iTri][3];
      BdrTag[idx][1] = iTri;
      BdrTag[idx][2] = SU2_TRIANGLE;
      idx++;
    }
    for (iQua=1; iQua<=Msh->NbrQua; iQua++)
    {
      BdrTag[idx][0] = Msh->Qua[iQua][4];
      BdrTag[idx][1] = iQua;
      BdrTag[idx][2] = SU2_RECTANGLE;
      idx++;
    }
    NbrBdr = Msh->NbrTri+Msh->NbrQua;
  }

  if ( NbrBdr > 0 )
  {
    qsort(BdrTag, NbrBdr, sizeof(int3), cmp_int2);
    NbrTag = 1;

    for (i=1; i<NbrBdr; i++)
    {
      if ( BdrTag[i][0] != BdrTag[i-1][0] )
      {
        NbrTag++;
      }
    }

    fprintf(OutFil, "NMARK= %d\n", NbrTag);

    start = 0;
    for (iTag=1; iTag<=NbrTag; iTag++)
    {
      fprintf(OutFil, "MARKER_TAG= %s\n", Msh->Markers[iTag]);

      cpt=1;
      for (i=start+1; i<NbrBdr; i++)
      {
        if ( BdrTag[i][0] != BdrTag[i-1][0] )
        {
          break;
        }
        cpt++;
      }

      fprintf(OutFil, "MARKER_ELEMS= %d\n", cpt);

      for (i=start; i<start+cpt; i++)
      {
        if ( Msh->Dim == 2 )
        {
          iEfr = BdrTag[i][1];
          fprintf(OutFil, "%d ", SU2_LINE);
          for (s=0; s<2; s++)
          {
            iVer = Msh->Efr[iEfr][s]-1;
            fprintf(OutFil, "%d ", iVer);
          }
          fprintf(OutFil, "\n");
        }
        else
        {
          if ( BdrTag[i][2] == SU2_TRIANGLE )
          {
            iTri = BdrTag[i][1];
            fprintf(OutFil, "%d ", SU2_TRIANGLE);
            for (s=0; s<3; s++)
            {
              iVer = Msh->Tri[iTri][s]-1;
              fprintf(OutFil, "%d ", iVer);
            }
            fprintf(OutFil, "\n");
          }
          else if ( BdrTag[i][2] == SU2_RECTANGLE )
          {
            iQua = BdrTag[i][1];
            fprintf(OutFil, "%d ", SU2_RECTANGLE);
            for (s=0; s<4; s++)
            {
              iVer = Msh->Qua[iQua][s]-1;
              fprintf(OutFil, "%d ", iVer);
            }
            fprintf(OutFil, "\n");
          }
        }
      }

      start = start+cpt;
    }//for iTag
  }
  else
    fprintf(OutFil, "NMARK= 0\n");

  //--- Write corners
  NbrCor = Msh->NbrCor;
  if( NbrCor > 0 )
  {
    fprintf(OutFil, "NCORNERS= %d\n", NbrCor);
    for (iCor=1; iCor<=Msh->NbrCor; iCor++)
    {
      fprintf(OutFil, "%d ", SU2_CORNER);
      fprintf(OutFil, "%d\n", Msh->Cor[iCor]-1);
    }
  }

  //--- close mesh file
  if ( OutFil ) fclose(OutFil);

  if ( BdrTag ) free(BdrTag);

  return;
}

int WriteSU2Solution (char *SolNam, int Dim, int NbrVer, double3 *Ver,  double *Sol, int SolSiz, char SolTag[100][256])
{
  int i, s, d;
  int iVer,idxVer;

  FILE *OutFil=NULL;
  OutFil = fopen(SolNam, "wb");

  if ( !OutFil )
    printf("  ## ERROR: WriteSU2Solution: Can't open %s\n", SolNam);

  //--- Write header

  fprintf(OutFil,"\"PointID\",\"x\",\"y\",");
  if ( Dim == 3 ) fprintf(OutFil,"\"z\",");
  for (i=0; i<SolSiz-1; i++)
  {
    fprintf(OutFil, "\"%s\",", SolTag[i]);
  }
  fprintf(OutFil, "\"%s\"\n", SolTag[i]);

  //--- Write solution at vertices

  for (iVer=1; iVer<=NbrVer; iVer++)
  {
    fprintf(OutFil, "%d, ", iVer-1);
    for (d=0; d<Dim; d++)
      fprintf(OutFil, "%.15le, ", Ver[iVer][d]);

    idxVer = iVer*SolSiz;
    for (i=0; i<SolSiz-1; i++)
    {
      fprintf(OutFil, "%.15le, ", Sol[idxVer+i]);
    }
    fprintf(OutFil, "%.15le\n", Sol[idxVer+i]);
  }

  //--- close mesh file
  if ( OutFil ) fclose(OutFil);


  return 0;
}

int WriteSU2SolutionBin (char *SolNam, int Dim, int NbrVer, double3 *Ver,  double *Sol, int SolSiz, char SolTag[100][256])
{
  int i;
  int var_buf_size = 5;
  int var_buf[5] = {535532, SolSiz+Dim, NbrVer, 0, 0};
  char str_buf[CGNS_STRING_SIZE];
  size_t ret;

  FILE *OutFil=NULL;
  OutFil = fopen(SolNam, "wb");

  if ( !OutFil )
  {
    printf("  ## ERROR: WriteSU2Solution: Can't open %s\n", SolNam);
    return 1;
  }

  //--- Write restart vars.
  ret = fwrite(var_buf, sizeof(char), var_buf_size*sizeof(int), OutFil);

  //--- Write SolTags, including coordinates.
  strncpy(str_buf, "x", CGNS_STRING_SIZE);
  ret = fwrite(str_buf, sizeof(char), CGNS_STRING_SIZE*sizeof(char), OutFil);
  strncpy(str_buf, "y", CGNS_STRING_SIZE);
  ret = fwrite(str_buf, sizeof(char), CGNS_STRING_SIZE*sizeof(char), OutFil);
  if (Dim == 3)
  {
    strncpy(str_buf, "z", CGNS_STRING_SIZE);
    ret = fwrite(str_buf, sizeof(char), CGNS_STRING_SIZE*sizeof(char), OutFil);
  }
  for (i = 0; i < SolSiz; i++)
  {
    strncpy(str_buf, SolTag[i], CGNS_STRING_SIZE);
    ret = fwrite(str_buf, sizeof(char), CGNS_STRING_SIZE*sizeof(char), OutFil);
  }

  //--- Write the actual solution.
  for (i = 1; i <= NbrVer; i++)
  {
    ret = fwrite(&Ver[i][0], sizeof(char), 2*sizeof(double), OutFil);
    if (Dim == 3) ret = fwrite(&Ver[i][2], sizeof(char), sizeof(double), OutFil);
    ret = fwrite(&Sol[i*SolSiz], sizeof(char), SolSiz*sizeof(double), OutFil);
  }

  //--- Close mesh file
  if ( OutFil ) fclose(OutFil);

  return 0;
}
