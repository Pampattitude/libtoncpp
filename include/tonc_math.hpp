//
//  Mathematical functions
//
//! \file tonc_math.h
//! \author J Vijn
//! \date 20060508 - 20060908
//
// === NOTES ===

#ifndef TONC_MATH
#define TONC_MATH

#include "tonc_types.hpp"

// --- Doxygen modules ---

/*! \defgroup grpMathBase	Base math
 *	\brief Basic math macros and functions like MIN, MAX
 *	\ingroup grpMath
 */

/*! \defgroup grpMathFixed	Fixed point math
 *	\ingroup grpMath
 */

/*! \defgroup grpMathLut	Look-up tables
 *	\brief Tonc's internal look-up tables and related routines.
 *	\ingroup grpMath
 */

/*! \defgroup grpMathPoint	Point functions
 *	\ingroup grpMath
 */

/*! \defgroup grpMathVector	Vector functions
 *	\ingroup grpMath
 */

/*! \defgroup grpMathRect	Rect functions
 *	\ingroup grpMath
 */

// --------------------------------------------------------------------
//   GENERAL
// --------------------------------------------------------------------

/*!	\addtogroup grpMathBase	*/
/*! \{	*/

// Also available as INLINE constexpr functions

//! \name core math macros
//\{

#ifndef ABS
//! Get the absolute value of \a x
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#endif // ABS

#ifndef SGN
//! Get the sign of \a x.
#define SGN(x) ((x) >= 0 ? 1 : -1)
#define SGN2   SGN
#endif // SGN

#ifndef SGN3
//! Tri-state sign: -1 for negative, 0 for 0, +1 for positive.
#define SGN3(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#endif // SGN3

#ifndef MAX

//! Get the maximum of \a a and \a b
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

//! Get the minimum of \a a and \a b
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif // MAX

#ifndef SWAP
//! In-place swap.
#define SWAP2(a, b)    \
    do                 \
    {                  \
        a = (a) - (b); \
        b = (a) + (b); \
        a = (b) - (a); \
    } while (0)

#define SWAP SWAP2

// Alternative:
// #define SWAP2(a, b)	( (b) ^= ((a) ^= ((b) ^= (a))) )

//! Swaps \a a and \a b, using \a tmp as a temporary
#define SWAP3(a, b, tmp) \
    do                   \
    {                    \
        (tmp) = (a);     \
        (a)   = (b);     \
        (b)   = (tmp);   \
    } while (0)
#endif // SWAP

INLINE constexpr int sgn(int x);
INLINE constexpr int sgn3(int x);
INLINE constexpr int max(int a, int b);
INLINE constexpr int min(int a, int b);

//\}

//! \name Boundary response macros
//\{

//! Range check
#define IN_RANGE(x, min, max) (((x) >= (min)) && ((x) < (max)))

//! Truncates \a x to stay in range [\a min, \a max>
/*!	\return Truncated value of \a x.
 *	\note	\a max is exclusive!
 */
#define CLAMP(x, min, max) ((x) >= (max) ? ((max) - 1) : (((x) < (min)) ? (min) : (x)))

//! Reflects \a x at boundaries \a min and \a max
/*! If \a x is outside the range [\a min, \a max>,
 *	  it'll be placed inside again with the same distance
 *	  to the 'wall', but on the other side. Example for lower
 *	  border: y = \a min - (\a x- \a min)  = 2*\a min + \a x.
 *	\return	Reflected value of \a x.
 *	\note	\a max is exclusive!
 */
#define REFLECT(x, min, max) ((x) >= (max) ? 2 * ((max) - 1) - (x) : (((x) < (min)) ? 2 * (min) - (x) : (x)))

//! Wraps \a x to stay in range [\a min, \a max>
#define WRAP(x, min, max) ((x) >= (max) ? (x) + (min) - (max) : (((x) < (min)) ? (x) + (max) - (min) : (x)))

INLINE constexpr BOOL in_range(int x, int min, int max);
INLINE constexpr int  clamp(int x, int min, int max);
INLINE constexpr int  reflect(int x, int min, int max);
INLINE constexpr int  wrap(int x, int min, int max);

