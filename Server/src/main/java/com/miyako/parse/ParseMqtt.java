package com.miyako.parse;

import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.miyako.dao.GPSDao;
import com.miyako.dao.OrderDao;
import com.miyako.model.GPS;
import com.miyako.model.Order;
import com.miyako.utils.LogUtil;
import com.miyako.utils.MapUtil;
import sun.rmi.runtime.Log;

import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

/**
 * ClassName ParseMqtt
 * Description //TODO
 * Author Miyako
 * Date 2020-05-02-0002 15:37
 */
public class ParseMqtt{

    private static final String TAG = ParseMqtt.class.getSimpleName();

    private static final int MQTT_MSG_HEAD_LEN = 2;
    private static final int MQTT_GPS_MSG_HEAD_LEN = 6;
    private static final int MQTT_RFID_MSG_HEAD_LEN = 5;

    //mqtt msg struct
    // | 0xfe | 数据模块个数<0xff | 模块1数据长度 | 模块2数据长度 | ... | 模块1具体数据 | 模块2具体数据 | ... |

    private static Map<String, Long> intervalMap = new HashMap<>();
    private static Map<String, String> regionMap = new HashMap<>();
    //private static long spanTime = 1000 * 60;
    //private static int span = 60;
    private static long spanTime = 60;
    private static int span = 1 * 1000;

    private static double convertGps(String val) {
        double res = Double.valueOf(val);
        int temp = (int) res;
        //LogUtil.d(TAG, "temp:"+temp);
        //LogUtil.d(TAG, "val:"+(res-temp));
        //LogUtil.d(TAG, "度:"+(temp/100));
        //LogUtil.d(TAG, "s:"+(temp%100/60.0));
        //LogUtil.d(TAG, "d:"+((res-temp) *60  / 3600.0));
        res = temp/100 + temp%100/60.0 + (res-temp) / 600.0;
        return res;
    }

