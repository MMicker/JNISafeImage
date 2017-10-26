package cn.micker.jnisafeimage;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button saveButton = (Button)findViewById(R.id.button);
        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                TextView textView1 = (TextView) findViewById(R.id.textView1);
                TextView textView2 = (TextView) findViewById(R.id.textView2);
                String[] texts = {(String) textView1.getText(),(String) textView2.getText()};
                Bitmap bitmap = JNISafe.saveTextsToImage(MainActivity.this,texts);
                File pictureFolder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getAbsoluteFile();
                File appDir = new File(pictureFolder ,"micker_safe");
                if (!appDir.exists()) {
                    appDir.mkdirs();
                }
                File file = new File(appDir, "jjjjffff.png");
                try {
                    FileOutputStream fileOutputStream = new FileOutputStream(file);
                    bitmap.compress(Bitmap.CompressFormat.PNG, 100, fileOutputStream);
                    fileOutputStream.flush();
                    fileOutputStream.close();

                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }

                ImageView imageView = (ImageView) findViewById(R.id.image);
                imageView.setImageBitmap(bitmap);
            }
        });

        Button readButton = (Button)findViewById(R.id.buttonr);
        readButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String[] a = JNISafe.getDataFromImage(MainActivity.this);
                TextView textView1 = (TextView)findViewById(R.id.textView);
                if (a != null) {

                    StringBuffer buffer = new StringBuffer();
                    for (String value:a) {
                        buffer.append(value);
                        buffer.append("\n");
                    }
                    textView1.setText(buffer.toString());
                } else {
                    textView1.setText("数据未通过检验或非官方应用");
                }
            }
        });

        {
            ImageView image22 = (ImageView)findViewById(R.id.image22);
            Bitmap bitmap = drawBitmap();
            savePic(bitmap,"aa");
            image22.setImageBitmap(bitmap);
        }
    }

    public  String getSignature(Context context)
    {
        try {
            /** 通过包管理器获得指定包名包含签名的包信息 **/
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
            /******* 通过返回的包信息获得签名数组 *******/
            Signature[] signatures = packageInfo.signatures;
            /******* 循环遍历签名数组拼接应用签名 *******/
            return signatures[0].toCharsString();
            /************** 得到应用签名 **************/
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }
    private Bitmap scaleBitmap(Bitmap origin, int newWidth, int newHeight) {
        if (origin == null) {
            return null;
        }
        int height = origin.getHeight();
        int width = origin.getWidth();

        Bitmap cropBitmap = origin;
        float ratio = newHeight * 1.0f/newWidth;
        float oRatio = height * 1.0f /width;
        if (oRatio < ratio ) {
            int cropX = width - (int)(height * 1.0f/ratio) ;
            origin = Bitmap.createBitmap(origin, cropX/2, 0, width - cropX, height, null, false);
            width = width - cropX;
        } else if (oRatio > ratio) {
            int cropY = height - (int)(width * 1.0f * ratio/2);
            origin = Bitmap.createBitmap(origin, 0, cropY/2, width, height - cropY, null, false);
            height = height-cropY;
        }

        float scaleWidth = ((float) newWidth) / width;
        float scaleHeight = ((float) newHeight) / height;
        Matrix matrix = new Matrix();
        matrix.postScale(scaleWidth, scaleHeight);
        Bitmap newBM = Bitmap.createBitmap(origin, 0, 0, width, height, matrix, false);
        if (!origin.isRecycled()) {
            origin.recycle();
        }
        return newBM;
    }

    private Bitmap doGetBitMap(int position) {
        InputStream fis = null;
        String[] images = {"b.png","jf.png","jjjjffff.png","a.png"};
        try {
            fis = getAssets().open(images[position]);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        Bitmap bitmap = BitmapFactory.decodeStream(fis);
        return bitmap;
    }

    private Bitmap drawBitmap() {
        Bitmap bitmap0 = doGetBitMap(0);
        Bitmap bitmap1 = doGetBitMap(1);
        Bitmap bitmap2 = doGetBitMap(2);
        Bitmap bitmap3 = doGetBitMap(3);

        bitmap3 = scaleBitmap(bitmap3,bitmap0.getWidth(),bitmap0.getHeight());
        bitmap1 = scaleBitmap(bitmap1,bitmap0.getWidth()/2-5,bitmap0.getHeight()/2);
        bitmap2 = scaleBitmap(bitmap2,bitmap1.getWidth(),bitmap1.getHeight());

        int padding = 10;
        Bitmap screenshot = Bitmap.createBitmap(
                bitmap0.getWidth() * 2 + 3 * padding ,
                bitmap0.getHeight() + bitmap1.getHeight()  + 3 * padding,
                Bitmap.Config.ARGB_8888);

        Canvas canvas = new Canvas(screenshot);
        canvas.drawRGB(255,255,255);

        canvas.drawBitmap(bitmap0,padding * 1, padding,null);
        canvas.drawBitmap(bitmap3,padding * 2 + bitmap0.getWidth(), padding,null);
        canvas.drawBitmap(bitmap1,padding *2  + bitmap0.getWidth() , padding * 2 + bitmap0.getHeight(),null);
        canvas.drawBitmap(bitmap2,(int)(padding * 2.5) + bitmap0.getWidth() + bitmap0.getWidth()/2 , padding * 2 + bitmap0.getHeight(),null);

        String testString = "这个文本不是垂直居中，往下偏了一点，我前面说了y不是这个字符中心在屏幕上的位置，而是baseline在屏幕上的位置，而我们的代码c";

        TextPaint textPaint = new TextPaint();
        textPaint.setColor(Color.BLACK);
        textPaint.setTextSize(20.0F);
        textPaint.setAntiAlias(true);
        StaticLayout layout = new StaticLayout(testString, textPaint, bitmap0.getWidth() - 2 * padding, Layout.Alignment.ALIGN_NORMAL, 1.0F, 0.0F, true);
        canvas.save();
        canvas.translate(padding * 2, padding * 3 + bitmap0.getHeight());
        layout.draw(canvas);
        canvas.restore();

        Paint paint = new Paint();
        paint.setColor(Color.LTGRAY);
        paint.setStrokeWidth(2);
        paint.setStyle(Paint.Style.STROKE);
        paint.setAntiAlias(true);
        canvas.drawRect(
                padding,
                padding * 2 + bitmap0.getHeight(),
                padding + bitmap0.getWidth(),
                padding * 2 + bitmap0.getHeight() + bitmap1.getHeight(),
                paint);
        return screenshot;

    }


    public  DisplayMetrics getDisplayMetrics()
    {
        WindowManager wm = (WindowManager) this.getSystemService(
                Context.WINDOW_SERVICE);
        DisplayMetrics metrics= new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(metrics);

        return metrics;
    }

    public  float getScreenHeight()
    {
        return getDisplayMetrics().heightPixels;
    }

    public  float getScreenWidth()
    {
        return getDisplayMetrics().widthPixels;
    }

    private static Bitmap convertViewToBitmap(View v) {
        try {
            v.measure(View.MeasureSpec.makeMeasureSpec(1000 , View.MeasureSpec.EXACTLY),
                    View.MeasureSpec.makeMeasureSpec(1000, View.MeasureSpec.UNSPECIFIED));
            v.layout(0, 0, v.getMeasuredWidth(), v.getMeasuredHeight());
            Bitmap screenshot = Bitmap.createBitmap(v.getWidth(), v.getHeight(), Bitmap.Config.ARGB_8888);
            v.draw(new Canvas(screenshot));
            return screenshot;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private void savePic(Bitmap b, String url) {


        final String dir = Environment.getExternalStorageDirectory() + "/jnisafe/";
        File fileDir = new File(dir);
        if (!fileDir.exists())
        {
            fileDir.mkdirs();
        }

        final String saveName = "pic" +  "1.jpg";
        final File file = new File(dir, saveName);
        if (file.exists()) {
            file.delete();
        }

        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(file);
            if (null != fos) {
                b.compress(Bitmap.CompressFormat.JPEG, 90, fos);
                fos.flush();
                fos.close();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}
