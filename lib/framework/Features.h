#ifndef Features_h
#define Features_h

#define IN_USE 1
#define NOT_IN_USE 0
#define USING(feature) feature

// security feature on by default
#ifndef FT_SECURITY
#define FT_SECURITY IN_USE
#endif

// project feature off by default
#ifndef FT_PROJECT
#define FT_PROJECT NOT_IN_USE
#endif

#endif