package com.miyako;

import com.miyako.dao.MissionDao;
import com.miyako.model.Mission;
import com.miyako.mqtt.MqttManager;
import com.miyako.mqtt.MqttTask;
import com.miyako.mysql.MySqlHelper;
import com.miyako.socket.MySocket;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;
import com.miyako.utils.VerifyUtil;

import java.io.IOException;
import java.util.List;

public class Main {

    private static final String TAG = Main.class.getSimpleName();

    public static void main(String[] args) {
	// write your code here
        LogUtil.DEBUG = true;
        try {
            ServerApp.readConfig();
            VerifyUtil.init();
            if (VerifyUtil.verfiyRegion(320105)) {
                LogUtil.d(TAG, "校验成功");
            }else{
                LogUtil.d(TAG, "校验失败");
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        //MySqlHelper.getInstance()
        //           .init(ServerApp.MYSQL_JDBC, ServerApp.MYSQL_URL,
        //                 ServerApp.MYSQL_USERNAME, ServerApp.MYSQL_PASSWORD, true);
        //new MySocket(12345).start(); // 监听指定端口
        //new MqttTask().start();
        //LogUtil.d(TAG, "server is running...");
        //LogUtil.d(TAG, "findByAllMission");
        //List<Mission> list = MissionDao.findAll();
        //list.forEach(s -> LogUtil.d(TAG, s.toString()));
    }
}
