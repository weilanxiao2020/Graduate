package com.miyako.result;

import com.miyako.result.msg.BaseMsg;

/**
 * ClassName ReqBody
 * Description //TODO
 * Author Miyako
 * Date 2020-03-27-0027 11:11
 */
public class ReqBody<T extends BaseMsg> extends Result<T> {

    public ReqBody() {}

    public ReqBody(int code, long timestamp, String message) {
        super(code, timestamp, message);
    }
}
