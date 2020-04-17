package com.miyako.graduation.module.user;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ListView;
import android.widget.TextView;

import com.amap.api.maps.MapView;
import com.miyako.graduation.R;
import com.miyako.graduation.base.Constants;
import com.miyako.graduation.entity.GpsOrder;
import com.miyako.graduation.socket.SocketManager;
import com.miyako.graduation.socket.msg.BaseMsg;
import com.miyako.graduation.socket.msg.GpsMsg;
import com.miyako.graduation.socket.msg.OrderMsg;
import com.miyako.graduation.utils.MapUtil;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

import io.reactivex.Observable;
import io.reactivex.android.schedulers.AndroidSchedulers;

public class UserOrderActivity extends AppCompatActivity {


    private static final String TAG = UserOrderActivity.class.getSimpleName();

    private TextView mTvOrderId;
    private TextView mTvOrderReceiver;
    private MapView mMapView;
    private ListView mListOrderGps;

    private String orderId;
    private String receiver;
    private String mission;
    private UserOrderAdapter mAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_order);
        initView(savedInstanceState);
        initValue();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        //在activity执行onDestroy时执行mMapView.onDestroy()，销毁地图
        mMapView.onDestroy();
        MapUtil.getInstance().cancelLocation();
    }
    @Override
    protected void onResume() {
        super.onResume();
        //在activity执行onResume时执行mMapView.onResume ()，重新绘制加载地图
        mMapView.onResume();
        showList(null);
    }
    @Override
    protected void onPause() {
        super.onPause();
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
        mTvOrderId = findViewById(R.id.tv_order_id);
        mTvOrderReceiver = findViewById(R.id.tv_order_receiver);
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
    }

    // 显示地图
    private void displayMap() {
        if (mMapView.getVisibility() == View.VISIBLE) {
            Log.d(TAG, "display map");
            MapUtil.getInstance()
                    .setContext(this)
                    .init(mMapView.getMap())
                    .location()
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
        MapUtil.getInstance().changedLocation(false);
    }

    public void showList(View view) {
        Log.d(TAG, "show list");
        stopMap();
        SocketManager.getInstance().getOrderGps(mission, new SocketManager.ISocketCall() {
            @Override
            public void success(List<BaseMsg> msg) {
                Log.d(TAG, "msg: "+msg.get(0));
                Observable.timer(0, TimeUnit.MILLISECONDS)
                        .observeOn(AndroidSchedulers.mainThread())
                        .subscribe(s->{
                            List<GpsOrder> orderList = new ArrayList<>(msg.size());
                            for(int i=0;i<msg.size();i++) {
                                GpsMsg gpsMsg = (GpsMsg) msg.get(i);
                                GpsOrder order = new GpsOrder();
                                order.setId(gpsMsg.getId());
                                order.setUtcTime(Long.valueOf(gpsMsg.getUtcTime()));
                                order.setLatitude(gpsMsg.getLatitude());
                                order.setN_S(gpsMsg.getN_S());
                                order.setLongitude(gpsMsg.getLongitude());
                                order.setE_W(gpsMsg.getE_W());
                                Log.d(TAG, order.toString());
                                orderList.add(order);
                            }
                            mAdapter.update(orderList);
                            mListOrderGps.setVisibility(View.VISIBLE);
                        });
            }

            @Override
            public void error(int errorCode, List<BaseMsg> msg) {

            }
        });
//        OkSocket.open(MyApplication.getInfo())
//                .send(new ISendable() {
//                    @Override
//                    public byte[] parse() {
//                        Log.d(TAG, "send data");
//                        ReqBody<QueryMsg> reqBody = new ReqBody<>(Constants.CMD_MISSION<<8, System.currentTimeMillis(), "Android");
//                        Gson gson = new Gson();
//                        QueryMsg msg = new QueryMsg();
//                        msg.setQueryId("wlx-41647");
//                        reqBody.setData(msg);
////                        ReqBody reqBody1 = gson.fromJson(json, ReqBody.class);
////                        Log.d(TAG, "data json:"+reqBody1);
//                        String json = gson.toJson(reqBody);
//                        Log.d(TAG, "data json:"+json);
//                        return json.getBytes();
//                    }
//                });
    }

    public void showMap(View view) {
        Log.d(TAG, "show map");
        startMap();
        MapUtil.getInstance().setDatas(Constants.getLatLngs()).drawPath();
    }
}
