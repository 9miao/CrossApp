
package org.CrossApp.lib;

import java.util.HashMap;

import android.content.Context;
import android.graphics.Typeface;

public class Cocos2dxTypefaces {
	// ===========================================================
	// Constants
	// ===========================================================

	// ===========================================================
	// Fields
	// ===========================================================

	private static final HashMap<String, Typeface> sTypefaceCache = new HashMap<String, Typeface>();

	// ===========================================================
	// Constructors
	// ===========================================================

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	// ===========================================================
	// Methods
	// ===========================================================

	public static synchronized Typeface get(final Context pContext, final String pAssetName) {
		if (!Cocos2dxTypefaces.sTypefaceCache.containsKey(pAssetName)) {
			Typeface typeface = null;
			if (pAssetName.startsWith("/"))
			{
				typeface = Typeface.createFromFile(pAssetName);
			}
			else
			{
				typeface = Typeface.createFromAsset(pContext.getAssets(), pAssetName);
			}
			Cocos2dxTypefaces.sTypefaceCache.put(pAssetName, typeface);
		}

		return Cocos2dxTypefaces.sTypefaceCache.get(pAssetName);
	}

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================

}
