

# User defined variables
radius <- 1.5
tifs <- dir(pattern = "\\.tif$", full.names = TRUE, recursive = TRUE)

# Attach Packages
library(future)
library(localmaximas)


plan("multisession", workers = 8)

system.time(local_maxima_search(tifs, radius))

gc()

.rs.restartR()
