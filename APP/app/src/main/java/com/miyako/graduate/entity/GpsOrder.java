package com.miyako.graduate.entity;

import com.miyako.graduate.base.BaseBean;

public class GpsOrder extends BaseBean {

    private long id;
    private long utcTime;
    private String latitude;
    private String N_S;
    private String longitude;
    private String E_W;
    private String region;

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public long getUtcTime() {
        return utcTime;
    }

    public void setUtcTime(long utcTime) {
        this.utcTime = utcTime;
    }

    public String getLatitude() {
        return latitude;
    }

    public void setLatitude(String latitude) {
        this.latitude = latitude;
    }

    public String getN_S() {
        return N_S;
    }

    public void setN_S(String n_S) {
        N_S = n_S;
    }

    public String getLongitude() {
        return longitude;
    }

    public void setLongitude(String longitude) {
        this.longitude = longitude;
    }

    public String getE_W() {
        return E_W;
    }

    public void setE_W(String e_W) {
        E_W = e_W;
    }

    public String getRegion() {
        return region;
    }

    public void setRegion(String region) {
        this.region = region;
    }

    @Override
    public String toString() {
        return "GpsOrder{" +
                "id=" + id +
                ", utcTime=" + utcTime +
                ", latitude='" + latitude + '\'' +
                ", N_S='" + N_S + '\'' +
                ", longitude='" + longitude + '\'' +
                ", E_W='" + E_W + '\'' +
                ", region='" + region + '\'' +
                '}';
    }
}
