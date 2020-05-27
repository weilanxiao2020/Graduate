package com.miyako.graduate.module.user;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.TextView;

import com.amap.api.maps.AMapUtils;
import com.amap.api.maps.CoordinateConverter;
import com.amap.api.maps.MapView;
import com.amap.api.maps.model.LatLng;
import com.miyako.graduate.R;
import com.miyako.graduate.base.Constants;
import com.miyako.graduate.entity.GpsOrder;
import com.miyako.graduate.socket.SocketManager;
import com.miyako.graduate.socket.msg.BaseMsg;
import com.miyako.graduate.socket.msg.GpsMsg;
import com.miyako.graduate.utils.MapUtil;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.TimeUnit;

import io.reactivex.Observable;
import io.reactivex.Scheduler;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.Disposable;
import io.reactivex.internal.schedulers.IoScheduler;
import io.reactivex.schedulers.Schedulers;

public class UserOrderActivity extends AppCompatActivity {


    private static final String TAG = UserOrderActivity.class.getSimpleName();

    private TextView mTvOrderId;
    private TextView mTvOrderReceiver;
    private TextView mTvOrderDistance;
    private MapView mMapView;
    private ListView mListOrderGps;

    private String orderId;
    private String receiver;
    private String mission;
    private UserOrderAdapter mAdapter;
    // 绘制点
    private LinkedList<LatLng> points;
    private LinkedList<LatLng> showDatas;

