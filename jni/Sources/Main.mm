#include "Main.h"

////// Languages
typedef enum {

    LANG_EN = 0,
    LANG_FR,
    LANG_DE,
    LANG_ES,
    LANG_IT,
    LANG_PT

} Language;
static const Language g_MainLang = LANG_EN;

////// Textures
#define NO_TEXTURE_LOADED       0xFF

BOOL engGetFontGrayscale() { return YES; }

#define TEXTURE_ID_CRYSTAL          2 // TEXTURE_ID_FONT + 1
#define TEXTURE_ID_TITLE            3
#define TEXTURE_ID_AVAILABLE        4
#define TEXTURE_ID_STORES           5
#define TEXTURE_ID_ENERGY           6

#define TEXTURE_ID_CLOSE            7

#define TEXTURE_ID_PUMPKIN          8
#define TEXTURE_ID_BACK             9
#define TEXTURE_ID_MOON             10
#define TEXTURE_ID_SCENE            11
#define TEXTURE_ID_TITLE_FWSP       12
#define TEXTURE_ID_STORES_FWSP      13
#define TEXTURE_ID_FACE             14

#define TEXTURE_ID_WITCH0           15
#define TEXTURE_ID_WITCH1           16
#define TEXTURE_ID_WITCH2           17
#define TEXTURE_ID_WITCH3           18
#define TEXTURE_ID_WITCH4           19
#define TEXTURE_ID_WITCH5           20
#define TEXTURE_ID_WITCH6           21
#define TEXTURE_ID_WITCH7           22

#define TEXTURE_ID_COMMAND          23
#define TEXTURE_ID_FLASH            24
#define TEXTURE_ID_PALETTE          25
#define TEXTURE_ID_TITLE_KALEIDO    26
#define TEXTURE_ID_PANEL            27

