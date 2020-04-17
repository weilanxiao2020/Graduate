package com.miyako.graduation.utils;

import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public class ServerSocket {

    private static final String TAG = ServerSocket.class.getSimpleName();
    private String serverIp;
    private int serverPort;
    private final Socket socket;
    private boolean isClosed;

    private final Thread mSendThread;
    private final Thread mReceiveThread;

    public ServerSocket(String ip, int port) throws IOException {
        this.serverIp = ip;
        this.serverPort = port;
        socket = new Socket(serverIp, serverPort);
        isClosed = false;
        mSendThread = new Thread(new SendThread(socket));
        mReceiveThread = new Thread(new ReceiveThread(socket));
    }

    public void startThread() {
        mSendThread.start();
        mReceiveThread.start();
    }

    public void close() {
        try {
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static final class SendThread implements Runnable {

        private final Socket socket;

        public SendThread(final Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            InputStream input = null;
            OutputStream output = null;
            BufferedWriter writer = null;
            try {
                input = socket.getInputStream();
                output = socket.getOutputStream();
                writer = new BufferedWriter(new OutputStreamWriter(output, StandardCharsets.UTF_8));
                //            Scanner cin = new Scanner(System.in);
                for(;;){
                    if (socket.isClosed()) {
                        break;
                    }
                    //                System.out.print(">>> "); // 打印提示
                    //                String send = cin.nextLine(); // 读取一行输入
                    //                if(!send.equals("send")&&!send.equals("exit")) writer.write(send);
                    //                else if(send.equals("exit")) {
                    //                    // 退出socket
                    //                    LogUtil.d(TAG, "退出socket");
                    //                    break;
                    //                } else {
                    //                    // "send"
                    //                    writer.newLine();
                    //                    writer.flush();
                    //                    writer.write(sendJson());
                    //                    writer.newLine();
                    //                    writer.flush();
                    //                }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }finally {
                try{
                    if (writer!=null) writer.close();
                    if (input!=null) input.close();
                    if (output!=null) output.close();
                    if (socket!=null) socket.close();
                }catch (Exception ex){
                    ex.printStackTrace();
                }
                Log.e(TAG, "close");
            }
        }
    }

    private final class ReceiveThread implements Runnable {

        private final Socket socket;
        public  ReceiveThread(final Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            BufferedReader reader = null;
            try {
                reader = new BufferedReader(new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8));
            } catch (IOException e) {
                e.printStackTrace();
            }
            String receive;
            for (;;) {
                try {
                    if (socket.isClosed()) {
                        break;
                    }
                    receive = reader.readLine();
                    Log.d(TAG, "<<< "+socket.getLocalAddress()+":"+receive);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
