package com.heaven7.android.libvips.app;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.heaven7.adapter.BaseSelector;
import com.heaven7.adapter.QuickRecycleViewAdapter;
import com.heaven7.adapter.util.ViewHelper2;
import com.heaven7.core.util.PermissionHelper;

import java.util.ArrayList;
import java.util.List;

public abstract class AbsMainActivity extends AppCompatActivity {

    RecyclerView mRv;
    protected final List<ActivityInfo> mInfos = new ArrayList<>();

    private final PermissionHelper mHelper = new PermissionHelper(this);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.ac_asb_main);
        mRv = findViewById(R.id.rv);
        mRv.setLayoutManager(new LinearLayoutManager(this));
        addDemos(mInfos);
        mRv.setAdapter(new QuickRecycleViewAdapter<ActivityInfo>(android.R.layout.simple_list_item_1, mInfos) {
            @Override
            protected void onBindData(Context context, int position, final ActivityInfo item, int itemLayoutId, ViewHelper2 helper) {
                helper.setText(android.R.id.text1, item.desc)
                        .setRootOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                onClickItem(item);
                            }
                        });
            }
        });

        mHelper.startRequestPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, 1, new PermissionHelper.ICallback() {
            @Override
            public void onRequestPermissionResult(String s, int i, boolean b) {
                System.out.println("WRITE_EXTERNAL_STORAGE = " + b);
            }
            @Override
            public boolean handlePermissionHadRefused(String s, int i, Runnable runnable) {
                return false;
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        mHelper.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    protected abstract void addDemos(List<ActivityInfo> list);


    protected void onClickItem(ActivityInfo info){
        startActivity(new Intent(AbsMainActivity.this, info.clazz));
    }

    public static class ActivityInfo extends BaseSelector {

        final String desc;
        final Class<? extends Activity> clazz;

        public ActivityInfo(Class<? extends Activity> clazz) {
            this.clazz = clazz;
            this.desc = clazz.getSimpleName();
        }
        public ActivityInfo(Class<? extends Activity> clazz, String desc) {
            this.clazz = clazz;
            this.desc = desc;
        }
        public ActivityInfo(Context context, Class<? extends Activity> clazz, int stringResId) {
            this.clazz = clazz;
            this.desc = context.getResources().getString(stringResId);
        }
    }
}
