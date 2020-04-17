package com.miyako.dao;

import com.miyako.dao.GPSDao;
import com.miyako.model.GPS;
import com.miyako.mysql.MySqlHelper;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.util.List;

/**
 * ClassName TestGpsDao
 * Description //TODO
 * Author Miyako
 * Date 2020-04-01-0001 15:33
 */
public class TestGpsDao{

    private static final String TAG = TestGpsDao.class.getSimpleName();

    @Before
    public void mysql() {
        LogUtil.DEBUG = true;
        LogUtil.d(TAG, "test before read mysql properties...");
        try {
            ServerApp.readConfig();
            LogUtil.d(TAG, "init mysql...");
            MySqlHelper.getInstance().init(ServerApp.MYSQL_JDBC, ServerApp.MYSQL_URL, ServerApp.MYSQL_USERNAME, ServerApp.MYSQL_PASSWORD);
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Test
    public void findByAllGPS(){
        LogUtil.d(TAG, "findByAllGPS");
        List<GPS> list = GPSDao.findAll();
        list.forEach(s -> LogUtil.d(TAG, s.toString()));
    }

    @Test
    public void findByMissionId() {
        LogUtil.d(TAG, "findByMissionId");
        List<GPS> list = GPSDao.findByMissionId("12345678-416421");
        list.forEach(s -> LogUtil.d(TAG, s.toString()));
    }

    private static void insertGPS(){

    }

    private static void updateGPS(){

    }
}