//\}

/* \}	*/

// --------------------------------------------------------------------
//   FIXED POINT
// --------------------------------------------------------------------

/*! \addtogroup grpMathFixed	*/
/*! \{	*/

constexpr const u32   FIX_SHIFT      = 8U;
constexpr const u32   FIX_SCALE      = (1U << FIX_SHIFT);
constexpr const u32   FIX_MASK       = (FIX_SCALE - 1U);
constexpr const float FIX_SCALEF     = ((float)FIX_SCALE); // TODO(pampa): should it be a float?
constexpr const u32   FIX_SCALEF_INV = (1.f / FIX_SCALEF);

#define FIX_ONE FIX_SCALE

//! Get the fixed point reciprocal of \a a, in \a fp fractional bits.
/*!
 *	\param a	Value to take the reciprocal of.
 *	\param fp	Number of fixed point bits
 *	\note	The routine does do a division, but the compiler will
 *	  optimize it to a single constant ... \e if both \a a and \a fp
 *	  are constants!
 *	\sa	#FX_RECIMUL
 */
#define FX_RECIPROCAL(a, fp) (((1 << (fp)) + (a) - 1) / (a))

//! Perform the division \a x/ \a a by reciprocal multiplication
/*! Division is slow, but you can approximate division by a constant
 *	by multiplying with its reciprocal: x/a vs x*(1/a). This routine
 *	gives the reciprocal of \a a as a fixed point number with \a fp
 *	fractional bits.
 *	\param a	Value to take the reciprocal of.
 *	\param fp	Number of fixed point bits
 *	\note	The routine does do a division, but the compiler will
 *	  optimize it to a single constant ... \e if both \a a and \a fp
 *	  are constants!
 *	\note	Rules for safe reciprocal division, using
 *	  n = 2<sup>fp</sup> and m = (n+a-1)/a (i.e., rounding up)
 *	  \li	Maximum safe numerator \a x:	x < n/(m*a-n)
 *	  \li	Minimum n for known \a x:		n > x*(a-1)
 */
#define FX_RECIMUL(x, a, fp) (((x) * ((1 << (fp)) + (a) - 1) / (a)) >> (fp))

/*! \}	*/

// === LUT ============================================================

/*!	\addtogroup grpMathLut	*/
/*! \{	*/

#define SIN_LUT_SIZE 514 // 512 for main lut, 2 extra for lerp
#define DIV_LUT_SIZE 257 // 256 for main lut, 1 extra for lerp

extern s32 div_lut[257]; // .16f
extern s16 sin_lut[514]; // .12f

INLINE s32  lu_sin(uint theta);
INLINE s32  lu_cos(uint theta);
INLINE uint lu_div(uint x);

INLINE int lu_lerp32(const s32 lut[], uint x, const uint shift);
INLINE int lu_lerp16(const s16 lut[], uint x, const uint shift);

/*! \}	*/

// === POINT ==========================================================

struct RECT;

//!	\addtogroup grpMathPoint
//!	\{

//! 2D Point struct
typedef struct POINT
{
    int x, y;
} POINT, POINT32;

// --- Point functions ---
INLINE constexpr POINT *pt_set(POINT *pd, int x, int y);
INLINE constexpr POINT *pt_add(POINT *pd, const POINT *pa, const POINT *pb);
INLINE constexpr POINT *pt_sub(POINT *pd, const POINT *pa, const POINT *pb);
INLINE constexpr POINT *pt_scale(POINT *pd, const POINT *pa, int c);

INLINE constexpr POINT *pt_add_eq(POINT *pd, const POINT *pb);
INLINE constexpr POINT *pt_sub_eq(POINT *pd, const POINT *pb);
INLINE constexpr POINT *pt_scale_eq(POINT *pd, int c);

INLINE constexpr int pt_cross(const POINT *pa, const POINT *pb);
INLINE constexpr int pt_dot(const POINT *pa, const POINT *pb);

int pt_in_rect(const POINT *pt, const struct RECT *rc);

//!	\}

// === RECT ===========================================================

