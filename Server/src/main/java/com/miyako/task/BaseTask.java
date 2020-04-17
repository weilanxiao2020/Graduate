package com.miyako.task;

import com.google.gson.JsonElement;
import com.miyako.result.ReqBody;
import com.miyako.result.msg.BaseMsg;
import com.miyako.result.ResBody;

/**
 * ClassName BaseTask
 * Description //TODO
 * Author Miyako
 * Date 2020-04-01-0001 15:58
 */
public abstract class BaseTask<T extends BaseMsg>{

    /** Id **/
    private long taskId;
    /** 类型：GPS，Mission，Order **/
    private String taskType;
    /** 模块号命令号 **/
    private JsonElement body;
    /** 时间戳 **/
    private long timestamp;
    /** 响应地址 **/
    private String address;

    public BaseTask(long taskId, JsonElement body, long timestamp, String address){
        this.taskId = taskId;
        this.body = body;
        this.timestamp = timestamp;
        this.address = address;
    }

    public long getTaskId(){
        return taskId;
    }

    public void setTaskId(long taskId){
        this.taskId = taskId;
    }

    public JsonElement getBody(){
        return body;
    }

    protected abstract String getTaskType();

    public void setTaskType(String taskType){
        this.taskType = taskType;
    }

    public long getTimestamp(){
        return timestamp;
    }

    public void setTimestamp(long timestamp){
        this.timestamp = timestamp;
    }

    public String getAddress(){
        return address;
    }

    public void setAddress(String address){
        this.address = address;
    }

    public abstract ResBody<T> doTask();

    @Override
    public String toString(){
        return "BaseTask{" +
               "taskId=" + taskId +
               ", taskType='" + taskType + '\'' +
               ", body=" + body +
               ", timestamp=" + timestamp +
               ", address='" + address + '\'' +
               '}';
    }
}
