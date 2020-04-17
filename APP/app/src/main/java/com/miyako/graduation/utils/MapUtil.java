package com.miyako.graduation.utils;

import android.content.Context;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.location.LocationManager;
import android.util.Log;
import android.view.Gravity;
import android.widget.Toast;

import com.amap.api.location.AMapLocation;
import com.amap.api.location.AMapLocationClient;
import com.amap.api.location.AMapLocationListener;
import com.amap.api.maps.AMap;
import com.amap.api.maps.CameraUpdateFactory;
import com.amap.api.maps.UiSettings;
import com.amap.api.maps.model.BitmapDescriptor;
import com.amap.api.maps.model.BitmapDescriptorFactory;
import com.amap.api.maps.model.LatLng;
import com.amap.api.maps.model.Marker;
import com.amap.api.maps.model.MarkerOptions;
import com.amap.api.maps.model.MyLocationStyle;
import com.amap.api.maps.model.PolylineOptions;
import com.miyako.graduation.R;

import java.util.ArrayList;
import java.util.List;

public class MapUtil {

    private static final String TAG = MapUtil.class.getSimpleName();

    private static final class Obj {
        static final MapUtil obj = new MapUtil();
    }

    private AMap mMap;
    private UiSettings mUiSettings;
    // 定位（当前位置）的绘制样式类
    private MyLocationStyle myLocationStyle;
    //声明AMapLocationClient类对象
    private AMapLocationClient mLocationClient;
    //声明当前经纬度
    private double currentLat,currentLon;
    // 当前位置
    private LatLng currentLatLng;
    // 设置一个flag 当 第一次进入地图 去将当前位置作为屏幕中心点
    private boolean flag;
    private int colorLine;
    private List<LatLng> mData;
    private MapLocationListener mListener;
    private boolean isDraw;

    private static double cnt = 0;

    private LatLng lastPoint;

