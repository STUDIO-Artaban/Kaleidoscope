#include "KaleidoLevel.h"

#include <libeng/Game/2D/Game2D.h>
#include <libeng/Storage/Storage.h>
#include <libeng/Tools/Tools.h>

#include <boost/thread.hpp>

#define LAST_LOAD_STEP          6
#define PHOTO_FOLDER_NAME       "Kaleidoscope"
#define PICTURE_EXTENSION       ".png"

#define PALETTE_TEX_SIZE        256

#define MAX_AD_DELAY            30 // In seconds
#define MID_AD_DELAY            15 // ...
#define MAX_TOUCH_DELAY         4  // ...

#define COLOR_Y0                45
#define COLOR_Y2                191
#define GRAY_Y0                 199
#define GRAY_Y2                 211

#define TIMER_BEEP_COUNT        4 // Beep count B4 taking a picture

#ifndef PAID_VERSION
#ifdef __ANDROID__
#define KALEIDOSCOPE_APP_URL    "market://details?id=com.studio.artaban.kaleidoscope"
#else
#define KALEIDOSCOPE_APP_URL    @"itms-apps://itunes.apple.com/us/app/kaleidoscope/id  ?????????  "
#endif
#endif

// Texture IDs
#define TEXTURE_ID_PALETTE      25

// Sound IDs
#ifdef PAID_VERSION
#define SOUND_ID_BEEP           3
#define SOUND_ID_FLASH          4
#endif

