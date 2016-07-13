#include "Command.h"

#include <libeng/Tools/Tools.h>

#ifdef __ANDROID__
#include <boost/math/tr1.hpp>
#else
#include <math.h>
#endif

#define COMMAND_X0              140
#define COMMAND_SIZE            124
#define PHOTO_X0                153
#define PHOTO_Y2                506

#define CMD_TRANS_X             0.065f // In unit
#define DISABLE_FACTOR          0.7f

#define ROTATION_DELAY          15

// Colors
#define RED_RED                 (0xf2 / MAX_COLOR)
#define RED_GREEN               (0xea / MAX_COLOR)
#define RED_BLUE                (0xd7 / MAX_COLOR)

#define YELLOW_RED              (0xf2 / MAX_COLOR)
#define YELLOW_GREEN            (0xea / MAX_COLOR)
#define YELLOW_BLUE             (0xd7 / MAX_COLOR)

#define GREEN_RED               (0xf2 / MAX_COLOR)
#define GREEN_GREEN             (0xea / MAX_COLOR)
#define GREEN_BLUE              (0xd7 / MAX_COLOR)

#define BLUE_RED                (0xf2 / MAX_COLOR)
#define BLUE_GREEN              (0xea / MAX_COLOR)
#define BLUE_BLUE               (0xd7 / MAX_COLOR)

// Texture IDs
#define TEXTURE_ID_COMMAND      23

static const float g_GradientTexCoords[] = { (COMMAND_X0 + (COMMAND_SIZE << 1)) / CMD_TEXTURE_WIDTH, 0.f,
        (COMMAND_X0 + (COMMAND_SIZE << 1)) / CMD_TEXTURE_WIDTH, COMMAND_SIZE / CMD_TEXTURE_HEIGHT, 1.f,
        COMMAND_SIZE / CMD_TEXTURE_HEIGHT, 1.f, 0.f };

static const float g_ShiftTexCoords[] = { COMMAND_X0 / CMD_TEXTURE_WIDTH, 0.f, COMMAND_X0 / CMD_TEXTURE_WIDTH,
        COMMAND_SIZE / CMD_TEXTURE_HEIGHT, (COMMAND_X0 + COMMAND_SIZE) / CMD_TEXTURE_WIDTH,
        COMMAND_SIZE / CMD_TEXTURE_HEIGHT, (COMMAND_X0 + COMMAND_SIZE) / CMD_TEXTURE_WIDTH, 0.f};

