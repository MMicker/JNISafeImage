package cn.micker.jnisafeimage;

import android.content.Context;
import android.graphics.Bitmap;

/**
 * Created by micker on 2017/10/9.
 */

public class JNISafe {
    static
    {
        System.loadLibrary("JNISafe");
    }

    public native static Bitmap saveTextsToImage(Context context, String[] texts);

    public native static String[] getDataFromImage(Context context);

    public native static String getSafeImageName();

}