//////
KaleidoLevel::KaleidoLevel(Game* game) : Level(game), mRed(0.f), mGreen(0.f), mBlue(0.f), mDisplayID(DISP_COMMAND),
        mPaletteBuffer(NULL), mCounter(0), mTakePic(false), mRedVel(0.f), mGreenVel(0.f), mBlueVel(0.f) {

    LOGV(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
    std::memset(&mColorArea, 0, sizeof(TouchArea));
    std::memset(&mGrayArea, 0, sizeof(TouchArea));

    game->mAccelPrecision = 0.f;
    Inputs::InputUse = USE_INPUT_TOUCH | USE_INPUT_ACCEL;

    mCamera = Camera::getInstance();
#ifndef PAID_VERSION
    mAdvertising = Advertising::getInstance();

    mAdDelay = 0;
    mTouchDelay = 0;
#endif

    mPicWidth = game->getScreen()->width;
    mPicHeight = game->getScreen()->height;
    mPicBuffer = new unsigned char[(mPicWidth * mPicHeight) << 2];

    mPalette = NULL;
    mHideCam = NULL;
}
KaleidoLevel::~KaleidoLevel() {

    LOGV(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (mPalette) delete mPalette;
    if (mHideCam) delete mHideCam;

    // Avoid to quit without having finish to save picture
    while (!Storage::getInstance()->getStatus()) // == STORE_IN_PROGRESS
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    delete [] mPicBuffer;
    delete [] mPaletteBuffer;
}

void KaleidoLevel::pause() {

    LOGV(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Level::pause();

    mDisplayID = DISP_COMMAND;
    mCounter = 0;

    mCommand.mPause = false; // Pause disabled coz camera buffer will be released
    if (mCommand.mColor) {

        assert(mPalette);
        delete mPalette;
        mPalette = NULL;

        mCommand.mColor = false;
    }
    mKaleido2D.setChecked(mCommand.mCheck, mCommand.mShift);

    mKaleido2D.pause();
    mPanel.pause();
    mCommand.pause();
    if (mHideCam)
        mHideCam->pause();

    assert(mPaletteBuffer);
    delete [] mPaletteBuffer;
    mPaletteBuffer = NULL;
}

bool KaleidoLevel::loaded(const Game* game) {

    LOGV(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(" - l:%d"), __PRETTY_FUNCTION__, __LINE__, mLoadStep);
    switch (mLoadStep) {
        case 1: {

            if (!mHideCam) {

                mHideCam = new Static2D;
                mHideCam->initialize(game2DVia(game));
                mHideCam->start(0, 0, 0);
                mHideCam->setTexCoords(FULL_TEXCOORD_BUFFER);
                mHideCam->setVertices(0, game->getScreen()->height, game->getScreen()->width, 0);
            }
            else
                mHideCam->resume(0, 0, 0);
            break;
        }
        case 2: {

            if (!mCommand.isStarted())
                mCommand.start(game2DVia(game));
            else
                mCommand.resume();

            mCommand.hide(game->getScreen()->bottom);
            mCommand.setConfig();
            break;
        }
        case 3: {

            assert(!mPalette);
            unsigned char textureIdx = Textures::loadTexture(TEXTURE_ID_PALETTE);

            assert(!mPaletteBuffer);
            mPaletteBuffer = (*mTextures)[textureIdx]->textureBuffer;
            mTextures->genTexture(textureIdx, false);

            short height = (GRAY_Y2 - COLOR_Y0) * game->getScreen()->width / (PALETTE_TEX_SIZE << 1); // Half
            mColorArea.top = (game->getScreen()->height >> 1) - height;
            mColorArea.bottom = mColorArea.top + ((COLOR_Y2 - COLOR_Y0) * game->getScreen()->width / PALETTE_TEX_SIZE);

            mGrayArea.bottom = (game->getScreen()->height >> 1) + height;
            mGrayArea.top = mGrayArea.bottom - ((GRAY_Y2 - GRAY_Y0) * game->getScreen()->width / PALETTE_TEX_SIZE);
            break;
        }
        case 4: {

#ifdef PAID_VERSION
            Player::loadSound(SOUND_ID_BEEP);
            Player::loadSound(SOUND_ID_FLASH);

            Player::getInstance()->setVolume(0, 1.f);
            Player::getInstance()->setVolume(1, 1.f);
#endif
            break;
        }
        case 5: {

#ifndef PAID_VERSION
            unsigned char adStatus = Advertising::getStatus();
            assert(adStatus != Advertising::STATUS_NONE);

            if ((adStatus == Advertising::STATUS_READY) || (adStatus == Advertising::STATUS_FAILED))
                mAdvertising->load();

            mAdvertising->resetPubDelay();
            mAdDelay = (*static_cast<const unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_AD_DELAY)));
            mTouchDelay = 0;
#endif
            break;
        }
        case LAST_LOAD_STEP: {

            if (!mPanel.isStarted())
                mPanel.start(game2DVia(game));
            else
                mPanel.resume();

            mRed = 0.f;
            mGreen = 0.f;
            mBlue = 0.f;

            mRedVel = (*static_cast<const unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_SHIFT_RED))) / MAX_COLOR;
            mRedVel /= static_cast<float>(DISPLAY_DELAY);
            mGreenVel = (*static_cast<const unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_SHIFT_GREEN))) / MAX_COLOR;
            mGreenVel /= static_cast<float>(DISPLAY_DELAY);
            mBlueVel = (*static_cast<const unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_SHIFT_BLUE))) / MAX_COLOR;
            mBlueVel /= static_cast<float>(DISPLAY_DELAY);

            mPanel.hide(game->getScreen()->bottom);
            break;
        }
        default: {

            if (!mCamera->isStarted())
                mCamera->start(CAM_WIDTH, CAM_HEIGHT);

            if (!mKaleido2D.load(game2DVia(game), mLoadStep - LAST_LOAD_STEP)) {

                mKaleido2D.setAlpha(0.f);
                mKaleido2D.setGradient(mCommand.mGradient);

                delete mHideCam;
                mHideCam = NULL;
                return true;
            }
            break;
        }
    }
    return false;
}
void KaleidoLevel::renderLoad() const {

    assert(mHideCam);
    mHideCam->render(false);
}

#ifdef PAID_VERSION
void KaleidoLevel::photo(short width, short height) {

    LOGV(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(" - w:%d; h:%d"), __PRETTY_FUNCTION__, __LINE__, width, height);
    mTakePic = true;

    mPicFile = PHOTO_FOLDER_NAME;
    mPicFile.append("/KAL_");

    time_t curDate = time(0);
    struct tm* now = localtime(&curDate);

    mPicFile.append(numToStr<short>(now->tm_year + 1900));
    if (now->tm_mon < 9)
        mPicFile += '0';
    mPicFile.append(numToStr<short>(now->tm_mon + 1));
    if (now->tm_mday < 10)
        mPicFile += '0';
    mPicFile.append(numToStr<short>(now->tm_mday));

    mPicFile += '_';
    if (now->tm_hour < 10)
        mPicFile += '0';
    mPicFile.append(numToStr<short>(now->tm_hour));
    if (now->tm_min < 10)
        mPicFile += '0';
    mPicFile.append(numToStr<short>(now->tm_min));
    if (now->tm_sec < 10)
        mPicFile += '0';
    mPicFile.append(numToStr<short>(now->tm_sec));
    mPicFile.append(PICTURE_EXTENSION);

    assert(Player::getInstance()->getIndex(SOUND_ID_FLASH) == 1);
    Player::getInstance()->play(1);
}
#endif

