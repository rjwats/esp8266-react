#ifndef Features_h
#define Features_h

#define FT_ENABLED(feature) feature

// security feature on by default
#ifndef FT_SECURITY
#define FT_SECURITY 1
#endif

// project feature on by default
#ifndef FT_MQTT
#define FT_MQTT 1
#endif

#endif
