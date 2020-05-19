package com.miyako.task;

import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.reflect.TypeToken;
import com.miyako.dao.OrderDao;
import com.miyako.model.Order;
import com.miyako.parse.ParseDao;
import com.miyako.result.ReqBody;
import com.miyako.result.ResBody;
import com.miyako.result.msg.OrderMsg;
import com.miyako.result.msg.QueryMsg;
import com.miyako.utils.LogUtil;
import com.miyako.utils.ServerApp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * ClassName OrderTask
 * Description //TODO
 * Author Miyako
 * Date 2020-04-01-0001 15:57
 */
public class OrderTask extends BaseTask{

    private static final String TAG = OrderTask.class.getSimpleName();

    public OrderTask(long taskId, JsonElement body, long timestamp, String address){
        super(taskId, body, timestamp, address);
    }

    @Override
    protected String getTaskType(){
        return "OrderTask";
    }

    @Override
    public ResBody doTask(){
        LogUtil.i(TAG, "doTask");
        ResBody<OrderMsg> result = null;
        switch (getBody().getAsJsonObject().get("code").getAsInt()) {
            case ServerApp.CMD_ORDER_READ_BY_TRACK:
                result = readOrderByTrack();
                break;
            case ServerApp.CMD_ORDER_READ_BY_MISSION:
                result = readOrderByMissionId();
                break;
            case ServerApp.CMD_ORDER_WRITE:
                result = writeOrder();
                break;
            default:
                result = null;
                break;
        }
        return result;
    }

    // code:0xf200，读取指定track的order
    private ResBody<OrderMsg> readOrderByTrack(){
        Gson gson = new Gson();
        ReqBody<QueryMsg> reqBody = gson.fromJson(getBody(), new TypeToken<ReqBody<QueryMsg>>() {}.getType());
        // 从数据库读取原始order数据
        String track = reqBody.getData().get(0).getQueryId();
        Order order = OrderDao.findByTrackId(track);
        if (order == null) {
            return new ResBody<>(ServerApp.CMD_ORDER_READ_BY_TRACK, System.currentTimeMillis(),
                                 ServerApp.RESPONSE_ERROR + getAddress());
        }
        OrderMsg msg = ParseDao.orderToOrderMsg(order);
        ResBody<OrderMsg> resBody = new ResBody<>(ServerApp.CMD_ORDER_READ_BY_MISSION, System.currentTimeMillis(),
                                                  ServerApp.RESPONSE_SUCCESS + getAddress());
        resBody.setData(Collections.singletonList(msg));
        return resBody;
    }

    // code:0xf202，读取指定mission的所有order
    private ResBody<OrderMsg> readOrderByMissionId(){
        Gson gson = new Gson();
        ReqBody<QueryMsg> reqBody = gson.fromJson(getBody(), new TypeToken<ReqBody<QueryMsg>>() {}.getType());
        // 从数据库读取原始order数据
        String missionId = reqBody.getData().get(0).getQueryId();
        List<Order> orders = OrderDao.findByMissionId(missionId);
        if (orders == null) {
            return new ResBody<>(ServerApp.CMD_ORDER_READ_BY_MISSION, System.currentTimeMillis(),
                                 ServerApp.RESPONSE_ERROR + getAddress());
        }
        List<OrderMsg> msgList = new ArrayList<>(orders.size());
        for (Order order : orders) {
            OrderMsg msg = ParseDao.orderToOrderMsg(order);
            msgList.add(msg);
        }
        ResBody<OrderMsg> resBody = new ResBody<>(ServerApp.CMD_ORDER_READ_BY_MISSION, System.currentTimeMillis(),
                                                    ServerApp.RESPONSE_SUCCESS + getAddress());
        resBody.setData(msgList);
        return resBody;
    }

    private ResBody<OrderMsg> writeOrder(){
        return new ResBody<>(ServerApp.CMD_ORDER_WRITE, System.currentTimeMillis(),
                             ServerApp.RESPONSE_ERROR + getAddress());
    }
}
