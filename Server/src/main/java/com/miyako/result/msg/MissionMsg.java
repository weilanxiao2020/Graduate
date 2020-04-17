package com.miyako.result.msg;

/**
 * ClassName MissionMsg
 * Description //socket通信数据结构--Mission数据
 * Author Miyako
 * Date 2020-04-02-0002 15:06
 */
public class MissionMsg extends BaseMsg{

    private String missionId;
    private long timestamp;
    private String address;
    private String status;

    public String getMissionId(){
        return missionId;
    }

    public void setMissionId(String missionId){
        this.missionId = missionId;
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

    public String getStatus(){
        return status;
    }

    public void setStatus(String status){
        this.status = status;
    }

    @Override
    public String toString(){
        return "MissionMsg{" +
               "missionId='" + missionId + '\'' +
               ", timestamp=" + timestamp +
               ", address='" + address + '\'' +
               ", status='" + status + '\'' +
               '}';
    }
}
