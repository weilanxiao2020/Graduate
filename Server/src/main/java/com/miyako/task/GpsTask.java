package com.miyako.task;

import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.reflect.TypeToken;
import com.miyako.dao.GPSDao;
import com.miyako.dao.OrderDao;
import com.miyako.model.GPS;
import com.miyako.model.Mission;
import com.miyako.model.Order;
import com.miyako.result.ReqBody;
import com.miyako.result.msg.BaseMsg;
import com.miyako.result.msg.GpsMsg;
import com.miyako.result.ResBody;
import com.miyako.result.msg.OrderMsg;
import com.miyako.result.msg.QueryMsg;
import com.miyako.utils.ConvertUtil;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * ClassName GpsTask
 * Description //TODO
 * Author Miyako
 * Date 2020-04-01-0001 15:57
 */
public class GpsTask extends BaseTask{

    private static final String TAG = GpsTask.class.getSimpleName();

    public GpsTask(long taskId, JsonElement body, long timestamp, String address){
        super(taskId, body, timestamp, address);
    }

    @Override
    protected String getTaskType(){
        return "GpsTask";
    }

    @Override
    public ResBody doTask(){
        LogUtil.d(TAG, "doTask");
        ResBody<GpsMsg> result;
        switch (getBody().getAsJsonObject().get("code").getAsInt()) {
            case ServerApp.CMD_GPS_READ:
                result = readGps();
                break;
            case ServerApp.CMD_GPS_WRITE:
                result = writeGps();
                break;
            default:
                result = null;
                break;
        }
        return result;
    }

    // code:0xf300，读取指定mission的gps数据
    private ResBody<GpsMsg> readGps() {
        // 从数据库读取原始gps数据
        Gson gson = new Gson();
        ReqBody<QueryMsg> reqBody = gson.fromJson(getBody(), new TypeToken<ReqBody<QueryMsg>>() {}.getType());
        String mission = reqBody.getData().get(0).getQueryId();
        List<GPS> gpsList = GPSDao.findByMissionId(mission);
        if (gpsList==null) {
            return new ResBody<>(ServerApp.CMD_GPS_READ, System.currentTimeMillis(),
                                 ServerApp.RESPONSE_ERROR + getAddress());
        }
        List<GpsMsg> msgs = new ArrayList<>(gpsList.size());
        for (GPS gps : gpsList) {
            GpsMsg msg = new GpsMsg();
            ConvertUtil.gpsToGpsMsg(gps, msg);
            msgs.add(msg);
        }
        ResBody<GpsMsg> resBody = new ResBody<>(ServerApp.CMD_GPS_READ, System.currentTimeMillis(),
                                                ServerApp.RESPONSE_SUCCESS + getAddress());

        resBody.setData(msgs);
        return resBody;
    }

    // code:0xf301，写入mission的gps数据
    private ResBody<GpsMsg> writeGps() {
        //// 从数据库读取原始gps数据
        //String missionId = ((GpsMsg) getBody().getData()).getMissionId();
        //GPS gps = GPSDao.findByMissionId(Integer.valueOf(missionId));
        Gson gson = new Gson();
        ReqBody<GpsMsg> reqBody = gson.fromJson(getBody(), new TypeToken<ReqBody<QueryMsg>>() {}.getType());
        GpsMsg gpsMsg = reqBody.getData().get(0);
        GPS gps = new GPS();
        ResBody<GpsMsg> resBody;
        if (gpsMsg == null) {
            resBody = new ResBody<>(ServerApp.CMD_GPS_WRITE, System.currentTimeMillis(),
                                                    ServerApp.RESPONSE_ERROR + getAddress());
        } else {
            ConvertUtil.gpsMsgToGps(gpsMsg, gps);
            resBody = new ResBody<>(ServerApp.CMD_GPS_WRITE, System.currentTimeMillis(),
                                                    ServerApp.RESPONSE_SUCCESS + getAddress());

        }
        resBody.setData(Collections.singletonList(gpsMsg));
        return resBody;
    }
}
