package com.miyako.graduation.socket;

import android.util.Log;
import android.util.SparseArray;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.miyako.graduation.base.Constants;
import com.miyako.graduation.socket.msg.BaseMsg;
import com.miyako.graduation.socket.msg.GpsMsg;
import com.miyako.graduation.socket.msg.MissionMsg;
import com.miyako.graduation.socket.msg.OrderMsg;
import com.miyako.graduation.socket.msg.QueryMsg;
import com.xuhao.didi.core.iocore.interfaces.ISendable;
import com.xuhao.didi.core.pojo.OriginalData;
import com.xuhao.didi.core.protocol.IReaderProtocol;
import com.xuhao.didi.socket.client.sdk.OkSocket;
import com.xuhao.didi.socket.client.sdk.client.ConnectionInfo;
import com.xuhao.didi.socket.client.sdk.client.OkSocketOptions;
import com.xuhao.didi.socket.client.sdk.client.action.SocketActionAdapter;
import com.xuhao.didi.socket.client.sdk.client.connection.IConnectionManager;

import java.lang.reflect.Type;
import java.nio.ByteOrder;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class SocketManager {

    private static final String TAG = SocketManager.class.getSimpleName();

    private static final class Obj {
        private static final SocketManager obj = new SocketManager();
    }

    private SocketManager() {}

    public static SocketManager getInstance() {return Obj.obj;}

    private Map<Integer, Type> mTypeMap;

    public SocketManager initTypeMap() {
        if (mTypeMap == null) {
            mTypeMap = new HashMap<>();
            mTypeMap.put(Constants.ERROR_CODE, null);
            mTypeMap.put(Constants.CMD_MISSION, new TypeToken<ResBody<MissionMsg>>() {}.getType());
            mTypeMap.put(Constants.CMD_ORDER, new TypeToken<ResBody<OrderMsg>>() {}.getType());
            mTypeMap.put(Constants.CMD_GPS, new TypeToken<ResBody<GpsMsg>>() {}.getType());
        }
        return this;
    }

    private IConnectionManager mManager;
    //连接参数设置(IP,端口号),这也是一个连接的唯一标识,不同连接,该参数中的两个值至少有其一不一样
    private ConnectionInfo info = new ConnectionInfo("192.168.1.102", 12345);

    private SparseArray<ISocketCall> callMap = new SparseArray<ISocketCall>();

    private void addCallMap(int id, ISocketCall socketCall) {
        Log.d(TAG, "add call id:"+id);
        callMap.put(id, socketCall);
    }

    private void removeCallMap(int id) {
        Log.d(TAG, "remove call id:"+id);
        callMap.remove(id);
    }

    public interface ISocketCall {
        public void success(List<BaseMsg> msg);
        public void error(int errorCode, List<BaseMsg> msg);
    }

    public void initSocket() {
        //调用OkSocket,开启这次连接的通道,拿到通道Manager
        mManager = OkSocket.open(info);
        //注册Socket行为监听器,SocketActionAdapter是回调的Simple类,其他回调方法请参阅类文档
        mManager.registerReceiver(new SocketActionAdapter(){
            @Override
            public void onSocketConnectionSuccess(ConnectionInfo info, String action) {
                //                Toast.makeText(this, "连接成功", Toast.LENGTH_SHORT).show();
                Log.d(TAG, "连接成功");
            }

            @Override
            public void onSocketReadResponse(ConnectionInfo info, String action, OriginalData data) {
                super.onSocketReadResponse(info, action, data);
                Log.d(TAG, "onSocketReadResponse");
                String res = new String(data.getBodyBytes());
                byte[] headBytes = data.getHeadBytes();
                Log.d(TAG, String.format("header byte:0x%02X%02X%02X%02X", headBytes[0], headBytes[1], headBytes[2], headBytes[3]));
                for(int i =0;i<data.getHeadBytes().length;i++) {
                    Log.d(TAG, String.format("header byte[%d]:0x%02X", i, data.getHeadBytes()[i]));
                }
                Log.d(TAG, "res body=>"+res);
                //                int type = (data.getHeadBytes()[2]&0xff<<8)|(data.getHeadBytes()[3]);
                int type = data.getHeadBytes()[2]&0xff;
                if (type==0xff) {
                    callMap.get(type).error(-233, null);
                    return;
                }
                Log.d(TAG, "call id:" + type);
                ResBody<BaseMsg> resBody = new Gson().fromJson(res, mTypeMap.get(type));
                //                MissionMsg msg = (MissionMsg) resBody.getData();
                Log.d(TAG, "msg:"+resBody.getData());
                if (callMap.get(type) != null) {
//                    if (resBody.getMessage().matches("^(\\[)SUCCESS(\\])$")) {
//                        callMap.get(type).success(resBody.getData());
//                    } else {
//                        if (resBody.getMessage().matches("^(\\[)ERROR(\\])$")) {
//                            callMap.get(type).error(-1, resBody.getData());
//                        }
//                    }
                    if (resBody.getMessage().contains("[SUCCESS]")) {
                        callMap.get(type).success(resBody.getData());
                    } else {
                        if (resBody.getMessage().contains("[ERROR]")) {
                            callMap.get(type).error(-1, resBody.getData());
                        }
                    }
                }

                removeCallMap(type);
            }
        });
        //调用通道进行连接
        mManager.connect();
        //设置自定义解析头
        OkSocketOptions mOkOptions = OkSocketOptions.getDefault();
        OkSocketOptions.Builder okOptionsBuilder = new OkSocketOptions.Builder(mOkOptions);
        okOptionsBuilder.setWritePackageBytes(1024);
        okOptionsBuilder.setReaderProtocol(new IReaderProtocol() {
            @Override
            public int getHeaderLength() {
                //返回自定义的包头长度,框架会解析该长度的包头
                return 8;
            }

            @Override
            public int getBodyLength(byte[] header, ByteOrder byteOrder) {
                //从header(包头数据)中解析出包体的长度,byteOrder是你在参配中配置的字节序,可以使用ByteBuffer比较方便解析
                int len =(header[4]&0xff)<<24
                        | (header[5]&0xff)<<16
                        | (header[6]&0xff)<<8
                        | (header[7]&0xff);
                Log.d(TAG, "body len:"+len);
                return len;
            }
        });
        //将新的修改后的参配设置给连接管理器
        mManager.option(okOptionsBuilder.build());
    }

    /**
     * 获取指定mission
     * @param missionId 唯一mission标识
     * @param socketCall 返回结果回调
     */
    public void getMission(String missionId, ISocketCall socketCall) {
        OkSocket.open(info)
                .send(new ISendable() {
                    @Override
                    public byte[] parse() {
                        addCallMap(Constants.CMD_MISSION, socketCall);
                        ReqBody<QueryMsg> reqBody = new ReqBody<>(Constants.CMD_MISSION<<8, System.currentTimeMillis(), "Android");
                        Gson gson = new Gson();
                        QueryMsg msg = new QueryMsg();
                        msg.setQueryId(missionId);
                        reqBody.setData(Collections.singletonList(msg));
                        String json = gson.toJson(reqBody);
                        Log.d(TAG, "send data json:"+json);
                        return json.getBytes();
                    }
                });
    }

    /**
     * 获取指定mission下的所有order
     * @param missionId 唯一mission标识
     * @param socketCall 返回结果回调
     */
    public void getOrderByMission(String missionId , ISocketCall socketCall) {
        OkSocket.open(info)
                .send(new ISendable() {
                    @Override
                    public byte[] parse() {
                        addCallMap(Constants.CMD_ORDER, socketCall);
                        ReqBody<QueryMsg> reqBody = new ReqBody<>(Constants.CMD_ORDER<<8|0x02, System.currentTimeMillis(), "Android");
                        Gson gson = new Gson();
                        QueryMsg msg = new QueryMsg();
                        msg.setQueryId(missionId);
                        reqBody.setData(Collections.singletonList(msg));
                        String json = gson.toJson(reqBody);
                        Log.d(TAG, "send data json:"+json);
                        return json.getBytes();
                    }
                });
    }

    /**
     * 获取指定order
     * @param trackId 唯一order标识
     * @param socketCall 返回结果回调
     */
    public void  getOrderByTrack(String trackId, ISocketCall socketCall) {
        OkSocket.open(info)
                .send(new ISendable() {
                    @Override
                    public byte[] parse() {
                        addCallMap(Constants.CMD_ORDER, socketCall);
                        ReqBody<QueryMsg> reqBody = new ReqBody<>(Constants.CMD_ORDER<<8, System.currentTimeMillis(), "Android");
                        Gson gson = new Gson();
                        QueryMsg msg = new QueryMsg();
                        msg.setQueryId(trackId);
                        reqBody.setData(Collections.singletonList(msg));
                        String json = gson.toJson(reqBody);
                        Log.d(TAG, "send data json:"+json);
                        return json.getBytes();
                    }
                });
    }

    /**
     * 获取指定order的所有Gps
     * @param orderId 唯一order标识，通过mission
     * @param socketCall 返回结果回调
     */
    public void getOrderGps(String orderId, ISocketCall socketCall) {
        OkSocket.open(info)
                .send(new ISendable() {
                    @Override
                    public byte[] parse() {
                        addCallMap(Constants.CMD_GPS, socketCall);
                        ReqBody<QueryMsg> reqBody = new ReqBody<>(Constants.CMD_GPS<<8, System.currentTimeMillis(), "Android");
                        Gson gson = new Gson();
                        QueryMsg msg = new QueryMsg();
                        msg.setQueryId(orderId);
                        reqBody.setData(Collections.singletonList(msg));
                        String json = gson.toJson(reqBody);
                        Log.d(TAG, "send data json:"+json);
                        return json.getBytes();
                    }
                });
    }
}
