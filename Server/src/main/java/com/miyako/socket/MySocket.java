package com.miyako.socket;

import com.google.gson.*;
import com.google.gson.reflect.TypeToken;
import com.google.gson.stream.JsonReader;
import com.miyako.model.Mission;
import com.miyako.result.Result;
import com.miyako.result.msg.BaseMsg;
import com.miyako.result.msg.OrderMsg;
import com.miyako.result.ReqBody;
import com.miyako.result.ResBody;
import com.miyako.task.EmptyTask;
import com.miyako.task.GpsTask;
import com.miyako.task.MissionTask;
import com.miyako.task.OrderTask;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * ClassName MySocket
 * Description //TODO
 * Author Miyako
 * Date 2020-04-01-0001 15:28
 */
public class MySocket{

    private static final String TAG = MySocket.class.getSimpleName();

    private ServerSocket server;
    private int serverPort;
    private List<String> clientIp;
    private List<Integer> clientPort;

    public MySocket(int serverPort){
        this.serverPort = serverPort;
        try {
            server = new ServerSocket(serverPort);
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void start(){
        try {
            LogUtil.d(TAG, "server local host: " + server.getLocalSocketAddress() + ":[" + server.getLocalPort() + "]");
            LogUtil.d(TAG, "server host: " + server.getInetAddress().getHostAddress() + ":[" + server.getLocalPort() + "]");
            LogUtil.d(TAG, "server start...");
            running();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void running() throws IOException{
        LogUtil.d(TAG, "server running...");
        new Thread(new Runnable(){
            @Override
            public void run(){
                while (true) {
                    try {
                        Socket client = server.accept();
                        LogUtil.d(TAG, "connected from " + client.getRemoteSocketAddress());
                        connect(client);
                    }
                    catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        },"socket-thread").start();
    }

    private void handle(InputStream input, OutputStream output) throws IOException{
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(output, StandardCharsets.UTF_8));
        BufferedReader reader = new BufferedReader(new InputStreamReader(input, StandardCharsets.UTF_8));
        writer.write("hello\n");
        writer.flush();
        for (; ; ) {
            String s = reader.readLine();
            if (s != null) {
                LogUtil.i(TAG, "<<< " + s);
                if (s.equals("exit")) {
                    //                writer.write("bye\n");
                    //                writer.flush();
                    break;
                }
                writer.write("ok: " + s + "\n");
                writer.flush();
            }
        }
    }

    //private ResBody<GpsMsg> sendGpsJson(String ip){
    //    GpsMsg gpsMsg = new GpsMsg();
    //    gpsMsg.setUtcTime(System.currentTimeMillis() + "");
    //    gpsMsg.setLatitude("30.3126719672");
    //    gpsMsg.setN_S("N");
    //    gpsMsg.setLongitude("120.3566998243");
    //    gpsMsg.setE_W("E");
    //    ResBody<GpsMsg> resBody = new ResBody<>(ServerApp.CMD_GPS, System.currentTimeMillis(),
    //                                            ServerApp.RESPONSE_SUCCESS + ip);
    //    resBody.setData(gpsMsg);
    //
    //    return resBody;
    //}


    // 解析对象
    public static Result readMessage(JsonReader reader) {
        ReqBody req = new ReqBody();
        try {
            reader.beginObject();
            while (reader.hasNext()) {
                String name = reader.nextName();
                if ("code".equals(name)) {
                    req.setCode(reader.nextInt());
                } else if ("timestamp".equals(name)) {
                    req.setTimestamp(reader.nextLong());
                } else if ("message".equals(name)) {
                    req.setMessage(reader.nextString());
                } else {
                    reader.skipValue();
                }
            }
            reader.endObject();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return req;
    }

    /**
     * 对手机App发送的json进行解析
     * @param ip 终端ip地址
     * @param json 终端发送的json数据
     * @param head 发送给终端的头部信息
     * @return 返回的操作结果json
     */
    private String handleJson(String ip, String json, byte[] head){
        LogUtil.i(TAG, "handle json:<<< " + ip + ":" + json);
        Gson gson = new Gson();
        ReqBody<BaseMsg> reqBody = null;
        ResBody resJson = null;
        try {
            // json解析器，解析json数据
            JsonParser parser = new JsonParser();
            JsonElement element = parser.parse(json);
            // json属于对象类型时
            if (element.isJsonObject()) {
                JsonObject object = element.getAsJsonObject();  // 转化为对象
                // 1. value为string时，取出string
                int code = object.get("code").getAsInt();
                LogUtil.d(TAG, "request code:"+code);
                // reqBody.getCode:0xf1 01
                // head = {0x0e,0x0e,模块号,命令号}
                head[2] = (byte) (code >> 8 & 0xff);
                head[3] = (byte) (code & 0xff);
                switch (code>>8 & 0xff) {
                    case ServerApp.CMD_EMPTY:
                        resJson = new EmptyTask(Thread.currentThread().getId(), element, System.currentTimeMillis(), ip).doTask();
                        break;
                    case ServerApp.CMD_MISSION:
                        resJson = new MissionTask(Thread.currentThread().getId(), element, System.currentTimeMillis(), ip).doTask();
                        break;
                    case ServerApp.CMD_ORDER:
                        resJson = new OrderTask(Thread.currentThread().getId(), element, System.currentTimeMillis(), ip).doTask();
                        break;
                    case ServerApp.CMD_GPS:
                        resJson = new GpsTask(Thread.currentThread().getId(), element, System.currentTimeMillis(), ip).doTask();
                        break;
                }
            }
        } catch (JsonSyntaxException ex) {
            LogUtil.e(TAG, "request json is error");
            ex.printStackTrace();
            head[2] = (byte) (0xff);
            head[3] = (byte) (0xff);
            resJson = new ResBody(ServerApp.ERROR_CODE, System.currentTimeMillis(), ServerApp.RESPONSE_ERROR+ip);
        }
        return gson.toJson(resJson);
    }

    private Map<String, HashMap<String, Integer>> socketTask;

    private Map<String, Integer> gpsTask;
    /**
     * 对每一个客户端开启一个新线程
     *
     * @param client
     */
    private void connect(Socket client){
        new Thread(new Runnable(){
            @Override
            public void run(){
                InputStream input = null;
                OutputStream output = null;
                try {
                    input = client.getInputStream();
                    output = client.getOutputStream();
                    BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(output, StandardCharsets.UTF_8));
                    BufferedReader reader = new BufferedReader(new InputStreamReader(input, StandardCharsets.UTF_8));
                    String receive;
                    int headLen = 0;
                    byte[] buf = new byte[1024];
                    while ((headLen = input.read(buf)) != -1) {
                        //if (1==1) {
                        //    //byte[] mission = "12345678-416421".getBytes();
                        //    //byte[] data = new byte[3+mission.length];
                        //    //data[0] = (byte) 0xfe;
                        //    //data[1] = 0x01;
                        //    //data[2] = (byte) mission.length;
                        //    //System.arraycopy(mission, 0, data, 3, mission.length);
                        //    //output.write(data);
                        //    LogUtil.d(TAG, "read:"+new String(buf));
                        //    output.write("hello stm32".getBytes());
                        //    output.flush();
                        //    continue;
                        //}
                        byte[] header = new byte[] {0x0e, 0x0e, 0x0f, 0x0f};
                        LogUtil.d(TAG, "read length:"+headLen);
                        String json = handleJson(client.getInetAddress().getHostAddress(),
                                                 new String(buf, StandardCharsets.UTF_8).trim(), header);
                        LogUtil.i(TAG, ">>> " + client.getInetAddress().getHostAddress() + ":" + json);
                        byte[] body = json.getBytes(StandardCharsets.UTF_8);
                        byte[] len = new byte[] {0x00, 0x00, 0x00, 0x00};
                        len[0] = (byte) ((body.length & 0xff000000) >> 24);
                        len[1] = (byte) ((body.length & 0x00ff0000) >> 16);
                        len[2] = (byte) ((body.length & 0x0000ff00) >> 8);
                        len[3] = (byte) ((body.length & 0x000000ff));
                        LogUtil.d(TAG, String.format("body len:0x%02X%02X%02X%02X", len[0], len[1], len[2], len[3]));
                        LogUtil.d(TAG, "body len:" + body.length);
                        output.write(header);
                        output.write(len);
                        output.write(body);
                        output.flush();
                        Arrays.fill(buf, (byte) 0);
                    }
                    //                    while((receive=reader.readLine())!=null){
                    //                        LogUtil.i(TAG, "<<< "+client.getInetAddress().getHostAddress()+":"+receive);
                    //                        String json = sendOrderJson();
                    //                        LogUtil.i(TAG, ">>> "+client.getInetAddress().getHostAddress()+":"+json);
                    ////                        writer.write(json);
                    ////                        writer.newLine();
                    ////                        writer.flush();
                    //                        byte[] header = new byte[]{0x0e,0x0e,0x0f,0x04};
                    //                        byte[] body = json.getBytes(StandardCharsets.UTF_8);
                    //                        byte[] len = new byte[4];
                    //                        len[0] = (byte) ((body.length&0xff000000)>>24);
                    //                        len[1] = (byte) ((body.length&0x00ff0000)>>16);
                    //                        len[2] = (byte) ((body.length&0x0000ff00)>>8);
                    //                        len[3] = (byte) ((body.length&0x000000ff)>>0);
                    //                        LogUtil.d(TAG, "body len:"+body.length);
                    //                        output.write(header);
                    //                        output.write(len);
                    //                        output.write(body);
                    //                        output.flush();
                    //                    }
                    //关闭相对应的资源
                    reader.close();
                    writer.close();
                    input.close();
                    output.close();
                    client.close();
                }
                catch (IOException e) {
                    e.printStackTrace();
                }
                LogUtil.e(TAG, "关闭socket");
            }
        }, "client-" + client.hashCode()).start();
    }
}