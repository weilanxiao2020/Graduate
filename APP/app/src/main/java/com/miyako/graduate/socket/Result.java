package com.miyako.graduate.socket;

import com.google.gson.annotations.SerializedName;
import com.miyako.graduate.socket.msg.BaseMsg;

import java.util.List;

/**
 * ClassName Result
 * Description //TODO
 * Author Miyako
 * Date 2020-03-22-0022 16:41
 */
public abstract class Result<T extends BaseMsg> {

    // 模块号命令号
    @SerializedName("code")
    private int code;
    // 时间戳
    @SerializedName("timestamp")
    private long timestamp;
    // 消息描述
    @SerializedName("message")
    private String message;

    @SerializedName("data")
    private List<T> data;

    public Result(int code, long timestamp, String message) {
        this.code = code;
        this.timestamp = timestamp;
        this.message = message;
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public List<T> getData() {
        return data;
    }

    public void setData(List<T> data) {
        this.data = data;
    }

    @Override
    public String toString() {
        return "Result{" +
                "code=" + code +
                ", timestamp=" + timestamp +
                ", message='" + message + '\'' +
                ", data=" + data +
                '}';
    }
}
