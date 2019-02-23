## Script testing the openMP parallel processing implementation
##

# User defined variables
radius <- 1.5
tifs <- dir(pattern = "\\.tif$", full.names = TRUE, recursive = TRUE)

# Attach Packages
library(RANN)
library(rgdal)
library(magrittr)
library(localmaximas)


## Helper functions
plot_tif <- function(f){
    readGDAL(f, silent = TRUE) %>% plot
}


N <- vapply(tifs, function(f){
    readGDAL(f, silent = TRUE) %>%
        slot("data") %>%
        extract2(1) %>%
        is.na %>%
        not %>%
        sum
}, numeric(1))

idx  <- order(N, decreasing = FALSE)
tifs <- tifs[idx]

system.time(local_maxima_search(tifs, radius, 4))

gc()
