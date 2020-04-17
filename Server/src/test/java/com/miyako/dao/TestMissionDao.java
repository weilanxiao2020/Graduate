package com.miyako.dao;

import com.miyako.dao.MissionDao;
import com.miyako.model.Mission;
import com.miyako.mysql.MySqlHelper;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.util.List;

/**
 * ClassName TestMissionDao
 * Description //TODO
 * Author Miyako
 * Date 2020-04-01-0001 15:31
 */
public class TestMissionDao {

    private static final String TAG = TestMissionDao.class.getSimpleName();

    @Before
    public void mysql() {
        LogUtil.DEBUG = true;
        LogUtil.d(TAG, "test before read mysql properties...");
        try {
            ServerApp.readConfig();
            LogUtil.d(TAG, "init mysql...");
            MySqlHelper.getInstance()
                       .init(ServerApp.MYSQL_JDBC, ServerApp.MYSQL_URL,
                             ServerApp.MYSQL_USERNAME, ServerApp.MYSQL_PASSWORD );
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Test
    public void findByAllMission(){
        LogUtil.d(TAG, "findByAllMission");
        List<Mission> list = MissionDao.findAll();
        list.forEach(s -> LogUtil.d(TAG, s.toString()));
    }

    @Test
    public void findByIdMission(){
        LogUtil.d(TAG, "findByIdMission");
        Mission data = MissionDao.findById(3);
        LogUtil.d(TAG, data != null ? data.toString() : null);
    }

    @Test
    public void findByCardMission(){
        LogUtil.d(TAG, "findByCardMission");
        Mission data = MissionDao.findByCard("123456789-416421");
        LogUtil.d(TAG, data != null ? data.toString() : null);
    }

    public void insertMission(){
        LogUtil.d(TAG, "insertMission");
        Mission data = new Mission();
        data.setId(0);
        data.setTimestamp(System.currentTimeMillis());
        data.setLicense("wlx2");
        data.setCode("416421");
        data.setAddress("成都2");
        data.setStatus("未完成");
        int result = MissionDao.insert(data);
        LogUtil.d(TAG, "insertMission result = " + result);
    }

    public void updateMission(){
        LogUtil.d(TAG, "updateMission");
        Mission data = MissionDao.findByCard("asdad51-0");
        data.setTimestamp(System.currentTimeMillis());
        data.setLicense("945134721");
        data.setCode("416421");
        data.setAddress("濮阳");
        data.setStatus("已完成");
        int result = MissionDao.update(data);
        LogUtil.d(TAG, "insertMission result = " + result);
    }
}
