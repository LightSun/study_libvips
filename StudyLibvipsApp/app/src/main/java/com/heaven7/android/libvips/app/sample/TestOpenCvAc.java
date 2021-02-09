package com.heaven7.android.libvips.app.sample;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Environment;
import android.widget.ImageView;

import com.heaven7.android.libvips.app.BaseActivity;
import com.heaven7.android.libvips.app.LibOpencv;
import com.heaven7.android.libvips.app.R;
import com.heaven7.core.util.Logger;
import com.heaven7.java.pc.schedulers.Schedulers;

import org.opencv.android.InstallCallbackInterface;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

import butterknife.BindView;
import butterknife.OnClick;

import static org.opencv.core.CvType.typeToString;

public class TestOpenCvAc extends BaseActivity {

    private static final String TAG = "TestOpenCvAc";

    @BindView(R.id.iv1)
    ImageView iv1;

    @Override
    protected int getLayoutId() {
        return R.layout.ac_test_cv;
    }
    @Override
    protected void init(BaseActivity ac) {
        LibOpencv.init();
    }

    @OnClick(R.id.bt_draw_base)
    public void onClickBaseDraw(){
        final String in = Environment.getExternalStorageDirectory() + "/temp/test_out_circle.png";
        final String out = Environment.getExternalStorageDirectory() + "/temp/test_out_cv1.png";
        //String in = Environment.getExternalStorageDirectory() + "/DCIM/Camera/IMG_20200308_143039.jpg";
        Schedulers.io().newWorker().schedule(new Runnable() {
            @Override
            public void run() {
                //Imgproc.d
                Mat mat = Imgcodecs.imread(in);
                Logger.d(TAG, "onClickBaseDraw","channels = " + mat.channels()
                        + " ,type = " + typeToString(mat.type())); //8UC3 : 8位 unsigned .3 channel

                Point p1 = new Point(200, 300);
                Point p2 = new Point(400, 300);
                Scalar color = new Scalar(0, 0, 255);
                Imgproc.line(mat, p1, p2, color, 5, 8, 0);

                Imgproc.circle(mat, p1, 30,  new Scalar(255, 0, 0), 10);

                //mat.convertTo(mat, CvType.CV_32SC4);

                Mat result = new Mat();
                Imgproc.cvtColor(mat, result, Imgproc.COLOR_BGR2BGRA);
                System.out.println("cvtColor ok");
                //result.get
                ArrayList<Mat> list = new ArrayList<>();
                Core.split(result, list);
                System.out.println("split ok");
                Collections.reverse(list);
                Core.merge(list, result);
                System.out.println("merge ok");
                Imgcodecs.imwrite(out, result);

                Size size = result.size();
                System.out.println("size = " + size);
                int w = (int) size.width;
                int h = (int) size.height;
                Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
                for (int ih = 0 ; ih < h ; ih ++){
                    for (int iw = 0 ; iw < w ; iw ++){
                        double[] a = result.get(ih, iw);
                        System.out.println("w_idx = " + iw + ", h_idx = " + ih + "\n" + Arrays.toString(a));
                        bitmap.setPixel(iw, ih, Color.argb((int)a[0], (int)a[1], (int)a[2], (int)a[3]));
                    }
                }
                System.out.println("set pixes ok");
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        iv1.setImageBitmap(bitmap);
                    }
                });
            }
        });
    }
}
