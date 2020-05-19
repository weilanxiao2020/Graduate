package com.miyako.task;

import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.reflect.TypeToken;
import com.miyako.dao.MissionDao;
import com.miyako.model.Mission;
import com.miyako.parse.ParseDao;
import com.miyako.result.ReqBody;
import com.miyako.result.ResBody;
import com.miyako.result.msg.MissionMsg;
import com.miyako.result.msg.QueryMsg;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;

import java.util.Collections;

/**
 * ClassName MissionTask
 * Description //TODO
 * Author Miyako
 * Date 2020-04-01-0001 15:57
 */
public class MissionTask extends BaseTask{

    private static final String TAG = MissionTask.class.getSimpleName();

    public MissionTask(long taskId, JsonElement body, long timestamp, String address){
        super(taskId, body, timestamp, address);
    }

    @Override
    protected String getTaskType(){
        return "MissionTask";
    }

    @Override
    public ResBody doTask(){
        LogUtil.i(TAG, "doTask");
        ResBody<MissionMsg> result;
        switch (getBody().getAsJsonObject().get("code").getAsInt()) {
            case ServerApp.CMD_MISSION_READ:
                result = readMission();
                break;
            case ServerApp.CMD_MISSION_WRITE:
                result = writeMission();
                break;
            default:
                result = null;
                break;
        }
        return result;
    }

    // code:0xf100，读取指定的mission
    private ResBody<MissionMsg> readMission(){
        Gson gson = new Gson();
        ReqBody<QueryMsg> reqBody = gson.fromJson(getBody(), new TypeToken<ReqBody<QueryMsg>>() {}.getType());
        // 从数据库读取原始mission数据
        String missionId = reqBody.getData().get(0).getQueryId();
        Mission mission = MissionDao.findByCard(missionId);
        if (mission == null) {
            return new ResBody<>(ServerApp.CMD_MISSION_READ, System.currentTimeMillis(),
                                 ServerApp.RESPONSE_ERROR + getAddress());
        }
        MissionMsg missionMsg = ParseDao.missionToMissionMsg(mission);
        ResBody<MissionMsg> resBody = new ResBody<>(ServerApp.CMD_MISSION_READ, System.currentTimeMillis(),
                                                ServerApp.RESPONSE_SUCCESS + getAddress());
        resBody.setData(Collections.singletonList(missionMsg));
        return resBody;
    }

    // code:0xf101，写入指定的mission
    private ResBody<MissionMsg> writeMission(){
        return new ResBody<>(ServerApp.CMD_MISSION_WRITE, System.currentTimeMillis(),
                             ServerApp.RESPONSE_ERROR + getAddress());
    }
}
