package com.miyako.dao;

import com.miyako.dao.OrderDao;
import com.miyako.model.Order;
import com.miyako.mysql.MySqlHelper;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.util.List;

/**
 * ClassName TestOrderDao
 * Description //TODO
 * Author Miyako
 * Date 2020-04-01-0001 15:32
 */
public class TestOrderDao{

    private static final String TAG = TestOrderDao.class.getSimpleName();

    @Before
    public void mysql() {
        LogUtil.DEBUG = true;
        LogUtil.d(TAG, "test before read mysql properties...");
        try {
            ServerApp.readConfig();
            LogUtil.d(TAG, "init mysql...");
            MySqlHelper.getInstance()
                       .init(ServerApp.MYSQL_JDBC, ServerApp.MYSQL_URL,
                             ServerApp.MYSQL_USERNAME, ServerApp.MYSQL_PASSWORD, true);
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    //@Test
    public void insertOrder(){
        LogUtil.d(TAG, "insertOrder");
        for(int i=0;i<20;i++) {
            Order data = new Order();
            data.setId(0);
            data.setTimestamp(System.currentTimeMillis());
            data.setSender("wlx"+i);
            data.setReceiver("yzh"+i);
            data.setAddress("四川-成都");
            data.setTrackId("465778"+i+"8783"+i);
            data.setMissionId("123456789-416421");
            int result = OrderDao.insert(data);
            LogUtil.d(TAG, "insertOrder result = " + result);
        }

    }

    //@Test
    public void updateOrder(){
        LogUtil.d(TAG, "updateOrder");
        List<Order> list = OrderDao.findByMissionId("123456789-416421");
        for(int i=0;i<list.size();i++) {
            Order data = list.get(i);
            data.setTimestamp(System.currentTimeMillis());
            data.setStatus("未完成");
            int result = OrderDao.update(data);
            LogUtil.d(TAG, "insertMission result = " + result);
        }

    }

    public void findByTrackId(){
        LogUtil.d(TAG, "findByTrackId");
        Order data = OrderDao.findByTrackId("454564");
        LogUtil.d(TAG, data != null ? data.toString() : null);
    }

    @Test
    public void findByIdOrder(){
        LogUtil.d(TAG, "findByIdOrder");
        Order data = OrderDao.findById(1);
        LogUtil.d(TAG, data != null ? data.toString() : null);
    }

    @Test
    public void findByAllOrder(){
        LogUtil.d(TAG, "findByAllOrder");
        List<Order> list = OrderDao.findAll();
        list.forEach(s -> LogUtil.d(TAG, s.toString()));
    }

    @Test
    public void findByMissionId(){
        LogUtil.d(TAG, "findByMissionId");
        List<Order> list = OrderDao.findByMissionId("123456789-416421");
        list.forEach(s -> LogUtil.d(TAG, s.toString()));
    }
}
