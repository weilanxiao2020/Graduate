package com.miyako.graduate.base;

import android.app.Application;
import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.miyako.graduate.socket.ResBody;
import com.miyako.graduate.socket.SocketManager;
import com.miyako.graduate.socket.msg.BaseMsg;
import com.miyako.graduate.socket.msg.GpsMsg;
import com.miyako.graduate.socket.msg.MissionMsg;
import com.miyako.graduate.socket.msg.OrderMsg;
import com.xuhao.didi.core.pojo.OriginalData;
import com.xuhao.didi.core.protocol.IReaderProtocol;
import com.xuhao.didi.socket.client.sdk.OkSocket;
import com.xuhao.didi.socket.client.sdk.client.ConnectionInfo;
import com.xuhao.didi.socket.client.sdk.client.OkSocketOptions;
import com.xuhao.didi.socket.client.sdk.client.action.SocketActionAdapter;
import com.xuhao.didi.socket.client.sdk.client.connection.IConnectionManager;

import java.lang.reflect.Type;
import java.nio.ByteOrder;
import java.util.HashMap;
import java.util.Map;

public class MyApplication extends Application {

    private static final String TAG = MyApplication.class.getSimpleName();

    @Override
    public void onCreate() {
        super.onCreate();
//        ToastUtil.controlToast(true);
        SocketManager.getInstance()
                .initTypeMap()
                .initSocket();
    }
}
