package cn.micker.jnisafeimage;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

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
}
