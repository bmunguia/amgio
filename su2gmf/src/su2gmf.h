/** @file su2gmf.h
 *  @brief Function prototypes for su2gmf interface.
 *
 *  Prototypes for interface to read and write SU2
 *  and GMF meshes and solutions, and convert between
 *  both formats.
 *
 *  @author Victorien Menier
 *  @author Brian Munguía
 *  @bug No known bugs.
 */

#include "common.h"
#include "Python.h"

/** @brief Wrapper function to convert SU2 mesh and solution to GMF.
 *
 *  @param[in] MshNam input SU2 mesh filename
 *  @param[in] SolNam input SU2 solution filename
 *  @param[in] OutNam output GMF mesh/solution basename
 *  @param[in] FldNam fields to output
 */
int SU2toGMF( char *MshNam, char *SolNam, char *OutNam, char *FldNam );

/** @brief Wrapper function to convert GMF mesh and solution to SU2.
 *
 *  @param[in] MshNam input SU2 mesh filename
 *  @param[in] SolNam input SU2 solution filename
 *  @param[in] OutNam output GMF mesh/solution filename
 */
int GMFtoSU2( char *MshNam, char *SolNam, char *OutNam );

/** @brief Wrapper function to convert GMF mesh (with SU2 boundary info) and solution to SU2.
 *
 *  @param[in] MshNam input GMF mesh filename
 *  @param[in] SolNam input GMF solution filename
 *  @param[in] BndMshNam input SU2 mesh filename
 *  @param[in] OutNam output SU2 mesh/solution filename
 */
int GMFWithBoundtoSU2( char *MshNam, char *SolNam, char *BndMshNam, char *OutNam );

/** @brief Wrapper function to extract GMF metric from solution file.
 *
 *  @param[in] MshNam input GMF mesh filename
 *  @param[in] SolNam input GMF solution filename
 *  @param[in] OutNam output GMF solution filename
 *
 *  @note Assumes the metric is stored in the last
 *        3*(nDim-1) columns of the solution.
 */
int GMFSoltoMet( char *MshNam, char *SolNam, char *OutNam );

/** @brief Wrapper function to read a SU2 or GMF mesh and solution into python.
 *
 *  @param[in] MshNam input mesh filename
 *  @param[in] SolNam input solution filename
 *  @param[out] pyVer mesh vertex coordinates
 *  @param[out] pyCor mesh corner indices
 *  @param[out] pyTri mesh triangle indices
 *  @param[out] pyTet mesh tetrahedron indices
 *  @param[out] pyEdg mesh edge indices
 *  @param[out] pyHex mesh hexahedron indices
 *  @param[out] pyQua mesh quadrilateral indices
 *  @param[out] pyPyr mesh pyramid indices
 *  @param[out] pyPri mesh prism indices
 *  @param[out] pySol solution
 *  @param[out] pySolHeader solution tags
 *  @param[out] pyMarkers mesh boundary marker tags
 */
void ReadMeshAndSol (char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet,
     PyObject *pyEdg, PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol,
     PyObject *pySolHeader,  PyObject *pyMarkers);

/** @brief Wrapper function to read a SU2 or GMF mesh into python.
 *
 *  @param[in] MshNam input mesh filename
 *  @param[out] pyVer mesh vertex coordinates
 *  @param[out] pyCor mesh corner indices
 *  @param[out] pyTri mesh triangle indices
 *  @param[out] pyTet mesh tetrahedron indices
 *  @param[out] pyEdg mesh edge indices
 *  @param[out] pyHex mesh hexahedron indices
 *  @param[out] pyQua mesh quadrilateral indices
 *  @param[out] pyPyr mesh pyramid indices
 *  @param[out] pyPri mesh prism indices
 *  @param[out] pyMarkers mesh boundary marker tags
 */
void ReadMesh (char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg,
     PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers);

/** @brief Wrapper function to read a SU2 or GMF solution into python.
 *
 *  @param[in] SolNam input solution filename
 *  @param[out] pySol solution
 *  @param[out] pySolHeader solution tags
 *  @param[out] pyMarkers mesh boundary marker tags
 *  @param[in] NbrVer size of mesh
 *  @param[in] Dim dimension of mesh
 */
void ReadSol (char *SolNam, PyObject *pySol, PyObject *pySolHeader, int NbrVer, int Dim);

/** @brief Wrapper function to write a SU2 or GMF mesh and solution from python.
 *
 *  @param[in] MshNam input mesh filename
 *  @param[in] SolNam input solution filename
 *  @param[in] pyVer mesh vertex coordinates
 *  @param[in] pyCor mesh corner indices
 *  @param[in] pyTri mesh triangle indices
 *  @param[in] pyTet mesh tetrahedron indices
 *  @param[in] pyEdg mesh edge indices
 *  @param[in] pyHex mesh hexahedron indices
 *  @param[in] pyQua mesh quadrilateral indices
 *  @param[in] pyPyr mesh pyramid indices
 *  @param[in] pyPri mesh prism indices
 *  @param[in] pySol solution
 *  @param[in] pySolHeader solution tags
 *  @param[in] pyMarkers mesh boundary marker tags
 *  @param[in] Dim dimension of mesh
 */
void WriteMeshAndSol(char *MshNam, char *SolNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet,
     PyObject *pyEdg,  PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pySol,
     PyObject *pySolHeader, PyObject *pyMarkers, int Dim);

/** @brief Wrapper function to write a SU2 or GMF mesh from python.
 *
 *  @param[in] MshNam input mesh filename
 *  @param[in] pyVer mesh vertex coordinates
 *  @param[in] pyCor mesh corner indices
 *  @param[in] pyTri mesh triangle indices
 *  @param[in] pyTet mesh tetrahedron indices
 *  @param[in] pyEdg mesh edge indices
 *  @param[in] pyHex mesh hexahedron indices
 *  @param[in] pyQua mesh quadrilateral indices
 *  @param[in] pyPyr mesh pyramid indices
 *  @param[in] pyPri mesh prism indices
 *  @param[in] pyMarkers mesh boundary marker tags
 *  @param[in] Dim dimension of mesh
 */
void WriteMesh(char *MshNam, PyObject *pyVer, PyObject *pyCor, PyObject *pyTri, PyObject *pyTet, PyObject *pyEdg,
     PyObject *pyHex, PyObject *pyQua, PyObject *pyPyr, PyObject *pyPri, PyObject *pyMarkers, int Dim);

/** @brief Wrapper function to write a SU2 or GMF solution from python.
 *
 *  @param[in] SolNam input solution filename
 *  @param[in] pyVer mesh vertex coordinates
 *  @param[in] pySol solution
 *  @param[in] pySolHeader solution tags
 *  @param[in] NbrVer size of mesh
 *  @param[in] Dim dimension of mesh
 */
void WriteSol(char *SolNam, PyObject *pyVer, PyObject *pySol, PyObject *pySolHeader, int NbrVer, int Dim);