    private boolean flag;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate...");
        setContentView(R.layout.activity_user_order);
        initView(savedInstanceState);
        initValue();
        MapUtil.getInstance()
                .setContext(this)
                .init(mMapView.getMap())
                .location()
                .setLocationListener(this::getLocation)
                .changedLocation(true);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        //在activity执行onDestroy时执行mMapView.onDestroy()，销毁地图
        Log.d(TAG, "onDestroy...");
        if (subscribe != null) {
            subscribe.dispose();
        }
        mMapView.onDestroy();
        MapUtil.getInstance().cancelLocation();
    }
    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume...");
        //在activity执行onResume时执行mMapView.onResume ()，重新绘制加载地图
        mMapView.onResume();
        getGpsLast();
        showList(null);

    }
    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "onPause...");
        // TODO Auto-generated method stub
        //在activity执行onPause时执行mMapView.onPause ()，暂停地图的绘制
        mMapView.onPause();
        //mLocationClient.stopLocation();//停止定位
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        //在activity执行onSaveInstanceState时执行mMapView.onSaveInstanceState (outState)，保存地图当前的状态
        mMapView.onSaveInstanceState(outState);
    }

    private void initView(Bundle savedInstanceState) {
        Log.d(TAG, "init view");
        mTvOrderId = findViewById(R.id.tv_title_order_id);
        mTvOrderReceiver = findViewById(R.id.tv_title_order_receiver);
        mTvOrderDistance = findViewById(R.id.tv_title_order_distance);
        mMapView = findViewById(R.id.map_order_record);
        mListOrderGps = findViewById(R.id.lv_order_record);
        mMapView.onCreate(savedInstanceState);
        displayMap();
        mAdapter = new UserOrderAdapter(this, new ArrayList<>());
        mListOrderGps.setAdapter(mAdapter);
    }

    private void initValue() {
        orderId = getIntent().getStringExtra(Constants.KEY_ORDER_ID);
        mission = getIntent().getStringExtra(Constants.KEY_ORDER_MISSION);
        receiver = getIntent().getStringExtra(Constants.KEY_ORDER_RECEIVER);
        mTvOrderId.setText(orderId);
        mTvOrderReceiver.setText(receiver);
        flag = false;
    }

    // 显示地图
    private void displayMap() {
        if (mMapView.getVisibility() == View.VISIBLE) {
            Log.d(TAG, "display map");
            MapUtil.getInstance()
                    .display();
        }
    }

    private void startMap() {
        mMapView.setVisibility(View.VISIBLE);
        mListOrderGps.setVisibility(View.GONE);
        MapUtil.getInstance().changedLocation(true);
        displayMap();
    }

    private void stopMap() {
        mMapView.setVisibility(View.GONE);
        mListOrderGps.setVisibility(View.VISIBLE);
//        MapUtil.getInstance().changedLocation(false);
        isShow = false;
    }

    int inter = 10;
    private Disposable subscribe;
    private void getGpsLast() {
        subscribe = Observable.interval(0, inter, TimeUnit.SECONDS)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(s -> {
                    if (flag) {
                        SocketManager.getInstance()
                                .getOrderGpsLast(mission, new SocketManager.ISocketCall() {
                                    @Override
                                    public void success(List<BaseMsg> msg) {
                                        Log.d(TAG, "msg: " + msg.get(0));
                                        GpsMsg last = (GpsMsg) msg.get(0);
                                        GpsOrder order = convert(last);
                                        long utcTime = mAdapter.getItem(0).getUtcTime();
                                        Log.d(TAG, "上次gps时间:" + utcTime);
                                        Log.d(TAG, "本次gps时间:" + order.getUtcTime());
                                        LatLng latLng = convertLatLng(order);
                                        mLastLatLng = latLng;
                                        if (order.getUtcTime() > utcTime) {
                                            showDatas.addFirst(latLng);
                                            mAdapter.update(order);
                                            if (mMapView.getVisibility() == View.VISIBLE) {
                                                points.addLast(latLng);
                                                runOnUiThread(new Runnable() {
                                                    @Override
                                                    public void run() {
                                                        MapUtil.getInstance().drawPath(latLng);
                                                    }
                                                });
                                            }
                                        }
                                        flag = true;
                                    }

                                    @Override
                                    public void error(int errorCode, List<BaseMsg> msg) {

                                    }
                                });
                    }
                });
    }

    private LatLng mLastLatLng;

    private void showDistance(double distance) {
        String format = String.format(distance > 1000.0 ? "%.2fkm" : "%.0fm", distance > 1000.0 ? distance / 1000:distance);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mTvOrderDistance.setText(getString(R.string.tv_user_order_distance, format));
            }
        });
    }

    // 定位回调
    public void getLocation(LatLng latLng) {
        Log.d(TAG, "get user location:"+latLng);
        Log.d(TAG, "get order last location:"+mLastLatLng);
        float distance = AMapUtils.calculateLineDistance(latLng, mLastLatLng);
        Log.d(TAG, "直线距离:"+distance);
        showDistance(distance);
    }

    private LatLng convertLatLng(GpsOrder order) {
        LatLng source = new LatLng(Double.valueOf(order.getLatitude()), Double.valueOf(order.getLongitude()));
        // 执行转换操作
        return MapUtil.getInstance().convert(source);
    }

    private GpsOrder convert(GpsMsg gpsMsg) {
        GpsOrder order = new GpsOrder();
        order.setId(gpsMsg.getId());
        order.setUtcTime(Long.valueOf(gpsMsg.getUtcTime()));
        order.setLatitude(gpsMsg.getLatitude());
        order.setN_S(gpsMsg.getN_S());
        order.setLongitude(gpsMsg.getLongitude());
        order.setE_W(gpsMsg.getE_W());
        order.setRegion(gpsMsg.getRegion());
        Log.d(TAG, order.toString());
        return order;
    }

    public void showList(View view) {
        Log.d(TAG, "show list");
        stopMap();
        Observable.timer(0, TimeUnit.MILLISECONDS)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(s->{
                    SocketManager.getInstance().getOrderGpsRegion(mission, new SocketManager.ISocketCall() {
                        @Override
                        public void success(List<BaseMsg> msg) {
                            Log.d(TAG, "msg: "+msg.get(0));
                            int size = msg.size();
                            List<GpsOrder> orderList = new ArrayList<>(size);
                            if (showDatas == null) {
                                showDatas = new LinkedList<>();
                            }
                            for(int i=0;i<size;i++) {
                                GpsMsg gpsMsg = (GpsMsg) msg.get(i);
                                GpsOrder order = convert(gpsMsg);
                                orderList.add(order);
                                // 添加到头部
                                showDatas.addFirst(convertLatLng(order));
                            }
                            Observable.timer(0, TimeUnit.MILLISECONDS)
                                    .observeOn(AndroidSchedulers.mainThread())
                                    .subscribe(s->{
                                        mAdapter.update(orderList);
                                        mListOrderGps.setVisibility(View.VISIBLE);
                                        flag = true;
                                    });
                        }

                        @Override
                        public void error(int errorCode, List<BaseMsg> msg) {

                        }
                    });
                });

    }

    private boolean isShow = false;

    public void showMap(View view) {
        Log.d(TAG, "show map");
        if(isShow) {
            return;
        }
        Observable.timer(0, TimeUnit.MILLISECONDS)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(s->{
                    SocketManager.getInstance().getOrderGpsAll(mission, new SocketManager.ISocketCall() {
                        @Override
                        public void success(List<BaseMsg> msg) {
                            Log.d(TAG, "msg: "+msg.get(0));
                            int size = msg.size();
                            List<GpsOrder> orderList = new ArrayList<>(size);
                            points = null;
                            points = new LinkedList<>();
                            for(int i=0;i<size;i++) {
                                GpsMsg gpsMsg = (GpsMsg) msg.get(i);
                                GpsOrder order = convert(gpsMsg);
                                orderList.add(order);
                                // 添加到尾部
                                points.addLast(convertLatLng(order));
                            }
                            mListOrderGps.setVisibility(View.GONE);
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    MapUtil.getInstance()
                                            .setDatas(points)
                                            .drawPath();
                                }
                            });
                        }

                        @Override
                        public void error(int errorCode, List<BaseMsg> msg) {

                        }
                    });
                });
        startMap();
        isShow = true;
    }
}