//////
Command::Command() : mGradient(true), mShift(false), mCheck(false), mPause(false), mOrientation(ORT_PORTRAIT),
        mRotation(0.f), mHidden(false), mColor(false), mTimer(false), mStarted(false) {

    LOGV(LOG_LEVEL_COMMAND, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    std::memset(&mPauseArea, 0, sizeof(TouchArea));
    std::memset(&mPlayArea, 0, sizeof(TouchArea));
    std::memset(&mTrashArea, 0, sizeof(TouchArea));
    std::memset(&mCheckArea, 0, sizeof(TouchArea));

    std::memset(&mShiftArea, 0, sizeof(TouchArea));
    std::memset(&mColorArea, 0, sizeof(TouchArea));
    std::memset(&mHideArea, 0, sizeof(TouchArea));
    std::memset(&mGradientArea, 0, sizeof(TouchArea));
    std::memset(&mTimerArea, 0, sizeof(TouchArea));
}
Command::~Command() { LOGV(LOG_LEVEL_COMMAND, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__); }

void Command::start(const Game2D* game) {

    LOGV(LOG_LEVEL_COMMAND, 0, LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
    mPause2D.initialize(game2DVia(game));
    mPlay2D.initialize(game2DVia(game));
    mTimer2D.initialize(game2DVia(game));
    mTrash2D.initialize(game2DVia(game));

    mShift2D.initialize(game2DVia(game));
    mColor2D.initialize(game2DVia(game));
    mGradient2D.initialize(game2DVia(game));
    mCheck2D.initialize(game2DVia(game));

    mPause2D.start(TEXTURE_ID_COMMAND);
    mPlay2D.start(TEXTURE_ID_COMMAND);
    mTimer2D.start(TEXTURE_ID_COMMAND);
    mTrash2D.start(TEXTURE_ID_COMMAND);

    mShift2D.start(TEXTURE_ID_COMMAND);
    mColor2D.start(TEXTURE_ID_COMMAND);
    mGradient2D.start(TEXTURE_ID_COMMAND);
    mCheck2D.start(TEXTURE_ID_COMMAND);

    float texCoords[8] = {0};
    std::memcpy(texCoords, g_ShiftTexCoords, sizeof(float) * 8);
    texCoords[0] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[2] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[4] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[6] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    mColor2D.setTexCoords(texCoords);

    mShift2D.setTexCoords(g_ShiftTexCoords);
    mGradient2D.setTexCoords(g_GradientTexCoords);

    texCoords[0] = (COMMAND_X0 + COMMAND_SIZE) / CMD_TEXTURE_WIDTH;
    texCoords[1] = COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
    texCoords[2] = texCoords[0];
    texCoords[3] = 2.f * texCoords[1];
    texCoords[4] = (COMMAND_X0 + (COMMAND_SIZE << 1)) / CMD_TEXTURE_WIDTH;
    texCoords[5] = texCoords[3];
    texCoords[6] = texCoords[4];
    texCoords[7] = texCoords[1];
    mCheck2D.setTexCoords(texCoords);

    texCoords[0] -= COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[1] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
    texCoords[2] -= COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[3] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
    texCoords[4] -= COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[5] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
    texCoords[6] -= COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[7] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
    mTrash2D.setTexCoords(texCoords);

    texCoords[0] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[2] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[4] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[6] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    mTimer2D.setTexCoords(texCoords);

    texCoords[0] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[2] += COMMAND_SIZE / CMD_TEXTURE_WIDTH;
    texCoords[4] = 1.f;
    texCoords[6] = 1.f;
    mPlay2D.setTexCoords(texCoords);

    texCoords[1] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
    texCoords[3] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
    texCoords[5] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
    texCoords[7] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
    mPause2D.setTexCoords(texCoords);

    static const float verCoords[8] = { -HALF_CMD_SIZE, HALF_CMD_SIZE, -HALF_CMD_SIZE, -HALF_CMD_SIZE, HALF_CMD_SIZE,
            -HALF_CMD_SIZE, HALF_CMD_SIZE, HALF_CMD_SIZE };
    mPause2D.setVertices(verCoords);
    mPlay2D.setVertices(verCoords);
    mTimer2D.setVertices(verCoords);
    mTrash2D.setVertices(verCoords);

    mShift2D.setVertices(verCoords);
    mColor2D.setVertices(verCoords);
    mGradient2D.setVertices(verCoords);
    mCheck2D.setVertices(verCoords);

    //
    mPause2D.translate(HALF_CMD_SIZE + CMD_TRANS_X - 1.f, 0.f);
    mPlay2D.translate((HALF_CMD_SIZE * 3.f) + (CMD_TRANS_X * 2.f) - 1.f, 0.f);
    mTimer2D.translate(1.f - (HALF_CMD_SIZE * 3.f) - (CMD_TRANS_X * 2.f), 0.f);
    mTrash2D.translate(1.f - HALF_CMD_SIZE - CMD_TRANS_X, 0.f);

    mShift2D.translate(HALF_CMD_SIZE + CMD_TRANS_X - 1.f, 0.f);
    mColor2D.translate((HALF_CMD_SIZE * 3.f) + (CMD_TRANS_X * 2.f) - 1.f, 0.f);
    mGradient2D.translate(1.f - (HALF_CMD_SIZE * 3.f) - (CMD_TRANS_X * 2.f), 0.f);
    mCheck2D.translate(1.f - HALF_CMD_SIZE - CMD_TRANS_X, 0.f);

    short height = static_cast<short>((PANEL_X2 << 1) / CMD_TEXTURE_HEIGHT * (game->getScreen()->width >> 1));
    short width = static_cast<short>(HALF_CMD_SIZE * 2.f * (game->getScreen()->width >> 1));

    mPauseArea.left = static_cast<short>(CMD_TRANS_X * (game->getScreen()->width >> 1));
    mPauseArea.top = game->getScreen()->height - height + ((height - width) >> 1);
    mPauseArea.right = mPauseArea.left + width;
    mPauseArea.bottom = mPauseArea.top + width;

    mPlayArea.left = (mPauseArea.left << 1) + width;
    mPlayArea.top = mPauseArea.top;
    mPlayArea.right = mPlayArea.left + width;
    mPlayArea.bottom = mPauseArea.bottom;

    mTrashArea.left = game->getScreen()->width - mPauseArea.right;
    mTrashArea.top = mPauseArea.top;
    mTrashArea.right = game->getScreen()->width - mPauseArea.left;
    mTrashArea.bottom = mPauseArea.bottom;

    mTimerArea.left = game->getScreen()->width - ((game->getScreen()->width - mTrashArea.left) << 1);
    mTimerArea.top = mPauseArea.top;
    mTimerArea.right = mTimerArea.left + width;
    mTimerArea.bottom = mPauseArea.bottom;

    mShiftArea.left = static_cast<short>(CMD_TRANS_X * (game->getScreen()->width >> 1));
    mShiftArea.top = (height - width) >> 1;
    mShiftArea.right = mShiftArea.left + width;
    mShiftArea.bottom = mShiftArea.top + width;

    mColorArea.left = mPlayArea.left;
    mColorArea.top = (height - width) >> 1;
    mColorArea.right = mColorArea.left + width;
    mColorArea.bottom = mShiftArea.bottom;

    mCheckArea.left = mTrashArea.left;
    mCheckArea.top = mShiftArea.top;
    mCheckArea.right = mTrashArea.right;
    mCheckArea.bottom = mShiftArea.bottom;

    mGradientArea.left = mTimerArea.left;
    mGradientArea.top = mShiftArea.top;
    mGradientArea.right = mTimerArea.right;
    mGradientArea.bottom = mShiftArea.bottom;

    mHideArea.left = (game->getScreen()->width >> 1) - (height >> 1);
    mHideArea.top = 0;
    mHideArea.right = mHideArea.left + height;
    mHideArea.bottom = height;

    mStarted = true;
}
void Command::resume() {

    LOGV(LOG_LEVEL_COMMAND, 0, LOG_FORMAT(), __PRETTY_FUNCTION__, __LINE__);
    mPause2D.resume(TEXTURE_ID_COMMAND);
    mPlay2D.resume(TEXTURE_ID_COMMAND);
    mTimer2D.resume(TEXTURE_ID_COMMAND);
    mTrash2D.resume(TEXTURE_ID_COMMAND);

    mShift2D.resume(TEXTURE_ID_COMMAND);
    mColor2D.resume(TEXTURE_ID_COMMAND);
    mGradient2D.resume(TEXTURE_ID_COMMAND);
    mCheck2D.resume(TEXTURE_ID_COMMAND);
}

void Command::setConfig() {

    LOGV(LOG_LEVEL_COMMAND, 0, LOG_FORMAT(" - (s:%s; g:%s; c:%s)"), __PRETTY_FUNCTION__, __LINE__,
            (mShift)? "true":"false",  (mGradient)? "true":"false",  (mCheck)? "true":"false");
    if (mGradient) {

        float texCoords[8] = {0};
        std::memcpy(texCoords, g_GradientTexCoords, sizeof(float) * 8);

        texCoords[1] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
        texCoords[3] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
        texCoords[5] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
        texCoords[7] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
        mGradient2D.setTexCoords(texCoords);

        mGradient2D.setRed(RED_RED * DISABLE_FACTOR);
        mGradient2D.setGreen(RED_GREEN * DISABLE_FACTOR);
        mGradient2D.setBlue(RED_BLUE * DISABLE_FACTOR);
    }
    else {

        mGradient2D.setTexCoords(g_GradientTexCoords);

        mGradient2D.setRed(RED_RED);
        mGradient2D.setGreen(RED_GREEN);
        mGradient2D.setBlue(RED_BLUE);
    }

    if (mShift) {

        float texCoords[8] = {0};
        std::memcpy(texCoords, g_ShiftTexCoords, sizeof(float) * 8);

        texCoords[1] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
        texCoords[3] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
        texCoords[5] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
        texCoords[7] += COMMAND_SIZE / CMD_TEXTURE_HEIGHT;
        mShift2D.setTexCoords(texCoords);

        mShift2D.setRed(GREEN_RED * DISABLE_FACTOR);
        mShift2D.setGreen(GREEN_GREEN * DISABLE_FACTOR);
        mShift2D.setBlue(GREEN_BLUE * DISABLE_FACTOR);

        if (mColor) {

            mColor2D.setRed(GREEN_RED * DISABLE_FACTOR);
            mColor2D.setGreen(GREEN_GREEN * DISABLE_FACTOR);
            mColor2D.setBlue(GREEN_BLUE * DISABLE_FACTOR);
        }
        else {

            mColor2D.setRed(GREEN_RED);
            mColor2D.setGreen(GREEN_GREEN);
            mColor2D.setBlue(GREEN_BLUE);
        }
    }
    else {

        mShift2D.setTexCoords(g_ShiftTexCoords);

        mShift2D.setRed(GREEN_RED);
        mShift2D.setGreen(GREEN_GREEN);
        mShift2D.setBlue(GREEN_BLUE);

        mColor2D.setRed(GREEN_RED * DISABLE_FACTOR);
        mColor2D.setGreen(GREEN_GREEN * DISABLE_FACTOR);
        mColor2D.setBlue(GREEN_BLUE * DISABLE_FACTOR);
    }
    if (mCheck) {

        mCheck2D.setRed(RED_RED * DISABLE_FACTOR);
        mCheck2D.setGreen(RED_GREEN * DISABLE_FACTOR);
        mCheck2D.setBlue(RED_BLUE * DISABLE_FACTOR);
    }
    else {

        mCheck2D.setRed(RED_RED);
        mCheck2D.setGreen(RED_GREEN);
        mCheck2D.setBlue(RED_BLUE);
    }
    if (mPause) {

        mPause2D.setRed(YELLOW_RED * DISABLE_FACTOR);
        mPause2D.setGreen(YELLOW_GREEN * DISABLE_FACTOR);
        mPause2D.setBlue(YELLOW_BLUE * DISABLE_FACTOR);

        mPlay2D.setRed(YELLOW_RED);
        mPlay2D.setGreen(YELLOW_GREEN);
        mPlay2D.setBlue(YELLOW_BLUE);
    }
    else {

        mPause2D.setRed(YELLOW_RED);
        mPause2D.setGreen(YELLOW_GREEN);
        mPause2D.setBlue(YELLOW_BLUE);

        mPlay2D.setRed(YELLOW_RED * DISABLE_FACTOR);
        mPlay2D.setGreen(YELLOW_GREEN * DISABLE_FACTOR);
        mPlay2D.setBlue(YELLOW_BLUE * DISABLE_FACTOR);
    }
    if (mTimer) {

        mTimer2D.setRed(BLUE_RED * DISABLE_FACTOR);
        mTimer2D.setGreen(BLUE_GREEN * DISABLE_FACTOR);
        mTimer2D.setBlue(BLUE_BLUE * DISABLE_FACTOR);
    }
    else {

        mTimer2D.setRed(BLUE_RED);
        mTimer2D.setGreen(BLUE_GREEN);
        mTimer2D.setBlue(BLUE_BLUE);
    }

    if ((!mGradient) || (mShift) || (mCheck) || (mPause) || (mTimer)) {

        mTrash2D.setRed(BLUE_RED);
        mTrash2D.setGreen(BLUE_GREEN);
        mTrash2D.setBlue(BLUE_BLUE);
    }
    else {

        mTrash2D.setRed(BLUE_RED * DISABLE_FACTOR);
        mTrash2D.setGreen(BLUE_GREEN * DISABLE_FACTOR);
        mTrash2D.setBlue(BLUE_BLUE * DISABLE_FACTOR);
    }
}

unsigned char Command::update(const Game* game) {

#ifdef DEBUG
    LOGV(LOG_LEVEL_KALEIDOLEVEL, (*game->getLog() % 100), LOG_FORMAT(" - g:%x"), __PRETTY_FUNCTION__, __LINE__, game);
#endif
    static unsigned char orientation = ORT_UNKNOWN;
    static unsigned char counter = 0;
    static unsigned char status = 0;
    static float rotationVel = 0.f;

    switch (status) {
        case 1: { // Delay

            if (++counter != ROTATION_DELAY)
                break;

            float rotate = mRotation - mPause2D.getTransform()[Dynamic2D::ROTATE_C];
            if (rotate > PI_F)
                rotate -= 2.f * PI_F;
            else if (rotate < -PI_F)
                rotate += 2.f * PI_F;
            rotationVel = rotate / (ROTATION_DELAY - 1);

            counter = 0;
            status = 2;
            //break;
        }
        case 2: { // Rotation

            if (++counter != ROTATION_DELAY) {

                float rotate = mPause2D.getTransform()[Dynamic2D::ROTATE_C] + rotationVel;

                mPause2D.rotate(rotate);
                mPlay2D.rotate(rotate);
                mTimer2D.rotate(rotate);
                mTrash2D.rotate(rotate);

                mShift2D.rotate(rotate);
                mColor2D.rotate(rotate);
                mGradient2D.rotate(rotate);
                mCheck2D.rotate(rotate);

                return CMD_NONE;
            }
            status = 0;
            mOrientation = orientation;
            break;
        }
    }

    unsigned char newOrientation = AccelInput::getOrientation(game->mAccelData);
    if (newOrientation != orientation) {

        switch (newOrientation) {
            default: { // ORT_UNKNOWN

                newOrientation = ORT_PORTRAIT;
                //break;
            }
            case ORT_PORTRAIT:      mRotation = 0.f; break;
            case ORT_REV_PORTRAIT:  mRotation = PI_F; break;
            case ORT_LANDSCAPE:     mRotation = -PI_F / 2.f; break;
            case ORT_REV_LANDSCAPE: mRotation = PI_F / 2.f; break;
        }
        orientation = newOrientation;
        counter = 0;
        status = 1;
    }

    // Touch...
    unsigned char touchCount = game->mTouchCount;
    while (touchCount--) {

        if (game->mTouchData[touchCount].Type != TouchInput::TOUCH_UP)
            continue;

        // Hide
        if ((game->mTouchData[touchCount].X > mHideArea.left) && (game->mTouchData[touchCount].X < mHideArea.right) &&
                (game->mTouchData[touchCount].Y > mHideArea.top) && (game->mTouchData[touchCount].Y < mHideArea.bottom))
            return CMD_HIDE;

        // Check
        if ((game->mTouchData[touchCount].X > mCheckArea.left) && (game->mTouchData[touchCount].X < mCheckArea.right) &&
                (game->mTouchData[touchCount].Y > mCheckArea.top) && (game->mTouchData[touchCount].Y < mCheckArea.bottom))
            return CMD_CHECK;

        // Trash
        if ((game->mTouchData[touchCount].X > mTrashArea.left) && (game->mTouchData[touchCount].X < mTrashArea.right) &&
                (game->mTouchData[touchCount].Y > mTrashArea.top) && (game->mTouchData[touchCount].Y < mTrashArea.bottom))
            return CMD_TRASH;

        // Gradient
        if ((game->mTouchData[touchCount].X > mGradientArea.left) && (game->mTouchData[touchCount].X < mGradientArea.right) &&
                (game->mTouchData[touchCount].Y > mGradientArea.top) && (game->mTouchData[touchCount].Y < mGradientArea.bottom))
            return CMD_GRADIENT;

        // Shift
        if ((game->mTouchData[touchCount].X > mShiftArea.left) && (game->mTouchData[touchCount].X < mShiftArea.right) &&
                (game->mTouchData[touchCount].Y > mShiftArea.top) && (game->mTouchData[touchCount].Y < mShiftArea.bottom))
            return CMD_SHIFT;

        // Pause
        if ((game->mTouchData[touchCount].X > mPauseArea.left) && (game->mTouchData[touchCount].X < mPauseArea.right) &&
                (game->mTouchData[touchCount].Y > mPauseArea.top) && (game->mTouchData[touchCount].Y < mPauseArea.bottom))
            return CMD_PAUSE;

        // Play
        if ((game->mTouchData[touchCount].X > mPlayArea.left) && (game->mTouchData[touchCount].X < mPlayArea.right) &&
                (game->mTouchData[touchCount].Y > mPlayArea.top) && (game->mTouchData[touchCount].Y < mPlayArea.bottom))
            return CMD_PLAY;

        // Photo
        short height = PANEL_X2 * game->getScreen()->width / static_cast<short>(CMD_TEXTURE_HEIGHT);
#ifdef __ANDROID__
        if (boost::math::tr1::hypotf((game->getScreen()->width >> 1) - game->mTouchData[touchCount].X,
                game->getScreen()->height - (height >> 1) - game->mTouchData[touchCount].Y) < (height >> 1))
#else
        if (hypotf((game->getScreen()->width >> 1) - game->mTouchData[touchCount].X,
                game->getScreen()->height - (height >> 1) - game->mTouchData[touchCount].Y) < (height >> 1))
#endif
            return CMD_PHOTO;

        // Color (background)
        if ((game->mTouchData[touchCount].X > mColorArea.left) && (game->mTouchData[touchCount].X < mColorArea.right) &&
                (game->mTouchData[touchCount].Y > mColorArea.top) && (game->mTouchData[touchCount].Y < mColorArea.bottom))
            return CMD_COLOR;

        // Timer
        if ((game->mTouchData[touchCount].X > mTimerArea.left) && (game->mTouchData[touchCount].X < mTimerArea.right) &&
                (game->mTouchData[touchCount].Y > mTimerArea.top) && (game->mTouchData[touchCount].Y < mTimerArea.bottom))
            return CMD_TIMER;
    }
    return CMD_NONE;
}

void Command::render() const {

    mPause2D.render(true);
    mPlay2D.render(true);
    mTimer2D.render(true);
    mTrash2D.render(true);

    mShift2D.render(true);
    mColor2D.render(true);
    mGradient2D.render(true);
    mCheck2D.render(true);
}
