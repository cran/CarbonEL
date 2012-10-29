.cel.set.sleep <- function(sleep = 0.2) {
  .Call("cel_set_sleep", sleep, PACKAGE="CarbonEL")
}

.cel.stop <- function() {
  .C("stopt", PACKAGE="CarbonEL")
  invisible(NULL)
}

.First.lib <- function(libname, pkgname) {
    if (!length(grep("^darwin",R.version$os))) {
        cat("This package works on Mac OS X only.\nIt does nothing on all other platforms.\n")
    } else {
        if (nchar(Sys.getenv("R_GUI_APP_VERSION")) && !nchar(Sys.getenv("FORCE_CARBONEL"))) {
            cat("CarbonEL is not needed in the R.app GUI.\nIf you want to force CarbonEL to load anyway,\nset the FORCE_CARBONEL environment variable.\n")
        } else {
            library.dynam("CarbonEL", pkgname, libname)

            inter.time <- if (is.null(getOption("cel.sleep"))) 0.2 else as.numeric(getOption("cel.sleep"))
            activate <- if (is.null(getOption("cel.activate"))) TRUE else as.logical(getOption("cel.activate"))

            invisible(.Call("aih", inter.time, activate, PACKAGE="CarbonEL"))
        }
    }
}
