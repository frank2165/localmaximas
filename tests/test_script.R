## Script testing the openMP parallel processing implementation
##

# User defined variables
radius <- 1.5
tifs <- dir(pattern = "\\.tif$", full.names = TRUE, recursive = TRUE)

# Attach Packages
library(rgdal)
library(future)
library(magrittr)
library(localmaximas)


# Setup cluster
plan("multisession", workers = 4)



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

system.time(maxima <- local_maxima_search(tifs, radius))

gc()


maxima <- lapply(maxima, function(xy) {
    SpatialPoints(coords = xy)
})


plot_tif(tifs[17])
plot(maxima[[17]], col = "red", add = TRUE)
