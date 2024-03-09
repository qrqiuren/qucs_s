# Qucs-S 24.1.0

## Important notes

* Versioning switched to CalVer. The first digit means year, the second digit means the release number,
  the third digit means patch number #328

## New features

* Added 3-pin RCD devices #349 to improve support of microelectronics PDKs
* Improvements of Ngspice support; allowed multiple simulation of the same type #463 by @ivandi69
* Automatic extraction of THD from FOURIER simulation #525 @ivandi69
* Nutmeg equations improvements; implemented simulation selection from drop-down list #463 #519
* Improvements of Nutmeg script simulation type; implemented custom prefixes for plots #476 #435 @ivandi69
* Export DC OP simulation data to dataset #463 #140 @ivandi69
* Implemented zoom by selection on the rectangular plots #305
* Implemeted potentiometer device #122
* Switched digital simulation from FreeHDL to GHDL #395 @iwbnwif
* Status bar improvements #493

## RF simulation features

* Added RCLG device support for SPICE #564
* Added YLIN device support for XYCE #451
* Fixed touchstone simulation issues for Ngspice #392
* Added BiasT device support for SPICE #374
* Added option to disable transient model for RF port #410 

## Libraries

* Fixed different issues related to creating libraries #432
* Added audio IC library
* Added loudspeaker library #421
* Added comparators library #436 
* Modified voltage regulators library #433
* Added voltage references library #439
* Added thermistor library #538
* Fixed ideal library #258 #428
* Fixed transformers library #362
* Added new devices in SpiceOpamp library #501

## General improvements

* Fixed different tuner issues
* Schematic zooming improvements #502
* Different improvements on schematic look and control
* Added schematic pan mode with the middle mouse button #488
* HiDPI support improvements including grid rendering
* Fixed schematic glitches using Wayland #227
* Added text document zoom and font setting #534

## Documentation

* Added new examples 
* Added Ngspice templates #441
* The readthdocs documentantion is a candidate for deprecation #554

## Localization

* Added Chinese translation #453 by @hvwyl

Thanks @wawuwo and @iwbnwif for providing refactoring PRs! Many thanks to @tomhajjar for providing libraries, examples and testing.

# Qucs-S 2.1.0

## New features

* Added tuner simulation mode #123 (used implementation by @tipofthesowrd)
* Added support for touchstone s2p file devices for Ngspice #233 (implementation by @transmitterdan)
* Redesign of the simulation window #309
* Diagrams are placed on schematic and `*.dpl` files are disabled by default  #309 
* Backport of the active Legendre filter synthesis from Qucs-0.0.20rc2 #318
* Added SPICE entry for MUT and MUT2 devices
* Backported circular waveguide and tapered line symbols from Qucs-0.0.20rc2
* Added zoom to selection mode #327
* Toolbar icons replaced by SVG; improved look for HiDPI #323 #304
* Implemented automatic generation of high resolution device icons #334 #304

## General improvements

* Added Tstart option for Spectrum simulaiton #336
* Added low voltage for VRect source #310
* Russian translation update #292 #324
* Added SPICE group for project tree

## Bugfixes

* Fixed simulation of the files with double extension #311
* Fixed simulation of the files containing unicode symbols #312
* Fixed display of the Xanalogue.lib library #308
* Fixed show checker errors for Xyce
* Fixed load symbol for Verilog-A device #321
* Fixed component rotation with right mouse button

## Library

* Added thyristor library #212
* Added optocoupler library #213
* Added photovoltaic relays library #222

## Documentation

* Added subcircuit symbol templates to `share/qucs_s/examples/symbols` (by @tomhajjar)
* Added sections on tuner mode and RF simulation to the Qucs-S tutorial

## Packaging

* Windows 32-bit package is not provided anymore

# Qucs-S 2.0.0

## New features

* Implemented quick switch of the simulator without application restart #274 (thanks @Zergud)

## Packaging

* Linux AppImage build switched to Github workflow #281 
* Added Windows 64-bit installer with Ngspice

## Component library

* Fixed issues in Transformers library #228 and #231

## General improvements

* Finished Qt6 porting #190
* Show warning if the directory protected by Windows UAC #282
* Fixed PDF print on Windows #285
* Added option to export grayscale image #288
* Fixed setting initial conditions in wire labels #291
* Fixed missing ground warning

# Qucs-S 1.1.0

## New features

* Qucs-S supports digital simulation mode using IcarusVerilog or FreeHDL backends #97
* Added analog model for digital source #265
* Qucs-S runs DC bias simulation mode if schematic contains DC simulation only #271
* Selection rectangle update #276

## Deprecated features

* XSPICE CodeModel synthesiszer has been deprected since v1.0.3. The precompiled CM libraries
  are still supported using *spiceinit* virtual device. See #204 

## Known issues of digital simulation

* Digital buses are not supported;
* Digital simulation represents its state as of Qucs-0.0.20-RC1; No new features added


# Qucs-S 1.0.2

## New features

* Added XSPICE flip-flop digital devices #262 (thanks @Radvall)
* Added INDQ and CAPQ devices representing inductor and capacitor with Q-factor #143
* Added .LIB directive support #242
* Added SPICE entries for I and V file sources #254
* Added symbols for 4 and 5 terminal BJT device #198

