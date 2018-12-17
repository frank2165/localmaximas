# .onLoad <- function(lib, pkg) {
#     library.dynam('localmaximas', pkg, lib)
# }

#' @useDynLib localmaximas
#' @importFrom Rcpp sourceCpp
NULL



.onUnload <- function(libpath) {
    library.dynam.unload('localmaximas', libpath)
}




rebuild <- function(path = NULL){
    tryCatch(detach("package:localmaximas"))
    tryCatch(unloadNamespace("localmaximas"))
    if(is.null(path)){
        path <- devtools::build()
    }
    install.packages(path, repos = NULL, type = "source")
    library(localmaximas)
}
