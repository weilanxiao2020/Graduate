package com.miyako;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.miyako.dao.GPSDao;
import com.miyako.model.GPS;
import com.miyako.mysql.MySqlHelper;
import com.miyako.parse.ParseDao;
import com.miyako.result.ReqBody;
import com.miyako.result.ResBody;
import com.miyako.result.msg.GpsMsg;
import com.miyako.result.msg.QueryMsg;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;
import com.miyako.utils.VerifyUtil;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * ClassName TestTask
 * Description //TODO
 * Author Miyako
 * Date 2020-05-07-0007 10:39
 */
public class TestTask{

    private static final String TAG = TestTask.class.getSimpleName();

    @Before
    public void mysql() {
        LogUtil.DEBUG = true;
        LogUtil.d(TAG, "test before read mysql properties...");
        try {
            ServerApp.readConfig();
            VerifyUtil.init();
            LogUtil.d(TAG, "init mysql...");
            MySqlHelper.getInstance().init(ServerApp.MYSQL_JDBC, ServerApp.MYSQL_URL, ServerApp.MYSQL_USERNAME, ServerApp.MYSQL_PASSWORD);
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Test
    public void testGps() {
        Gson gson = new Gson();
        String mission = "";
        ResBody<GpsMsg> resBody = null;
        List<GPS> gpsList = GPSDao.findByMissionIdRegion(mission);
        if (gpsList==null) {
            resBody = new ResBody<>(ServerApp.CMD_GPS_READ, System.currentTimeMillis(),
                                 ServerApp.RESPONSE_ERROR + "127.0.0.1");
        }
        List<GpsMsg> msgList = new ArrayList<>(gpsList.size());
        for (GPS gps : gpsList) {
            GpsMsg msg = ParseDao.gpsToGpsMsg(gps);
            try {
                msg.setRegion(VerifyUtil.verfiyRegion(Integer.valueOf(gps.getRegion())));
            }catch (Exception ex) {
                msg.setRegion("none");
            }
            msgList.add(msg);
        }
        resBody = new ResBody<>(ServerApp.CMD_GPS_READ, System.currentTimeMillis(),
                                                ServerApp.RESPONSE_SUCCESS + "127.0.0.1");

        resBody.setData(msgList);
        LogUtil.d(TAG, resBody.toString());
    }


}
