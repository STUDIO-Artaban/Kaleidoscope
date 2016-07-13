#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifdef __ANDROID__
////// DEBUG | RELEASE

// Debug
//#ifndef DEBUG
//#define DEBUG
//#endif
//#undef NDEBUG

// Relase
#ifndef NDEBUG
#define NDEBUG
#endif
#undef DEBUG

#endif

#define PAID_VERSION // Not defined for the free version with advertising

#define DATA_KEY_SHIFT_RED          "ShiftRed"
#define DATA_KEY_SHIFT_GREEN        "ShiftGreen"
#define DATA_KEY_SHIFT_BLUE         "ShiftBlue"
#ifndef PAID_VERSION
#define DATA_KEY_AD_DELAY           "AdDelay"
#endif

#define CAM_WIDTH                   640
#define CAM_HEIGHT                  480

#ifndef PAID_VERSION
#ifdef __ANDROID__
#define GOOGLE_PLAY_PACKAGE         "com.android.vending"
#define GOOGLE_PLAY_ACTIVITY        "com.google.android.finsky.activities.LaunchUrlHandlerActivity"
#endif
#endif

#define DISPLAY_DELAY               50
#define PANEL_X2                    108

// Texture IDs
#ifndef PAID_VERSION
#define TEXTURE_ID_CLOSE            7
#define TEXTURE_ID_AVAILABLE        4
#endif

// Log levels (< 5 to log)
#define LOG_LEVEL_KALEIDOSCOPE      5
#ifndef PAID_VERSION
#define LOG_LEVEL_CRYSTALBALL       5
#define LOG_LEVEL_SPHERE            5
#define LOG_LEVEL_BALL              5
#define LOG_LEVEL_RESPONSE          5
#define LOG_LEVEL_WITCHSPET         5
#define LOG_LEVEL_WITCH             5
#endif
#define LOG_LEVEL_KALEIDOLEVEL      5
#define LOG_LEVEL_KALEIDO2D         5
#define LOG_LEVEL_PANEL             5
#define LOG_LEVEL_COMMAND           5

typedef struct {

    short left;
    short right;
    short top;
    short bottom;

} TouchArea;

#endif // GLOBAL_H_
