package com.miyako.graduation.socket.msg;

/**
 * ClassName OrderMsg
 * Description //socket通信数据结构--Order数据
 * Author Miyako
 * Date 2020-03-22-0022 16:47
 */
public class OrderMsg extends BaseMsg{

    private String trackId;
    private long timestamp;
    private String sender;
    private String receiver;
    private String address;
    private String status;
    private String missionId;

    public String getTrackId() {
        return trackId;
    }

    public void setTrackId(String trackId) {
        this.trackId = trackId;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public long getTimespan() {
        return timestamp;
    }

    public void setTimespan(long timestamp) {
        this.timestamp = timestamp;
    }

    public String getSender() {
        return sender;
    }

    public void setSender(String sender) {
        this.sender = sender;
    }

    public String getReceiver() {
        return receiver;
    }

    public void setReceiver(String receiver) {
        this.receiver = receiver;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getStatus(){
        return status;
    }

    public void setStatus(String status){
        this.status = status;
    }

    public String getMissionId(){
        return missionId;
    }

    public void setMissionId(String missionId){
        this.missionId = missionId;
    }

    @Override
    public String toString(){
        return "OrderMsg{" +
               "trackId='" + trackId + '\'' +
               ", timestamp=" + timestamp +
               ", sender='" + sender + '\'' +
               ", receiver='" + receiver + '\'' +
               ", address='" + address + '\'' +
               ", status='" + status + '\'' +
               ", missionId='" + missionId + '\'' +
               '}';
    }
}
