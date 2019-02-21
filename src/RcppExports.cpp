// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// FindLocalMaxima
Rcpp::List FindLocalMaxima(Rcpp::List handles, double radius, int numCores);
RcppExport SEXP _localmaximas_FindLocalMaxima(SEXP handlesSEXP, SEXP radiusSEXP, SEXP numCoresSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::List >::type handles(handlesSEXP);
    Rcpp::traits::input_parameter< double >::type radius(radiusSEXP);
    Rcpp::traits::input_parameter< int >::type numCores(numCoresSEXP);
    rcpp_result_gen = Rcpp::wrap(FindLocalMaxima(handles, radius, numCores));
    return rcpp_result_gen;
END_RCPP
}
// SearchNeighbours
arma::Col<unsigned int> SearchNeighbours(arma::Mat<double>& xy, arma::Col<double>& z, double eps);
RcppExport SEXP _localmaximas_SearchNeighbours(SEXP xySEXP, SEXP zSEXP, SEXP epsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< arma::Mat<double>& >::type xy(xySEXP);
    Rcpp::traits::input_parameter< arma::Col<double>& >::type z(zSEXP);
    Rcpp::traits::input_parameter< double >::type eps(epsSEXP);
    rcpp_result_gen = Rcpp::wrap(SearchNeighbours(xy, z, eps));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_localmaximas_FindLocalMaxima", (DL_FUNC) &_localmaximas_FindLocalMaxima, 3},
    {"_localmaximas_SearchNeighbours", (DL_FUNC) &_localmaximas_SearchNeighbours, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_localmaximas(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
