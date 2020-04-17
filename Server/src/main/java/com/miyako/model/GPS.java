package com.miyako.model;

import java.io.Serializable;

/**
 * ClassName Order
 * Description //TODO
 * Author Miyako
 * Date 2020-03-29-0029 13:09
 */
public class GPS implements Serializable, Cloneable {

    /** 主键 **/
    private int id;
    /** 时间 **/
    private long timestamp;
    /** 纬度 **/
    private String latitude;
    /** 经度 **/
    private String longitude;
    /** 地区 **/
    private String region;
    /** 任务外键 **/
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

    public String getLatitude() {
        return latitude;
    }

    public void setLatitude(String latitude) {
        this.latitude = latitude;
    }

    public String getLongitude() {
        return longitude;
    }

    public void setLongitude(String longitude) {
        this.longitude = longitude;
    }

    public String getRegion() {
        return region;
    }

    public void setRegion(String region) {
        this.region = region;
    }

    public String getMissionId() {
        return missionId;
    }

    public void setMissionId(String missionId) {
        this.missionId = missionId;
    }

    @Override
    public String toString(){
        return "GPS{" +
               "id=" + id +
               ", timestamp=" + timestamp +
               ", latitude='" + latitude + '\'' +
               ", longitude='" + longitude + '\'' +
               ", region='" + region + '\'' +
               ", missionId='" + missionId + '\'' +
               '}';
    }
}