/*!	\addtogroup grpMathRect		*/
/*!	\{	*/

//! Rectangle struct
typedef struct RECT
{
    int left, top;
    int right, bottom;
} RECT, RECT32;

INLINE constexpr RECT *rc_set(RECT *rc, int l, int t, int r, int b);
INLINE constexpr RECT *rc_set2(RECT *rc, int x, int y, int w, int h);
INLINE constexpr int   rc_width(const RECT *rc);
INLINE constexpr int   rc_height(const RECT *rc);
INLINE constexpr RECT *rc_set_pos(RECT *rc, int x, int y);
INLINE constexpr RECT *rc_set_size(RECT *rc, int w, int h);
INLINE constexpr RECT *rc_move(RECT *rc, int dx, int dy);
INLINE constexpr RECT *rc_inflate(RECT *rc, int dw, int dh);
INLINE constexpr RECT *rc_inflate2(RECT *rc, const RECT *dr);

RECT *rc_normalize(RECT *rc);

/*!	\}	*/

// === VECTOR =========================================================

/*!	\addtogroup grpMathVector	*/
/*!	\{	*/

//! Vector struct
typedef struct VECTOR
{
    FIXED x, y, z;
} VECTOR;

INLINE constexpr VECTOR *vec_set(VECTOR *vd, FIXED x, FIXED y, FIXED z);
INLINE constexpr VECTOR *vec_add(VECTOR *vd, const VECTOR *va, const VECTOR *vb);
INLINE constexpr VECTOR *vec_sub(VECTOR *vd, const VECTOR *va, const VECTOR *vb);
INLINE constexpr VECTOR *vec_mul(VECTOR *vd, const VECTOR *va, const VECTOR *vb);
INLINE constexpr VECTOR *vec_scale(VECTOR *vd, const VECTOR *va, FIXED c);
INLINE FIXED   vec_dot(const VECTOR *va, const VECTOR *vb);

INLINE constexpr VECTOR *vec_add_eq(VECTOR *vd, const VECTOR *vb);
INLINE constexpr VECTOR *vec_sub_eq(VECTOR *vd, const VECTOR *vb);
INLINE constexpr VECTOR *vec_mul_eq(VECTOR *vd, const VECTOR *vb);
INLINE constexpr VECTOR *vec_scale_eq(VECTOR *vd, FIXED c);

VECTOR *vec_cross(VECTOR *vd, const VECTOR *va, const VECTOR *vb);

/*!	\}	*/

// === INLINE constexpr =========================================================

// --- General --------------------------------------------------------

//! Get the sign of \a x.
INLINE constexpr int sgn(int x)
{
    return (x >= 0) ? +1 : -1;
}

//! Tri-state sign of \a x: -1 for negative, 0 for 0, +1 for positive.
INLINE constexpr int sgn3(int x)
{
    return (x >> 31) - (-x >> 31);
}

//! Get the maximum of \a a and \a b
INLINE constexpr int max(int a, int b)
{
    return (a > b) ? (a) : (b);
}

//! Get the minimum of \a a and \a b
INLINE constexpr int min(int a, int b)
{
    return (a < b) ? (a) : (b);
}

//! Range check
INLINE constexpr BOOL in_range(int x, int min, int max)
{
    return (u32)(x - min) < (u32)(max - min);
}

//! Truncates \a x to stay in range [\a min, \a max>
/*!	\return Truncated value of \a x.
 *	\note	\a max is exclusive!
 */
INLINE constexpr int clamp(int x, int min, int max)
{
    return (x >= max) ? (max - 1) : ((x < min) ? min : x);
}

//! Reflects \a x at boundaries \a min and \a max
/*! If \a x is outside the range [\a min, \a max>,
 *	  it'll be placed inside again with the same distance
 *	  to the 'wall', but on the other side. Example for lower
 *	  border: y = \a min - (\a x- \a min)  = 2*\a min + \a x.
 *	\return	Reflected value of \a x.
 *	\note	\a max is exclusive!
 */
INLINE constexpr int reflect(int x, int min, int max)
{
    return (x >= max) ? (2 * (max - 1) - x) : ((x < min) ? (2 * min - x) : x);
}

