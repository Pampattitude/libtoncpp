//
//  no$gba messaging functionality
//
//! \file tonc_nocash.h
//! \author J Vijn
//! \date 20080422 - 20080422
//
/* === NOTES ===
 */

#ifndef TONC_NOCASH
#define TONC_NOCASH

#include "tonc_types.hpp"

/*!	\defgroup grpNocash no$gba debugging
    \ingroup grpCore
    The non-freeware versions of no$gba have window to which you
    can output messages for debugging purposes. These functions allow
    you to work with that.
*/

/*! \addtogroup grpNocash	*/
/*!	\{	*/

// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------

extern EWRAM_DATA char nocash_buffer[80];

// --------------------------------------------------------------------
// PROTOTYPES
// --------------------------------------------------------------------

extern "C"
{
    //!	Output a string to no$gba debugger.
    /*!
        \param str	Text to print.
        \return		Number of characters printed.
    */
    int nocash_puts(const char *str);

    //! Print the current \a nocash_buffer to the no$gba debugger.
    EWRAM_CODE void nocash_message(void);

    /*!	\}	*/
}

#endif // TONC_NOCASH

// EOF
