#ifndef __Itermios1
#define __Itermios1

#ifdef	__cplusplus
extern "C" {
#endif

#include <sys/termios.h>

extern speed_t	cfgetospeed(struct termios *);
extern int	cfsetospeed(struct termios *,speed_t);
extern speed_t	cfgetispeed(struct termios *);
extern int	cfsetispeed(struct termios *,speed_t);

extern int	tcgetattr(int, struct termios *);
extern int	tcsetattr(int, int, struct termios *);
extern int	tcsendbreak(int, int);
extern int	tcdrain(int);
extern int	tcflush(int, int);
extern int	tcflow(int, int);

#ifdef	__cplusplus
}
#endif
#endif
