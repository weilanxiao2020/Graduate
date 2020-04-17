package com.miyako.graduation.utils;

import android.util.Log;

import java.io.IOException;
import java.net.Socket;

public class SocketUtil {

    private static final String TAG = SocketUtil.class.getSimpleName();

    private static final class Obj {
        public static final SocketUtil obj = new SocketUtil();
    }

    public static SocketUtil getInstance() {return Obj.obj;}

    private SocketUtil(){}

    private ServerSocket server;
    private Thread mSendThread;
    private Thread mReceveThread;
    public SocketUtil setServer(String ip, int port) {
        try {
            server = new ServerSocket(ip, port);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return this;
    }

    public SocketUtil start() {
        Log.d(TAG, "socket start");
        reset();
        return this;
    }

    public SocketUtil reset() {
        Log.d(TAG, "socket reset");
        if (server != null ) {
            server.startThread();
        } else {
        }
        return this;
    }

    public SocketUtil close() {
        Log.d(TAG, "close");
        server.close();
        return this;
    }
}
