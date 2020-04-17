package com.miyako.model;

import java.io.Serializable;

/**
 * ClassName Mission
 * Description //TODO
 * Author Miyako
 * Date 2020-03-28-0028 14:52
 */
public class Mission implements Serializable, Cloneable{

    /** 主键 **/
    private int id;
    /** 时间 **/
    private long timestamp;
    /** 车辆标识 **/
    private String license;
    /** 验证码 **/
    private String code;
    /** 目的 **/
    private String address;
    /** 状态 **/
    private String status;

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

    public String getLicense() {
        return license;
    }

    public void setLicense(String license) {
        this.license = license;
    }

    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
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

    @Override
    public String toString() {
        return "Mission{" +
                "id=" + id +
                ", timestamp=" + timestamp +
                ", license='" + license + '\'' +
                ", code='" + code + '\'' +
                ", address='" + address + '\'' +
                ", status='" + status + '\'' +
                '}';
    }
}