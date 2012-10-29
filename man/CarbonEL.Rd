\name{CarbonEL}
\alias{CarbonEL}
\title{
  Carbon Event Loop
}
\description{
  \code{CarbonEL} package installs a Carbon Event Loop in a running R process.
}
\details{
  When loaded, this package hooks a Carbon even loop handler into R's
  event loop and declares the currect process as a foreground
  application.

  This allows an interactive use of Apple UI, such as the Quartz device
  from shell R console or ESS. Note that this is superfluous if R was
  started using the R.app.

  There are two global options respected by this package at load time:

  \code{cel.sleep} determines the sleep between event polls (in seconds,
  fractions allowed). Longer sleeps reduce the CPU load, shorter sleeps
  improve reaction times (smoothness of the interface). The default is
  0.2 (i.e. 200ms).

  This value can be also set after the initialization by the
  \code{.cel.set.sleep} function.
  
  \code{cel.acitvate} is a boolean determining whether the process
  should be activated as a foreground process. The default is
  \code{TRUE} for yes.

  If CarbonEL detects that it is loaded from within the R.app GUI, it
  will not start the event loop unless \code{FORCE_CARBONEL} environment
  variable is set. R.app runs a Cocoa loop which is functionally
  equivalent to a Carbon loop, so there is no need for CarbonEL in the
  R.app GUI.
  
  NOTE: This package is intended only for Mac OS X. It does nothing on
  all other platforms.
}
\keyword{interface}