## Component library

* Added behavioral XSPICE 555 timer model #252

## General improvements

* Added option to save SPICE netlist without simulation #225
* Improved Qt6 compatibility (thanks @Zergud)
* Improved MacOS compatibility (thanks @nanoant) #214
* CMake build system update to bring features of the modern CMake (thanks @dsm) #216


# Qucs-S 1.0.1

## New features

* Added support for Verilog-A defined devices using Ngspice+OpenVAF #197. Old Qucsator+ADMS
  workflow is marked as deprecated. Ngspice >=39 is required to support OpenVAF.
* Implemented variable parameter sweep analysis #189
* Added possibility to build Qucs-S with Qt6 #190 The -DWITH=QT6=ON option for cmake is required. 
  The source code of the Qucs-S is Qt6 compatible now. Thanks @Zergud !
* Simulator prefixes like "ngspice/" are not show anymore on plot traces if the plot contains data 
  from only one simulator #178 


## Component library

* Added TL071/TL072 opamp models
* Added 555_timer.lib library containing the 555 timer model
* Added extended BJT exteneded and Schottky diodes libraries

## Bugfix and general improvements

* Change default RCL device values and simulation settings to prevent convergence error when putting 
  together device with default values #195
* Fixed auto-scaling issue on Windows 64-bit platform #133

# Qucs-S 1.0.0

## New features

* Added basic support for logic gates simulation (Ngspice >= 38) required #97
* Added possibility to set engineering notation (like 1k, 1M, etc.) for numbers on diagrams #92
* Added symbol preview and search feature for component library in main windows #135
* Backported qucs-attenuator, qucs-transcalc and qucs-powercombining from Qucs-0.0.20 #142
* Added possibility to set dB units for Y-axis on digrams from the diagram properties dialog #92
* Backported advanced features for projects tree from Qucs-0.0.20 #95
* Show warning if schematic file name changed, but dataset/display file name are not updated #111
* Revised examples tree (by Tom Hajjar @tomhajjar); legacy Ngspice-incompatible examples 
  moved to `examples/qucsator` subdirectory

## Component library

* Qucs-lib tool has been removed from the application; all its features are
  availbale now from the main window left panel #139
* Fixed Tubes.lib and LEDs.lib libraries (by @tomhajjar)

## General improvements

* Fixed numerous bug reports. Thanks @tomhajjar and @Zergud for testing.

## Windows

* Added 32-bit Windows portable package

## Known issues

* Parameter sweep doesn't work when using the Fourier simulation with XYCE backend #73
* Netlister may produce duplicate .SUBCKT entry when using the same component from user and system library #82
* Auto-scaling on diagram may fail on some conditions for Windows 64-bit platform #133 Other platforms are not affected
* The 1-point AC simulation produce a set of 1x1 plot if ParameterSweep instead of smooth curve is attached #156


# Qucs-S 0.0.24

## New simulation types

* Added S-parameter simulation using Ngspice and Xyce backends; Ngspice>=37 requiered #84, #79
* Added FFT analysis with Ngspice backend #88

## GUI improvements

* Implemented dark desktop theme support #66
* Implemented manual setting of the X-axis markers postion #92
* Added separate setting for schematic and application font

## Qt5 support

* Removed QtScript dependency (thanks @Zergud) #77
* Fixed Qt5.15 deprecation warnings #75

## Component libraries

* Added quartz crystal library (Crystal.lib); the quartz model is optimized for filter design;
* Added TL431 device in Regulators library;

## General bugfixes

* Fixed crash on HB simulation with XYCE #28
* Fixed empty path for working directory #86
* Show error when schematic contains only DC simulation (no effect for Ngspice) #83
* Fixed layout in arrow properties dialog #81
* Fixed drag'n'drop operation from left dock to schematic
* Fixed newlines in SPICE resitors #62
* Fixed crash on Wayland #76

## Documentation and examples

* Added offline PDF tutorial; the link for old Qucs tutorial removed from Help menu #78
* Added examples for new S-parameter and FFT simulations
* Fixed distortion simulation example #83

## Known issues

* Parameter sweep doesn't work when using the Fourier simulation with XYCE backend #73
* Netlister may produce duplicate .SUBCKT entry when using the same component from user and system library #82

# Qucs-S 0.0.23

## Qt5 support and new features

* The Qucs-S application is now fully ported to Qt5 and could be compiled on modern Linux distributions; #72 #32
* Added two new component libraries: BF998 and Tubes containing two-gate MOSFET and vacuum tubes (triodes and penthodes provided by @olegkapitonov ) models;
* Windows binary switched to 64-bit build. The old 32-bit binaries are not provided anymore;
* Ngspice is now the default simulation kernel on the first application start;
* The Qucs-S doesn't use a shared settings file with Qucs anymore #71

## General bugfixes

* Fixed "Show last netlists" not showing SPICE netlist when SPICE engine is selected #41
* Use system default sans serif font instead of "Helvetica", because "Helvetica" cannot be rendered correctly on some platforms;
* Fixed .INCLUDE directive support with SpiceOpus #16
* Partially fixed .FOUR simulation support for Xyce backend #73
* Removed warning about experimental feature in the simulation dialog. Qucs-S is considered to be stable now;

