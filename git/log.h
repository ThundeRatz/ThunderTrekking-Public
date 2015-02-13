#ifndef __LOG_H__
#define __LOG_H__

#ifdef DEBUG
#define LOG		printf
#else
#define LOG(a, ...) do {} while (0)
#endif

#endif
