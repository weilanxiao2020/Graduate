package com.miyako.mqtt;

import com.miyako.utils.LogUtil;
import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.util.HashMap;
import java.util.Map;

import static com.sun.corba.se.impl.util.Utility.printStackTrace;

/**
 * ClassName MqttManager
 * Description //TODO
 * Author Miyako
 * Date 2020-04-10-0010 14:22
 */
public class MqttManager{

    private static final String TAG = MqttManager.class.getSimpleName();

    private static final class Obj {
        private static final MqttManager obj = new MqttManager();
    }

    private MqttManager() {}

    public static MqttManager getInstance() {
        synchronized (MqttManager.class) {
            return Obj.obj;
        }
    }

    private String productKey;
    private String deviceName;
    private String deviceSecret;
    private String broker;
    private MqttSign sign;
    // Paho MQTT 客户端
    private MqttClient mqttClient;
    // Paho MQTT 连接参数
    private MqttConnectOptions connOpts;
    private Map<String, ISubscribeCall> callMap;
    public interface ISubscribeCall {
        public void success(byte[] data);
    }

    public MqttManager init(String productKey, String deviceName, String deviceSecret) {
        this.productKey = productKey;
        this.deviceName = deviceName;
        this.deviceSecret = deviceSecret;
        // 计算MQTT连接参数
        sign = new MqttSign();
        sign.calculate(productKey, deviceName, deviceSecret);
        LogUtil.i(TAG, "username: " + sign.getUsername());
        LogUtil.i(TAG, "password: " + sign.getPassword());
        LogUtil.i(TAG, "clientId: " + sign.getClientId());
        return this;
    }

    public MqttManager setBroker(String type) {
        if ("ssl".equals(type)) {
            broker = "ssl://" + productKey + ".iot-as-mqtt.cn-shanghai.aliyuncs.com" + ":" + 443;
        } else {
            broker = "tcp://" + productKey + ".iot-as-mqtt.cn-shanghai.aliyuncs.com" + ":" + 1883;
        }
        return this;
    }

    public MqttManager connect() {
        if (mqttClient != null) {
            try {
                mqttClient.disconnect();
            }
            catch (MqttException e) {
                e.printStackTrace();
            }
        }
        mqttClient = null;
        try {
            mqttClient = new MqttClient(broker, sign.getClientId(), new MemoryPersistence());
            connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            connOpts.setKeepAliveInterval(180);
            connOpts.setUserName(sign.getUsername());
            connOpts.setPassword(sign.getPassword().toCharArray());
            connOpts.setAutomaticReconnect(true);
            mqttClient.setCallback(new MqttCallbackExtended() {

                @Override
                public void connectionLost(Throwable throwable){
                    LogUtil.d(TAG, "connectionLost");
                    LogUtil.e(TAG, throwable.getCause().getMessage());
                    throwable.printStackTrace();
                }

                @Override
                public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception{
                    LogUtil.i(TAG, "messageArrived");
                    LogUtil.d(TAG, "接收消息主题 : " + topic);
                    for(Map.Entry<String, ISubscribeCall> entry:callMap.entrySet()) {
                        LogUtil.d(TAG, "待匹配消息主题 : " + entry.getKey());
                        if(topic.equals(entry.getKey())) {
                            LogUtil.d(TAG, "匹配主题成功");
                            LogUtil.d(TAG, "接收消息Qos : " + mqttMessage.getQos());
                            //String str = new String(mqttMessage.getPayload());
                            LogUtil.i(TAG, "接收消息内容字节 : " + mqttMessage.getPayload().length);
                            entry.getValue().success(mqttMessage.getPayload());
                            break;
                        }
                    }
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken){

                }

                @Override
                public void connectComplete(boolean b, String s){
                    if (callMap != null) {
                        callMap.forEach((k,v)->subscribe(k,null));
                    }

                    //callMap.put(topic, callback);
                }
            });
            //mqttClient.setCallback(new MqttCallback(){
            //    @Override
            //    public void connectionLost(Throwable throwable){
            //
            //    }
            //
            //    @Override
            //    public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception{
            //
            //    }
            //
            //    @Override
            //    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken){ }
            //});
            //判断拦截状态，这里注意一下，如果没有这个判断，是非常坑的
            if (!mqttClient.isConnected()) {
                mqttClient.connect(connOpts);
                LogUtil.d(TAG, "连接成功");
            }else {//这里的逻辑是如果连接成功就重新连接
                mqttClient.disconnect();
                mqttClient.connect(connOpts);
                LogUtil.d(TAG, "连接成功");
            }
            //mqttClient.connect(connOpts);
        }
        catch (MqttException e) {
            e.printStackTrace();
        }
        return this;
    }

    public MqttManager publish(String topic, byte[] data) {
        try {
            MqttMessage msg = new MqttMessage(data);
            msg.setQos(0);
            mqttClient.publish(topic, msg);
            LogUtil.d(TAG, "发布topic=>"+topic);
        }
        catch (MqttException e) {
            e.printStackTrace();
        }
        return this;
    }

    public MqttManager subscribe(String topic, ISubscribeCall callback) {
        try {
            if (callMap == null) {
                callMap = new HashMap<>();
            }
            if (callMap.containsKey(topic)) {
                return this;
            }
            if (mqttClient == null) {
                return this;
            }
            mqttClient.subscribe(topic);
            callMap.put(topic, callback);
            LogUtil.i(TAG, "订阅topic=>"+topic);
        }
        catch (MqttException e) {
            e.printStackTrace();
        }
        return this;
    }

    public MqttManager unsubscribe(String topic) {
        try {
            if (callMap == null) {
                return this;
            }
            if (callMap.containsKey(topic)) {
                if (mqttClient == null) {
                    return this;
                }
                mqttClient.unsubscribe(topic);
                callMap.remove(topic);
            }
        }
        catch (MqttException e) {
            e.printStackTrace();
        }
        return this;
    }
}
