package com.studio.artaban.kaleidoscope.data;

import com.studio.artaban.kaleidoscope.EngActivity;
import com.studio.artaban.kaleidoscope.EngAdvertising;
import com.studio.artaban.kaleidoscope.EngLibrary;
import com.studio.artaban.kaleidoscope.EngResources;

import android.util.Log;

public class EngData {

    ////// JNI
    static public final String PROJECT_NAME_LIB = "Kaleidoscope";

    ////// Languages
    static private final short LANG_EN = 0;
    @SuppressWarnings("unused") static private final short LANG_FR = 1;
    @SuppressWarnings("unused") static private final short LANG_DE = 2;
    @SuppressWarnings("unused") static private final short LANG_ES = 3;
    @SuppressWarnings("unused") static private final short LANG_IT = 4;
    @SuppressWarnings("unused") static private final short LANG_PT = 5;

    @SuppressWarnings("unused") static private final short mLanguage = LANG_EN;

    ////// Permissions
    static public final boolean USES_PERMISSION_CAMERA = true; // android.permission.CAMERA
    static public final boolean USES_PERMISSION_MIC = false; // android.permission.RECORD_AUDIO
    static public final boolean USES_PERMISSION_INTERNET = true; // android.permission.INTERNET & ACCESS_NETWORK_STATE
    static public final boolean USES_PERMISSION_STORAGE = true; // android.permission.WRITE_INTERNAL_STORAGE & WRITE_EXTERNAL_STORAGE
    static public final boolean USES_PERMISSION_BLUETOOTH = false; // android.permission.BLUETOOTH & BLUETOOTH_ADMIN

    ////// Font
    static public final boolean FONT_TEXTURE_GRAYSCALE = true;

    ////// Textures
    static private final short TEXTURE_ID_CRYSTAL = 2; // EngActivity.TEXTURE_ID_FONT + 1
    static private final short TEXTURE_ID_TITLE = 3;
    static private final short TEXTURE_ID_AVAILABLE = 4;
    static private final short TEXTURE_ID_STORES = 5;
    static private final short TEXTURE_ID_ENERGY = 6;

    static private final short TEXTURE_ID_CLOSE = 7;

    static private final short TEXTURE_ID_PUMPKIN = 8;
    static private final short TEXTURE_ID_BACK = 9;
    static private final short TEXTURE_ID_MOON = 10;
    static private final short TEXTURE_ID_SCENE = 11;
    static private final short TEXTURE_ID_TITLE_FWSP = 12;
    static private final short TEXTURE_ID_STORES_FWSP = 13;
    static private final short TEXTURE_ID_FACE = 14;

    static private final short TEXTURE_ID_WITCH0 = 15;
    static private final short TEXTURE_ID_WITCH1 = 16;
    static private final short TEXTURE_ID_WITCH2 = 17;
    static private final short TEXTURE_ID_WITCH3 = 18;
    static private final short TEXTURE_ID_WITCH4 = 19;
    static private final short TEXTURE_ID_WITCH5 = 20;
    static private final short TEXTURE_ID_WITCH6 = 21;
    static private final short TEXTURE_ID_WITCH7 = 22;

    static private final short TEXTURE_ID_COMMAND = 23;
    static private final short TEXTURE_ID_FLASH = 24;
    static private final short TEXTURE_ID_PALETTE = 25;
    static private final short TEXTURE_ID_TITLE_KALEIDO = 26;
    static private final short TEXTURE_ID_PANEL = 27;

