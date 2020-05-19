package com.miyako;

import com.miyako.dao.GPSDao;
import com.miyako.dao.MissionDao;
import com.miyako.dao.OrderDao;
import com.miyako.model.Mission;
import com.miyako.mqtt.MqttManager;
import com.miyako.mqtt.MqttTask;
import com.miyako.mysql.MySqlHelper;
import com.miyako.socket.MySocket;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;
import com.miyako.utils.VerifyUtil;
import io.reactivex.Observable;

import java.io.IOException;
import java.util.List;
import java.util.Random;
import java.util.concurrent.TimeUnit;

public class Main {

    private static final String TAG = Main.class.getSimpleName();

    public static void main(String[] args) {
	// write your code here
    //    LogUtil.DEBUG = true;
        LogUtil.init(true);
        try {
            ServerApp.readConfig();
            VerifyUtil.init();
            //String region = VerifyUtil.verfiyRegion(500115);
            //if (region.isEmpty()) {
            //    LogUtil.d(TAG, "校验失败");
            //}else{
            //    LogUtil.d(TAG, "校验成功");
            //    LogUtil.d(TAG, region);
            //}
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        MySqlHelper.getInstance()
                   .init(ServerApp.MYSQL_JDBC, ServerApp.MYSQL_URL,
                         ServerApp.MYSQL_USERNAME, ServerApp.MYSQL_PASSWORD, true);
        new MySocket(12345).start(); // 监听指定端口
        new MqttTask().start();
        LogUtil.i(TAG, "server is running...");

        //testMySql();
    }

    private static void testMySql() {
        try {
            Thread.sleep(5000);
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }
        for (int i = 0; i < 100; i++) {
            new Thread(new Runnable(){
                @Override
                public void run(){
                    long start = System.currentTimeMillis();
                    Random random = new Random(start);
                    int fi = random.nextInt();
                    switch (fi % 6) {
                        case 0: MissionDao.findByCard("12345678-416421");break;
                        case 1: OrderDao.findByMissionId("12345678-416421");break;
                        case 2: GPSDao.findByMissionId("12345678-416421");break;
                        case 3: GPSDao.findByMissionIdLast("12345678-416421");break;
                        case 4: GPSDao.findByMissionIdRegion("12345678-416421");break;
                    }
                    long end = System.currentTimeMillis();
                    System.out.println(String.format("T:%s test total time: %sms", Thread.currentThread().getName(), end-start));
                }
            }, "mysql-test"+i).start();
        }
    }
}