unsigned char engLoadTexture(EngResources* resources, unsigned char Id) {
    switch (Id) {

        case TEXTURE_ID_CRYSTAL: {

            unsigned char* data = [resources getBufferPNG:@"crystal" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_CRYSTAL, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_TITLE: {

            unsigned char* data = [resources getBufferPNG:@"titleCB" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_TITLE, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_AVAILABLE: {

            unsigned char* data = [resources getBufferPNG:@"available" inGrayScale:YES];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_AVAILABLE, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data, true);
        }
        case TEXTURE_ID_STORES: {

            unsigned char* data = [resources getBufferPNG:@"stores" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_STORES, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_ENERGY: {

            unsigned char* data = [resources getBufferPNG:@"energy" inGrayScale:YES];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_ENERGY, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data, true);
        }

        case TEXTURE_ID_CLOSE: {

            unsigned char* data = [resources getBufferPNG:@"close" inGrayScale:YES];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_CLOSE, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data, true);
        }

        case TEXTURE_ID_PUMPKIN: {

            unsigned char* data = [resources getBufferPNG:@"pumpkin" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_PUMPKIN, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_BACK: {
            
            unsigned char* data = [resources getBufferPNG:@"back" inGrayScale:YES];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_BACK, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data, true);
        }
        case TEXTURE_ID_MOON: {

            unsigned char* data = [resources getBufferPNG:@"moon" inGrayScale:YES];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_MOON, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data, true);
        }
        case TEXTURE_ID_SCENE: {
            
            unsigned char* data = [resources getBufferPNG:@"scene" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_SCENE, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_TITLE_FWSP: {
            
            unsigned char* data = [resources getBufferPNG:@"title" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_TITLE_FWSP, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_STORES_FWSP: {
            
            unsigned char* data = [resources getBufferPNG:@"storesFWSP" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_STORES_FWSP, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_FACE: {
            
            unsigned char* data = [resources getBufferPNG:@"face" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_FACE, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_WITCH0:
        case TEXTURE_ID_WITCH1:
        case TEXTURE_ID_WITCH2:
        case TEXTURE_ID_WITCH3:
        case TEXTURE_ID_WITCH4:
        case TEXTURE_ID_WITCH5:
        case TEXTURE_ID_WITCH6:
        case TEXTURE_ID_WITCH7: {
            
            NSString* animImg = [[NSString alloc] initWithFormat:@"%@%d", @"witch-", (Id - TEXTURE_ID_WITCH0)];
            unsigned char* data = [resources getBufferPNG:animImg inGrayScale:NO];
            [animImg release];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(Id, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }

        case TEXTURE_ID_COMMAND: {
            
            unsigned char* data = [resources getBufferPNG:@"cmd" inGrayScale:YES];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_COMMAND, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data, true);
        }
        case TEXTURE_ID_FLASH: {
            
            unsigned char* data = [resources getBufferPNG:@"flash" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_FLASH, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_PALETTE: {

            unsigned char* data = [resources getBufferPNG:@"palette" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_PALETTE, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_TITLE_KALEIDO: {
            
            unsigned char* data = [resources getBufferPNG:@"titleKAL" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_TITLE_KALEIDO, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_PANEL: {
            
            unsigned char* data = [resources getBufferPNG:@"panel" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_PANEL, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }

        default: {
            
            NSLog(@"ERROR: Failed to load PNG ID %d", Id);
            break;
        }
    }
    return NO_TEXTURE_LOADED;
}

////// Sounds
#define SOUND_ID_INTRO      1 // SOUND_ID_LOGO + 1
#define SOUND_ID_FLAP       2
#define SOUND_ID_BEEP       3
#define SOUND_ID_FLASH      4

void engLoadSound(EngResources* resources, unsigned char Id) {
    switch (Id) {

        case SOUND_ID_INTRO: {
            
            unsigned char* data = [resources getBufferOGG:@"introCB"];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get OGG buffer (line:%d)", __LINE__);
                break;
            }
            platformLoadOgg(SOUND_ID_INTRO, resources.oggLength, data, true);
            break;
        }
        case SOUND_ID_FLAP: {

            unsigned char* data = [resources getBufferOGG:@"flap"];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get OGG buffer (line:%d)", __LINE__);
                break;
            }
            platformLoadOgg(SOUND_ID_FLAP, resources.oggLength, data, false);
            break;
        }
        case SOUND_ID_BEEP: {

            unsigned char* data = [resources getBufferOGG:@"beep"];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get OGG buffer (line:%d)", __LINE__);
                break;
            }
            platformLoadOgg(SOUND_ID_BEEP, resources.oggLength, data, false);
            break;
        }
        case SOUND_ID_FLASH: {
            
            unsigned char* data = [resources getBufferOGG:@"flash"];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get OGG buffer (line:%d)", __LINE__);
                break;
            }
            platformLoadOgg(SOUND_ID_FLASH, resources.oggLength, data, false);
            break;
        }

        default: {
            
            NSLog(@"ERROR: Failed to load OGG ID %d", Id);
            break;
        }
    }
}

////// Advertising
#ifdef LIBENG_ENABLE_ADVERTISING

static NSString* ADV_UNIT_ID = @"ca-app-pub-1474300545363558/5181985025";
#ifdef DEBUG
static const NSString* IPAD_DEVICE_UID = @"655799b1c803de3417cbb36833b6c40c";
static const NSString* IPHONE_YACIN_UID = @"10053bb6983c6568b88812fbcfd7ab89";
#endif

BOOL engGetAdType() { return TRUE; } // TRUE: Interstitial; FALSE: Banner
void engLoadAd(EngAdvertising* ad, GADRequest* request) {

    [ad createNewInterstitial];
    [ad getInterstitial].adUnitID = ADV_UNIT_ID;
#ifdef DEBUG
    request.testDevices = [NSArray arrayWithObjects: GAD_SIMULATOR_ID, IPAD_DEVICE_UID, IPHONE_YACIN_UID, nil];
#endif
    [[ad getInterstitial] loadRequest:request];
}
void engDisplayAd(EngAdvertising* ad, unsigned char Id) {

    ad.status = static_cast<unsigned char>(Advertising::STATUS_DISPLAYING);
    [[ad getInterstitial] presentFromRootViewController:[ad getViewController]];
}
void engHideAd(EngAdvertising* ad, unsigned char Id) { }
#endif

////// Social
#ifdef LIBENG_ENABLE_SOCIAL
BOOL engReqInfoField(SocialField field, unsigned char socialID) { return NO; }
#endif
