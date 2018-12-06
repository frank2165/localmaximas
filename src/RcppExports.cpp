// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// test_get_coordinates
Rcpp::NumericMatrix test_get_coordinates(Rcpp::S4 handle);
RcppExport SEXP _localmaximas_test_get_coordinates(SEXP handleSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::S4 >::type handle(handleSEXP);
    rcpp_result_gen = Rcpp::wrap(test_get_coordinates(handle));
    return rcpp_result_gen;
END_RCPP
}
// test_get_heights
Rcpp::NumericVector test_get_heights(Rcpp::S4 handle);
RcppExport SEXP _localmaximas_test_get_heights(SEXP handleSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::S4 >::type handle(handleSEXP);
    rcpp_result_gen = Rcpp::wrap(test_get_heights(handle));
    return rcpp_result_gen;
END_RCPP
}
// test_index_missing
Rcpp::IntegerVector test_index_missing(Rcpp::S4 handle, Rcpp::NumericVector rcppHeights);
RcppExport SEXP _localmaximas_test_index_missing(SEXP handleSEXP, SEXP rcppHeightsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::S4 >::type handle(handleSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type rcppHeights(rcppHeightsSEXP);
    rcpp_result_gen = Rcpp::wrap(test_index_missing(handle, rcppHeights));
    return rcpp_result_gen;
END_RCPP
}
// R_maxima_search
IntegerVector R_maxima_search(NumericMatrix xy, NumericVector z, double eps);
RcppExport SEXP _localmaximas_R_maxima_search(SEXP xySEXP, SEXP zSEXP, SEXP epsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericMatrix >::type xy(xySEXP);
    Rcpp::traits::input_parameter< NumericVector >::type z(zSEXP);
    Rcpp::traits::input_parameter< double >::type eps(epsSEXP);
    rcpp_result_gen = Rcpp::wrap(R_maxima_search(xy, z, eps));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_localmaximas_test_get_coordinates", (DL_FUNC) &_localmaximas_test_get_coordinates, 1},
    {"_localmaximas_test_get_heights", (DL_FUNC) &_localmaximas_test_get_heights, 1},
    {"_localmaximas_test_index_missing", (DL_FUNC) &_localmaximas_test_index_missing, 2},
    {"_localmaximas_R_maxima_search", (DL_FUNC) &_localmaximas_R_maxima_search, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_localmaximas(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
