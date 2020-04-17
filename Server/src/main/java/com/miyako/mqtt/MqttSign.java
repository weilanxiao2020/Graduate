package com.miyako.mqtt;

import com.miyako.utils.CryptoUtil;

/**
 * ClassName MqttSign
 * Description //Mqtt登录
 * Author Miyako
 * Date 2020-04-08-0008 15:42
 */
public class MqttSign{

    private String username = "";

    private String password = "";

    private String clientId = "";

    public String getUsername() { return this.username;}

    public String getPassword() { return this.password;}

    public String getClientId() { return this.clientId;}

    public void calculate(String productKey, String deviceName, String deviceSecret) {
        if (productKey == null || deviceName == null || deviceSecret == null) {
            return;
        }

        try {
            //MQTT用户名
            this.username = deviceName + "&" + productKey;

            //MQTT密码
            String timestamp = Long.toString(System.currentTimeMillis());
            String plainPasswd = "clientId" + productKey + "." + deviceName + "deviceName" +
                                 deviceName + "productKey" + productKey + "timestamp" + timestamp;
            this.password = CryptoUtil.hmacSha256(plainPasswd, deviceSecret);

            //MQTT ClientId
            this.clientId = productKey + "." + deviceName + "|" + "timestamp=" + timestamp +
                            ",_v=paho-java-1.0.0,securemode=2,signmethod=hmacsha256|";
        }catch (Exception e) {
            e.printStackTrace();
        }
    }
}

