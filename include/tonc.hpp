//
//  Main tonc header
//
//! \file tonc.h
//! \author J Vijn
//! \date 20060508 - 20080825
//
// === NOTES ===


#ifndef TONC_MAIN
#define TONC_MAIN

#include "tonc_types.hpp"
#include "tonc_memmap.hpp"
#include "tonc_memdef.hpp"

#include "tonc_bios.hpp"
#include "tonc_core.hpp"
#include "tonc_input.hpp"
#include "tonc_irq.hpp"
#include "tonc_math.hpp"
#include "tonc_oam.hpp"
#include "tonc_tte.hpp"
#include "tonc_video.hpp"
#include "tonc_surface.hpp"

#include "tonc_nocash.hpp"

// For old times' sake
#include "tonc_text.hpp"

// --- Doxygen modules: ---

/*!	\defgroup grpBios	Bios Calls			*/
/*!	\defgroup grpCore	Core				*/
/*! \defgroup grpDma	DMA					*/
/*! \defgroup grpInput	Input				*/
/*! \defgroup grpIrq	Interrupt			*/
/*! \defgroup grpMath	Math				*/
/*!	\defgroup grpMemmap Memory Map			*/
/*! \defgroup grpAudio	Sound				*/
/*! \defgroup grpTTE	Tonc Text Engine	*/
/*! \defgroup grpText	Old Text			*/
/*! \defgroup grpTimer	Timer				*/
/*! \defgroup grpVideo	Video				*/



/*!	\mainpage	Tonclib 1.4 (20080825)
	<p>
	Tonclib is the library accompanying the set of GBA tutorials known 
	as <a href="http://www.coranac.com/tonc/">Tonc</a>  Initially, it 
	was just a handful of macros and functions for dealing with the 
	GBA hardware: the memory map and its bits, affine transformation 
	code and things like that. More recently, more general items 
	have been added like tonccpy() and toncset(), the TSurface system 
	and TTE. All these items should provide a firm basis on which to 
	build GBA software.
	</p>
*/

#endif // TONC_MAIN

