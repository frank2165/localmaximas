
/* Apparently RcppExport is a macro for extern "C"
#ifdef __cplusplus
extern "C" {
#endif  
*/

#ifndef R_NO_REMAP
#define R_NO_REMAP
#endif

#include <R.h>
#include <Rinternals.h>


SEXP test_get_coordinates(SEXP sxpHandle);
SEXP test_get_heights(SEXP sxpHandle);
SEXP test_index_missing(SEXP sxpHandle);


/*
#ifdef __cplusplus
}
#endif
*/