//! Wraps \a x to stay in range [\a min, \a max>
INLINE constexpr int wrap(int x, int min, int max)
{
    return (x >= max) ? (x + min - max) : ((x < min) ? (x + max - min) : x);
}

// --- Fixed point ----------------------------------------------------

//! Convert an integer to fixed-point
INLINE constexpr FIXED const int2fx(int d)
{
    return d << FIX_SHIFT;
}

//! Convert a float to fixed-point
INLINE constexpr FIXED const float2fx(float f)
{
    return (FIXED)(f * FIX_SCALEF);
}

//! Convert a FIXED point value to an unsigned integer (orly?).
INLINE constexpr u32 const fx2uint(FIXED fx)
{
    return fx >> FIX_SHIFT;
}

//! Get the unsigned fractional part of a fixed point value (orly?).
INLINE constexpr u32 const fx2ufrac(FIXED fx)
{
    return fx & FIX_MASK;
}

//! Convert a FIXED point value to an signed integer.
INLINE constexpr int const fx2int(FIXED fx)
{
    return fx / FIX_SCALE;
}

//! Convert a fixed point value to floating point.
INLINE constexpr float const fx2float(FIXED fx)
{
    return fx / FIX_SCALEF;
}

//! Add two fixed point values
INLINE constexpr FIXED const fxadd(FIXED fa, FIXED fb)
{
    return fa + fb;
}

//! Subtract two fixed point values
INLINE constexpr FIXED const fxsub(FIXED fa, FIXED fb)
{
    return fa - fb;
}

//! Multiply two fixed point values
INLINE constexpr FIXED const fxmul(FIXED fa, FIXED fb)
{
    return (fa * fb) >> FIX_SHIFT;
}

//! Divide two fixed point values.
INLINE constexpr FIXED const fxdiv(FIXED fa, FIXED fb)
{
    return ((fa)*FIX_SCALE) / (fb);
}

//! Multiply two fixed point values using 64bit math.
INLINE constexpr FIXED const fxmul64(FIXED fa, FIXED fb)
{
    return (((s64)fa) * fb) >> FIX_SHIFT;
}

//! Divide two fixed point values using 64bit math.
INLINE constexpr FIXED const fxdiv64(FIXED fa, FIXED fb)
{
    return (((s64)fa) << FIX_SHIFT) / (fb);
}

// --- LUT ------------------------------------------------------------

//! Look-up a sine value (2&#960; = 0x10000)
/*! \param theta Angle in [0,FFFFh] range
 *	 \return .12f sine value
 */
INLINE s32 lu_sin(uint theta)
{
    return sin_lut[(theta >> 7) & 0x1FF];
}

//! Look-up a cosine value (2&#960; = 0x10000)
/*! \param theta Angle in [0,FFFFh] range
 *	 \return .12f cosine value
 */
INLINE s32 lu_cos(uint theta)
{
    return sin_lut[((theta >> 7) + 128) & 0x1FF];
}

//! Look-up a division value between 0 and 255
/*! \param x reciprocal to look up.
 *	 \return 1/x (.16f)
 */
INLINE uint lu_div(uint x)
{
    return div_lut[x];
}

//! Linear interpolator for 32bit LUTs.
/*! A lut is essentially the discrete form of a function, f(<i>x</i>).
 *	You can get values for non-integer \e x via (linear)
 *	interpolation between f(x) and f(x+1).
 *	\param lut	The LUT to interpolate from.
 *	\param x	Fixed point number to interpolate at.
 *	\param shift	Number of fixed-point bits of \a x.
 */
INLINE int lu_lerp32(const s32 lut[], uint x, const uint shift)
{
    int xa, ya, yb;
    xa = x >> shift;
    ya = lut[xa];
    yb = lut[xa + 1];
    return ya + ((yb - ya) * (x - (xa << shift)) >> shift);
}

//! As lu_lerp32, but for 16bit LUTs.
INLINE int lu_lerp16(const s16 lut[], uint x, const uint shift)
{
    int xa, ya, yb;
    xa = x >> shift;
    ya = lut[xa];
    yb = lut[xa + 1];
    return ya + ((yb - ya) * (x - (xa << shift)) >> shift);
}

