package com.miyako.mqtt;

import com.miyako.model.GPS;
import com.miyako.model.Order;
import com.miyako.parse.ParseMqtt;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;

import java.rmi.activation.ActivationGroup;

/**
 * ClassName MqttTask
 * Description //TODO
 * Author Miyako
 * Date 2020-04-10-0010 15:01
 */
public class MqttTask {

    private static final String TAG = MqttTask.class.getSimpleName();

    public void start() {
        MqttManager.getInstance()
                   .init(ServerApp.MQTT_PRODUCTKEY, ServerApp.MQTT_DEVICENAME, ServerApp.MQTT_DEVICESECRET)
                   .setBroker("ssl")
                   .connect();
        setSubscribe();
        //MqttManager.getInstance().publish(ServerApp.MQTT_TOPIC_CARD_MISSION_SET, buildMissionMsg());
    }

    private byte[] buildMissionMsg() {
        byte[] mission = "12345678-416421".getBytes();
        byte[] data = new byte[3+mission.length];
        data[0] = (byte) 0xfe;
        data[1] = 0x01;
        data[2] = (byte) mission.length;
        System.arraycopy(mission, 0, data, 3, mission.length);
        return data;
    }

    private void setSubscribe() {
        MqttManager.getInstance()
                   .subscribe(ServerApp.MQTT_TOPIC_SCAN_RFID_GET, this::getScanCard)
                   .subscribe(ServerApp.MQTT_TOPIC_CARD_GPS_GET, this::getCardGps);
    }

    private void getCardInfoTest(byte[] data){
        LogUtil.i(TAG, "getCardInfoTest byte size=>"+data.length);
        StringBuilder sb = new StringBuilder();
        for (byte datum : data) {
            LogUtil.d(TAG, String.format("%02x", datum));
        }
    }

    /**
     * 处理车辆扫描rfid的订单数据写入数据库，例如订单由"运输中"->"已揽收"
     * @param data 透传的数据
     */
    private void getScanCard(byte[] data) {
        LogUtil.i(TAG, "getScanCard");
        Order order = ParseMqtt.parseRfidMqtt(data);
    }

    /**
     * 处理车辆Gps实时数据写入数据库
     * @param data 透传的数据
     */
    private void getCardGps(byte[] data) {
        LogUtil.i(TAG, "getCardGps");
        GPS gps = ParseMqtt.parseGpsMqtt(data);
    }
}