bool KaleidoLevel::update(const Game* game) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_KALEIDOLEVEL, (*game->getLog() % 100), LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
#endif
    glClearColor(mRed, mGreen, mBlue, 1.f);

#ifdef __ANDROID__
#ifdef PAID_VERSION
    // Restart player (needed for lock/unlock operation)
    Player::getInstance()->resume();
#else

    if (mCamera->getCamTexIdx() == TEXTURE_IDX_INVALID)
        mCamera->interstitialAdCamResume();
    // BUG: When displaying the AdMob Interstitial advertising a lock/unlock screen operation is done except that no
    //      change on surface occurs (no 'EngSurface::surfaceChanged' java method call)
    // -> Resume the camera manually
#endif
#endif
    if (!mCommand.mPause)
        mCamera->updateTexture();

    if (mCounter < DISPLAY_DELAY) {

        switch (mDisplayID) {
            case DISP_COMMAND: { // Display kaleidoscope & Command

                if (mKaleido2D.getAlpha() < 1.f)
                    mKaleido2D.setAlpha(mKaleido2D.getAlpha() + ALPHA_VEL_B);
                if (!mCommand.mHidden) {

                    mPanel.display(game->getScreen()->bottom, false);
                    mCommand.display(false);
                }

                // Set background colors here
                mRed += mRedVel;
                mGreen += mGreenVel;
                mBlue += mBlueVel;
                break;
            }
            case DISP_CHECK: mKaleido2D.check(); break; // Display 'check' command
            case DISP_UNCHECK: mKaleido2D.uncheck((mCounter + 1) != DISPLAY_DELAY); break; // Hide 'check' command
            case DISP_HIDDEN: {

                mPanel.display(game->getScreen()->bottom, true);
                mCommand.display(true);
                break;
            }
        }
        ++mCounter;
        return true;
    }

#ifndef PAID_VERSION
    // Advertising...
    unsigned char adDelay = 0;
    unsigned char adStatus = Advertising::getStatus();

    if ((adStatus == Advertising::STATUS_READY) || (adStatus == Advertising::STATUS_FAILED)) {

        LOGI(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(" - Load advertising"), __PRETTY_FUNCTION__, __LINE__);
#ifdef __ANDROID__
        mCommand.setConfig(); // Needed coz pause option is disabled when displaying the advertising
#endif
        mAdvertising->resetPubDelay();
        mAdvertising->load();
    }
    else if (adStatus == Advertising::STATUS_LOADED) {

        adDelay = mAdvertising->getPubDelay() + mAdDelay;
        if ((adDelay > MAX_AD_DELAY) || ((mTouchDelay) && ((adDelay - mTouchDelay) > MAX_TOUCH_DELAY))) {

            LOGI(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(" - Display advertising"), __PRETTY_FUNCTION__, __LINE__);
            *static_cast<unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_AD_DELAY)) = 0;
            mAdDelay = mTouchDelay = 0;
#ifdef __ANDROID__
            mCommand.mPause = false; // Disable pause option coz the camera buffer will be released
            // WARNING: Crash when advertising is clicked + Back to the app + Pause/Resume operation (camera issue)
#endif
            mAdvertising->resetPubDelay();
            Advertising::display(0); // Advertising::STATUS_DISPLAYING
        }
        else
            *static_cast<unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_AD_DELAY)) = adDelay;
    }

#else
    // Photo (timer)
    static unsigned char beepCount = 0;
    if (mDisplayID == DISP_TIMER) {

        if (++beepCount == TIMER_BEEP_COUNT) { // Flash ...take a picture

            mDisplayID = DISP_COMMAND;
            photo(game->getScreen()->width, game->getScreen()->height);
        }
        else { // Timer

            LOGI(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(" - Beep!"), __PRETTY_FUNCTION__, __LINE__);
            assert(!Player::getInstance()->getIndex(SOUND_ID_BEEP));
            Player::getInstance()->play(0);
            mCounter = 0;
        }
        return true;
    }

