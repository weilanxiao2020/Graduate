package com.miyako.graduation.module;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.miyako.graduation.R;
import com.miyako.graduation.module.staff.StaffLoginActivity;
import com.miyako.graduation.module.user.UserActivity;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getSimpleName();

    private Button mBtnStaff;
    private Button mBtnUser;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
    }

    private void initView() {
        mBtnStaff = findViewById(R.id.btn_staff_login);
        mBtnUser = findViewById(R.id.btn_user);

        mBtnStaff.setOnClickListener(this::goStaff);
        mBtnUser.setOnClickListener(this::goUser);
    }

    private void goStaff(View view) {
        Log.d(TAG, "staff activity");
        startActivity(new Intent(this, StaffLoginActivity.class));
    }

    private void goUser(View view) {
        Log.d(TAG, "user activity");
        startActivity(new Intent(this, UserActivity.class));
    }
}
