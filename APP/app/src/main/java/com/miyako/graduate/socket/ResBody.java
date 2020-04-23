package com.miyako.graduate.socket;


import com.miyako.graduate.socket.msg.BaseMsg;

/**
 * ClassName ResBody
 * Description //TODO
 * Author Miyako
 * Date 2020-03-27-0027 11:12
 */
public class ResBody<T extends BaseMsg> extends Result<T> {

    public ResBody(int code, long timestamp, String message) {
        super(code, timestamp, message);
    }
}