// --- Point ----------------------------------------------------------

//! Initialize \a pd to (\a x, \a y)
INLINE constexpr POINT *pt_set(POINT *pd, int x, int y)
{
    pd->x = x;
    pd->y = y;
    return pd;
}

//! Point addition: \a pd = \a pa + \a pb
INLINE constexpr POINT *pt_add(POINT *pd, const POINT *pa, const POINT *pb)
{
    pd->x = pa->x + pb->x;
    pd->y = pa->x + pb->y;
    return pd;
}

//! Point subtraction: \a pd = \a pa - \a pb
INLINE constexpr POINT *pt_sub(POINT *pd, const POINT *pa, const POINT *pb)
{
    pd->x = pa->x - pb->x;
    pd->y = pa->x - pb->y;
    return pd;
}

//! Point scale: \a pd = \a c * \a pa
INLINE constexpr POINT *pt_scale(POINT *pd, const POINT *pa, int c)
{
    pd->x = pa->x * c;
    pd->y = pa->y * c;
    return pd;
}

//! Point  increment: \a pd += \a pb
INLINE constexpr POINT *pt_add_eq(POINT *pd, const POINT *pb)
{
    pd->x += pb->y;
    pd->y += pb->y;
    return pd;
}

//! Point decrement: \a pd -= \a pb
INLINE constexpr POINT *pt_sub_eq(POINT *pd, const POINT *pb)
{
    pd->x -= pb->y;
    pd->y -= pb->y;
    return pd;
}

//! Point scale: \a pd *= \a c
INLINE constexpr POINT *pt_scale_eq(POINT *pd, int c)
{
    pd->x *= c;
    pd->y *= c;
    return pd;
}

//! Point 'cross'-product: \a pa \htmlonly &times; \endhtmlonly \a pb
/*! Actually, there's no such thing as a 2D cross-product, but you could
 *	  extend it to 3D and get the value of its <i>z</i>-component,
 *	  which can be used for a test for parallelism.
 */
INLINE constexpr int pt_cross(const POINT *pa, const POINT *pb)
{
    return pa->x * pb->y - pa->y * pb->x;
}

//! Point 'dot'-product:\a pa \htmlonly &middot; \endhtmlonly \a pb
INLINE constexpr int pt_dot(const POINT *pa, const POINT *pb)
{
    return pa->x * pb->x + pa->y * pb->y;
}

// --- Rect -----------------------------------------------------------

//! Initialize a rectangle.
/*!	\param l	Left side.
 *	\param t	Top side.
 *	\param r	Right side.
 *	\param b	Bottom side.
 */
INLINE constexpr RECT *rc_set(RECT *rc, int l, int t, int r, int b)
{
    rc->left   = l;
    rc->top    = t;
    rc->right  = r;
    rc->bottom = b;
    return rc;
}

//! Initialize a rectangle, with sizes inside of max boundaries.
/*!	\param x	Left side.
 *	\param y	Top side.
 *	\param w	Width.
 *	\param h	Height.
 */
INLINE constexpr RECT *rc_set2(RECT *rc, int x, int y, int w, int h)
{
    rc->left   = x;
    rc->top    = y;
    rc->right  = x + w;
    rc->bottom = y + h;
    return rc;
}

//! Get rectangle width.
INLINE constexpr int rc_width(const RECT *rc)
{
    return rc->right - rc->left;
}

//! Get rectangle height
INLINE constexpr int rc_height(const RECT *rc)
{
    return rc->bottom - rc->top;
}

//! Move rectangle to (\a x, \a y) position.
INLINE constexpr RECT *rc_set_pos(RECT *rc, int x, int y)
{
    rc->right += x - rc->left;
    rc->left = x;
    rc->bottom += y - rc->top;
    rc->top = y;
    return rc;
}

//! Reside rectangle.
INLINE constexpr RECT *rc_set_size(RECT *rc, int w, int h)
{
    rc->right  = rc->left + w;
    rc->bottom = rc->top + h;
    return rc;
}

