/*! \mainpage pClasses DOCUMENTATION
\author G.Pasquali
 
  \section intro_sec General overview
 
  A small library of classes to simulate the components of a digitizing system. It is not
  optimized in any way (I would like it to be optimized for clarity...). Certainly not
  optimized for calculation speed, memory use, style of C++ programming!
 
  Apart from the basic classes, a few root macros are provided:
  <ul>
  <li> init_pClasses.C: used to load pClasses libraries at the start of a session as follows: after launching root,
  type at the root prompt
 \code
 .x init_pClasses.C
 \endcode
 <li> test.C: contains various test functions. See test.C page for details.
</ul>

\section install Installing pClasses on your PC

<p>pClasses need the ROOT Data Analysis Framework. They have been tested under Linux using a few ROOT versions,
between 5.28 and 5.34. Therefore, we expect them to work with any version >=5.28 and <6.00 (no attempt
to port them to ROOT 6 has been attempted).</p>
<p>To install ROOT, refer to http://root.cern.ch/drupal/content/downloading-root. We suggest to install
a "Pro" version (presently the 5.34) using sources. After downloading ROOT sources, follow instructions on
http://root.cern.ch/drupal/content/installing-root-source. You'll probably need to install the prerequisite
packages, see http://root.cern.ch/drupal/content/build-prerequisites.</p>
<p>You should also install <em>doxygen</em>, an application to automatically produce source
documentation. We used doxygen to produce the docs you're reading. Doxygen will be used
to produce the same docs on your PC. Make sure you installed also <em>graphviz</em>, an
utility needed by <em>doxygen</em> which sometimes is not automatically installed. </p>
<p>After ROOT installation and testing (be sure to set the needed environment variables LD_LIBRARY_PATH and ROOTSYS
correctly), download the pClasses.zip or the pClasses.tgz archive
from the course site (page: http://www.lnl.infn.it/~garfweb/e_digit/index.php?pID=3) and unpack
it in your working directory. Now change directory to pClasses and type
\code
make
\endcode
This should compile pClasses libraries. If no errors are produced, you should be able to test
your installation by running some of the provided macros. For instance, after
starting ROOT, type at the ROOT prompt:
\code
 .x init_pClasses.C
 .L test.C
 test_crrc()
 \endcode
This should produce a plot like this, showing a simulated charge preamp signal and a CR-RC shaped signal
obtained from the former:
\image html  test_crrc.png "Plot produced by test_crrc() function"
Apart from a deficit of the maximum amplitude of the red signal with respect to the black one,
a zoom on the Y-axis should evidence a long negative tail. Both effects are due to the missing pole-zero correction (PZC).
\image html  test_crrc_zoom.png "Long negative tail due to not corrected exponential decay on the input signal"
To see the effect of a CR-RC shaper including PZC you can give the command:
\code
 test_crrc_pzc()
 \endcode
which should give you this plot:
\image html  test_crrc_pzc.png "Plot produced by test_crrc_pzc() function"
The maximum amplitude of the shaped signal now is equal to the maximum amplitude of the preamp signal.
The negative tail should not be present, as can be seen zooming the Y-axis:
\image html  test_crrc_pzc_zoom.png "Long negative tail due to not corrected exponential decay on the input signal"

<p>If you have been able to perform all of the above, you have probably installed the pClasses correctly.</p>

  \section classes Classes
  
  The main tasks in a digitizing system are: 
  <ul>
  <li> obtaining a digitized sequence (digital signal) from a continuous signal
  <li> performing processing on the digitized signal in order to extract the desired info
 </ul> 
  Therefore I decide to start from the following basic classes:
 <ol>
 <li>pADC: describes the sampling operation, simulating sampling of typical signals (sine wave, charge preamp signal,...). Included the effects of sampling noise and clock jitter, if desired.
            Output can be in integer (e.g -2048-2047)  of fractional format (from -1 to, almost, 1)
  <li>pSignal: stores the digitized signal obtained from sampling or from elaboration of other signals
 <li> pDSP: performs various Digital Signal Processing (DSP) algorithm on signals
 <li> pUtility: various utility functions 
 <li> pCFD: (added October 2016) hosts a handful of digital CFD algorithms based on different interpolation schemes. The dCFD already present 
 in pSignal have been kept for backward compatibility. However, the pCFD class is now considered the "standard" way to access to the dCFD
 capability in pClasses.
</ol>
  
\section dat2root dat2root
A small utility called dat2root is also provided (source in src/dat2root.C) to convert data
files from .dat format (used by some acquisition programs employed in our lab) to
.root files. Signals are converted to pSignal objects and stored into a TTree object.
See documentation of dat2root for details.

\section doc Documentation
All the documentation (including the page you are reading :P ) is generated via DoxyGen. The trick is to add comments 
with / / ! syntax just before the classes/methods/variables definitions: doxygen will read all the files and build an html documentation
with all your comments. Simple and effective.

This page is placed in manual.h


*/
