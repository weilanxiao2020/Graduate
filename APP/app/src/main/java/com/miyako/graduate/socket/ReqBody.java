package com.miyako.graduate.socket;


import com.miyako.graduate.socket.msg.BaseMsg;

/**
 * ClassName ReqBody
 * Description //TODO
 * Author Miyako
 * Date 2020-03-27-0027 11:11
 */
public class ReqBody<T extends BaseMsg> extends Result<T> {

    public ReqBody(int code, long timestamp, String message) {
        super(code, timestamp, message);
    }
}
