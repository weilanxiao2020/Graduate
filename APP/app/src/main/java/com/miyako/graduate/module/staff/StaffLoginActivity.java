package com.miyako.graduate.module.staff;

import android.support.v7.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.miyako.graduate.R;
import com.miyako.graduate.base.Constants;
import com.miyako.graduate.socket.SocketManager;
import com.miyako.graduate.socket.msg.BaseMsg;
import com.miyako.graduate.socket.msg.MissionMsg;

import java.util.List;
import java.util.concurrent.TimeUnit;

import io.reactivex.Observable;
import io.reactivex.android.schedulers.AndroidSchedulers;


public class StaffLoginActivity extends AppCompatActivity {

    private static final String TAG = StaffLoginActivity.class.getSimpleName();

    private EditText mEtStaffId;
    private EditText mEtStaffCode;
    private Button mBtnStaffLogin;

    private Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_staff);
        initView();
    }

    private void initView() {
        mContext = this;
        mEtStaffId = findViewById(R.id.et_staff_id);
        mEtStaffCode = findViewById(R.id.et_staff_code);
        mBtnStaffLogin = findViewById(R.id.btn_staff_login);
        mBtnStaffLogin.setOnClickListener(this::login);
        mEtStaffId.setText("1234567890");
        mEtStaffCode.setText("167943");
    }

    private void login(View view) {
//        if(mEtStaffId.length() < Constants.LENGTH_STAFF_ID || mEtStaffCode.length() < Constants.LENGTH_STAFF_CODE) {
//            Toast.makeText(this, "请输入正确的工号或代码", Toast.LENGTH_SHORT).show();
//            return;
//        }
        Log.d(TAG, "login=>"+mEtStaffId.getText()+","+mEtStaffCode.getText());

        SocketManager.getInstance().getMission(mEtStaffId.getText()+"-"+mEtStaffCode.getText(), new SocketManager.ISocketCall() {
            @Override
            public void success(List<BaseMsg> msg) {
                Log.d(TAG, "msg: "+msg.get(0));
                MissionMsg missionMsg = (MissionMsg) msg.get(0);
                Observable.timer(0, TimeUnit.MILLISECONDS)
                        .observeOn(AndroidSchedulers.mainThread())
                        .subscribe(s->{
                            Intent intent = new Intent(mContext, StaffManagerActivity.class);
                            Log.d(TAG, "license:"+mEtStaffId.getText().toString());
                            Log.d(TAG, "code:"+mEtStaffCode.getText().toString());
                            intent.putExtra(Constants.KEY_STAFF_LICENSE, mEtStaffId.getText().toString());
                            intent.putExtra(Constants.KEY_STAFF_CODE, mEtStaffCode.getText().toString());
                            intent.putExtra(Constants.KEY_STAFF_ADDRESS, missionMsg.getAddress());
                            startActivity(intent);
                        });
            }

            @Override
            public void error(int errorCode, List<BaseMsg> msg) {
                Log.d(TAG, "error "+errorCode);
                Observable.timer(0,TimeUnit.MILLISECONDS)
                        .observeOn(AndroidSchedulers.mainThread())
                        .subscribe(s->{
                            Toast.makeText(mContext, "请输入正确的工号或代码", Toast.LENGTH_SHORT).show();
                        });
            }
        });
    }
}
