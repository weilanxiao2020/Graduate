package com.miyako.utils;

import com.miyako.model.GPS;
import com.miyako.model.Mission;
import com.miyako.model.Order;
import com.miyako.result.ResBody;
import com.miyako.result.msg.GpsMsg;
import com.miyako.result.msg.MissionMsg;
import com.miyako.result.msg.OrderMsg;

import java.util.List;

/**
 * ClassName ConvertUtil
 * Description //TODO
 * Author Miyako
 * Date 2020-04-03-0003 16:52
 */
public class ConvertUtil{

    private static final String TAG = ConvertUtil.class.getSimpleName();

    // gpsmsg为utc时间
    public static void gpsToGpsMsg(GPS gps, GpsMsg gpsMsg){
        //LogUtil.d(TAG, "GPS -> GpsMsg");
        if (gps == null) {
            gpsMsg.setId(0);
            gpsMsg.setUtcTime("0");
            gpsMsg.setLatitude("0.0");
            gpsMsg.setN_S("none");
            gpsMsg.setLongitude("0.0");
            gpsMsg.setE_W("none");
        } else {
            gpsMsg.setId(gps.getId());
            gpsMsg.setMissionId(String.valueOf(gps.getMissionId()));
            gpsMsg.setUtcTime(String.valueOf(gps.getTimestamp()));
            gpsMsg.setLatitude(gps.getLatitude().substring(0,gps.getLatitude().length()-1));
            String n_s = gps.getLatitude().substring(gps.getLatitude().length() - 1);
            gpsMsg.setN_S(n_s.matches("[nsNS]")?n_s:"none");
            gpsMsg.setLongitude(gps.getLongitude().substring(0,gps.getLongitude().length()-1));
            String e_w = gps.getLongitude().substring(gps.getLongitude().length() - 1);
            gpsMsg.setE_W(e_w.matches("[ewEW]")?e_w:"none");
        }
    }

    // gpsmsg为utc时间
    public static void gpsMsgToGps(GpsMsg gpsMsg, GPS gps){
        //LogUtil.d(TAG, "GpsMsg -> GPS");
        gps.setMissionId(gpsMsg.getMissionId());
        gps.setTimestamp(Long.valueOf(gpsMsg.getUtcTime()));
        gps.setLatitude(gpsMsg.getLatitude()+gpsMsg.getN_S());
        gps.setLongitude(gpsMsg.getLongitude()+gpsMsg.getE_W());
        gpsMsg.setUtcTime(String.valueOf(gps.getTimestamp()));
    }

    public static String missionId2LicenseCode(String license, String code) {
        return license+"-"+code;
    }

    public static void missionToMissionMsg(Mission mission, MissionMsg missionMsg){
        //LogUtil.d(TAG, "Mission -> MissionMsg");
        missionMsg.setMissionId(missionId2LicenseCode(mission.getLicense(), mission.getCode()));
        missionMsg.setTimestamp(mission.getTimestamp());
        missionMsg.setAddress(mission.getAddress());
        missionMsg.setStatus(mission.getStatus());
    }

    public static void orderToOrderMsg(Order order, OrderMsg orderMsg){
        //LogUtil.d(TAG, "Order -> OrderMsg");
        orderMsg.setTrackId(order.getTrackId());
        orderMsg.setTimestamp(order.getTimestamp());
        orderMsg.setSender(order.getSender());
        orderMsg.setReceiver(order.getReceiver());
        orderMsg.setAddress(order.getAddress());
        orderMsg.setStatus(order.getStatus());
        orderMsg.setMissionId(order.getMissionId());
    }
}
