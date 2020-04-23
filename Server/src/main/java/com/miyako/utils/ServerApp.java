package com.miyako.utils;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.Properties;

/**
 * ClassName ServerApp
 * Description //TODO
 * Author Miyako
 * Date 2020-03-27-0027 11:04
 */
public class ServerApp{

    /** MySql **/
    public static String MYSQL_JDBC;
    public static String MYSQL_URL;
    public static String MYSQL_USERNAME;
    public static String MYSQL_PASSWORD;

    /** Mqtt **/
    public static String MQTT_PRODUCTKEY;
    public static String MQTT_DEVICENAME;
    public static String MQTT_DEVICESECRET;
    public static String MQTT_TOPIC_CARD_INFO_GET_TEST;
    public static String MQTT_TOPIC_SCAN_RFID_GET;
    public static String MQTT_TOPIC_CARD_GPS_GET;
    public static String MQTT_TOPIC_CARD_MISSION_SET;

    private static final String CONFIG = "config.properties";

    public static void readConfig() throws IOException{
        Properties properties = new Properties();
        properties.load(new InputStreamReader(Object.class.getResourceAsStream("/"+CONFIG), StandardCharsets.UTF_8));
        MYSQL_JDBC  = properties.getProperty("mysql_jdbc");
        MYSQL_URL = properties.getProperty("mysql_url");
        MYSQL_USERNAME  = properties.getProperty("mysql_username");
        MYSQL_PASSWORD  = properties.getProperty("mysql_password");
        MQTT_PRODUCTKEY = properties.getProperty("mqtt_productKey");
        MQTT_DEVICENAME = properties.getProperty("mqtt_deviceName");
        MQTT_DEVICESECRET = properties.getProperty("mqtt_deviceSecret");
        MQTT_TOPIC_SCAN_RFID_GET = properties.getProperty("mqtt_tpoic_scan_rfid_get");
        MQTT_TOPIC_CARD_GPS_GET = properties.getProperty("mqtt_topic_card_gps_get");
        MQTT_TOPIC_CARD_MISSION_SET = properties.getProperty("mqtt_topic_card_mission_set");
        MQTT_TOPIC_CARD_INFO_GET_TEST = properties.getProperty("mqtt_tpoic_card_info_get_test");

    }

    public static final int ERROR_CODE = 0x00;
    public static final int CMD_EMPTY = 0xf0;
    public static final int CMD_MISSION = 0xf1;
    public static final int CMD_ORDER = 0xf2;
    public static final int CMD_GPS = 0xf3;

    public static final int CMD_MISSION_READ = (CMD_MISSION << 8);
    public static final int CMD_MISSION_WRITE= (CMD_MISSION << 8) | 0x01;

    public static final int CMD_ORDER_READ_BY_TRACK = (CMD_ORDER << 8);
    public static final int CMD_ORDER_READ_BY_MISSION = (CMD_ORDER << 8) | 0x02;
    public static final int CMD_ORDER_WRITE = (CMD_ORDER << 8) | 0x01;


    public static final int CMD_GPS_READ = (CMD_GPS << 8);
    public static final int CMD_GPS_WRITE = (CMD_GPS << 8) | 0x01;

    public static final String RESPONSE_SUCCESS = "[SUCCESS]";
    public static final String RESPONSE_ERROR = "[ERROR]";
}
