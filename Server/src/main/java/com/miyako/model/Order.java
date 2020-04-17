package com.miyako.model;

import java.io.Serializable;

/**
 * ClassName Order
 * Description //TODO
 * Author Miyako
 * Date 2020-03-29-0029 13:09
 */
public class Order implements Serializable, Cloneable {

    /** 主键 **/
    private int id;
    /** 时间 **/
    private long timestamp;
    /** 订单号 **/
    private String trackId;
    /** 发送者 **/
    private String sender;
    /** 接收者 **/
    private String receiver;
    /** 目的 **/
    private String address;
    /** 状态 **/
    private String status;
    /** 运送任务id **/
    private String missionId;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public String getTrackId() {
        return trackId;
    }

    public void setTrackId(String trackId) {
        this.trackId = trackId;
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

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
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
        return "Order{" +
               "id=" + id +
               ", timestamp=" + timestamp +
               ", trackId='" + trackId + '\'' +
               ", sender='" + sender + '\'' +
               ", receiver='" + receiver + '\'' +
               ", address='" + address + '\'' +
               ", status='" + status + '\'' +
               ", missionId='" + missionId + '\'' +
               '}';
    }
}
