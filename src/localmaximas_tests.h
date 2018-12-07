
#include "localmaximas.h"


/* Apparently RcppExport is a macro for extern "C"
#ifdef __cplusplus
extern "C" {
#endif  
*/


SEXP test_get_coordinates(SEXP sxpHandle);
SEXP test_get_heights(SEXP sxpHandle);
SEXP test_index_missing(SEXP sxpHandle, SEXP sxpHeights);


/*
#ifdef __cplusplus
}
#endif
*/