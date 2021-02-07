package com.heaven7.android.libvips.app.sample;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.ImageView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.heaven7.android.libvips.app.Libvips;
import com.heaven7.android.libvips.app.R;
import com.heaven7.core.util.Logger;
import com.heaven7.java.base.util.IOUtils;
import com.heaven7.java.pc.schedulers.Schedulers;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;

public class TestLibvipsAc extends AppCompatActivity {
    private static final String TAG = "TestLibvipsAc";

    ImageView iv1;
    ImageView iv2;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ac_test_libvips);
        iv1 = findViewById(R.id.iv1);
        iv2 = findViewById(R.id.iv2);
    }

    public void onClickTest1(View view) {
        String in = Environment.getExternalStorageDirectory() + "/DCIM/Camera/IMG_20200308_143039.jpg";
       // String out = Environment.getExternalStorageDirectory() + "/temp/test_out.png";
        String out = Environment.getExternalStorageDirectory() + "/temp/test_out_circle.png";
        Schedulers.io().newWorker().schedule(new Runnable() {
            @Override
            public void run() {
                doTest1(in, out);
            }
        });
    }

    private void doTest1(String in, String out){
        Logger.d(TAG, "doTest1", "test start -------");
       // String in = Environment.getExternalStorageDirectory() + "/DCIM/Camera/IMG_20200308_143039.jpg";
        Bitmap bitmap = BitmapFactory.decodeFile(in);
        if(in.endsWith(".jpeg") || in.endsWith(".jpg")){
            String in2 = Environment.getExternalStorageDirectory() + "/temp/test_in.png";
            File fileIn2 = new File(in2);
            if(fileIn2.exists()){
                fileIn2.delete();
            }
            FileOutputStream outStream = null;
            try {
                outStream = new FileOutputStream(in2);
                bitmap.compress(Bitmap.CompressFormat.PNG, 100,  outStream);
                outStream.flush();
                in = in2;
            } catch (Exception e) {
                e.printStackTrace();
                return;
            }finally {
                IOUtils.closeQuietly(outStream);
            }
        }

        //String out = Environment.getExternalStorageDirectory() + "/temp/test_out.png";
        if(!new File(in).exists()){
            System.err.println("file not exit. file = " + in);
            return;
        }
        new File(out).delete();
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                iv1.setImageBitmap(bitmap);
            }
        });
        Logger.d(TAG , "doTest1", "start native");
        boolean result = Libvips.test1(in, out);
        if(result){
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    iv2.setImageBitmap(BitmapFactory.decodeFile(out));
                }
            });
        }else {
            Logger.d(TAG, "doTest1", "test failed");
        }
        Logger.d(TAG, "doTest1", "test end --------");
    }
}
