package com.miyako.graduate.socket.msg;

/**
 * ClassName GpsMsg
 * Description //socket通信数据结构--GPS数据
 * Author Miyako
 * Date 2020-03-27-0027 11:18
 */
public class GpsMsg extends BaseMsg{

    private int id;
    private String missionId;
    private String utcTime;
    private String latitude;
    private String N_S;
    private String longitude;
    private String E_W;
    private String region;

    public int getId(){
        return id;
    }

    public void setId(int id){
        this.id = id;
    }

    public String getMissionId(){
        return missionId;
    }

    public void setMissionId(String missionId){
        this.missionId = missionId;
    }

    public String getUtcTime() {
        return utcTime;
    }

    public void setUtcTime(String utcTime) {
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
        return "GpsMsg{" +
                "id=" + id +
                ", missionId='" + missionId + '\'' +
                ", utcTime='" + utcTime + '\'' +
                ", latitude='" + latitude + '\'' +
                ", N_S='" + N_S + '\'' +
                ", longitude='" + longitude + '\'' +
                ", E_W='" + E_W + '\'' +
                ", region='" + region + '\'' +
                '}';
    }
}