#endif

    // Touch...
    unsigned char touchCount = game->mTouchCount;
    while (touchCount--) {

        // Palette color
        if (mCommand.mColor) {

            switch (game->mTouchData[touchCount].Type) {
                case TouchInput::TOUCH_DOWN:
                case TouchInput::TOUCH_MOVE: {

                    if ((game->mTouchData[touchCount].Y > mColorArea.top) &&
                            (game->mTouchData[touchCount].Y < mColorArea.bottom)) { // Color

                        long colorIdx = ((game->mTouchData[touchCount].X * PALETTE_TEX_SIZE / game->getScreen()->width) +
                                ((COLOR_Y0 + ((game->mTouchData[touchCount].Y - mColorArea.top) * (COLOR_Y2 - COLOR_Y0) /
                                        (mColorArea.bottom - mColorArea.top))) * PALETTE_TEX_SIZE)) << 2;
                        assert(colorIdx < ((COLOR_Y2 * PALETTE_TEX_SIZE) << 2));
                        assert(colorIdx >= ((COLOR_Y0 * PALETTE_TEX_SIZE) << 2));

                        mRed = mPaletteBuffer[colorIdx + 0] / MAX_COLOR;
                        mGreen = mPaletteBuffer[colorIdx + 1] / MAX_COLOR;
                        mBlue = mPaletteBuffer[colorIdx + 2] / MAX_COLOR;
                    }
                    else if ((game->mTouchData[touchCount].Y > mGrayArea.top) &&
                            (game->mTouchData[touchCount].Y < mGrayArea.bottom)) { // Grayscale

                        long colorIdx = ((game->mTouchData[touchCount].X * PALETTE_TEX_SIZE / game->getScreen()->width) +
                                ((GRAY_Y0 + ((game->mTouchData[touchCount].Y - mGrayArea.top) * (GRAY_Y2 - GRAY_Y0) /
                                        (mGrayArea.bottom - mGrayArea.top))) * PALETTE_TEX_SIZE)) << 2;
                        assert(colorIdx < ((GRAY_Y2 * PALETTE_TEX_SIZE) << 2));
                        assert(colorIdx >= ((GRAY_Y0 * PALETTE_TEX_SIZE) << 2));

                        mRed = mPaletteBuffer[colorIdx + 0] / MAX_COLOR;
                        mGreen = mPaletteBuffer[colorIdx + 1] / MAX_COLOR;
                        mBlue = mPaletteBuffer[colorIdx + 2] / MAX_COLOR;
                    }
                    return true;
                }
                default: {

                    delete mPalette;
                    mPalette = NULL;

                    *static_cast<unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_SHIFT_RED)) = mRed * MAX_COLOR;
                    *static_cast<unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_SHIFT_GREEN)) = mGreen * MAX_COLOR;
                    *static_cast<unsigned char*>(game->getData(DATA_CHAR, DATA_KEY_SHIFT_BLUE)) = mBlue * MAX_COLOR;

                    mCommand.mColor = false;
                    mCommand.setConfig();
                    return true;
                }
            }
        }

        if (game->mTouchData[touchCount].Type != TouchInput::TOUCH_UP)
            continue;

        // Show commands
        if (mCommand.mHidden) {

            mCommand.mHidden = false;
            mDisplayID = DISP_COMMAND;
            mCounter = 0;

            mRedVel = 0.f;
            mGreenVel = 0.f;
            mBlueVel = 0.f;
            return true;
        }
    }
    if ((mCommand.mHidden) || (mCommand.mColor))
        return true;

    // Command...
