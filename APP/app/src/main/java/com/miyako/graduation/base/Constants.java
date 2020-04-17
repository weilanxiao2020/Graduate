package com.miyako.graduation.base;

import com.amap.api.maps.model.LatLng;

import java.util.ArrayList;
import java.util.List;

/**
 * @Description
 * @Author Miyako
 * @Date 2020-02-28-0028
 */
public class Constants {

    public static final String KEY_STAFF_LICENSE = "StaffLicense";
    public static final String KEY_STAFF_CODE = "StaffCode";
    public static final String KEY_STAFF_ADDRESS = "StaffAddress";

    public static final String KEY_ORDER_ID = "OrderId";
    public static final String KEY_ORDER_MISSION = "OrderMission";
    public static final String KEY_ORDER_RECEIVER = "OrderReceiver";

    public static final int LENGTH_STAFF_ID = 9;
    public static final int LENGTH_STAFF_CODE = 6;

    public static final int LENGTH_ORDER_ID = 15;



    public static List<LatLng> getLatLngs(){
        List<LatLng> latLngs = new ArrayList<>();
        latLngs.add(new LatLng(30.3126719672,120.3566998243));
        latLngs.add(new LatLng(30.3121857093,120.3566837311));
        latLngs.add(new LatLng(30.3116485074,120.3563940525));
        latLngs.add(new LatLng(30.3119078466,120.3556001186));
        latLngs.add(new LatLng(30.3128757317,120.3556591272));
        latLngs.add(new LatLng(30.3133573552,120.3570002317));
        return latLngs;
    }

    public static final int ERROR_CODE = 0x00;
    public static final int CMD_EMPTY = 0xf0;
    public static final int CMD_MISSION = 0xf1;
    public static final int CMD_ORDER = 0xf2;
    public static final int CMD_GPS = 0xf3;
}
