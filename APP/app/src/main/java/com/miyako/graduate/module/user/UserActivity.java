package com.miyako.graduate.module.user;

import android.support.v7.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.miyako.graduate.R;
import com.miyako.graduate.base.Constants;
import com.miyako.graduate.socket.SocketManager;
import com.miyako.graduate.socket.msg.BaseMsg;
import com.miyako.graduate.socket.msg.MissionMsg;
import com.miyako.graduate.socket.msg.OrderMsg;

import java.util.List;
import java.util.Objects;
import java.util.concurrent.TimeUnit;

import io.reactivex.Observable;
import io.reactivex.android.schedulers.AndroidSchedulers;

public class UserActivity extends AppCompatActivity {

    private static final String TAG = UserActivity.class.getSimpleName();

    private EditText mEtSearch;

    private Context mContext;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user);
        initView();
    }

    @SuppressLint("ClickableViewAccessibility")
    private void initView() {
        mEtSearch = findViewById(R.id.et_user_search);
        mEtSearch.setOnTouchListener(this::onTouch);
        // 对输入框设置输入法完成回调，返回上级界面，对带宽进行检验，回车默认隐藏键盘
        mEtSearch.setOnEditorActionListener(this::onEditorAction);
        mContext = this;

    }

    public static void hideSoft(Activity mActive) {
        View curFoc = mActive.getCurrentFocus();
        if (curFoc != null) {
            InputMethodManager imm = (InputMethodManager) mActive.getSystemService(Context.INPUT_METHOD_SERVICE);
            Objects.requireNonNull(imm).hideSoftInputFromWindow(curFoc.getWindowToken(), 0);
        }
    }

    private boolean onEditorAction(TextView textView, int actionId, KeyEvent keyEvent) {
        Log.d(TAG, "onEditorAction: "+actionId);
        if (actionId == EditorInfo.IME_ACTION_DONE) {
            Log.d(TAG, "按下回车键");
            // 需要将EditText设置为单行
            search(null);
            return true;
        }
        return false;
    }


    private boolean onTouch(View view, MotionEvent motionEvent) {
        // et.getCompoundDrawables()得到一个长度为4的数组，分别表示左右上下四张图片
        Drawable drawable = mEtSearch.getCompoundDrawables()[2];
        //如果右边没有图片，不再处理
        if (drawable == null)
            return false;
        //如果不是按下事件，不再处理
        if (motionEvent.getAction() != MotionEvent.ACTION_UP)
            return false;
        if (motionEvent.getX() > mEtSearch.getWidth()
                - mEtSearch.getPaddingRight()
                - drawable.getIntrinsicWidth()){
//            search();
            return true;
//            mEtSearch.setCompoundDrawablesWithIntrinsicBounds(0,0,0,0);
        }
        return false;
    }

    // 订单号
    // 64 xxx (0~41) xxxx xxxx
    public void search(View view) {
//        if (mEtSearch.length() < Constants.LENGTH_ORDER_ID) {
//            Toast.makeText(this, "请输入正确的运输单号(15位)", Toast.LENGTH_SHORT).show();
//            return;
//        }
        Log.d(TAG, "搜索单号"+mEtSearch.getText());
        hideSoft(this);
        SocketManager.getInstance().getOrderByTrack(mEtSearch.getText().toString(), new SocketManager.ISocketCall() {
            @Override
            public void success(List<BaseMsg> msg) {
                Log.d(TAG, "msg: "+msg.get(0));
                OrderMsg orderMsg = (OrderMsg) msg.get(0);
                Observable.timer(0, TimeUnit.MILLISECONDS)
                        .observeOn(AndroidSchedulers.mainThread())
                        .subscribe(s->{
                            Intent intent = new Intent(mContext, UserOrderActivity.class);
                            Log.d(TAG, "trackId:"+mEtSearch.getText());
                            intent.putExtra(Constants.KEY_ORDER_ID, mEtSearch.getText().toString());
                            intent.putExtra(Constants.KEY_ORDER_MISSION, orderMsg.getMissionId());
                            intent.putExtra(Constants.KEY_ORDER_RECEIVER, orderMsg.getReceiver());
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