    static public short loadTexture(EngResources resources, short id) {

        switch (id) {
            case TEXTURE_ID_CRYSTAL: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("crystal.png");
                return EngLibrary.loadTexture(TEXTURE_ID_CRYSTAL, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_TITLE: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("titleCB.png");
                return EngLibrary.loadTexture(TEXTURE_ID_TITLE, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_AVAILABLE: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("available.png");
                return EngLibrary.loadTexture(TEXTURE_ID_AVAILABLE, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
            }
            case TEXTURE_ID_STORES: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("stores.png");
                return EngLibrary.loadTexture(TEXTURE_ID_STORES, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_ENERGY: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("energy.png");
                return EngLibrary.loadTexture(TEXTURE_ID_ENERGY, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
            }

            case TEXTURE_ID_CLOSE: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("close.png");
                return EngLibrary.loadTexture(TEXTURE_ID_CLOSE, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
            }

            case TEXTURE_ID_PUMPKIN: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("pumpkin.png");
                return EngLibrary.loadTexture(TEXTURE_ID_PUMPKIN, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_BACK: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("back.png");
                return EngLibrary.loadTexture(TEXTURE_ID_BACK, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
            }
            case TEXTURE_ID_MOON: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("moon.png");
                return EngLibrary.loadTexture(TEXTURE_ID_MOON, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
            }
            case TEXTURE_ID_SCENE: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("scene.png");
                return EngLibrary.loadTexture(TEXTURE_ID_SCENE, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_TITLE_FWSP: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("title.png");
                return EngLibrary.loadTexture(TEXTURE_ID_TITLE_FWSP, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_STORES_FWSP: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("storesFWSP.png");
                return EngLibrary.loadTexture(TEXTURE_ID_STORES_FWSP, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_FACE: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("face.png");
                return EngLibrary.loadTexture(TEXTURE_ID_FACE, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_WITCH0:
            case TEXTURE_ID_WITCH1:
            case TEXTURE_ID_WITCH2:
            case TEXTURE_ID_WITCH3:
            case TEXTURE_ID_WITCH4:
            case TEXTURE_ID_WITCH5:
            case TEXTURE_ID_WITCH6:
            case TEXTURE_ID_WITCH7: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("witch-" + (id - TEXTURE_ID_WITCH0) + ".png");
                return EngLibrary.loadTexture(id, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }

            case TEXTURE_ID_COMMAND: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("cmd.png");
                return EngLibrary.loadTexture(TEXTURE_ID_COMMAND, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
            }
            case TEXTURE_ID_FLASH: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("flash.png");
                return EngLibrary.loadTexture(TEXTURE_ID_FLASH, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_PALETTE: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("palette.png");
                return EngLibrary.loadTexture(TEXTURE_ID_PALETTE, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_TITLE_KALEIDO: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("titleKAL.png");
                return EngLibrary.loadTexture(TEXTURE_ID_TITLE_KALEIDO, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }
            case TEXTURE_ID_PANEL: {
                EngResources.BmpInfo bmpInfo = resources.getBufferPNG("panel.png");
                return EngLibrary.loadTexture(TEXTURE_ID_PANEL, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
            }

            default: {
                Log.e("EngData", "Failed to load PNG ID: " + Integer.toString(id));
                return EngActivity.NO_TEXTURE_LOADED;
            }
        }
    }

    ////// Sounds
    static private final short SOUND_ID_INTRO = 1; // EngActivity.SOUND_ID_LOGO + 1
    static private final short SOUND_ID_FLAP = 2;
    static private final short SOUND_ID_BEEP = 3;
    static private final short SOUND_ID_FLASH = 4;

    static public void loadOgg(EngResources resources, short id) {

        switch (id) {
            case SOUND_ID_INTRO: {
                EngLibrary.loadOgg(SOUND_ID_INTRO, resources.getBufferOGG("introCB.ogg"), true);
                break;
            }
            case SOUND_ID_FLAP: {
                EngLibrary.loadOgg(SOUND_ID_FLAP, resources.getBufferOGG("flap.ogg"), false);
                break;
            }
            case SOUND_ID_BEEP: {
                EngLibrary.loadOgg(SOUND_ID_BEEP, resources.getBufferOGG("beep.ogg"), false);
                break;
            }
            case SOUND_ID_FLASH: {
                EngLibrary.loadOgg(SOUND_ID_FLASH, resources.getBufferOGG("flash.ogg"), false);
                break;
            }

            default: {
                Log.e("EngData", "Failed to load OGG ID: " + Integer.toString(id));
                break;
            }
        }
    }

    ////// Advertising
    static public final boolean INTERSTITIAL_AD = true; // TRUE: InterstitialAd; FALSE: AdView
    static private final boolean TEST_ADVERTISING = false; // Set to 'false' in release mode
    static private final String ADV_UNIT_ID = "ca-app-pub-1474300545363558/4573209427";

    static public void loadAd(EngActivity activity) {

    	if (!USES_PERMISSION_INTERNET)
    		Log.e("EngData", "Missing INTERNET & ACCESS_NETWORK_STATE permissions");

    	//
		EngAdvertising.setUnitID(ADV_UNIT_ID);
		if (TEST_ADVERTISING)
			EngAdvertising.load(activity.getContentResolver());
		else
			EngAdvertising.load(null);
    }
    static public void displayAd(short id, EngActivity activity) { EngAdvertising.display(); }
    static public void hideAd(short id, EngActivity activity) { }

}
