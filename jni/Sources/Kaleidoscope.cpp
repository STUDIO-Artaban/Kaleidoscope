#include "Kaleidoscope.h"
#include "Global.h"

#include <libeng/Log/Log.h>
#include <libeng/Storage/Storage.h>
#include <boost/thread.hpp>

#ifndef PAID_VERSION
#include <libeng/Features/Internet/Internet.h>
#endif

// Textures IDs
#define TEXTURE_ID_TITLE_KALEIDO    26

//////
#ifdef DEBUG
Kaleidoscope::Kaleidoscope() : Game2D(1), mTitleStep(0) {
#else
Kaleidoscope::Kaleidoscope() : Game2D(0), mTitleStep(0) {
#endif
    LOGV(LOG_LEVEL_KALEIDOSCOPE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (!mGameLevel)
#ifndef PAID_VERSION
        mGameIntro = new Intro(Intro::LANG_EN, true);
#else
        mGameIntro = new Intro(Intro::LANG_EN, false);
#endif
    mGameData = new StoreData;

#ifndef PAID_VERSION
    Dynamic2D::initIncreaseBuffer();

    mCrystalBall = NULL;
    mWitchSPet = NULL;
    mWaitNet = NULL;
#endif
    mKaleidoTitle = NULL;
    mKaleido = NULL;
}
Kaleidoscope::~Kaleidoscope() {

    LOGV(LOG_LEVEL_KALEIDOSCOPE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
#ifndef PAID_VERSION
    Dynamic2D::freeIncreaseBuffer();
#endif
}

bool Kaleidoscope::start() {

    LOGV(LOG_LEVEL_KALEIDOSCOPE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    if (Game2D::start()) {

        if (Storage::loadFile(FILE_NAME)) {

            Storage* store = Storage::getInstance();
            if (!store->isEmpty(FILE_NAME)) {
                if (!Storage::File2Data(mGameData, (*store)[FILE_NAME])) {

                    LOGW(LOG_FORMAT(" - Failed to get data from '%s' file"), __PRETTY_FUNCTION__, __LINE__, FILE_NAME);
                    assert(NULL);
                }
            }
            else { // No data stored: Create data

                mGameData->add(DATA_CHAR, DATA_KEY_SHIFT_RED, static_cast<void*>(new unsigned char(0)));
                mGameData->add(DATA_CHAR, DATA_KEY_SHIFT_GREEN, static_cast<void*>(new unsigned char(0)));
                mGameData->add(DATA_CHAR, DATA_KEY_SHIFT_BLUE, static_cast<void*>(new unsigned char(0)));
#ifndef PAID_VERSION
                mGameData->add(DATA_CHAR, DATA_KEY_AD_DELAY, static_cast<void*>(new unsigned char(0)));
#endif
            }
        }
#ifdef DEBUG
        else {

            LOGW(LOG_FORMAT(" - Failed to open existing '%s' file"), __PRETTY_FUNCTION__, __LINE__, FILE_NAME);
            assert(NULL);
        }
#endif
        return true;
    }
    else if (mGameLevel) {

        if (!(mGameLevel & 0x01))
            --mGameLevel;

#ifndef PAID_VERSION
        // Check if still online
#ifdef __ANDROID__
        if (!Internet::getInstance()->isOnline(2000)) {
#else
        if (!Internet::getInstance()->isOnline()) {
#endif
            assert(!mWaitNet);
            mGameLevel = 0; // Restart from the begining

            mWaitNet = new WaitNet;
            mWaitNet->initialize(game2DVia(this));
            mWaitNet->start(mScreen);
        }
    }
    else if (mWaitNet) {

        // Check if still not online
#ifdef __ANDROID__
        if (!Internet::getInstance()->isOnline(2000))
#else
        if (!Internet::getInstance()->isOnline())
#endif
            mWaitNet->resume();

        else {

            delete mWaitNet;
            mWaitNet = NULL;

            mGameLevel = 1;

            if (mCrystalBall) delete mCrystalBall;
            if (mWitchSPet) delete mWitchSPet;
            if (mKaleido) delete mKaleido;

            mCrystalBall = NULL;
            mWitchSPet = NULL;
            mKaleido = NULL;
        }
#endif
    }
    return false;
}
void Kaleidoscope::pause() {

    LOGV(LOG_LEVEL_KALEIDOSCOPE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Game2D::pause();

#ifndef PAID_VERSION
    if (mCrystalBall) mCrystalBall->pause();
    if (mWitchSPet) mWitchSPet->pause();
    if (mWaitNet) mWaitNet->pause();
#endif
    if (mKaleido) mKaleido->pause();
}
void Kaleidoscope::destroy() {

    LOGV(LOG_LEVEL_KALEIDOSCOPE, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    Game2D::destroy();

#ifndef PAID_VERSION
    if (mWaitNet) delete mWaitNet;
    // WARNING: Should be deleted B4 level class inheritance below coz all textures are deleted at level destructor

    if (mCrystalBall) delete mCrystalBall;
    if (mWitchSPet) delete mWitchSPet;
#endif
    if (mKaleido) delete mKaleido;
}

void Kaleidoscope::wait(float millis) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_KALEIDOSCOPE, (mLog % 100), LOG_FORMAT(" - Delay: %f milliseconds"), __PRETTY_FUNCTION__, __LINE__, millis);
#endif
    boost::this_thread::sleep(boost::posix_time::microseconds(static_cast<unsigned long>(millis * 500)));
}
void Kaleidoscope::update() {

#ifdef DEBUG
    LOGV(LOG_LEVEL_KALEIDOSCOPE, (mLog % 100), LOG_FORMAT(" - g:%d"), __PRETTY_FUNCTION__, __LINE__, mGameLevel);
#endif
    switch (mGameLevel) {

#ifndef PAID_VERSION
        case 0: {

            if (!mWaitNet)
                updateIntro(); // Introduction

            else { // Wait Internet

                static unsigned char wait = 0;
                if (wait < (LIBENG_WAITNET_DELAY << 2)) {

                    ++wait;
                    break;
                }
                mWaitNet->update();
            }
            break;
        }

        ////// CrystalBall
        case 1: {

            if (!mCrystalBall) {

                mCrystalBall = new CrystalBall(this);
                mCrystalBall->initialize();
            }
            if (!mCrystalBall->load(this))
                break;

            ++mGameLevel;
            //break;
        }

        case 2: {

            if (mCrystalBall->update(this))
                break;

            delete mCrystalBall;
            mCrystalBall = NULL;
            ++mGameLevel;
            //break;
        }

        ////// WitchSPet
        case 3: {

            if (!mWitchSPet) {

                mWitchSPet = new WitchSPet(this);
                mWitchSPet->initialize();
            }
            if (!mWitchSPet->load(this))
                break;

            ++mGameLevel;
            //break;
        }
        case 4: {

            if (mWitchSPet->update(this))
                break;

            delete mWitchSPet;
            mWitchSPet = NULL;
            ++mGameLevel;
            //break;
        }

        ////// Kaleidoscope (title)
        case 5: {
#else
        case 0: updateIntro(); break; // Introduction

        ////// Kaleidoscope (title)
        case 1: {
#endif
            if (!mKaleidoTitle) {

                mKaleidoTitle = new Static2D;
                mKaleidoTitle->initialize(game2DVia(this));
                mKaleidoTitle->start(TEXTURE_ID_TITLE_KALEIDO);
                mKaleidoTitle->setTexCoords(FULL_TEXCOORD_BUFFER);
                mKaleidoTitle->setVertices(0, (mScreen->height >> 1) + (mScreen->width >> 1), mScreen->width,
                        (mScreen->height >> 1) - (mScreen->width >> 1));
            }
            else
                mKaleidoTitle->resume(TEXTURE_ID_TITLE_KALEIDO);
            mKaleidoTitle->setAlpha(0.f);

            mTitleStep = 0;
            ++mGameLevel;
            //break;
        }
#ifndef PAID_VERSION
        case 6: {
#else
        case 2: {
#endif
            switch (mTitleStep) {
                case 0: { // Display

                    if (mKaleidoTitle->getAlpha() < 1.f)
                        mKaleidoTitle->setAlpha(mKaleidoTitle->getAlpha() + ALPHA_VEL_B);
                    else
                        ++mTitleStep;
                    break;
                }
                default: ++mTitleStep; break; // Wait
                case (DISPLAY_DELAY << 1): { // Hide

                    if (mKaleidoTitle->getAlpha() > ALPHA_VEL_B)
                        mKaleidoTitle->setAlpha(mKaleidoTitle->getAlpha() - ALPHA_VEL_B);
                    else
                        ++mGameLevel;
                    break;
                }
            }
            break;
        }

        ////// Kaleidoscope
#ifndef PAID_VERSION
        case 7: {
#else
        case 3: {
#endif
            if (!mKaleido) {

                assert(mKaleidoTitle);
                delete mKaleidoTitle;
                mKaleidoTitle = NULL;

                mKaleido = new KaleidoLevel(this);
                mKaleido->initialize();
            }
            if (!mKaleido->load(this))
                break;

            ++mGameLevel;
            //break;
        }
#ifndef PAID_VERSION
        case 8: {
#else
        case 4: {
#endif
            mKaleido->update(this);
            break;
        }

#ifdef DEBUG
        default: {

            LOGW(LOG_FORMAT(" - Unknown game level to update: %d"), __PRETTY_FUNCTION__, __LINE__, mGameLevel);
            assert(NULL);
            break;
        }
#endif
    }
}

void Kaleidoscope::render() const {
    switch (mGameLevel) {

#ifndef PAID_VERSION
        case 0: {

            if (!mWaitNet)
                renderIntro(); // Introduction
            else
                mWaitNet->render(); // Wait Internet
            break;
        }
        case 1: if (mCrystalBall) mCrystalBall->renderLoad(); break;
        case 2: mCrystalBall->renderUpdate(); break;
        case 3: if (mWitchSPet) mWitchSPet->renderLoad(); break;
        case 4: mWitchSPet->renderUpdate(); break;
        case 5:
        case 6: mKaleidoTitle->render(false); break;
        case 7: if (mKaleido) mKaleido->renderLoad(); break;
        case 8: mKaleido->renderUpdate(); break;
#else
        case 0: renderIntro(); break; // Introduction
        case 1:
        case 2: mKaleidoTitle->render(false); break;
        case 3: if (mKaleido) mKaleido->renderLoad(); break;
        case 4: mKaleido->renderUpdate(); break;
#endif

#ifdef DEBUG
        default: {

            LOGW(LOG_FORMAT(" - Unknown game level to render: %d"), __PRETTY_FUNCTION__, __LINE__, mGameLevel);
            assert(NULL);
            break;
        }
#endif
    }
}
