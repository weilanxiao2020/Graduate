package com.miyako.task;

import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.reflect.TypeToken;
import com.miyako.result.ReqBody;
import com.miyako.result.ResBody;
import com.miyako.result.msg.BaseMsg;
import com.miyako.utils.LogUtil;

/**
 * ClassName EmptyTask
 * Description //TODO
 * Author Miyako
 * Date 2020-04-03-0003 16:33
 */
public class EmptyTask extends BaseTask{

    private static final String TAG = EmptyTask.class.getSimpleName();

    public EmptyTask(long taskId, JsonElement body, long timestamp, String address){
        super(taskId, body, timestamp, address);
    }

    @Override
    protected String getTaskType(){
        return "EmptyTask";
    }

    @Override
    public ResBody doTask(){
        LogUtil.d(TAG, "doTask");
        return null;
    }
}
