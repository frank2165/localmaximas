## Files 1, 6, 12 Worked
## Files 2, 5, Frozen
## Files 16 Crashed

# User defined variables
radius <- 1.5
tifs <- dir(pattern = "\\.tif$", full.names = TRUE, recursive = TRUE)

# Attach Packages
library(localmaximas)

system.time(local_maxima_search(tifs[5], radius, 1))

gc()




## Erata

library(rgdal)
library(magrittr)

N <- vapply(tifs, function(f){
    readGDAL(f, silent = TRUE) %>%
        slot("data") %>%
        extract2(1) %>%
        is.na %>%
        not %>%
        sum
}, numeric(1))

order(N, decreasing = FALSE)


plot_tif <- function(f){
    readGDAL(f, silent = TRUE) %>% plot
}


## FindLocalMaxima
tif.data <- readGDAL(tifs[12], silent = TRUE)

tif.xy <- coordinates(tif.data)
tif.z <- tif.data@data[[1]]
idx.na <- which(is.na(tif.z))

tif.xy <- tif.xy[-idx.na, ]
tif.z <- tif.z[-idx.na]

## kd_tree
tif.bbox <- apply(tif.xy, 2, range)


## kd_split
max_length <- apply(tif.bbox, 2, function(x) max(diff(x))) %>% max
spr <- max_length
cut_dim <- 1 # 0

ideal_cut_val <- (tif.bbox[1, cut_dim] + tif.bbox[2, cut_dim])/2
cut_dim.min <- tif.bbox[1, cut_dim]
cut_dim.max <- tif.bbox[2, cut_dim]

