#include "CrystalBall.h"

#ifndef PAID_VERSION
#include <libeng/Game/2D/Game2D.h>
#include <libeng/Tools/Tools.h>

#include <boost/algorithm/string.hpp>
#include <map>

#ifndef __ANDROID__
#import <Foundation/Foundation.h>
#endif

#define MAX_LOAD_STEP           10
#define VOLUME_VELOCITY         0.005f // Note that the default sound volume is 0.5 and not 1 (see DEFAULT_VOLUME)

#define DELTA_REF_CRYSTAL       15880L
#define DELTA_REF_VOLUME        18150L
#define DELTA_REF_BACK          20920L

// Delay(s)
#ifdef __ANDROID__ // Android
#ifdef DEBUG
#define DDELAY_DISPLAY          150
#endif
#define DELAY_DISPLAY           1323064.f

#else // iOS
#ifdef DEBUG
#define DDELAY_DISPLAY          60
#endif
#define DELAY_DISPLAY           792842.f

#endif

// Texture IDs
#define TEXTURE_ID_CRYSTAL      2

// Sound IDs
#define SOUND_ID_INTRO          1

#ifdef __ANDROID__
static const char* g_CrystalBallLang[] = { "EN", "FR", "DE", "ES", "PT", "IT" };
#endif

//////
CrystalBall::CrystalBall(Game* game) : Level(game), mStep(0) {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
    std::memset(&mCloseArea, 0, sizeof(TouchArea));

    game->mAccelPrecision = 0.f;
    Inputs::InputUse = USE_INPUT_TOUCH | USE_INPUT_ACCEL;

    mCamera = Camera::getInstance();
    mPlayer = Player::getInstance();

#ifdef __ANDROID__
    std::map<const std::string, unsigned char> mapCountry;

    // ISO 3166-1 alpha-3 (Google Play)
    mapCountry["BEL"] = COUNTRY_IDX_FR;
    mapCountry["BEN"] = COUNTRY_IDX_FR;
    mapCountry["DZA"] = COUNTRY_IDX_FR;
    mapCountry["BFA"] = COUNTRY_IDX_FR;
    mapCountry["CMR"] = COUNTRY_IDX_FR;
    mapCountry["CAN"] = COUNTRY_IDX_FR;
    mapCountry["CIV"] = COUNTRY_IDX_FR;
    mapCountry["FRA"] = COUNTRY_IDX_FR;
    mapCountry["GAB"] = COUNTRY_IDX_FR;
    mapCountry["HTI"] = COUNTRY_IDX_FR;
    mapCountry["LUX"] = COUNTRY_IDX_FR;
    mapCountry["MLI"] = COUNTRY_IDX_FR;
    mapCountry["MAR"] = COUNTRY_IDX_FR;
    mapCountry["NER"] = COUNTRY_IDX_FR;
    mapCountry["RWA"] = COUNTRY_IDX_FR;
    mapCountry["SEN"] = COUNTRY_IDX_FR;
    mapCountry["TGO"] = COUNTRY_IDX_FR;
    mapCountry["TUN"] = COUNTRY_IDX_FR;

    mapCountry["DEU"] = COUNTRY_IDX_DE;
    mapCountry["AUT"] = COUNTRY_IDX_DE;
    mapCountry["CHE"] = COUNTRY_IDX_DE;

    mapCountry["ARG"] = COUNTRY_IDX_ES;
    mapCountry["BOL"] = COUNTRY_IDX_ES;
    mapCountry["CHL"] = COUNTRY_IDX_ES;
    mapCountry["COL"] = COUNTRY_IDX_ES;
    mapCountry["CRI"] = COUNTRY_IDX_ES;
    mapCountry["ESP"] = COUNTRY_IDX_ES;
    mapCountry["GTM"] = COUNTRY_IDX_ES;
    mapCountry["HND"] = COUNTRY_IDX_ES;
    mapCountry["MEX"] = COUNTRY_IDX_ES;
    mapCountry["NIC"] = COUNTRY_IDX_ES;
    mapCountry["PRY"] = COUNTRY_IDX_ES;
    mapCountry["PER"] = COUNTRY_IDX_ES;
    mapCountry["PAN"] = COUNTRY_IDX_ES;
    mapCountry["DOM"] = COUNTRY_IDX_ES;
    mapCountry["SLV"] = COUNTRY_IDX_ES;
    mapCountry["URY"] = COUNTRY_IDX_ES;
    mapCountry["VEN"] = COUNTRY_IDX_ES;

    mapCountry["ITA"] = COUNTRY_IDX_IT;

    mapCountry["AGO"] = COUNTRY_IDX_PT;
    mapCountry["BRA"] = COUNTRY_IDX_PT;
    mapCountry["CPV"] = COUNTRY_IDX_PT;
    mapCountry["GNB"] = COUNTRY_IDX_PT;
    mapCountry["MOZ"] = COUNTRY_IDX_PT;
    mapCountry["PRT"] = COUNTRY_IDX_PT;

#ifdef DEBUG
    LOGI(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - Country: %s"), __PRETTY_FUNCTION__, __LINE__,
            getCountry(LOG_LEVEL_CRYSTALBALL).c_str());
#endif
    std::string country(boost::to_upper_copy<std::string>(getCountry(LOG_LEVEL_CRYSTALBALL)));
    std::map<const std::string, unsigned char>::const_iterator iter = mapCountry.find(country);
    mAppCountry = (iter != mapCountry.end())? iter->second:COUNTRY_IDX_EN;

#else
    #define APP_ITUNES_FR @"fr/id870117380"
    #define APP_ITUNES_DE @"de/id870117249"
    #define APP_ITUNES_ES @"es/id870118936"
    #define APP_ITUNES_PT @"pt/id870119335"
    #define APP_ITUNES_IT @"it/id869891682"

    // ISO 3166-1 alpha-2 (AppStore)
    NSDictionary* countryMap = [[NSDictionary alloc] initWithObjectsAndKeys:APP_ITUNES_FR, @"DZ",
            APP_ITUNES_FR, @"BE", APP_ITUNES_FR, @"BJ", APP_ITUNES_FR, @"BF", APP_ITUNES_FR, @"TD",
            APP_ITUNES_FR, @"CG", APP_ITUNES_FR, @"FR", APP_ITUNES_FR, @"LU", APP_ITUNES_FR, @"MG",
            APP_ITUNES_FR, @"ML", APP_ITUNES_FR, @"NE", APP_ITUNES_FR, @"SN", APP_ITUNES_FR, @"SC",
            APP_ITUNES_FR, @"TN",
            APP_ITUNES_PT, @"AO", APP_ITUNES_PT, @"BM", APP_ITUNES_PT, @"BR", APP_ITUNES_PT, @"CV",
            APP_ITUNES_PT, @"GW", APP_ITUNES_PT, @"MZ", APP_ITUNES_PT, @"PT", APP_ITUNES_PT, @"ST",
            APP_ITUNES_ES, @"AR", APP_ITUNES_ES, @"BO", APP_ITUNES_ES, @"CL", APP_ITUNES_ES, @"CO",
            APP_ITUNES_ES, @"CR", APP_ITUNES_ES, @"DO", APP_ITUNES_ES, @"EC", APP_ITUNES_ES, @"SV",
            APP_ITUNES_ES, @"GT", APP_ITUNES_ES, @"HN", APP_ITUNES_ES, @"MX", APP_ITUNES_ES, @"NI",
            APP_ITUNES_ES, @"PA", APP_ITUNES_ES, @"PY", APP_ITUNES_ES, @"PE", APP_ITUNES_ES, @"ES",
            APP_ITUNES_ES, @"UY", APP_ITUNES_ES, @"VE",
            APP_ITUNES_IT, @"IT",
            APP_ITUNES_DE, @"AT", APP_ITUNES_DE, @"DE", APP_ITUNES_DE, @"CH",
            nil];
    @try {
        mAppCountry = [[NSString alloc] initWithString:[countryMap objectForKey:[getCountry() uppercaseString]]];
    }
    @catch (NSException* e) {
        #define APP_ITUNES_EN @"en/id869891415"

        mAppCountry = [[NSString alloc] initWithString:APP_ITUNES_EN];
    }
#endif
    mProgress = NULL;
    mClose = NULL;
    mBackground = NULL;
    mResponse = NULL;
    mBall = NULL;
    mCrystal = NULL;
}
CrystalBall::~CrystalBall() {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    assert(mTextures->getIndex(TEXTURE_ID_COLOR) != TEXTURE_IDX_INVALID);
    mTextures->delTexture(mTextures->getIndex(TEXTURE_ID_COLOR));

    if (mProgress) delete mProgress;
    if (mClose) delete mClose;
    if (mBackground) delete mBackground;
    if (mResponse) delete mResponse;
    if (mBall) delete mBall;
    if (mCrystal) delete mCrystal;

#ifndef __ANDROID__
    [mAppCountry release];
#endif
}