    private AMapLocationListener mLocationListener = new AMapLocationListener() {
        @Override
        public void onLocationChanged(AMapLocation amapLocation) {
            // 从这里开始就会持续回调
            if (checkGps()&&!isDraw) {
                if (amapLocation != null) {
                    if (amapLocation.getErrorCode() == 0) {
                        //定位成功回调信息，设置相关消息
                        amapLocation.getLocationType();//获取当前定位结果来源，如网络定位结果，详见定位类型表
                        currentLat = amapLocation.getLatitude();//获取纬度
                        currentLon = amapLocation.getLongitude();//获取经度
                        currentLatLng = new LatLng(currentLat, currentLon);
                        amapLocation.getAccuracy();//获取精度信息
                        if(lastPoint == null) {
                            lastPoint = new LatLng(currentLat, currentLon);
                        }
                        LatLng newPoint = new LatLng(currentLat+(cnt), currentLon+(cnt));
                        cnt = cnt+0.02;
                        Log.d(TAG, "cnt "+cnt);
                        mMap.addPolyline(new PolylineOptions().add(lastPoint, newPoint).width(10).color(colorLine));
                        lastPoint = newPoint;
                        Log.i("mylocation", "currentLat : " + currentLat + " currentLon : " + currentLon);
                        if (flag) {
                            // 将当前位置移至屏幕中心 设置了flag并且设置了上面的定位模式 这里就只一次
                            mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(currentLatLng, 17));
                            mMap.addPolyline(new PolylineOptions().addAll(mData).width(10).color(colorLine));
                            flag = false;
                        }
                    } else {
                        //显示错误信息ErrCode是错误码，errInfo是错误信息，详见错误码表。
                        Log.e("AmapError", "location error, ErrCode:"
                                + amapLocation.getErrorCode() + ", errInfo:"
                                + amapLocation.getErrorInfo());
                    }
                }
            }
        }
    };

    public static MapUtil getInstance() {
        return Obj.obj;
    }

    private MapUtil() {
        flag = true;
        colorLine = Color.argb(255, 255, 1, 1);
    }

    private Context mContext;

    public MapUtil setContext(Context context) {
        mContext = context;
        return this;
    }

    public MapUtil init(AMap map) {
        if (mMap == null && map!=null) {
            mMap = map;
            mUiSettings = mMap.getUiSettings();
            mMap.getUiSettings().setLogoPosition(-23);  //将LOGO 和 比例尺 往下移
        }
        setMapButton();
        setLocationStyle();
        isDraw = false;
        // 聚焦到指定位置
//        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(data.get(0), 17));
//        drawMarkers(data);
//        drawPath();
        return this;
    }


    public MapUtil location() {
        initLocation();
        return this;
    }

    public MapUtil display() {
        return this;
    }

    /**
     * 获取位置
     */
    public MapUtil initLocation(){
        isDraw = false;
        //初始化定位
        mLocationClient = new AMapLocationClient(mContext);
        //设置定位回调监听
        mLocationClient.setLocationListener(mLocationListener);
        //启动定位
        mLocationClient.startLocation();
        return this;
    }

    public MapUtil changedLocation(boolean flag) {
        if (!flag) {
            mLocationClient.stopLocation();// 停止定位
        } else {
            mLocationClient.startLocation();// 开启定位
        }
        return this;
    }

    public interface MapLocationListener {
        public void locationChanged();
    }

    /**
     * 当前位置显示样式
     */
    public MapUtil setLocationStyle() {
        myLocationStyle = new MyLocationStyle();//初始化定位蓝点样式类 默认为 连续定位、且将视角移动到地图中心点，定位点依照设备方向旋转，并且会跟随设备移动。（1秒1次定位）
        myLocationStyle.myLocationType(MyLocationStyle.LOCATION_TYPE_FOLLOW_NO_CENTER);//连续定位、蓝点不会移动到地图中心点，定位点依照设备方向旋转，并且蓝点会跟随设备移动。
        myLocationStyle.interval(2000); //设置连续定位模式下的定位间隔，只在连续定位模式下生效，单次定位模式下不会生效。单位为毫秒。
        customLocationIcon();
        mMap.setMyLocationStyle(myLocationStyle);//设置定位蓝点的Style
        mMap.setMyLocationEnabled(true);// 设置为true表示启动显示定位蓝点，false表示隐藏定位蓝点并不进行定位，默认是false。
        return this;
    }

    /**
     * 当前位置显示图标
     */
    public MapUtil customLocationIcon(){
        //设置图标
        BitmapDescriptor bitMapLocation = BitmapDescriptorFactory.fromBitmap(BitmapFactory.decodeResource(mContext.getResources(),
                R.mipmap.ic_launcher));
        myLocationStyle.myLocationIcon(bitMapLocation);//设置定位蓝点的icon图标方法，需要用到BitmapDescriptor类对象作为参数
        myLocationStyle.strokeColor(Color.GRAY);      //设置定位蓝点精度圆圈的边框颜色的方法。 int 类型的
        //        myLocationStyle.radiusFillColor(Color.BLUE);  //设置定位蓝点精度圆圈的填充颜色的方法 默认为紫色
        return this;
    }

    // 显示Map功能按钮
    public MapUtil setMapButton() {
        //显示比例尺
        mUiSettings.setScaleControlsEnabled(true);
        //显示指南针
        mUiSettings.setCompassEnabled(true);
        // 显示定位按钮
        mMap.getUiSettings().setMyLocationButtonEnabled(true);//设置默认定位按钮是否显示 默认不显示
        return this;
    }

    public static void convert() {

    }

    // 设置绘制坐标点
    public MapUtil setDatas(List<LatLng> data) {
        if (mData == null) {
            mData = new ArrayList<>(data.size());
            convert();
            mData.addAll(data);
        } else {
//            mData.clear();
            mData = new ArrayList<>(data.size());
            convert();
            mData.addAll(data);
        }
        return this;
    }

    public MapUtil cancelLocation() {
        mLocationClient.onDestroy();//销毁定位客户端。
        return this;
    }

    public MapUtil drawPath() {
        Log.d(TAG, "绘制移动路线");
        isDraw = true;
        //绘制移动路线
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(mData.get(0), 17));
        for(int i=0;i<mData.size();i++) {
            Log.d(TAG, mData.get(i).toString());
        }
        mMap.addPolyline(new PolylineOptions().addAll(mData).width(10).color(colorLine));
        mMap.addPolyline(new PolylineOptions().add(mData.get(mData.size()-1), new LatLng(30.3133573552,120.3590002317)).width(10).color(colorLine));
        return this;
    }

    private List<Marker> markers;

    private void drawMarkers(List<LatLng> latLngs) {
        Log.d(TAG, "绘制标记点");
        markers = new ArrayList<>();
        for (LatLng latLng : latLngs) {
            MarkerOptions markerOption = new MarkerOptions();
            markerOption.position(latLng)
                    .icon(BitmapDescriptorFactory.fromBitmap(BitmapFactory.decodeResource(mContext.getResources(), R.drawable.ic_launcher_background)));
            markers.add(mMap.addMarker(markerOption));
        }
    }

    private boolean checkGps() {
        if (!IsGpsWork.isGpsEnabled(mContext)){
            Toast toast = Toast.makeText(mContext, "GPS不可用", Toast.LENGTH_SHORT);
            toast.setGravity(Gravity.CENTER, 0, 0);
            toast.show();
            return false;
        }else {
            return true;
        }
    }

    public static class IsGpsWork {
        //判断GPS是否开启
        public static boolean isGpsEnabled(Context context) {
            LocationManager locationManager = (LocationManager) context
                    .getSystemService(Context.LOCATION_SERVICE);
            // 判断GPS模块是否开启
            return locationManager != null && locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
        }
    }
}
