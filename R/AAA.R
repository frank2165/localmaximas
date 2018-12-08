.onLoad <- function(lib, pkg) {
    library.dynam('localmaximas', pkg, lib)
}

.onUnload <- function(libpath) {
    library.dynam.unload('localmaximas', libpath)
}
