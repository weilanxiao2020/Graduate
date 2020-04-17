package com.miyako.mqtt;

import com.miyako.dao.GPSDao;
import com.miyako.dao.OrderDao;
import com.miyako.model.GPS;
import com.miyako.model.Order;
import com.miyako.utils.LogUtil;

import java.nio.charset.StandardCharsets;
import java.util.List;

/**
 * ClassName MqttMsg
 * Description //TODO
 * Author Miyako
 * Date 2020-04-13-0013 10:13
 */
public class MqttMsg {
    private static final String TAG = MqttMsg.class.getSimpleName();

    private static final int MQTT_MSG_HEAD_LEN = 2;
    private static final int MQTT_GPS_MSG_HEAD_LEN = 6;
    private static final int MQTT_RFID_MSG_HEAD_LEN = 5;

    //mqtt msg struct
    // | 0xfe | 数据模块个数<0xff | 模块1数据长度 | 模块2数据长度 | ... | 模块1具体数据 | 模块2具体数据 | ... |

    public static void parseGpsMsg(byte[] data) {
        if(data[0] != (byte) 0xfe) {
            LogUtil.d(TAG, "接收mqtt消息异常");
            return;
        } else {
            LogUtil.d(TAG, "解析gps mqtt数据");
            // 得到数据个数
            int len = data[1]&0xff;
            if (len != MQTT_GPS_MSG_HEAD_LEN) {
                LogUtil.d(TAG, "接收gps mqtt长度异常");
                return;
            } else {
                int index = MQTT_MSG_HEAD_LEN + MQTT_GPS_MSG_HEAD_LEN;
                int i = MQTT_MSG_HEAD_LEN;
                String missionId = parseData(data, index, data[i++] & 0xff);
                index +=(data[i-1]&0xff);
                String utc = parseData(data, index, data[i++] & 0xff);
                index +=(data[i-1]&0xff);
                String latitude = parseData(data, index, data[i++] & 0xff);
                index +=(data[i-1]&0xff);
                String n_s = parseData(data, index, data[i++] & 0xff);
                index +=(data[i-1]&0xff);
                String longitude = parseData(data, index, data[i++] & 0xff);
                index +=(data[i-1]&0xff);
                String e_w = parseData(data, index, data[i++] & 0xff);
                GPS gps = new GPS();
                gps.setMissionId(missionId);
                gps.setTimestamp(Long.valueOf(utc));
                gps.setLatitude(latitude+n_s);
                gps.setLongitude(longitude+e_w);
                //if (GPSDao.insert(gps) != -1) {
                //    LogUtil.d(TAG, "mqtt消息处理成功");
                //} else {
                //    LogUtil.d(TAG, "mqtt消息处理异常");
                //}
            }
        }
    }

    public static void parseRfid(byte[] data) {
        if(data[0] != (byte) 0xfe) {
            LogUtil.d(TAG, "接收mqtt消息异常");
            return;
        } else {
            LogUtil.d(TAG, "解析rfid mqtt数据");
            // 得到数据个数
            int len = data[1]&0xff;
            if (len != MQTT_RFID_MSG_HEAD_LEN) {
                LogUtil.d(TAG, "接收rfid mqtt长度异常");
                return;
            } else {
                int index = MQTT_MSG_HEAD_LEN + MQTT_RFID_MSG_HEAD_LEN;
                int i=MQTT_MSG_HEAD_LEN;
                String missionId = parseData(data, index, data[i++] & 0xff);
                index +=(data[i-1]&0xff);
                String trackId = parseData(data, index, data[i++] & 0xff);
                index +=(data[i-1]&0xff);
                String timestamp = parseData(data, index, data[i++]&0xff);
                index +=(data[i-1]&0xff);
                String address = parseData(data, index, data[i++]&0xff);
                index +=(data[i-1]&0xff);
                String status = parseData(data, index, data[i++]&0xff);
                Order order = OrderDao.findByTrackId(trackId);
                order.setTimestamp(Long.valueOf(timestamp));
                order.setStatus(status);
                //if (OrderDao.update(order) != -1) {
                //    LogUtil.d(TAG, "mqtt消息处理成功");
                //} else {
                //    LogUtil.d(TAG, "mqtt消息处理异常");
                //}
            }
        }
    }

    private static String parseData(byte[] data, int startIndex, int len) {
        //LogUtil.d(TAG, "parseData");
        //LogUtil.d(TAG, "startIndex=>" + startIndex);
        //LogUtil.d(TAG, "len=>" + len);
        return new String(data, startIndex, len, StandardCharsets.UTF_8);
    }
}