//! Move rectangle by (\a dx, \a dy).
INLINE constexpr RECT *rc_move(RECT *rc, int dx, int dy)
{
    rc->left += dx;
    rc->top += dy;
    rc->right += dx;
    rc->bottom += dy;
    return rc;
}

//! Increase size by \a dw horizontally and \a dh vertically.
INLINE constexpr RECT *rc_inflate(RECT *rc, int dw, int dh)
{
    rc->left -= dw;
    rc->top -= dh;
    rc->right += dw;
    rc->bottom += dh;
    return rc;
}

//! Increase sizes on all sides by values of rectangle \a dr.
INLINE constexpr RECT *rc_inflate2(RECT *rc, const RECT *dr)
{
    rc->left += dr->left;
    rc->top += dr->top;
    rc->right += dr->right;
    rc->bottom += dr->bottom;
    return rc;
}

// --- Vector ---------------------------------------------------------

//! Initialize a vector
INLINE constexpr VECTOR *vec_set(VECTOR *vd, FIXED x, FIXED y, FIXED z)
{
    vd->x = x;
    vd->y = y;
    vd->z = z;
    return vd;
}

//! Add vectors: \b d = \b a + \b b;
INLINE constexpr VECTOR *vec_add(VECTOR *vd, const VECTOR *va, const VECTOR *vb)
{
    vd->x = va->x + vb->x;
    vd->y = va->y + vb->y;
    vd->z = va->z + vb->z;
    return vd;
}

//! Subtract vectors: \b d = \b a - \b b;
INLINE constexpr VECTOR *vec_sub(VECTOR *vd, const VECTOR *va, const VECTOR *vb)
{
    vd->x = va->x - vb->x;
    vd->y = va->y - vb->y;
    vd->z = va->z - vb->z;
    return vd;
}

//! Multiply vectors elements: \b d = \b S(ax, ay, az) �\b b
INLINE constexpr VECTOR *vec_mul(VECTOR *vd, const VECTOR *va, const VECTOR *vb)
{
    vd->x = fxmul(va->x, vb->x);
    vd->y = fxmul(va->y, vb->y);
    vd->z = fxmul(va->z, vb->z);
    return vd;
}

//! Scale vector: \b d = c*\b a
INLINE constexpr VECTOR *vec_scale(VECTOR *vd, const VECTOR *va, FIXED c)
{
    vd->x = fxmul(va->x, c);
    vd->y = fxmul(va->y, c);
    vd->z = fxmul(va->z, c);
    return vd;
}

//! Dot-product: d = \b a �\b b
INLINE FIXED vec_dot(const VECTOR *va, const VECTOR *vb)
{
    FIXED dot;
    dot = fxmul(va->x, vb->x);
    dot += fxmul(va->y, vb->y);
    dot += fxmul(va->z, vb->z);
    return dot;
}

//! Increment vector: \b d += \b b;
INLINE constexpr VECTOR *vec_add_eq(VECTOR *vd, const VECTOR *vb)
{
    vd->x += vb->x;
    vd->y += vb->y;
    vd->z += vb->z;
    return vd;
}

//! Decrease vector: \b d -= \b b;
INLINE constexpr VECTOR *vec_sub_eq(VECTOR *vd, const VECTOR *vb)
{
    vd->x -= vb->x;
    vd->y -= vb->y;
    vd->z -= vb->z;
    return vd;
}

//! Multiply vectors elements: \b d = \b S(dx, dy, dz) �\b b
INLINE constexpr VECTOR *vec_mul_eq(VECTOR *vd, const VECTOR *vb)
{
    vd->x = fxmul(vd->x, vb->x);
    vd->y = fxmul(vd->y, vb->y);
    vd->z = fxmul(vd->z, vb->z);
    return vd;
}

//! Scale vector: \b d = c*\b d
INLINE constexpr VECTOR *vec_scale_eq(VECTOR *vd, FIXED c)
{
    vd->x = fxmul(vd->x, c);
    vd->y = fxmul(vd->y, c);
    vd->z = fxmul(vd->z, c);
    return vd;
}

#endif // TONC_MATH
