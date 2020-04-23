package com.miyako.graduate.entity;

import com.miyako.graduate.base.BaseBean;

/**
 * @Description
 * @Author Miyako
 * @Date 2020-02-29-0029
 */
public class StaffOrder extends BaseBean {

    private String id;
    private String address;
    private String receiver;
    private long date;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getReceiver() {
        return receiver;
    }

    public void setReceiver(String receiver) {
        this.receiver = receiver;
    }

    public long getDate() {
        return date;
    }

    public void setDate(long date) {
        this.date = date;
    }

    @Override
    public String toString() {
        return "StaffOrder{" +
                "id=" + id +
                ", address='" + address + '\'' +
                ", receiver='" + receiver + '\'' +
                ", date=" + date +
                '}';
    }
}