#ifndef PAID_VERSION
    unsigned char commandID = mCommand.update(game);
    if ((commandID) && (!mTouchDelay) && (adDelay > MID_AD_DELAY)) {

        LOGI(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(" - Touch delay..."), __PRETTY_FUNCTION__, __LINE__);
        mTouchDelay = adDelay;
    }
    switch (commandID) {
#else
    switch (mCommand.update(game)) {
#endif
        case CMD_SHIFT: {

            mCommand.mShift = !mCommand.mShift;
            mCommand.setConfig();
            mKaleido2D.setShift(mCommand.mShift);
            break;
        }
        case CMD_COLOR: {

            if (mCommand.mShift) {

                LOGI(LOG_LEVEL_KALEIDOLEVEL, 0, LOG_FORMAT(" - Display palette (p:%x)"), __PRETTY_FUNCTION__, __LINE__,
                        mPalette);
                assert(!mPalette);

                mCommand.mColor = true;
                mCommand.setConfig();

                mPalette = new Element2D(false);
                mPalette->initialize(game2DVia(game));
                mPalette->start(mTextures->getIndex(TEXTURE_ID_PALETTE));
                mPalette->setTexCoords(FULL_TEXCOORD_BUFFER);

                mPalette->setVertices(0, (game->getScreen()->height >> 1) + (game->getScreen()->width >> 1),
                        game->getScreen()->width, (game->getScreen()->height >> 1) - (game->getScreen()->width >> 1));
            }
            break;
        }
        case CMD_GRADIENT: {

            mCommand.mGradient = !mCommand.mGradient;
            mCommand.setConfig();
            mKaleido2D.setGradient(mCommand.mGradient);
            break;
        }
        case CMD_CHECK: {

            mCommand.mCheck = !mCommand.mCheck;
            mCommand.setConfig();
            mDisplayID = (mCommand.mCheck)? DISP_CHECK:DISP_UNCHECK;
            mCounter = 0;
            break;
        }
        case CMD_PAUSE: {

            if (mCommand.mPause)
                break;

            mCommand.mPause = true;
            mCommand.setConfig();
            break;
        }
        case CMD_PLAY: {

            if (!mCommand.mPause)
                break;

            mCommand.mPause = false;
            mCommand.setConfig();
            break;
        }
        case CMD_TRASH: {

            mCommand.mGradient = true;
            mCommand.mShift = false;
            mCommand.mCheck = false;
            mCommand.mPause = false;
            mCommand.mTimer = false;
            mCommand.setConfig();

            mKaleido2D.setGradient(true);
            mKaleido2D.setShift(false);
            mKaleido2D.setChecked(false, false);
            break;
        }
        case CMD_TIMER: {

            mCommand.mTimer = !mCommand.mTimer;
            mCommand.setConfig();
            break;
        }
        case CMD_HIDE: {

#ifndef PAID_VERSION
#ifdef __ANDROID__
            Launcher launchApp(GOOGLE_PLAY_PACKAGE, GOOGLE_PLAY_ACTIVITY, KALEIDOSCOPE_APP_URL);
            launchApp.go();
#else
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:KALEIDOSCOPE_APP_URL]];
#endif
#else
            mCommand.mHidden = true;
            mDisplayID = DISP_HIDDEN;
            mCounter = 0;
#endif
            break;
        }
        case CMD_PHOTO: {

#ifndef PAID_VERSION
#ifdef __ANDROID__
            Launcher launchApp(GOOGLE_PLAY_PACKAGE, GOOGLE_PLAY_ACTIVITY, KALEIDOSCOPE_APP_URL);
            launchApp.go();
#else
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:KALEIDOSCOPE_APP_URL]];
#endif
#else
            if (!Storage::getInstance()->getStatus()) // == STORE_IN_PROGRESS
                break;
            // Do not take a photo if there is a previous picture which has not been stored yet

            if ((!mCommand.mPause) && (mCommand.mTimer)) {

                mDisplayID = DISP_TIMER;
                mCounter = DISPLAY_DELAY;
                beepCount = 0;
                break;
            }
            photo(game->getScreen()->width, game->getScreen()->height);
#endif
            break;
        }
    }
    return true;
}
void KaleidoLevel::renderUpdate() const {

    mKaleido2D.render();
    if (mTakePic) {

        glReadPixels(0, 0, mPicWidth, mPicHeight, GL_RGBA, GL_UNSIGNED_BYTE, mPicBuffer);
#ifdef __ANDROID__
        Storage::getInstance()->savePicture(mPicFile, true, mPicWidth, mPicHeight, mPicBuffer);
#else
        Storage::getInstance()->savePicture(PIC_FORMAT_RGBA, mPicWidth, mPicHeight, mPicBuffer);
#endif
        mTakePic = false;
    }
    mPanel.render();
    mCommand.render();

    if (mPalette)
        mPalette->render(true);
}
