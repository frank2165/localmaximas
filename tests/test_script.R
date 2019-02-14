## Files 1, 6, 12 Worked
## Files 2, 5, Frozen
## Files 16 Crashed

# User defined variables
radius <- 1.5
tifs <- dir(pattern = "\\.tif$", full.names = TRUE, recursive = TRUE)

# Attach Packages
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

system.time(local_maxima_search(tifs[c(1, 2, 1, 2)], radius, 2))

gc()

