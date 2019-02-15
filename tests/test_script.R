## Files 1, 6, 12 Worked
## Files 2, 5, Frozen
## Files 16 Crashed

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

system.time(local_maxima_search(tifs[c(3, 4)], radius, 2))

gc()



# ## Setup
# chm <- readGDAL(tifs[8], silent = TRUE)
# coords <- coordinates(chm)
# z <- chm@data[[1]]
# 
# idx.keep <- which(!is.na(z))
# coords <- coords[idx.keep, ]
# z <- z[idx.keep]
# 
# i <- 1
# nn.idx <- nn2(data = coords, query = coords[i, , drop = FALSE], 
#               searchtype = "radius", radius = radius) %>% 
#     extract2("nn.idx")
# 
# z[nn.idx]
# which.max(z[nn.idx])