    public static GPS parseGpsMqtt(byte[] data) {
        GPS gps = null;
        String json = new String(data, StandardCharsets.UTF_8);
        JsonObject object = validateJson(json);  // 转化为对象
        if (object == null) {
            LogUtil.d(TAG, "接收mqtt消息异常");
            return gps;
        }
        byte code = object.get("code").getAsByte();
        LogUtil.d(TAG, String.format("post code:0x%02x",code));
        String missionId = object.get("mission").getAsString();
        String latitude = object.get("latitude").getAsString();
        String longitude = object.get("longitude").getAsString();
        LogUtil.d(TAG, "mission:"+missionId);

        String[] lat_s = latitude.split("-");
        String[] lon_s = longitude.split("-");
        if("x".equals(lat_s[1]) || "x".equals(lon_s[1])) {
            LogUtil.d(TAG, "gps无效");
            return null;
        }
        double lat = convertGps(lat_s[0]);
        double lon = convertGps(lon_s[0]);
        LogUtil.d(TAG, String.format("convert gps:%s,%s",lat,lon));
        gps = new GPS();
        gps.setMissionId(missionId);
        gps.setTimestamp(System.currentTimeMillis());
        gps.setLatitude(lat+lat_s[1]);
        gps.setLongitude(lon+lon_s[1]);
        if (!intervalMap.containsKey(missionId)) {
            intervalMap.put(missionId, System.currentTimeMillis());
            gps.setRegion(MapUtil.gpsParse(lat, lon));
            regionMap.put(missionId, gps.getRegion());
        } else {
            long diff = gps.getTimestamp() - intervalMap.get(missionId);
            LogUtil.d(TAG, "diff:"+diff);
            if(diff / (spanTime)> span) {
                // 时间间隔达到阈值，更新gps区域
                gps.setRegion(MapUtil.gpsParse(lat,lon));
                regionMap.put(missionId, gps.getRegion());
            } else {
                // 时间间隔未达到阈值
                gps.setRegion(regionMap.get(missionId));
            }
        }

        //if (GPSDao.insert(gps) != -1) {
        //    LogUtil.d(TAG, "mqtt消息处理成功");
        //} else {
        //    LogUtil.d(TAG, "mqtt消息处理异常");
        //    gps = null;
        //}

        //if(data[0] != (byte) 0xfe) {
        //    LogUtil.d(TAG, "接收mqtt消息异常");
        //    return gps;
        //} else {
        //    LogUtil.d(TAG, "解析gps mqtt数据");
        //    // 得到数据个数
        //    int len = data[1]&0xff;
        //    if (len != MQTT_GPS_MSG_HEAD_LEN) {
        //        LogUtil.d(TAG, "接收gps mqtt长度异常");
        //        return gps;
        //    } else {
        //        int index = MQTT_MSG_HEAD_LEN + MQTT_GPS_MSG_HEAD_LEN;
        //        int i = MQTT_MSG_HEAD_LEN;
        //        String missionId = parseData(data, index, data[i++] & 0xff);
        //        index +=(data[i-1]&0xff);
        //        String utc = parseData(data, index, data[i++] & 0xff);
        //        index +=(data[i-1]&0xff);
        //        String latitude = parseData(data, index, data[i++] & 0xff);
        //        index +=(data[i-1]&0xff);
        //        String n_s = parseData(data, index, data[i++] & 0xff);
        //        index +=(data[i-1]&0xff);
        //        String longitude = parseData(data, index, data[i++] & 0xff);
        //        index +=(data[i-1]&0xff);
        //        String e_w = parseData(data, index, data[i++] & 0xff);
        //        gps = new GPS();
        //        gps.setMissionId(missionId);
        //        gps.setTimestamp(Long.valueOf(utc));
        //        gps.setLatitude(latitude+n_s);
        //        gps.setLongitude(longitude+e_w);
        //        gps.setRegion(MapUtil.gpsParse(Double.valueOf(latitude), Double.valueOf(longitude)));
        //        //if (GPSDao.insert(gps) != -1) {
        //        //    LogUtil.d(TAG, "mqtt消息处理成功");
        //        //} else {
        //        //    LogUtil.d(TAG, "mqtt消息处理异常");
        //        //}
        //    }
        //}
        return gps;
    }

    public static Order parseRfidMqtt(byte[] data) {
        Order order = null;
        String json = new String(data, StandardCharsets.UTF_8);
        JsonObject object = validateJson(json);  // 转化为对象
        if (object == null) {
            LogUtil.d(TAG, "接收mqtt消息异常");
            return order;
        }
        byte code = object.get("code").getAsByte();
        LogUtil.d(TAG, String.format("post code:0x%02x",code));
        String missionId = object.get("mission").getAsString();
        String trackId = object.get("trackId").getAsString();
        byte status = object.get("status").getAsByte();
        order = OrderDao.findByTrackId(trackId);
        if (order == null) {
            LogUtil.d(TAG, "查询为空");
            return order;
        }
        if (missionId.equals(order.getMissionId())) {
            switch (status) {
                case 0x00: order.setStatus("未配送"); break;
                case 0x01: order.setStatus("已装车"); break;
                case 0x02: order.setStatus("运输中"); break;
                case 0x03: order.setStatus("已送达"); break;
                case 0x04: order.setStatus("已完成"); break;
            }
            if (OrderDao.update(order) != -1) {
                LogUtil.d(TAG, "mqtt消息处理成功");
            } else {
                LogUtil.d(TAG, "mqtt消息处理异常");
            }
        } else {
            LogUtil.d(TAG, "mission数据不符");
            return null;
        }
        return order;
    }

    private static JsonObject validateJson(String json) {
        // json解析器，解析json数据
        LogUtil.d(TAG, "mqtt json:"+json);
        JsonParser parser = new JsonParser();
        JsonElement element = parser.parse(json);
        if (!element.isJsonObject()) {
            return null;
        } else {
            return element.getAsJsonObject();
        }
    }

    private static String parseData(byte[] data, int startIndex, int len) {
        return new String(data, startIndex, len, StandardCharsets.UTF_8);
    }
}
