/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-97 by Ilfak Guilfanov.
 *      ALL RIGHTS RESERVED.
 *                              E-mail: ig@estar.msk.su
 *                              FIDO:   2:5020/209
 *
 */

#ifndef _ERR_H
#define _ERR_H
#pragma pack(push, 1)

#include <errno.h>

//
//      This file contains error handling functions.
//      They are not used in IDA yet.
//

/*--------------------------------------------------*/
/* The 2 following functions use buffer 3 of the help system */
idaman void  ida_export vqperror( char *str, va_list va );
idaman char *ida_export qstrerror( int _qerrno );

#ifdef __cplusplus
inline void qperror( char *str, ... )
{
  va_list va;
  va_start(va, str);
  vqperror(str, va);
  va_end(va);
}
#endif

/*==================================================*/
/* error handlers */
/*--------------------------------------------------*/
/* Error handler function returns non-zero (true) if error is fatal */
typedef int (*errhndfun_t)( error_t myerrno, void *errdesc, void *data );
typedef struct
{
   errhndfun_t fun;
   void *data;
} errhnd_t;
/* Example of handler call:
   if( ErrHnd.fun( e..., ..., ErrHnd.data ))
   {
      ...fatal
      qerrno = e...;
      return -1;
   }
*/
/*--------------------------------------------------*/
/* This function returns previous error handler
   Example of module's set handler function:
static errhnd_t ErrHnd;
errhnd_t *<mod>SetErrHnd( errhndfun_t fun, void *data, errhnd_t *oldhnd )
{
   if( oldhnd )
      *oldhnd = ErrHnd;
   ErrHnd.fun = fun;
   ErrHnd.data = data;
   return oldhnd;
}
*/
/*--------------------------------------------------*/
/* standard error handler */
/* error always is fatal */
int StdErrHnd0( error_t _qerrno, void *errdesc, void *data );
/* always ignore error */
int StdErrHnd1( error_t _qerrno, void *errdesc, void *data );

/*
        Error parameters
*/

extern char errprm1[MAXSTR];
extern char errprm2[MAXSTR];
extern char errprm3[MAXSTR];
extern char errprm4[MAXSTR];

extern long errval1;
extern long errval2;
extern long errval3;
extern long errval4;

#define QPRM_TYPE(t,n,x)        ((errval ## n) = (t)(x))
#define QPRM_CHAR(n,x)          QPRM_TYPE(char,n,x)
#define QPRM_SHORT(n,x)         QPRM_TYPE(short,n,x)
#define QPRM_INT(n,x)           QPRM_TYPE(int,n,x)
#define QPRM_LONG(n,x)          QPRM_TYPE(long,n,x)
#define QPRM_UCHAR(n,x)         QPRM_TYPE(uchar,n,x)
#define QPRM_USHORT(n,x)        QPRM_TYPE(ushort,n,x)
#define QPRM_UINT(n,x)          QPRM_TYPE(uint,n,x)
#define QPRM_ULONG(n,x)         QPRM_TYPE(ulong,n,x)
#define QPRM(n,x)               (errval ## n = long(errprm ## n),qstrncpy(errprm ## n,x, sizeof(errprm ## n)))

#pragma pack(pop)
#endif

