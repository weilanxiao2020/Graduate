package com.miyako.graduate.module.staff;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.miyako.graduate.R;
import com.miyako.graduate.base.Constants;
import com.miyako.graduate.entity.StaffOrder;
import com.miyako.graduate.socket.SocketManager;
import com.miyako.graduate.socket.msg.BaseMsg;
import com.miyako.graduate.socket.msg.MissionMsg;
import com.miyako.graduate.socket.msg.OrderMsg;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

import io.reactivex.Observable;
import io.reactivex.ObservableEmitter;
import io.reactivex.ObservableOnSubscribe;
import io.reactivex.android.schedulers.AndroidSchedulers;

public class StaffManagerActivity extends AppCompatActivity {

    private static final String TAG = StaffManagerActivity.class.getSimpleName();

    private ImageView mIvStaff;
    private TextView mTvStaffId;
    private TextView mTvStaffCode;
    private TextView mTvStaffAddress;
    private ListView mListStaffOrder;

    private StaffManagerAdapter mAdapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_staff_manager);
        initView();
        initValue();
    }

    @Override
    protected void onResume() {
        super.onResume();
        getDatas();
    }

    List<StaffOrder> datas;

    private String license;
    private String code;
    private String address;

    private void initValue() {
        license = getIntent().getStringExtra(Constants.KEY_STAFF_LICENSE);
        code = getIntent().getStringExtra(Constants.KEY_STAFF_CODE);
        address = getIntent().getStringExtra(Constants.KEY_STAFF_ADDRESS);
        Log.d(TAG, "id:"+ license);
        Log.d(TAG, "code:"+code);

        mTvStaffId.setText(license);
        mTvStaffCode.setText(code);
        mTvStaffAddress.setText(address);
        mAdapter = new StaffManagerAdapter(this, new ArrayList<>());
        mListStaffOrder.setAdapter(mAdapter);
    }

    private void initView() {
        mTvStaffId = findViewById(R.id.tv_mission_license);
        mTvStaffCode = findViewById(R.id.tv_mission_code);
        mListStaffOrder = findViewById(R.id.list_order);
        mTvStaffAddress = findViewById(R.id.tv_mission_address);

    }

    private Observable<MissionMsg> getMission() {
        return Observable.create(new ObservableOnSubscribe<MissionMsg>() {
            @Override
            public void subscribe(ObservableEmitter<MissionMsg> observer) {
                SocketManager.getInstance().getMission("wlx-41647", new SocketManager.ISocketCall() {
                    @Override
                    public void success(List<BaseMsg> msg) {
                        Log.d(TAG, "msg: "+msg.get(0));
                        MissionMsg missionMsg = (MissionMsg) msg.get(0);
                        observer.onNext(missionMsg);
                        observer.onComplete();
//                        Observable.timer(0, TimeUnit.MILLISECONDS)
                        ////                                .observeOn(AndroidSchedulers.mainThread())
                        ////                                .subscribe(s->{
                        ////                                    mTvStaffId.setText(missionMsg.getMissionId().split("-")[1]);
                        ////                                    mTvStaffCode.setText(missionMsg.getMissionId().split("-")[0]);
                        ////                                    mTvStaffAddress.setText(missionMsg.getAddress());
                        ////                                });
                    }

                    @Override
                    public void error(int errorCode, List<BaseMsg> msg) {
                        Log.d(TAG, "error "+errorCode);
                        observer.onError(new Throwable(errorCode+"-"+msg));
//                        Observable.timer(0, TimeUnit.MILLISECONDS)
//                                .observeOn(AndroidSchedulers.mainThread())
//                                .subscribe(s->{
//                                    mTvStaffId.setText("none");
//                                    mTvStaffCode.setText("none");
//                                    mTvStaffAddress.setText("none");
//                                });
                    }
                });
            }
        });
    }

    private Observable<List<OrderMsg>> getOrderList() {
        return Observable.create(new ObservableOnSubscribe<List<OrderMsg>>() {
            @Override
            public void subscribe(ObservableEmitter<List<OrderMsg>> observer) {
                SocketManager.getInstance().getMission("wlx-41647", new SocketManager.ISocketCall() {
                    @Override
                    public void success(List<BaseMsg> msgList) {
                        Log.d(TAG, "msg: "+msgList);
                        List<OrderMsg> orderMsgs = new ArrayList<>(msgList.size());
                        for (BaseMsg msg:msgList) {
                            orderMsgs.add((OrderMsg) msg);
                        }
                        observer.onNext(orderMsgs);
                        observer.onComplete();
                        //                        Observable.timer(0, TimeUnit.MILLISECONDS)
                        ////                                .observeOn(AndroidSchedulers.mainThread())
                        ////                                .subscribe(s->{
                        ////                                    mTvStaffId.setText(missionMsg.getMissionId().split("-")[1]);
                        ////                                    mTvStaffCode.setText(missionMsg.getMissionId().split("-")[0]);
                        ////                                    mTvStaffAddress.setText(missionMsg.getAddress());
                        ////                                });
                    }

                    @Override
                    public void error(int errorCode, List<BaseMsg> msg) {
                        Log.d(TAG, "error "+errorCode);
                        observer.onError(new Throwable(errorCode+"-"+msg));
                        //                        Observable.timer(0, TimeUnit.MILLISECONDS)
                        //                                .observeOn(AndroidSchedulers.mainThread())
                        //                                .subscribe(s->{
                        //                                    mTvStaffId.setText("none");
                        //                                    mTvStaffCode.setText("none");
                        //                                    mTvStaffAddress.setText("none");
                        //                                });
                    }
                });
            }
        });
    }

    int length = 30;
    private void getDatas() {
        SocketManager.getInstance().getOrderByMission(license+"-"+code, new SocketManager.ISocketCall() {
            @Override
            public void success(List<BaseMsg> msg) {
                Observable.timer(0, TimeUnit.MILLISECONDS)
                        .observeOn(AndroidSchedulers.mainThread())
                        .subscribe(s->{
                            List<StaffOrder> orderList = new ArrayList<>(msg.size());
                            for(int i=0;i<msg.size();i++) {
                                OrderMsg orderMsg = (OrderMsg) msg.get(i);
                                StaffOrder order = new StaffOrder();
                                order.setId(orderMsg.getTrackId());
                                order.setAddress(orderMsg.getAddress());
                                order.setReceiver(orderMsg.getReceiver());
                                order.setDate(orderMsg.getTimestamp());
                                Log.d(TAG, order.toString());
                                orderList.add(order);
                            }
                            mAdapter.update(orderList);
                        });
            }

            @Override
            public void error(int errorCode, List<BaseMsg> msg) {

            }
        });
    }
}
