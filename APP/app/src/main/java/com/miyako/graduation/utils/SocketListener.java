package com.miyako.graduation.utils;

import android.util.Log;

import com.xuhao.didi.core.iocore.interfaces.IPulseSendable;
import com.xuhao.didi.core.iocore.interfaces.ISendable;
import com.xuhao.didi.core.pojo.OriginalData;
import com.xuhao.didi.socket.client.sdk.client.ConnectionInfo;
import com.xuhao.didi.socket.client.sdk.client.action.ISocketActionListener;

public class SocketListener implements ISocketActionListener {

    private static final String TAG = SocketListener.class.getSimpleName();
    @Override
    public void onSocketIOThreadStart(String s) {

    }

    @Override
    public void onSocketIOThreadShutdown(String s, Exception e) {

    }

    @Override
    public void onSocketReadResponse(ConnectionInfo connectionInfo, String s, OriginalData originalData) {
        Log.d(TAG, "onSocketReadResponse");
        String res = new String(originalData.getBodyBytes());
        Log.d(TAG, "res body=>"+res);
    }

    @Override
    public void onSocketWriteResponse(ConnectionInfo connectionInfo, String s, ISendable iSendable) {
        Log.d(TAG, "onSocketWriteResponse");
    }

    @Override
    public void onPulseSend(ConnectionInfo connectionInfo, IPulseSendable iPulseSendable) {

    }

    @Override
    public void onSocketDisconnection(ConnectionInfo connectionInfo, String s, Exception e) {

    }

    @Override
    public void onSocketConnectionSuccess(ConnectionInfo connectionInfo, String s) {

    }

    @Override
    public void onSocketConnectionFailed(ConnectionInfo connectionInfo, String s, Exception e) {

    }
}