void CrystalBall::pause() {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    if (mProgress) mProgress->pause();
    if (mClose) mClose->pause();
    if (mBackground) mBackground->pause();
    if (mResponse) mResponse->stop(); // pause
    if (mBall) mBall->stop(); // pause
    if (mCrystal) mCrystal->pause();
}

bool CrystalBall::loaded(const Game* game) {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - l:%d"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
    switch (mLoadStep) {
        case 1: {

            assert(mTextures->getIndex(TEXTURE_ID_COLOR) == TEXTURE_IDX_INVALID);
            mTextures->genTexture(mTextures->addTexColor());

            if (!mProgress) {

                mProgress = new Progress(false);
                mProgress->initialize(game2DVia(game));
                Progress::Info progInfo;
                progInfo.maxPos = MAX_LOAD_STEP;

                progInfo.doneRed = 0xff;
                progInfo.doneGreen = 0xff;
                progInfo.doneBlue = 0xff;

                progInfo.todoRed = 0x80;
                progInfo.todoGreen = 0x80;
                progInfo.todoBlue = 0x80;

                mProgress->start(&progInfo);
                mProgress->position(0, 2, game->getScreen()->width, 0);
            }
            else
                mProgress->resume();
            break;
        }
        case 2: {

            if (!mCrystal) {

                mCrystal = new Element2D;
                mCrystal->initialize(game2DVia(game));
                mCrystal->start(TEXTURE_ID_CRYSTAL);
                mCrystal->setVertices(0, (game->getScreen()->height - game->getScreen()->width) >> 1, game->getScreen()->width,
                        game->getScreen()->height - ((game->getScreen()->height - game->getScreen()->width) >> 1));

                mCrystal->setTexCoords(FULL_TEXCOORD_BUFFER);
                mCrystal->rotate(135.f * PI_F / 180.f);

                mCrystal->setAlpha(0.f);
    #ifndef __ANDROID__
                mCrystal->setRed(1.6f);
                mCrystal->setGreen(1.6f);
                mCrystal->setBlue(1.6f);
    #endif
            }
            else
                mCrystal->resume(TEXTURE_ID_CRYSTAL);
            break;
        }
        case 3: {

            if (!mClose) {

                mClose = new Static2D;
                mClose->initialize(game2DVia(game));
                mClose->start(TEXTURE_ID_CLOSE);
                mClose->setTexCoords(FULL_TEXCOORD_BUFFER);
                mClose->setVertices(game->getScreen()->width - (game->getScreen()->width >> 3), game->getScreen()->height,
                        game->getScreen()->width, game->getScreen()->height - (game->getScreen()->width >> 3));

                mCloseArea.left = game->getScreen()->width - (game->getScreen()->width >> 3);
                mCloseArea.right = game->getScreen()->width;
                mCloseArea.top = 0;
                mCloseArea.bottom = game->getScreen()->width >> 3;

                mClose->setAlpha(0.f);
            }
            else
                mClose->resume(TEXTURE_ID_CLOSE);
            break;
        }
        case 4: {

            if (!mResponse) {

                mResponse = new Response;
                mResponse->initialize(game2DVia(game));
                mResponse->start();
            }
            break;
        }
        case 5: Player::loadSound(SOUND_ID_INTRO); break;

        case (RESPONSE_ID_TITLE + 6):
        case (RESPONSE_ID_AVAILABLE + 6):
        case (RESPONSE_ID_STORES + 6): {

            mResponse->load(mLoadStep - 6);
            break;
        }
        case 9: {

            if (!mBackground) {

                mBackground = new Static2D(false);
                mBackground->initialize(game2DVia(game));
                mCamera->start(CAM_WIDTH, CAM_HEIGHT);
                mBackground->start(mCamera->getCamTexIdx());

                short camRatio = (((game->getScreen()->height * CAM_HEIGHT) / CAM_WIDTH) - game->getScreen()->width) >> 1;
                mBackground->setVertices(-camRatio, game->getScreen()->height, game->getScreen()->width + camRatio, 0);

                float texCoords[8] = {0};
                texCoords[1] = CAM_HEIGHT / static_cast<float>((*mTextures)[mCamera->getCamTexIdx()]->height);
                texCoords[2] = CAM_WIDTH / static_cast<float>((*mTextures)[mCamera->getCamTexIdx()]->width);
                texCoords[3] = texCoords[1];
                texCoords[4] = texCoords[2];
                mBackground->setTexCoords(texCoords);
#ifndef __ANDROID__
                mBackground->setBGRA(true);
#endif
                mBackground->setAlpha(0.f);
            }
            else
                mBackground->resume(mCamera->getCamTexIdx());
            break;
        }
        case MAX_LOAD_STEP: {

            if (!mBall) {

                mBall = new Ball;
                mBall->initialize(game2DVia(game));
                mBall->start(game);
                mBall->scale(MAX_SPHERE_SCALE, MAX_SPHERE_SCALE);

                //mBall->setAlpha(0.f); // Blending disabled
                mBall->setRed(0.f);
                mBall->setGreen(0.f);
                mBall->setBlue(0.f);

                mBall->reset();
            }
            else
                mBall->resume();
            break;
        }
#ifdef DEBUG
        default: {

            LOGW(LOG_FORMAT(" - Unknown load step: %d"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
            assert(NULL);
            break;
        }
#endif
    }
    if (mLoadStep != MAX_LOAD_STEP) {

        mProgress->set(mLoadStep);
        return false;
    }
    delete mProgress;
    mProgress = NULL;
    return true;
}
void CrystalBall::renderLoad() const {

#ifdef DEBUG
    if (!mProgress) {
        LOGW(LOG_FORMAT(" - Progress bar not defined!"), __PRETTY_FUNCTION__, __LINE__);
        assert(NULL);
    }
#endif
    glDisable(GL_BLEND);
    mProgress->render(false);
    glEnable(GL_BLEND);
}

void CrystalBall::reduceVolume(unsigned char track, clock_t delta) {

    LOGV(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - t:%d; d:%u"), __PRETTY_FUNCTION__, __LINE__, track, delta);
    if (mPlayer->getStatus(track) == AL_PLAYING) {

        float volume = mPlayer->getVolume(track) - (VOLUME_VELOCITY * delta / DELTA_REF_VOLUME);
        if (volume > 0.f)
            mPlayer->setVolume(track, volume);
    }
#ifdef DEBUG
    else {
        LOGI(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - Track %d is not playing"), __PRETTY_FUNCTION__, __LINE__, track);
    }
#endif
}

bool CrystalBall::update(const Game* game) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_CRYSTALBALL, (*game->getLog() % 100), LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
#endif
#ifdef __ANDROID__
    // Restart player (needed for lock/unlock operation)
    mPlayer->resume();
#endif
    mCamera->updateTexture();

    if (!mStep) { // Display crystal ball

        if (mBackground->getAlpha() < 1.f) {

            mBackground->setAlpha(mBackground->getAlpha() + (ALPHA_VEL_A * game->getDelta() / DELTA_REF_BACK));
            mCrystal->setAlpha(mBackground->getAlpha());

            mBall->setRed(mBackground->getAlpha() * MAX_BALL_COLOR);
            mBall->setGreen(mBackground->getAlpha() * MAX_BALL_COLOR);
            mBall->setBlue(mBackground->getAlpha() * MAX_BALL_COLOR);

            if ((mPlayer->getStatus(0) != AL_PLAYING) && (!mPlayer->getResumeFlag(0)))
                mPlayer->play(0);
            return true;
        }
        ++mStep;
        mDelay.reset();
    }

    // Touch...
    unsigned char touchCount = game->mTouchCount;
    while (touchCount--) {

        if (game->mTouchData[touchCount].Type != TouchInput::TOUCH_UP)
            continue;

        // Check 'Close' touched
        if ((mStep == CLOSE_DISPLAYED) && (game->mTouchData[touchCount].X > mCloseArea.left) &&
                (game->mTouchData[touchCount].X < mCloseArea.right) && (game->mTouchData[touchCount].Y > mCloseArea.top) &&
                (game->mTouchData[touchCount].Y < mCloseArea.bottom)) {

            mPlayer->stop(0);
            mCamera->stop();
            return false;
        }

#ifdef __ANDROID__
        std::string appURL("market://details?id=com.studio.artaban.crystalball");
        appURL.append(g_CrystalBallLang[mAppCountry]);

        Launcher launchApp(GOOGLE_PLAY_PACKAGE, GOOGLE_PLAY_ACTIVITY, appURL);
        launchApp.go();
#else
        #define APP_ITUNES_URL @"itms-apps://itunes.apple.com/us/app/crystal-ball-%@"

        NSString* strAppURL = [[NSString alloc] initWithFormat:APP_ITUNES_URL, mAppCountry];
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strAppURL]];
        [strAppURL release];
#endif
        return true;
    }

    // Working...
    switch (mStep) {

        case WAIT_TITLE: {
#ifdef DEBUG
            if (mDelay.isElapsed(game->getDelta(), DDELAY_DISPLAY, DELAY_DISPLAY, __PRETTY_FUNCTION__, __LINE__)) {
#else
            if (mDelay.isElapsed(game->getDelta(), DELAY_DISPLAY)) {
#endif
                LOGI(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - Display title"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->display(RESPONSE_ID_TITLE);
                ++mStep;
            }
            break;
        }
        case DISPLAYING_TITLE: {

            if (mResponse->getAlpha() < MAX_ALPHA)
                break; // Not displayed yet

            LOGI(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - Title displayed"), __PRETTY_FUNCTION__, __LINE__);
            mDelay.reset();
            ++mStep;
            break;
        }
        case TITLE_DISPLAYED: {

#ifdef DEBUG
            if (mDelay.isElapsed(game->getDelta(), DDELAY_DISPLAY, DELAY_DISPLAY, __PRETTY_FUNCTION__, __LINE__)) {
#else
            if (mDelay.isElapsed(game->getDelta(), DELAY_DISPLAY)) {
#endif
                LOGI(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - Cancel title"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->cancel();
                ++mStep;
            }
            break;
        }
        case CANCELLING_TITLE: {

            if (!mResponse->toRender()) {

                LOGI(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - Display available"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->display(RESPONSE_ID_AVAILABLE);
                ++mStep;
            }
            break;
        }
        case DISPLAYING_AVAILABLE: {

            if (mResponse->getAlpha() < MAX_ALPHA)
                break; // Not displayed yet

            LOGI(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - Available displayed"), __PRETTY_FUNCTION__, __LINE__);
            mDelay.reset();
            ++mStep;
            break;
        }
        case AVAILABLE_DISPLAYED: {

#ifdef DEBUG
            if (mDelay.isElapsed(game->getDelta(), DDELAY_DISPLAY, DELAY_DISPLAY, __PRETTY_FUNCTION__, __LINE__)) {
#else
            if (mDelay.isElapsed(game->getDelta(), DELAY_DISPLAY)) {
#endif
                LOGI(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - Cancel available"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->cancel();
                ++mStep;
            }
            break;
        }
        case CANCELLING_AVAILABLE: {

            if (!mResponse->toRender()) {

                LOGI(LOG_LEVEL_CRYSTALBALL, 0, LOG_FORMAT(" - Display stores"), __PRETTY_FUNCTION__, __LINE__);
                mResponse->display(RESPONSE_ID_STORES);
                ++mStep;
            }
            break;
        }
        case DISPLAYING_STORES: {

            unsigned char light = mBall->lighten(game->getDelta());
            if (!light) {

                reduceVolume(0, game->getDelta());
                mDelay.reset();
                ++mStep;
            }
            else if (light > (INCREASE_BUFFER_SIZE - 1))
                reduceVolume(0, game->getDelta());
            else if (mCrystal->getAlpha() < 1.f)
                mCrystal->setAlpha(mCrystal->getAlpha() + (ALPHA_VEL_A * game->getDelta() / (DELTA_REF_CRYSTAL >> 1)));
            break;
        }
        case STORES_DISPLAYED: {

            if (mClose->getAlpha() < 1.f) {

                reduceVolume(0, game->getDelta());
                mClose->setAlpha(mClose->getAlpha() + (ALPHA_VEL_A * 4.f));
            }
            else
                ++mStep;
            break;
        }
        case CLOSE_DISPLAYED: {

            reduceVolume(0, game->getDelta());
            break;
        }
    }
    mResponse->update(game, this);
    return true;
}
void CrystalBall::renderUpdate() const {

    mBackground->render(false);
    mBall->render();

    if (mResponse->toRender())
        mResponse->render(true);

    mCrystal->render(true);
    mClose->render(true);
}

#endif // !PAID_VERSION
